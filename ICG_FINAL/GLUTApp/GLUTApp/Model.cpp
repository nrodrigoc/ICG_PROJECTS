#include "Model.h"

string getBasePath(const string& path) {
	size_t pos = path.find_last_of("\\/");
	return (string::npos == pos) ? "" : path.substr(0, pos + 1);
}

Model::Model() {
}

Model::~Model() {
	//delete tt;

	textureIdMap.clear();

	if (textureIds) {
		delete[] textureIds;
		textureIds = NULL;
	}

	aiReleaseImport(scene);    //清除new的空间，防止内存泄露
}

bool Model::importModel(const string& pFile) {
	//先检查文件是否存在
	ifstream fin(pFile.c_str());
	if (!fin.fail())
		fin.close();
	else
		return false;

	scene = aiImportFile(pFile.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (!scene)
		return false;
	else {
		cout << "Import successfully!" << endl;
	}
	return true;
}

void Model::renderTheModel(float scale = 1.0f, bool isAmbient = true) {
	glEnable(GL_TEXTURE_2D);
	recursiveRender(scene, scene->mRootNode, scale, isAmbient);
	glDisable(GL_TEXTURE_2D);
}

void Model::recursiveRender(const struct aiScene *sc, const struct aiNode* nd, float scale = 1.0f, bool isAmbient = true) {
	aiMatrix4x4 mTrans = nd->mTransformation;
	aiMatrix4x4 m2;
	aiMatrix4x4::Scaling(aiVector3D(scale, scale, scale), m2);
	mTrans = mTrans * m2;

	//更新每个节点的变换方式
	mTrans.Transpose();
	glPushMatrix();
	glMultMatrixf((float*)&mTrans);

	//对当前节点，遍历该节点的所有mMeshes(contains index to a mesh in scene.mMeshes[])
	for (int m = 0; m < nd->mNumMeshes; m++) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[m]];

		//添加texture
		applyMaterial(sc->mMaterials[mesh->mMaterialIndex], isAmbient);


		if (mesh->mNormals == NULL)
			glDisable(GL_LIGHTING);
		else
			glEnable(GL_LIGHTING);

		//对当前的mesh，遍历所有面face
		for (int f = 0; f < mesh->mNumFaces; f++) {
			const struct aiFace* face = &(mesh->mFaces[f]);

			GLenum face_mode;

			switch (face->mNumIndices) {
			case 1:
				face_mode = GL_POINTS;
				break;
			case 2:
				face_mode = GL_LINES;
				break;
			case 3:
				face_mode = GL_TRIANGLES;
				break;
			default:
				face_mode = GL_POLYGON;
				break;
			}

			glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);    //模型加漫反射光

			const GLfloat edgeColor[] = { 0.f, 1.0f, 1.f, 1.0f };
			const GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };

			if (isAmbient) {
				glColor4fv(edgeColor);
				glEnable(GL_COLOR_MATERIAL);
			}

			glBegin(face_mode);

			int i;
			//对当前的face，遍历所有顶点索引（在Mesh的位置）
			for (i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];

				//环境光不加载纹理和法线
				if (mesh->mNormals != NULL && !isAmbient) {
					if (mesh->HasTextureCoords(0)) {    //有纹理坐标时
						glTexCoord2f(mesh->mTextureCoords[0][index].x,
							1 - mesh->mTextureCoords[0][index].y); //mTextureCoords[channel][vertex]
					}
					glNormal3fv(&mesh->mNormals[index].x);
				}
				glVertex3fv(&(mesh->mVertices[index].x));
			}
			glEnd();

			if (isAmbient)
				glDisable(GL_COLOR_MATERIAL);
		}
	}

	//递归绘制其他子节点
	for (int n = 0; n < nd->mNumChildren; ++n) {
		recursiveRender(sc, nd->mChildren[n], scale, isAmbient);
	}

	glPopMatrix();
}


void color4_to_float4(const aiColor4D *c, float f[4]) {
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

void set_float4(float f[4], float a, float b, float c, float d) {
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}
void Model::applyMaterial(const aiMaterial *mtl, bool isAmbient) {
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;
	float shininess, strength;
	int two_sided;
	unsigned int max;

	int texIndex = 0;
	aiString texPath;	//contains filename of texture

	if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath)) {
		//bind texture
		unsigned int texId = *(textureIdMap[texPath.data]);
		if (isAmbient)
			glBindTexture(GL_TEXTURE_2D, 0);
		else
			glBindTexture(GL_TEXTURE_2D, texId);
	}

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	if (ret1 == AI_SUCCESS) {
		max = 1;
		ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
		if (ret2 == AI_SUCCESS)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
		else
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	}
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	max = 1;
	if ((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);
}


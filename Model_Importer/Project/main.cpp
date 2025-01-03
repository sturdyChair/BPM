#include <iostream>
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <fstream>
#include <filesystem>
#include <sstream>

using namespace std;
using namespace Assimp;
namespace fs = filesystem;

struct XMFLOAT3
{
	float x;
	float y;
	float z;
};

struct XMFLOAT2
{
	float x;
	float y;
};

struct VTXMESH
{
	XMFLOAT3		vPosition;
	XMFLOAT3		vNormal;
	XMFLOAT2		vTexcoord;
	XMFLOAT3		vTangent;
};

template<typename T>
std::ostream& write_data(std::ostream& stream, const T& value) {
	return stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

template<typename T>
std::istream& read_data(std::istream& stream, T& value) {
	return stream.read(reinterpret_cast<char*>(&value), sizeof(T));
}


string normalize_path(const std::string& path) {
	std::string normalized;
	int iBs = 0;
	for (char ch : path) {
		if (ch == '\"')
		{

		}
		else if (ch == '\\') {
			if(iBs == 0)
			{
				normalized += '/';
				++iBs;
			}
			else
			{
				iBs = 0;
			}
			
		}
		else {
			normalized += ch;
		}
	}

	return normalized;
}

int CountBones(int& iBoneCount, const aiNode* pNode)
{
	++iBoneCount;
	int iNumChildren = pNode->mNumChildren;
	for (int i = 0; i < iNumChildren; ++i)
	{
		CountBones(iBoneCount, pNode->mChildren[i]);
	}
	return iBoneCount;
}

void ReadBones(const aiNode* pNode, ofstream& out)
{
	int iNumChildren = pNode->mNumChildren;
	out << "[BoneName] " << pNode->mName.data << '\n';

	out << "[ParentName] ";
	if (!pNode->mParent)
	{
		string str = "NOTEXIST";
		out << "NOTEXIST\n";

	}
	else
	{
		out << pNode->mParent->mName.C_Str() << '\n';

	}
	out << "[Transformation]\n"
		<< pNode->mTransformation.a1 << ' ' << pNode->mTransformation.b1 << ' ' << pNode->mTransformation.c1 << ' ' << pNode->mTransformation.d1 << '\n'
		<< pNode->mTransformation.a2 << ' ' << pNode->mTransformation.b2 << ' ' << pNode->mTransformation.c2 << ' ' << pNode->mTransformation.d2 << '\n'
		<< pNode->mTransformation.a3 << ' ' << pNode->mTransformation.b3 << ' ' << pNode->mTransformation.c3 << ' ' << pNode->mTransformation.d3 << '\n'
		<< pNode->mTransformation.a4 << ' ' << pNode->mTransformation.b4 << ' ' << pNode->mTransformation.c4 << ' ' << pNode->mTransformation.d4 << '\n';


	for (int i = 0; i < iNumChildren; ++i)
	{
		ReadBones(pNode->mChildren[i], out);
	}
}
void ReadBonesBin(const aiNode* pNode, ofstream& out)
{
	int iNumChildren = pNode->mNumChildren;
	//out << pNode->mName.length << pNode->mName.data;
	write_data(out, pNode->mName.length);
	for (int i = 0; i < pNode->mName.length; ++i)
	{
		write_data(out, pNode->mName.data[i]);
	}
	if (!pNode->mParent)
	{
		string str = "NOTEXIST";
		//out << str.length() << "NOTEXIST";
		write_data(out, str.length());
		for (int i = 0; i < pNode->mName.length; ++i)
		{
			write_data(out, str[i]);
		}
	}
	else
	{
		write_data(out, pNode->mParent->mName.length);
		for (int i = 0; i < pNode->mParent->mName.length; ++i)
		{
			write_data(out, pNode->mParent->mName.C_Str()[i]);
		}
		//out << pNode->mParent->mName.length << pNode->mParent->mName.C_Str();
	}
	write_data(out, pNode->mTransformation.a1);
	write_data(out, pNode->mTransformation.b1);
	write_data(out, pNode->mTransformation.c1);
	write_data(out, pNode->mTransformation.d1);
	write_data(out, pNode->mTransformation.a2);
	write_data(out, pNode->mTransformation.b2);
	write_data(out, pNode->mTransformation.c2);
	write_data(out, pNode->mTransformation.d2);
	write_data(out, pNode->mTransformation.a3);
	write_data(out, pNode->mTransformation.b3);
	write_data(out, pNode->mTransformation.c3);
	write_data(out, pNode->mTransformation.d3);
	write_data(out, pNode->mTransformation.a4);
	write_data(out, pNode->mTransformation.b4);
	write_data(out, pNode->mTransformation.c4);
	write_data(out, pNode->mTransformation.d4);

	//out << pNode->mTransformation.a1 << ' ' << pNode->mTransformation.b1 << ' ' << pNode->mTransformation.c1 << ' ' << pNode->mTransformation.d1 << ' '
	//	<< pNode->mTransformation.a2 << ' ' << pNode->mTransformation.b2 << ' ' << pNode->mTransformation.c2 << ' ' << pNode->mTransformation.d2 << ' '
	//	<< pNode->mTransformation.a3 << ' ' << pNode->mTransformation.b3 << ' ' << pNode->mTransformation.c3 << ' ' << pNode->mTransformation.d3 << ' '
	//	<< pNode->mTransformation.a4 << ' ' << pNode->mTransformation.b4 << ' ' << pNode->mTransformation.c4 << ' ' << pNode->mTransformation.d4 << ' ';


	for (int i = 0; i < iNumChildren; ++i)
	{
		ReadBonesBin(pNode->mChildren[i], out);
	}
}
void ReadFile(fs::path ModelDirectory, unsigned int iFlag, Importer& Core, bool bAnim)
{
	fs::recursive_directory_iterator ditr(ModelDirectory);

	while (fs::end(ditr) != ditr)
	{
		const fs::directory_entry& entry = *ditr;
		std::cout << entry.path() << '\n';
		stringstream ss;
		ss << entry.path();
		if (entry.is_regular_file())
		{
			const aiScene* pScene;
			string normPath = normalize_path(ss.str());
			try
			{
				pScene = Core.ReadFile(normPath, iFlag);
			}
			catch (...)
			{
				std::cout << Core.GetErrorString();
				return;
			}
			if (!pScene)
			{
				ditr++;
				continue;
			}
			ofstream out(string("../Out/") + entry.path().stem().string() + string(".txt"));

			if (!out.is_open())
				throw;

			int iNumMeshes = pScene->mNumMeshes;
			int iNumMaterials = pScene->mNumMaterials;

			out << "[NumMeshes] " << iNumMeshes << '\n';

			out << "[Animated] ";
			if (bAnim)
			{
				out << 1 << '\n';

			}
			else
			{
				out << 0 << '\n';

			}
			if (bAnim)
			{
				int iNumBone = 0;
				CountBones(iNumBone, pScene->mRootNode);
				out << "[NumBones] " << iNumBone << '\n';

				ReadBones(pScene->mRootNode, out);
			}
			for (int i = 0; i < iNumMeshes; ++i)
			{
				aiMesh* pAIMesh = pScene->mMeshes[i];
				out << "[Mesh" << i << "] " << pAIMesh->mName.C_Str() << '\n';


				int iNumVertices = pAIMesh->mNumVertices;
				int iNumIndices = pAIMesh->mNumFaces * 3;
				out << "[NumVertices] " << iNumVertices << "\n[NumIndices] " << iNumIndices << "\n";

				int iMaterialIndex = pAIMesh->mMaterialIndex;
				out << "[MaterialIndex] " << iMaterialIndex << '\n';

				if (bAnim)
				{
					out << "[NumBones] " << pAIMesh->mNumBones << '\n';

					for (int BoneIdx = 0; BoneIdx < pAIMesh->mNumBones; ++BoneIdx)
					{
						out << "[BoneName] " << pAIMesh->mBones[BoneIdx]->mName.C_Str() << '\n';
						out << "[NumWeight] " << pAIMesh->mBones[BoneIdx]->mNumWeights << '\n';
						out << "[Vertex_ID&Weights]\n";
						for (int widx = 0; widx < pAIMesh->mBones[BoneIdx]->mNumWeights; ++widx)
						{
							out << pAIMesh->mBones[BoneIdx]->mWeights[widx].mVertexId << ' ' << pAIMesh->mBones[BoneIdx]->mWeights[widx].mWeight << '\n';
						}
						out << "[Offset_Matrix]\n";
						auto& OffsetMatrix = pAIMesh->mBones[BoneIdx]->mOffsetMatrix;
						out << OffsetMatrix.a1 << ' ' << OffsetMatrix.b1 << ' ' << OffsetMatrix.c1 << ' ' << OffsetMatrix.d1 << '\n'
							<< OffsetMatrix.a2 << ' ' << OffsetMatrix.b2 << ' ' << OffsetMatrix.c2 << ' ' << OffsetMatrix.d2 << '\n'
							<< OffsetMatrix.a3 << ' ' << OffsetMatrix.b3 << ' ' << OffsetMatrix.c3 << ' ' << OffsetMatrix.d3 << '\n'
							<< OffsetMatrix.a4 << ' ' << OffsetMatrix.b4 << ' ' << OffsetMatrix.c4 << ' ' << OffsetMatrix.d4 << '\n';
					}
				}

				for (int i = 0; i < iNumVertices; ++i)
				{
					out << "[Vertex" << i << "]\n" << "[Position] " << pAIMesh->mVertices[i].x << ' ' << pAIMesh->mVertices[i].y << ' ' << pAIMesh->mVertices[i].z << ' ';
					out << "[Normal] " << pAIMesh->mNormals[i].x << ' ' << pAIMesh->mNormals[i].y << ' ' << pAIMesh->mNormals[i].z << ' ';
					out << "[Texcoord] " << pAIMesh->mTextureCoords[0][i].x << ' ' << pAIMesh->mTextureCoords[0][i].y << ' ';
					out << "[Tangent] " << pAIMesh->mTangents[i].x << ' ' << pAIMesh->mTangents[i].y << ' ' << pAIMesh->mTangents[i].z << '\n';
				}
				
				out << "[Indicies]\n";
				for (int i = 0; i < pAIMesh->mNumFaces; ++i)
				{
					out << pAIMesh->mFaces[i].mIndices[0] << '\n';
					out << pAIMesh->mFaces[i].mIndices[1] << '\n';
					out << pAIMesh->mFaces[i].mIndices[2] << '\n';
				}
			}


			out << "[NumMaterials] " << iNumMaterials << '\n';
			for (int i = 0; i < iNumMaterials; ++i)
			{
				aiMaterial* pMaterial = pScene->mMaterials[i];
				out << "[Material" << i << "]\n";
				for (int j = 1; j < 18; ++j)
				{
					aiString strPath;

					aiReturn art = pMaterial->GetTexture((aiTextureType)j, 0, &strPath);
					if (art != aiReturn_SUCCESS)
					{
						out << 0 << '\n';
						continue;
					}
					char szFileName[256] = {};
					char szEXT[256] = {};
					out << 1 << ' ';
					_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, 256, szEXT, 256);
					out << szFileName << szEXT << '\n';
				}
			}
			
			if(bAnim)
			{
				int iNumAnimation = pScene->mNumAnimations;
				out << "[NumAnimations] " << iNumAnimation << '\n';
				for (int i = 0; i < iNumAnimation; ++i)
				{
					auto& Animation = pScene->mAnimations[i];
					out << "[AnimationName] " << Animation->mName.C_Str() <<'\n';
					out << "[NumChannel] " << Animation->mNumChannels <<'\n';
					out << "[Duration] " << (float)Animation->mDuration <<'\n';
					out << "[TPS] " << (float)Animation->mTicksPerSecond <<'\n';

					for (int j = 0; j < Animation->mNumChannels; ++j)
					{
						auto& Channel = Animation->mChannels[j];
						out << "[ChannelName] " << Channel->mNodeName.C_Str() <<'\n';
						int iNumScalingKey = Channel->mNumScalingKeys;
						int iNumRotationKey = Channel->mNumRotationKeys;
						int iNumTranslationKey = Channel->mNumPositionKeys;
						out << "[NumScalingKey] " << iNumScalingKey << '\n';
						out << "[NumRotationKey] " << iNumRotationKey << '\n';
						out << "[NumTranslationKey] " << iNumTranslationKey << '\n';

						out << "[ScalingKey]\n";
						for (int k = 0; k < iNumScalingKey; ++k)
						{
							out << Channel->mScalingKeys[k].mValue.x << ' ' << Channel->mScalingKeys[k].mValue.y << ' ' << Channel->mScalingKeys[k].mValue.z << '\n';
						}
						out << "[RotationKey]\n";
						for (int k = 0; k < iNumRotationKey; ++k)
						{
							out << Channel->mRotationKeys[k].mValue.x << ' ' << Channel->mRotationKeys[k].mValue.y << ' ' 
								<< Channel->mRotationKeys[k].mValue.z << ' '<< Channel->mRotationKeys[k].mValue.w<<'\n';
						}
						out << "[TranslationKey]\n";
						for (int k = 0; k < iNumTranslationKey; ++k)
						{
							out << Channel->mPositionKeys[k].mValue.x << ' ' << Channel->mPositionKeys[k].mValue.y << ' ' << Channel->mPositionKeys[k].mValue.z << '\n';
						}
						int iMaxKey = max(iNumRotationKey, max(iNumTranslationKey, iNumScalingKey));
						{
							out << "[TrackPosition]\n";
							if (iMaxKey == iNumScalingKey)
							{
								for (int k = 0; k < iMaxKey; ++k)
								{
									out << Channel->mScalingKeys[k].mTime << ' ';
								}
							}
							else if (iMaxKey == iNumRotationKey)
							{
								for (int k = 0; k < iMaxKey; ++k)
								{
									out << Channel->mRotationKeys[k].mTime << ' ';
								}
							}
							else
							{
								for (int k = 0; k < iMaxKey; ++k)
								{
									out << Channel->mPositionKeys[k].mTime << ' ';
								}
							}
						}

					}



				}

			}



			out.close();
		}
		ditr++;
	}
}

void ReadFileBin(fs::path ModelDirectory, unsigned int iFlag, Importer& Core, bool bAnim)
{
	fs::recursive_directory_iterator ditr(ModelDirectory);

	while (fs::end(ditr) != ditr)
	{
		const fs::directory_entry& entry = *ditr;
		std::cout << entry.path() << '\n';
		stringstream ss;
		ss << entry.path();
		if (entry.is_regular_file())
		{
			const aiScene* pScene;
			string normPath = normalize_path(ss.str());
			try
			{
				pScene = Core.ReadFile(normPath, iFlag);
			}
			catch (...)
			{
				std::cout << Core.GetErrorString();
				return;
			}
			if (!pScene)
			{
				ditr++;
				continue;
			}
			ofstream out(string("../Out/") + entry.path().stem().string() + string(".dat"), ios::binary);

			if (!out.is_open())
				throw;

			int iNumMeshes = pScene->mNumMeshes;
			int iNumMaterials = pScene->mNumMaterials;
			write_data(out, iNumMeshes);
			//out << iNumMeshes << ' ';
			if (bAnim)
			{
				write_data(out, 1);
				//out << 1 << ' ';
			}
			else
			{
				write_data(out, 0);
				//out << 0 << ' ';
			}
			if (bAnim)
			{
				int iNumBone = 0;
				CountBones(iNumBone, pScene->mRootNode);
				write_data(out, iNumBone);
				//out << iNumBone << ' ';
				ReadBonesBin(pScene->mRootNode, out);
			}
			for (int i = 0; i < iNumMeshes; ++i)
			{
				aiMesh* pAIMesh = pScene->mMeshes[i];
				write_data(out, pAIMesh->mName.length);
				for (int i = 0; i < pAIMesh->mName.length; ++i)
				{
					write_data(out, pAIMesh->mName.C_Str()[i]);
				}
				//out << pAIMesh->mName.length << pAIMesh->mName.C_Str();

				int iNumVertices = pAIMesh->mNumVertices;
				int iNumIndices = pAIMesh->mNumFaces * 3;
				write_data(out, iNumVertices);
				write_data(out, iNumIndices);
				//out << iNumVertices << ' ' << iNumIndices << ' ';

				int iMaterialIndex = pAIMesh->mMaterialIndex;
				write_data(out, iMaterialIndex);
				//out << iMaterialIndex << ' ';

				if (bAnim)
				{
					//out << pAIMesh->mNumBones << ' ';
					write_data(out, pAIMesh->mNumBones);

					for (int BoneIdx = 0; BoneIdx < pAIMesh->mNumBones; ++BoneIdx)
					{
						write_data(out, pAIMesh->mBones[BoneIdx]->mName.length);
						for (int i = 0; i < pAIMesh->mBones[BoneIdx]->mName.length; ++i)
						{
							write_data(out, pAIMesh->mBones[BoneIdx]->mName.C_Str()[i]);
						}
						//out << pAIMesh->mBones[BoneIdx]->mName.length << pAIMesh->mBones[BoneIdx]->mName.C_Str() ;
						write_data(out, pAIMesh->mBones[BoneIdx]->mNumWeights);
						//out << pAIMesh->mBones[BoneIdx]->mNumWeights << ' ';
						for (int widx = 0; widx < pAIMesh->mBones[BoneIdx]->mNumWeights; ++widx)
						{
							write_data(out, pAIMesh->mBones[BoneIdx]->mWeights[widx].mVertexId);
							write_data(out, pAIMesh->mBones[BoneIdx]->mWeights[widx].mWeight);
							//out << pAIMesh->mBones[BoneIdx]->mWeights[widx].mVertexId << ' ' << pAIMesh->mBones[BoneIdx]->mWeights[widx].mWeight << ' ';
						}
						auto& OffsetMatrix = pAIMesh->mBones[BoneIdx]->mOffsetMatrix;
						write_data(out, OffsetMatrix.a1);
						write_data(out, OffsetMatrix.b1);
						write_data(out, OffsetMatrix.c1);
						write_data(out, OffsetMatrix.d1);
						write_data(out, OffsetMatrix.a2);
						write_data(out, OffsetMatrix.b2);
						write_data(out, OffsetMatrix.c2);
						write_data(out, OffsetMatrix.d2);
						write_data(out, OffsetMatrix.a3);
						write_data(out, OffsetMatrix.b3);
						write_data(out, OffsetMatrix.c3);
						write_data(out, OffsetMatrix.d3);
						write_data(out, OffsetMatrix.a4);
						write_data(out, OffsetMatrix.b4);
						write_data(out, OffsetMatrix.c4);
						write_data(out, OffsetMatrix.d4);
						//out << OffsetMatrix.a1<< ' ' <<OffsetMatrix.b1 << ' ' << OffsetMatrix.c1 << ' ' << OffsetMatrix.d1 << ' '
						//	<< OffsetMatrix.a2 << ' ' << OffsetMatrix.b2 << ' ' << OffsetMatrix.c2 << ' ' << OffsetMatrix.d2 << ' '
						//	<< OffsetMatrix.a3 << ' ' << OffsetMatrix.b3 << ' ' << OffsetMatrix.c3 << ' ' << OffsetMatrix.d3 << ' '
						//	<< OffsetMatrix.a4 << ' ' << OffsetMatrix.b4 << ' ' << OffsetMatrix.c4 << ' ' << OffsetMatrix.d4 << ' ';
					}
				}

				for (int i = 0; i < iNumVertices; ++i)
				{
					write_data(out, pAIMesh->mVertices[i].x);
					write_data(out, pAIMesh->mVertices[i].y);
					write_data(out, pAIMesh->mVertices[i].z);
					//out << pAIMesh->mVertices[i].x << ' ' << pAIMesh->mVertices[i].y << ' ' << pAIMesh->mVertices[i].z << ' ';//Position
					write_data(out, pAIMesh->mNormals[i].x);
					write_data(out, pAIMesh->mNormals[i].y);
					write_data(out, pAIMesh->mNormals[i].z);
					//out << pAIMesh->mNormals[i].x << ' ' << pAIMesh->mNormals[i].y << ' ' << pAIMesh->mNormals[i].z << ' ';//Normal
					write_data(out, pAIMesh->mTextureCoords[0][i].x);
					write_data(out, pAIMesh->mTextureCoords[0][i].y);
					//out << pAIMesh->mTextureCoords[0][i].x << ' ' << pAIMesh->mTextureCoords[0][i].y << ' ';//Tex
					write_data(out, pAIMesh->mTangents[i].x);
					write_data(out, pAIMesh->mTangents[i].y);
					write_data(out, pAIMesh->mTangents[i].z);
					//out << pAIMesh->mTangents[i].x << ' ' << pAIMesh->mTangents[i].y << ' ' << pAIMesh->mTangents[i].z << ' ';//Tan
				}

				for (int i = 0; i < pAIMesh->mNumFaces; ++i)
				{
					write_data(out, pAIMesh->mFaces[i].mIndices[0]);
					write_data(out, pAIMesh->mFaces[i].mIndices[1]);
					write_data(out, pAIMesh->mFaces[i].mIndices[2]);
					//out << pAIMesh->mFaces[i].mIndices[0] << ' ';
					//out << pAIMesh->mFaces[i].mIndices[1] << ' ';
					//out << pAIMesh->mFaces[i].mIndices[2] << ' ';
				}
			}
			write_data(out, iNumMaterials);
			//out << iNumMaterials << ' ';
			for (int i = 0; i < iNumMaterials; ++i)
			{
				aiMaterial* pMaterial = pScene->mMaterials[i];
				for (int j = 1; j < 18; ++j)
				{
					aiString strPath;

					aiReturn art = pMaterial->GetTexture((aiTextureType)j, 0, &strPath);
					if (art != aiReturn_SUCCESS)
					{
						write_data(out, 0);
						//out << 0 << ' ';
						continue;
					}
					char szFileName[256] = {};
					char szEXT[256] = {};
					write_data(out, 1);
					//out << 1 << ' ';
					_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, 256, szEXT, 256);
					string strFiles = szFileName;
					strFiles += string(szEXT);
					write_data(out, strFiles.length());
					for (int i = 0; i < strFiles.length(); ++i)
					{
						write_data(out, strFiles[i]);
					}
					//out << strFiles.length() << strFiles;
				}
			}

			if (bAnim)
			{
				int iNumAnimation = pScene->mNumAnimations;
				write_data(out, iNumAnimation);
				//out << iNumAnimation << ' ';
				for (int i = 0; i < iNumAnimation; ++i)
				{
					auto& Animation = pScene->mAnimations[i];
					write_data(out, Animation->mName.length);
					for (int i = 0; i < Animation->mName.length; ++i)
					{
						write_data(out, Animation->mName.C_Str()[i]);
					}
					//out << Animation->mName.length << Animation->mName.C_Str();
					write_data(out, Animation->mNumChannels);
					//out << Animation->mNumChannels << ' ';
					write_data(out, (float)Animation->mDuration);
					//out << (float)Animation->mDuration << ' ';
					write_data(out, (float)Animation->mTicksPerSecond);
					//out << (float)Animation->mTicksPerSecond << ' ';

					for (int j = 0; j < Animation->mNumChannels; ++j)
					{
						auto& Channel = Animation->mChannels[j];
						write_data(out, Channel->mNodeName.length);
						for (int i = 0; i < Channel->mNodeName.length; ++i)
						{
							write_data(out, Channel->mNodeName.C_Str()[i]);
						}
						//out << Channel->mNodeName.length << Channel->mNodeName.C_Str();
						int iNumScalingKey = Channel->mNumScalingKeys;
						int iNumRotationKey = Channel->mNumRotationKeys;
						int iNumTranslationKey = Channel->mNumPositionKeys;
						write_data(out, iNumScalingKey);
						write_data(out, iNumRotationKey);
						write_data(out, iNumTranslationKey);
						//out << iNumScalingKey << ' ';
						//out << iNumRotationKey << ' ';
						//out << iNumTranslationKey << ' ';

						for (int k = 0; k < iNumScalingKey; ++k)
						{
							write_data(out, Channel->mScalingKeys[k].mValue.x);
							write_data(out, Channel->mScalingKeys[k].mValue.y);
							write_data(out, Channel->mScalingKeys[k].mValue.z);
							//out << Channel->mScalingKeys[k].mValue.x << ' ' << Channel->mScalingKeys[k].mValue.y << ' ' << Channel->mScalingKeys[k].mValue.z << ' ';
						}

						for (int k = 0; k < iNumRotationKey; ++k)
						{
							write_data(out, Channel->mRotationKeys[k].mValue.x);
							write_data(out, Channel->mRotationKeys[k].mValue.y);
							write_data(out, Channel->mRotationKeys[k].mValue.z);
							write_data(out, Channel->mRotationKeys[k].mValue.w);
							//out << Channel->mRotationKeys[k].mValue.x << ' ' <<  Channel->mRotationKeys[k].mValue.y << ' '
							//	<< Channel->mRotationKeys[k].mValue.z << ' ' << Channel->mRotationKeys[k].mValue.w << ' ';
						}

						for (int k = 0; k < iNumTranslationKey; ++k)
						{
							write_data(out, Channel->mPositionKeys[k].mValue.x);
							write_data(out, Channel->mPositionKeys[k].mValue.y);
							write_data(out, Channel->mPositionKeys[k].mValue.z);
							//out << Channel->mPositionKeys[k].mValue.x << ' ' << Channel->mPositionKeys[k].mValue.y << ' ' << Channel->mPositionKeys[k].mValue.z << ' ';
						}
						int iMaxKey = max(iNumRotationKey, max(iNumTranslationKey, iNumScalingKey));
						{
							if (iMaxKey == iNumScalingKey)
							{
								for (int k = 0; k < iMaxKey; ++k)
								{
									write_data(out, Channel->mScalingKeys[k].mTime);
									//out << Channel->mScalingKeys[k].mTime << ' ';
								}
							}
							else if (iMaxKey == iNumRotationKey)
							{
								for (int k = 0; k < iMaxKey; ++k)
								{
									write_data(out, Channel->mScalingKeys[k].mTime);
									//out << Channel->mRotationKeys[k].mTime << ' ';
								}
							}
							else
							{
								for (int k = 0; k < iMaxKey; ++k)
								{
									write_data(out, Channel->mScalingKeys[k].mTime);
									//out << Channel->mPositionKeys[k].mTime << ' ';
								}
							}
						}

					}



				}

			}



			out.close();
		}
		ditr++;
	}
}

int main()
{
	Importer Core{};

	fs::path ModelDirectory("../Model/NonAnim");
	if (!fs::exists(ModelDirectory))
	{
		throw;
	}
	unsigned int iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	ReadFile(ModelDirectory, iFlag, Core, false);
	ReadFileBin(ModelDirectory, iFlag, Core, false);
	/*
	fs::recursive_directory_iterator ditr(ModelDirectory);
	
	while (fs::end(ditr) != ditr)
	{
		const fs::directory_entry& entry = *ditr;
		std::cout << entry.path() << '\n';
		stringstream ss;
		ss << entry.path();
		if (entry.is_regular_file())
		{
			const aiScene* pScene;
			string normPath = normalize_path(ss.str());
			try
			{
				pScene = Core.ReadFile(normPath, iFlag);
			}
			catch (...)
			{
				std::cout << Core.GetErrorString();
				return 0;
			}
			if (!pScene)
			{
				ditr++;
				continue;
			}
			ofstream out(string("../Out/") + entry.path().stem().string() + string(".txt"));
			if (!out.is_open())
				throw;

			int iNumMeshes = pScene->mNumMeshes;
			int iNumMaterials = pScene->mNumMaterials;

			out << "[NumMeshes] " << iNumMeshes << '\n';
			for (int i = 0; i < iNumMeshes; ++i)
			{
				aiMesh* pAIMesh = pScene->mMeshes[i];
				out << "[Mesh" << i << "] " << pAIMesh->mName.C_Str() << '\n';
				int iNumVertices = pAIMesh->mNumVertices;
				int iNumIndices = pAIMesh->mNumFaces * 3;
				out << "[NumVertices] " << iNumVertices << "\n[NumIndices] " << iNumIndices << "\n";
				
				int iMaterialIndex = pAIMesh->mMaterialIndex;
				out << "[MaterialIndex] " << iMaterialIndex << '\n';


				for (int i = 0; i < iNumVertices; ++i)
				{
					out << "[Vertex" << i << "]\n" << "[Position] " << pAIMesh->mVertices[i].x << ' ' << pAIMesh->mVertices[i].y << ' ' << pAIMesh->mVertices[i].z << ' ';
					out << "[Normal] " << pAIMesh->mNormals[i].x << ' ' << pAIMesh->mNormals[i].y << ' ' << pAIMesh->mNormals[i].z << ' ';
					out << "[Texcoord] " << pAIMesh->mTextureCoords[0][i].x << ' ' << pAIMesh->mTextureCoords[0][i].y << ' ';
					out << "[Tangent] " << pAIMesh->mTangents[i].x << ' ' << pAIMesh->mTangents[i].y << ' ' << pAIMesh->mTangents[i].z << '\n';
				}
				out << "[Indicies]\n";
				for (int i = 0; i < pAIMesh->mNumFaces; ++i)
				{
					out << pAIMesh->mFaces[i].mIndices[0] << '\n';
					out	<< pAIMesh->mFaces[i].mIndices[1] << '\n';
					out	<< pAIMesh->mFaces[i].mIndices[2] << '\n';
				}
			}

			int iNumMaterials = pScene->mNumMaterials;
			out << "[NumMaterials] " << iNumMaterials << '\n';
			for (int i = 0; i < iNumMaterials; ++i)
			{
				aiMaterial* pMaterial = pScene->mMaterials[i];
				out << "[Material" << i << "]\n";
				for (int j = 1; j < 18; ++j)
				{
					aiString strPath;
					
					aiReturn art = pMaterial->GetTexture((aiTextureType)j, 0, &strPath);
					if (art != aiReturn_SUCCESS)
					{
						out << 0 << '\n';
						continue;
					}
					char szFileName[256] = {};
					char szEXT[256] = {};
					out << 1 << ' ';
					_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, 256, szEXT, 256);
					out << szFileName << szEXT << '\n';
				}
			}


			out.close();
		}
		ditr++;
	}
	*/
	fs::path AnimDirectory("../Model/Anim");
	if (!fs::exists(AnimDirectory))
	{
		throw;
	}
	iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	ReadFile(AnimDirectory, iFlag, Core, true);
	ReadFileBin(AnimDirectory, iFlag, Core, true);

	return 0;
}

#include "Model.h"
#include "Texture.h"
#include "GameInstance.h"
#include "PhysxManager.h"
#include "Bone.h"
#include "Shader.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
	CComponent(pDevice, pContext)
{
}

CModel::CModel(const CModel& rhs) :
	CComponent(rhs), m_iNumMesh(rhs.m_iNumMesh), m_vecMaterials(rhs.m_vecMaterials), m_iNumBones(rhs.m_iNumBones), m_eModelType(rhs.m_eModelType),
	m_iNumAnimations(rhs.m_iNumAnimations), m_TransformMatrix(rhs.m_TransformMatrix), m_AnimTagIndex(rhs.m_AnimTagIndex), m_BoneTagIndex(rhs.m_BoneTagIndex)
{
	for (auto& pMesh : rhs.m_Meshes)
	{
		m_Meshes.push_back(pMesh);
		Safe_AddRef(pMesh);
	}
	for (auto& vecTextures : m_vecMaterials)
	{
		for (auto& pTexture : vecTextures)
		{
			Safe_AddRef(pTexture);
		}
	}
	for (auto& pProBone : rhs.m_Bones)
	{
		m_Bones.push_back(pProBone->Clone());
	}
	for (auto& pProAnim : rhs.m_Animations)
	{
		m_Animations.push_back(pProAnim->Clone());
	}
}

HRESULT CModel::Initialize_Prototype(const _char* pModelPath, _fmatrix TransformMatrix, const string& strTextureDirectory)
{

	if(FAILED(Read_Model(pModelPath, TransformMatrix, strTextureDirectory)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{


	return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

HRESULT CModel::Render_MeshGroup(_uint iGroupIndex)
{
	for (auto& pMesh : m_MeshGroup[iGroupIndex])
	{
		pMesh->Render();
	}

	return S_OK;
}

void CModel::Set_AnimTPS(_uint iIdx, _float fTPS)
{
	m_Animations[iIdx]->Set_TickPerSec(fTPS);
}

void CModel::Set_AnimTPS(const string& strTag, _float fTPS)
{
	m_Animations[m_AnimTagIndex[strTag]]->Set_TickPerSec(fTPS);
}

_float CModel::Get_AnimTPS(_uint iIdx)
{
	return m_Animations[iIdx]->Get_TickPerSec();
}

_float CModel::Get_AnimTPS(const string& strTag)
{
	return m_Animations[m_AnimTagIndex[strTag]]->Get_TickPerSec();
}

const _float3& CModel::Get_Min(_uint iMeshIdx) const
{
	return m_Meshes[iMeshIdx]->Get_Min();
}

const _float3& CModel::Get_Max(_uint iMeshIdx) const
{
	return m_Meshes[iMeshIdx]->Get_Max();
}

HRESULT CModel::Ready_Meshes(const vector<VTXMESH>& vecVerticies, const vector<_uint>& vecIndicies, const CMesh::MESH_DESC& desc)
{
	CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, vecVerticies, vecIndicies, desc);
	m_Meshes.push_back(pMesh);

	return S_OK;
}

HRESULT CModel::Ready_Meshes(const vector<VTXANIMMESH>& vecVerticies, const vector<_uint>& vecIndicies, const vector<_float4x4>& vecOffset, const CMesh::MESH_DESC& desc)
{
	CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, vecVerticies, vecIndicies, vecOffset,m_Bones, desc);
	m_Meshes.push_back(pMesh);

	return S_OK;
}

HRESULT CModel::Read_Model(const string& strPath, _fmatrix TransformMatrix, const string& strTextureDirectory)
{
	_uint iPathLen = strPath.length();
	if (strPath[iPathLen - 3] == L't'&& strPath[iPathLen - 2] == L'x'&& strPath[iPathLen - 1] == L't')
	{
		ifstream in(strPath);
		if (!in.is_open())
		{
			return E_FAIL;
		}
		string strTexturePath = strTextureDirectory;
		if (strTextureDirectory == "")
		{
			_char szDrive[256]{};
			_char szDirectory[256]{};
			_splitpath_s(strPath.c_str(), szDrive, 256, szDirectory, 256, nullptr, 0, nullptr, 0);
			strTexturePath = string(szDrive) + string(szDirectory);
		}
		string strTemp;
		string strName;
		_int iTemp = 0;
		_float fTemp = 0.f;
		
		in >> strTemp;

	
		in >> m_iNumMesh;
		in >> strTemp;

	
		in >> iTemp;
		if (iTemp)
		{
			m_eModelType = ANIM;
			Ready_Bones(in);
			vector<VTXANIMMESH> vecVerticies;
			vector<_uint> vecIndicies;
			Read(in, vecVerticies, vecIndicies, TransformMatrix, strTexturePath);
		}
		else
		{
			m_eModelType = NONANIM;
			vector<VTXMESH> vecVerticies;
			vector<_uint> vecIndicies;
			Read(in, vecVerticies, vecIndicies, TransformMatrix, strTexturePath);
		}
	}
	else
	{
		ifstream in(strPath, ios::binary);
		if (!in.is_open())
		{
			return E_FAIL;
		}
		string strTexturePath = strTextureDirectory;
		if (strTextureDirectory == "")
		{
			_char szDrive[256]{};
			_char szDirectory[256]{};
			_splitpath_s(strPath.c_str(), szDrive, 256, szDirectory, 256, nullptr, 0, nullptr, 0);
			strTexturePath = string(szDrive) + string(szDirectory);
		}
		string strTemp;
		string strName;
		_uint iTemp = 0;
		_float fTemp = 0.f;

		read_data(in, m_iNumMesh);
		//in >> m_iNumMesh;
		read_data(in, iTemp);
		//in >> iTemp;
		if (iTemp)
		{
			m_eModelType = ANIM;
			Ready_Bones_Bin(in);
			vector<VTXANIMMESH> vecVerticies;
			vector<_uint> vecIndicies;
			ReadBin(in, vecVerticies, vecIndicies, TransformMatrix, strTexturePath);
		}
		else
		{
			m_eModelType = NONANIM;
			vector<VTXMESH> vecVerticies;
			vector<_uint> vecIndicies;
			ReadBin(in, vecVerticies, vecIndicies, TransformMatrix, strTexturePath);
		}
	}
	return S_OK;
}

HRESULT CModel::Read(ifstream& in, vector<VTXMESH>& vecVerticies, vector<_uint>& vecIndicies, _fmatrix TransformMatrix, const string& strTexturePath)
{
	string strTemp, strName;
	for (int iNM = 0; iNM < m_iNumMesh; ++iNM)
	{
		CMesh::MESH_DESC desc;

		_int iNumVtx = 0;
		_int iNumIdx = 0;
		_int iMatIdx = 0;
		in >> strTemp >> strName;
		desc.m_strName = strName;
		in >> strTemp >> iNumVtx >> strTemp >> iNumIdx;
		in >> strTemp >> iMatIdx;
		desc.iMaterial_Index = iMatIdx;
		vecVerticies.resize(iNumVtx);
		vecIndicies.resize(iNumIdx);
		for (int i = 0; i < iNumVtx; ++i)
		{
			in >> strTemp >> strTemp;
			in >> vecVerticies[i].vPosition.x >> vecVerticies[i].vPosition.y >> vecVerticies[i].vPosition.z;
			in >> strTemp;
			in >> vecVerticies[i].vNormal.x >> vecVerticies[i].vNormal.y >> vecVerticies[i].vNormal.z;
			in >> strTemp;
			in >> vecVerticies[i].vTexcoord.x >> vecVerticies[i].vTexcoord.y;
			in >> strTemp;
			in >> vecVerticies[i].vTangent.x >> vecVerticies[i].vTangent.y >> vecVerticies[i].vTangent.z;

			XMStoreFloat3(&vecVerticies[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&vecVerticies[i].vPosition), TransformMatrix));
			XMStoreFloat3(&vecVerticies[i].vNormal, XMVector3TransformCoord(XMLoadFloat3(&vecVerticies[i].vNormal), TransformMatrix));
			XMStoreFloat3(&vecVerticies[i].vTangent, XMVector3TransformCoord(XMLoadFloat3(&vecVerticies[i].vTangent), TransformMatrix));

		}
		in >> strTemp;
		for (int i = 0; i < iNumIdx; ++i)
		{
			in >> vecIndicies[i];
		}
		Ready_Meshes(vecVerticies, vecIndicies, desc);
	}
	_int iNumMaterial = 0;
	in >> strTemp >> iNumMaterial;
	m_vecMaterials.resize(iNumMaterial);
	for (int k = 0; k < iNumMaterial; ++k)
	{
		in >> strTemp;
		for (int j = 1; j < 18; ++j)
		{
			int iExist = 0;
			in >> iExist;
			if (iExist)
			{
				string strFileName;
				in >> strFileName;
				string strFullPath = strTexturePath + strFileName;
				wstring lstFullPath(strFullPath.begin(), strFullPath.end());
				auto iter_mat = m_mapMaterial.find(lstFullPath);
				if (iter_mat == m_mapMaterial.end())
				{
					m_mapMaterial.emplace(lstFullPath, CTexture::Create(m_pDevice, m_pContext, lstFullPath, 1));
					iter_mat = m_mapMaterial.find(lstFullPath);
				}
				m_vecMaterials[k].push_back(iter_mat->second);
				Safe_AddRef(iter_mat->second);
			}
			else
			{
				m_vecMaterials[k].push_back(nullptr);
			}

		}
	}

	in.close();
	return S_OK;
}
HRESULT CModel::ReadBin(ifstream& in, vector<VTXMESH>& vecVerticies, vector<_uint>& vecIndicies, _fmatrix TransformMatrix, const string& strTexturePath)
{
	string strTemp, strName;
	for (int iNM = 0; iNM < m_iNumMesh; ++iNM)
	{
		CMesh::MESH_DESC desc;

		_uint iNumVtx = 0;
		_uint iNumIdx = 0;
		_uint iMatIdx = 0;
		_uint iLength = 0;
		_char szBuffer[256];
		read_data(in, iLength);
		//in >> iLength;
		in.read(szBuffer, iLength);
		szBuffer[iLength] = '\0';
		strName = szBuffer;
		desc.m_strName = strName;
		read_data(in, iNumVtx);
		read_data(in, iNumIdx);
		read_data(in, iMatIdx);
		//in >> iNumVtx >> iNumIdx;
		//in >> iMatIdx;
		desc.iMaterial_Index = iMatIdx;
		vecVerticies.resize(iNumVtx);
		vecIndicies.resize(iNumIdx);
		for (int i = 0; i < iNumVtx; ++i)
		{
			read_data(in, vecVerticies[i].vPosition);
			read_data(in, vecVerticies[i].vNormal);
			read_data(in, vecVerticies[i].vTexcoord);
			read_data(in, vecVerticies[i].vTangent);
			//in >> vecVerticies[i].vPosition.x >> vecVerticies[i].vPosition.y >> vecVerticies[i].vPosition.z;
			//in >> vecVerticies[i].vNormal.x >> vecVerticies[i].vNormal.y >> vecVerticies[i].vNormal.z;
			//in >> vecVerticies[i].vTexcoord.x >> vecVerticies[i].vTexcoord.y;
			//in >> vecVerticies[i].vTangent.x >> vecVerticies[i].vTangent.y >> vecVerticies[i].vTangent.z;

			XMStoreFloat3(&vecVerticies[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&vecVerticies[i].vPosition), TransformMatrix));
			XMStoreFloat3(&vecVerticies[i].vNormal, XMVector3TransformCoord(XMLoadFloat3(&vecVerticies[i].vNormal), TransformMatrix));
			XMStoreFloat3(&vecVerticies[i].vTangent, XMVector3TransformCoord(XMLoadFloat3(&vecVerticies[i].vTangent), TransformMatrix));

		}
		for (int i = 0; i < iNumIdx; ++i)
		{
			read_data(in, vecIndicies[i]);
			//in >> vecIndicies[i];
		}
		Ready_Meshes(vecVerticies, vecIndicies, desc);
	}
	_int iNumMaterial = 0;
	read_data(in, iNumMaterial);
	//in >> iNumMaterial;
	m_vecMaterials.resize(iNumMaterial);
	for (int k = 0; k < iNumMaterial; ++k)
	{
		for (int j = 1; j < 18; ++j)
		{
			int iExist = 0;
			//in >> iExist;
			read_data(in, iExist);
			if (iExist)
			{
				string strFileName;
				unsigned long long iLength = 0;
				_char szBuffer[256];
				read_data(in, iLength);
				//in >> iLength;
				in.read(szBuffer, iLength);
				szBuffer[iLength] = '\0';
				strFileName = szBuffer;
				string strFullPath = strTexturePath + strFileName;
				wstring lstFullPath(strFullPath.begin(), strFullPath.end());
				auto iter_mat = m_mapMaterial.find(lstFullPath);
				if (iter_mat == m_mapMaterial.end())
				{
					m_mapMaterial.emplace(lstFullPath, CTexture::Create(m_pDevice, m_pContext, lstFullPath, 1));
					iter_mat = m_mapMaterial.find(lstFullPath);
				}
				m_vecMaterials[k].push_back(iter_mat->second);
				Safe_AddRef(iter_mat->second);
			}
			else
			{
				m_vecMaterials[k].push_back(nullptr);
			}
		}
	}

	in.close();
	return S_OK;
}

HRESULT CModel::Read(ifstream& in, vector<VTXANIMMESH>& vecVerticies, vector<_uint>& vecIndicies, _fmatrix TransformMatrix, const string& strTexturePath)
{
	string strTemp, strName;
	XMStoreFloat4x4(&m_TransformMatrix, TransformMatrix);

	for (int iNM = 0; iNM < m_iNumMesh; ++iNM)
	{
		CMesh::MESH_DESC desc;
		vector<_float4x4> vecOffset;
		_int iNumVtx = 0;
		_int iNumIdx = 0;
		_int iMatIdx = 0;
		_int iNumBones = 0;
		in >> strTemp >> strName;
		desc.m_strName = strName;
		
		in >> strTemp >> iNumVtx >> strTemp >> iNumIdx;
		in >> strTemp >> iMatIdx;
		in >> strTemp >> iNumBones;
		desc.iNumBones = iNumBones;
		desc.iMaterial_Index = iMatIdx;
		vecVerticies.resize(iNumVtx);
		vecIndicies.resize(iNumIdx);
		vecOffset.resize(iNumBones);
		desc.ArrBones = new _uint[iNumBones];
		for (int i = 0; i < iNumVtx; ++i)
		{
			vecVerticies[i].vBlendIndex0.x = 0;
			vecVerticies[i].vBlendIndex0.y = 0;
			vecVerticies[i].vBlendIndex0.z = 0;
			vecVerticies[i].vBlendIndex0.w = 0;
			vecVerticies[i].vBlendWeight0.x = 0.f;
			vecVerticies[i].vBlendWeight0.y = 0.f;
			vecVerticies[i].vBlendWeight0.z = 0.f;
			vecVerticies[i].vBlendWeight0.w = 0.f;
			
			vecVerticies[i].vBlendIndex1.x = 0;
			vecVerticies[i].vBlendIndex1.y = 0;
			vecVerticies[i].vBlendIndex1.z = 0;
			vecVerticies[i].vBlendIndex1.w = 0;
			vecVerticies[i].vBlendWeight1.x = 0.f;
			vecVerticies[i].vBlendWeight1.y = 0.f;
			vecVerticies[i].vBlendWeight1.z = 0.f;
			vecVerticies[i].vBlendWeight1.w = 0.f;
			/*
			vecVerticies[i].vBlendIndex2.x = 0;
			vecVerticies[i].vBlendIndex2.y = 0;
			vecVerticies[i].vBlendIndex2.z = 0;
			vecVerticies[i].vBlendIndex2.w = 0;
			vecVerticies[i].vBlendWeight2.x = 0.f;
			vecVerticies[i].vBlendWeight2.y = 0.f;
			vecVerticies[i].vBlendWeight2.z = 0.f;
			vecVerticies[i].vBlendWeight2.w = 0.f;

			vecVerticies[i].vBlendIndex3.x = 0;
			vecVerticies[i].vBlendIndex3.y = 0;
			vecVerticies[i].vBlendIndex3.z = 0;
			vecVerticies[i].vBlendIndex3.w = 0;
			vecVerticies[i].vBlendWeight3.x = 0.f;
			vecVerticies[i].vBlendWeight3.y = 0.f;
			vecVerticies[i].vBlendWeight3.z = 0.f;
			vecVerticies[i].vBlendWeight3.w = 0.f;

			vecVerticies[i].vBlendIndex4.x = 0;
			vecVerticies[i].vBlendIndex4.y = 0;
			vecVerticies[i].vBlendIndex4.z = 0;
			vecVerticies[i].vBlendIndex4.w = 0;
			vecVerticies[i].vBlendWeight4.x = 0.f;
			vecVerticies[i].vBlendWeight4.y = 0.f;
			vecVerticies[i].vBlendWeight4.z = 0.f;
			vecVerticies[i].vBlendWeight4.w = 0.f;

			vecVerticies[i].vBlendIndex5.x = 0;
			vecVerticies[i].vBlendIndex5.y = 0;
			vecVerticies[i].vBlendIndex5.z = 0;
			vecVerticies[i].vBlendIndex5.w = 0;
			vecVerticies[i].vBlendWeight5.x = 0.f;
			vecVerticies[i].vBlendWeight5.y = 0.f;
			vecVerticies[i].vBlendWeight5.z = 0.f;
			vecVerticies[i].vBlendWeight5.w = 0.f;*/

		}
		for (int i = 0; i < iNumBones; ++i)
		{
			string strBoneName;
			_int iNumWeight = 0;
			in >> strTemp >> strBoneName;

			in >> strTemp >> iNumWeight;
			in >> strTemp;
			_int iVertIdx = 0;
			_float fWeight = 0.f;
			for (int j = 0; j < iNumWeight; ++j)
			{
				in >> iVertIdx >> fWeight;
				if(0.f == vecVerticies[iVertIdx].vBlendWeight0.x)
				{
					vecVerticies[iVertIdx].vBlendIndex0.x = i;
					vecVerticies[iVertIdx].vBlendWeight0.x = fWeight;
				}
				else if (0.f == vecVerticies[iVertIdx].vBlendWeight0.y)
				{
					vecVerticies[iVertIdx].vBlendIndex0.y = i;
					vecVerticies[iVertIdx].vBlendWeight0.y = fWeight;
				}
				else if (0.f == vecVerticies[iVertIdx].vBlendWeight0.z)
				{
					vecVerticies[iVertIdx].vBlendIndex0.z = i;
					vecVerticies[iVertIdx].vBlendWeight0.z = fWeight;
				}
				else if (0.f == vecVerticies[iVertIdx].vBlendWeight0.w)
				{
					vecVerticies[iVertIdx].vBlendIndex0.w = i;
					vecVerticies[iVertIdx].vBlendWeight0.w = fWeight;
				}
				else if (0.f == vecVerticies[iVertIdx].vBlendWeight1.x)
				{
					vecVerticies[iVertIdx].vBlendIndex1.x = i;
					vecVerticies[iVertIdx].vBlendWeight1.x = fWeight;
				}
				else if (0.f == vecVerticies[iVertIdx].vBlendWeight1.y)
				{
					vecVerticies[iVertIdx].vBlendIndex1.y = i;
					vecVerticies[iVertIdx].vBlendWeight1.y = fWeight;
				}
				else if (0.f == vecVerticies[iVertIdx].vBlendWeight1.z)
				{
					vecVerticies[iVertIdx].vBlendIndex1.z = i;
					vecVerticies[iVertIdx].vBlendWeight1.z = fWeight;
				}
				else if (0.f == vecVerticies[iVertIdx].vBlendWeight1.w)
				{
					vecVerticies[iVertIdx].vBlendIndex1.w = i;
					vecVerticies[iVertIdx].vBlendWeight1.w = fWeight;
				}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight2.x)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex2.x = i;
				//	vecVerticies[iVertIdx].vBlendWeight2.x = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight2.y)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex2.y = i;
				//	vecVerticies[iVertIdx].vBlendWeight2.y = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight2.z)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex2.z = i;
				//	vecVerticies[iVertIdx].vBlendWeight2.z = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight2.w)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex2.w = i;
				//	vecVerticies[iVertIdx].vBlendWeight2.w = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight3.x)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex3.x = i;
				//	vecVerticies[iVertIdx].vBlendWeight3.x = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight3.y)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex3.y = i;
				//	vecVerticies[iVertIdx].vBlendWeight3.y = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight3.z)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex3.z = i;
				//	vecVerticies[iVertIdx].vBlendWeight3.z = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight3.w)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex3.w = i;
				//	vecVerticies[iVertIdx].vBlendWeight3.w = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight4.x)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex4.x = i;
				//	vecVerticies[iVertIdx].vBlendWeight4.x = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight4.y)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex4.y = i;
				//	vecVerticies[iVertIdx].vBlendWeight4.y = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight4.z)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex4.z = i;
				//	vecVerticies[iVertIdx].vBlendWeight4.z = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight4.w)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex4.w = i;
				//	vecVerticies[iVertIdx].vBlendWeight4.w = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight5.x)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex5.x = i;
				//	vecVerticies[iVertIdx].vBlendWeight5.x = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight5.y)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex5.y = i;
				//	vecVerticies[iVertIdx].vBlendWeight5.y = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight5.z)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex5.z = i;
				//	vecVerticies[iVertIdx].vBlendWeight5.z = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight5.w)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex5.w = i;
				//	vecVerticies[iVertIdx].vBlendWeight5.w = fWeight;
				//}
				else
				{
					//MSG_BOX(L"it's over 24000!!!!!!!!!!!!1, Bones");
				}
			}
			in >> strTemp;
			in >> vecOffset[i]._11 >> vecOffset[i]._12 >> vecOffset[i]._13 >> vecOffset[i]._14
			   >> vecOffset[i]._21 >> vecOffset[i]._22 >> vecOffset[i]._23 >> vecOffset[i]._24
			   >> vecOffset[i]._31 >> vecOffset[i]._32 >> vecOffset[i]._33 >> vecOffset[i]._34
			   >> vecOffset[i]._41 >> vecOffset[i]._42 >> vecOffset[i]._43 >> vecOffset[i]._44;
			
			auto iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone) 
				{
					return pBone->Get_Name() == strBoneName;
				});
			desc.ArrBones[i] = iter - m_Bones.begin();
		}

		for (int i = 0; i < iNumVtx; ++i)
		{
			in >> strTemp >> strTemp;
			in >> vecVerticies[i].vPosition.x >> vecVerticies[i].vPosition.y >> vecVerticies[i].vPosition.z;
			in >> strTemp;
			in >> vecVerticies[i].vNormal.x >> vecVerticies[i].vNormal.y >> vecVerticies[i].vNormal.z;
			in >> strTemp;
			in >> vecVerticies[i].vTexcoord.x >> vecVerticies[i].vTexcoord.y;
			in >> strTemp;
			in >> vecVerticies[i].vTangent.x >> vecVerticies[i].vTangent.y >> vecVerticies[i].vTangent.z;

			//XMStoreFloat3(&vecVerticies[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&vecVerticies[i].vPosition), TransformMatrix));
			//XMStoreFloat3(&vecVerticies[i].vNormal, XMVector3TransformCoord(XMLoadFloat3(&vecVerticies[i].vNormal), TransformMatrix));

		}
		in >> strTemp;
		for (int i = 0; i < iNumIdx; ++i)
		{
			in >> vecIndicies[i];
		}
		Ready_Meshes(vecVerticies, vecIndicies, vecOffset, desc);
		delete[] desc.ArrBones;
	}
	_int iNumMaterial = 0;
	in >> strTemp >> iNumMaterial;
	m_vecMaterials.resize(iNumMaterial);
	for (int k = 0; k < iNumMaterial; ++k)
	{
		in >> strTemp;
		for (int j = 1; j < 18; ++j)
		{
			int iExist = 0;
			in >> iExist;
			if (iExist)
			{
				string strFileName;
				in >> strFileName;
				string strFullPath = strTexturePath + strFileName;
				wstring lstFullPath(strFullPath.begin(), strFullPath.end());

				auto iter_mat = m_mapMaterial.find(lstFullPath);
				if (iter_mat == m_mapMaterial.end())
				{
					m_mapMaterial.emplace(lstFullPath, CTexture::Create(m_pDevice, m_pContext, lstFullPath, 1));
					iter_mat = m_mapMaterial.find(lstFullPath);
				}
				m_vecMaterials[k].push_back(iter_mat->second);
				Safe_AddRef(iter_mat->second);
			}
			else
			{
				m_vecMaterials[k].push_back(nullptr);
			}
		}
	}
	in >> strTemp >> m_iNumAnimations;
	for (int k = 0; k < m_iNumAnimations; ++k)
	{
		m_Animations.push_back(CAnimation::Create(in, m_Bones));
	}

	Hash_Animation();
	Hash_Bone();


	in.close();
	return S_OK;
}

HRESULT CModel::ReadBin(ifstream& in, vector<VTXANIMMESH>& vecVerticies, vector<_uint>& vecIndicies, _fmatrix TransformMatrix, const string& strTexturePath)
{
	string strName;
	XMStoreFloat4x4(&m_TransformMatrix, TransformMatrix);

	for (int iNM = 0; iNM < m_iNumMesh; ++iNM)
	{
		CMesh::MESH_DESC desc;
		vector<_float4x4> vecOffset;
		_uint iNumVtx = 0;
		_uint iNumIdx = 0;
		_uint iMatIdx = 0;
		_uint iNumBones = 0;
		
		_uint iLength = 0;
		
		read_data(in, iLength);
		//in >> iLength;
		_char szBuffer[256];
		in.read(szBuffer, iLength);
		szBuffer[iLength] = '\0';
		desc.m_strName = szBuffer;

		read_data(in, iNumVtx);
		read_data(in, iNumIdx);
		read_data(in, iMatIdx);
		read_data(in, iNumBones);
		//in >> iNumVtx >> iNumIdx;
		//in >> iMatIdx;
		//in >> iNumBones;
		desc.iNumBones = iNumBones;
		desc.iMaterial_Index = iMatIdx;
		vecVerticies.resize(iNumVtx);
		vecIndicies.resize(iNumIdx);
		vecOffset.resize(iNumBones);
		desc.ArrBones = new _uint[iNumBones];
		for (int i = 0; i < iNumVtx; ++i)
		{
			vecVerticies[i].vBlendIndex0.x = 0;
			vecVerticies[i].vBlendIndex0.y = 0;
			vecVerticies[i].vBlendIndex0.z = 0;
			vecVerticies[i].vBlendIndex0.w = 0;
			vecVerticies[i].vBlendWeight0.x = 0.f;
			vecVerticies[i].vBlendWeight0.y = 0.f;
			vecVerticies[i].vBlendWeight0.z = 0.f;
			vecVerticies[i].vBlendWeight0.w = 0.f;

			vecVerticies[i].vBlendIndex1.x = 0;
			vecVerticies[i].vBlendIndex1.y = 0;
			vecVerticies[i].vBlendIndex1.z = 0;
			vecVerticies[i].vBlendIndex1.w = 0;
			vecVerticies[i].vBlendWeight1.x = 0.f;
			vecVerticies[i].vBlendWeight1.y = 0.f;
			vecVerticies[i].vBlendWeight1.z = 0.f;
			vecVerticies[i].vBlendWeight1.w = 0.f;
			/*
			vecVerticies[i].vBlendIndex2.x = 0;
			vecVerticies[i].vBlendIndex2.y = 0;
			vecVerticies[i].vBlendIndex2.z = 0;
			vecVerticies[i].vBlendIndex2.w = 0;
			vecVerticies[i].vBlendWeight2.x = 0.f;
			vecVerticies[i].vBlendWeight2.y = 0.f;
			vecVerticies[i].vBlendWeight2.z = 0.f;
			vecVerticies[i].vBlendWeight2.w = 0.f;

			vecVerticies[i].vBlendIndex3.x = 0;
			vecVerticies[i].vBlendIndex3.y = 0;
			vecVerticies[i].vBlendIndex3.z = 0;
			vecVerticies[i].vBlendIndex3.w = 0;
			vecVerticies[i].vBlendWeight3.x = 0.f;
			vecVerticies[i].vBlendWeight3.y = 0.f;
			vecVerticies[i].vBlendWeight3.z = 0.f;
			vecVerticies[i].vBlendWeight3.w = 0.f;

			vecVerticies[i].vBlendIndex4.x = 0;
			vecVerticies[i].vBlendIndex4.y = 0;
			vecVerticies[i].vBlendIndex4.z = 0;
			vecVerticies[i].vBlendIndex4.w = 0;
			vecVerticies[i].vBlendWeight4.x = 0.f;
			vecVerticies[i].vBlendWeight4.y = 0.f;
			vecVerticies[i].vBlendWeight4.z = 0.f;
			vecVerticies[i].vBlendWeight4.w = 0.f;

			vecVerticies[i].vBlendIndex5.x = 0;
			vecVerticies[i].vBlendIndex5.y = 0;
			vecVerticies[i].vBlendIndex5.z = 0;
			vecVerticies[i].vBlendIndex5.w = 0;
			vecVerticies[i].vBlendWeight5.x = 0.f;
			vecVerticies[i].vBlendWeight5.y = 0.f;
			vecVerticies[i].vBlendWeight5.z = 0.f;
			vecVerticies[i].vBlendWeight5.w = 0.f;*/

		}
		for (int i = 0; i < iNumBones; ++i)
		{
			string strBoneName;
			_uint iNumWeight = 0;
			read_data(in, iLength);
			//in >> iLength;
			in.read(szBuffer, iLength);
			szBuffer[iLength] = '\0';
			strBoneName = szBuffer;
			read_data(in, iNumWeight);
			//in >> iNumWeight;
			_int iVertIdx = 0;
			_float fWeight = 0.f;
			for (int j = 0; j < iNumWeight; ++j)
			{
				read_data(in, iVertIdx);
				read_data(in, fWeight);
				//in >> iVertIdx >> fWeight;
				if (0.f == vecVerticies[iVertIdx].vBlendWeight0.x)
				{
					vecVerticies[iVertIdx].vBlendIndex0.x = i;
					vecVerticies[iVertIdx].vBlendWeight0.x = fWeight;
				}
				else if (0.f == vecVerticies[iVertIdx].vBlendWeight0.y)
				{
					vecVerticies[iVertIdx].vBlendIndex0.y = i;
					vecVerticies[iVertIdx].vBlendWeight0.y = fWeight;
				}
				else if (0.f == vecVerticies[iVertIdx].vBlendWeight0.z)
				{
					vecVerticies[iVertIdx].vBlendIndex0.z = i;
					vecVerticies[iVertIdx].vBlendWeight0.z = fWeight;
				}
				else if (0.f == vecVerticies[iVertIdx].vBlendWeight0.w)
				{
					vecVerticies[iVertIdx].vBlendIndex0.w = i;
					vecVerticies[iVertIdx].vBlendWeight0.w = fWeight;
				}
				else if (0.f == vecVerticies[iVertIdx].vBlendWeight1.x)
				{
					vecVerticies[iVertIdx].vBlendIndex1.x = i;
					vecVerticies[iVertIdx].vBlendWeight1.x = fWeight;
				}
				else if (0.f == vecVerticies[iVertIdx].vBlendWeight1.y)
				{
					vecVerticies[iVertIdx].vBlendIndex1.y = i;
					vecVerticies[iVertIdx].vBlendWeight1.y = fWeight;
				}
				else if (0.f == vecVerticies[iVertIdx].vBlendWeight1.z)
				{
					vecVerticies[iVertIdx].vBlendIndex1.z = i;
					vecVerticies[iVertIdx].vBlendWeight1.z = fWeight;
				}
				else if (0.f == vecVerticies[iVertIdx].vBlendWeight1.w)
				{
					vecVerticies[iVertIdx].vBlendIndex1.w = i;
					vecVerticies[iVertIdx].vBlendWeight1.w = fWeight;
				}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight2.x)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex2.x = i;
				//	vecVerticies[iVertIdx].vBlendWeight2.x = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight2.y)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex2.y = i;
				//	vecVerticies[iVertIdx].vBlendWeight2.y = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight2.z)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex2.z = i;
				//	vecVerticies[iVertIdx].vBlendWeight2.z = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight2.w)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex2.w = i;
				//	vecVerticies[iVertIdx].vBlendWeight2.w = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight3.x)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex3.x = i;
				//	vecVerticies[iVertIdx].vBlendWeight3.x = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight3.y)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex3.y = i;
				//	vecVerticies[iVertIdx].vBlendWeight3.y = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight3.z)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex3.z = i;
				//	vecVerticies[iVertIdx].vBlendWeight3.z = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight3.w)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex3.w = i;
				//	vecVerticies[iVertIdx].vBlendWeight3.w = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight4.x)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex4.x = i;
				//	vecVerticies[iVertIdx].vBlendWeight4.x = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight4.y)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex4.y = i;
				//	vecVerticies[iVertIdx].vBlendWeight4.y = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight4.z)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex4.z = i;
				//	vecVerticies[iVertIdx].vBlendWeight4.z = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight4.w)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex4.w = i;
				//	vecVerticies[iVertIdx].vBlendWeight4.w = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight5.x)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex5.x = i;
				//	vecVerticies[iVertIdx].vBlendWeight5.x = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight5.y)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex5.y = i;
				//	vecVerticies[iVertIdx].vBlendWeight5.y = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight5.z)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex5.z = i;
				//	vecVerticies[iVertIdx].vBlendWeight5.z = fWeight;
				//}
				//else if (0.f == vecVerticies[iVertIdx].vBlendWeight5.w)
				//{
				//	vecVerticies[iVertIdx].vBlendIndex5.w = i;
				//	vecVerticies[iVertIdx].vBlendWeight5.w = fWeight;
				//}
				else
				{
					//MSG_BOX(L"it's over 24000!!!!!!!!!!!!1, Bones");
				}
			}
			read_data(in, vecOffset[i]);
			//in >> vecOffset[i]._11 >> vecOffset[i]._12 >> vecOffset[i]._13 >> vecOffset[i]._14
			//	>> vecOffset[i]._21 >> vecOffset[i]._22 >> vecOffset[i]._23 >> vecOffset[i]._24
			//	>> vecOffset[i]._31 >> vecOffset[i]._32 >> vecOffset[i]._33 >> vecOffset[i]._34
			//	>> vecOffset[i]._41 >> vecOffset[i]._42 >> vecOffset[i]._43 >> vecOffset[i]._44;

			auto iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)
				{
					return pBone->Get_Name() == strBoneName;
				});
			desc.ArrBones[i] = iter - m_Bones.begin();
		}

		for (int i = 0; i < iNumVtx; ++i)
		{
			read_data(in, vecVerticies[i].vPosition);
			//in >> vecVerticies[i].vPosition.x >> vecVerticies[i].vPosition.y >> vecVerticies[i].vPosition.z;
			read_data(in, vecVerticies[i].vNormal);
			//in >> vecVerticies[i].vNormal.x >> vecVerticies[i].vNormal.y >> vecVerticies[i].vNormal.z;
			read_data(in, vecVerticies[i].vTexcoord);
			//in >> vecVerticies[i].vTexcoord.x >> vecVerticies[i].vTexcoord.y;
			read_data(in, vecVerticies[i].vTangent);
			in >> vecVerticies[i].vTangent.x >> vecVerticies[i].vTangent.y >> vecVerticies[i].vTangent.z;

			//XMStoreFloat3(&vecVerticies[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&vecVerticies[i].vPosition), TransformMatrix));
			//XMStoreFloat3(&vecVerticies[i].vNormal, XMVector3TransformCoord(XMLoadFloat3(&vecVerticies[i].vNormal), TransformMatrix));

		}
		for (int i = 0; i < iNumIdx; ++i)
		{
			read_data(in, vecIndicies[i]);
			//in >> vecIndicies[i];
		}
		Ready_Meshes(vecVerticies, vecIndicies, vecOffset, desc);
		delete[] desc.ArrBones;
	}
	_uint iNumMaterial = 0;
	read_data(in, iNumMaterial);
	//in >> iNumMaterial;
	m_vecMaterials.resize(iNumMaterial);
	for (int k = 0; k < iNumMaterial; ++k)
	{
		for (int j = 1; j < 18; ++j)
		{
			_uint iExist = 0;
			read_data(in, iExist);
			//in >> iExist;
			if (iExist == 1)
			{
				_uint iLength = 0;
				_char szBuffer[256];
				read_data(in, iLength);
				//in >> iLength;
				in.read(szBuffer, iLength);
				szBuffer[iLength] = '\0';
				string strFileName{ szBuffer };

				string strFullPath = strTexturePath + strFileName;
				wstring lstFullPath(strFullPath.begin(), strFullPath.end());
				auto iter_mat = m_mapMaterial.find(lstFullPath);
				if (iter_mat == m_mapMaterial.end())
				{
					m_mapMaterial.emplace(lstFullPath, CTexture::Create(m_pDevice, m_pContext, lstFullPath, 1));
					iter_mat = m_mapMaterial.find(lstFullPath);
				}
				m_vecMaterials[k].push_back(iter_mat->second);
				Safe_AddRef(iter_mat->second);
			}
			else
			{
				m_vecMaterials[k].push_back(nullptr);
			}
		}
	}
	read_data(in, m_iNumAnimations);
	//in >>  m_iNumAnimations;
	for (int k = 0; k < m_iNumAnimations; ++k)
	{
		m_Animations.push_back(CAnimation::Create(in, m_Bones, true));
	}

	Hash_Animation();
	Hash_Bone();


	in.close();
	return S_OK;
}

HRESULT CModel::Ready_Bones(ifstream& in)
{
	string strTemp, strName, strParent;
	_int iNumBones = 0;
	_float4x4 Transform;
	in >> strTemp >> iNumBones;

	m_iNumBones = iNumBones;
	map<string, _uint> mapBoneIdx;
	for(_uint i = 0;i<m_iNumBones;++i)
	{
		in >> strTemp >> strName;


		in >> strTemp >> strParent;

		in >> strTemp;
		in >> Transform._11 >> Transform._12 >> Transform._13 >> Transform._14;
		in >> Transform._21 >> Transform._22 >> Transform._23 >> Transform._24;
		in >> Transform._31 >> Transform._32 >> Transform._33 >> Transform._34;
		in >> Transform._41 >> Transform._42 >> Transform._43 >> Transform._44;

		CBone::BONE_DESC desc;
		desc.strName = strName;
		desc.strParentName = strParent;
		desc.Transformation = Transform;
		m_Bones.push_back(CBone::Create(desc));
		mapBoneIdx[strName] = i;
	}
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		if ("NOTEXIST" == m_Bones[i]->m_strParentName)
		{
			m_Bones[i]->m_bRootNode = true;
			m_Bones[i]->m_iParentIdx = -1;
			continue;
		}
		m_Bones[i]->m_iParentIdx = mapBoneIdx[m_Bones[i]->m_strParentName];
		m_Bones[m_Bones[i]->m_iParentIdx]->Add_Child(i);
	}
	

	return S_OK;
}

HRESULT CModel::Ready_Bones_Bin(ifstream& in)
{
	string strTemp, strName, strParent;
	_uint iNumBones = 0;
	_float4x4 Transform;
	read_data(in, iNumBones);
	//in >> iNumBones;
	m_iNumBones = iNumBones;
	map<string, _uint> mapBoneIdx;
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		_uint iLength;
		_char szBuffer[256];
		read_data(in, iLength);
		//in >> iLength;
		in.read(szBuffer, iLength);
		szBuffer[iLength] = '\0';
		strName = szBuffer;

		read_data(in, iLength);
		//in >> iLength;
		in.read(szBuffer, iLength);
		szBuffer[iLength] = '\0';
		strParent = szBuffer;

		read_data(in, Transform);
		//in >> Transform._11 >> Transform._12 >> Transform._13 >> Transform._14;
		//in >> Transform._21 >> Transform._22 >> Transform._23 >> Transform._24;
		//in >> Transform._31 >> Transform._32 >> Transform._33 >> Transform._34;
		//in >> Transform._41 >> Transform._42 >> Transform._43 >> Transform._44;

		CBone::BONE_DESC desc;
		desc.strName = strName;
		desc.strParentName = strParent;
		desc.Transformation = Transform;
		m_Bones.push_back(CBone::Create(desc));
		mapBoneIdx[strName] = i;
	}
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		if ("NOTEXIST" == m_Bones[i]->m_strParentName)
		{
			m_Bones[i]->m_bRootNode = true;
			m_Bones[i]->m_iParentIdx = -1;
			continue;
		}
		m_Bones[i]->m_iParentIdx = mapBoneIdx[m_Bones[i]->m_strParentName];
		m_Bones[m_Bones[i]->m_iParentIdx]->Add_Child(i);
	}


	return S_OK;
}

const vector<class CTexture*>& CModel::Get_Material(_int iMeshIndex)
{
	return m_vecMaterials[m_Meshes[iMeshIndex]->Get_Material_Index()];
}

HRESULT CModel::Make_Shape(const _wstring& strTag, _float fScale)
{
	auto pGameInstance = CGameInstance::Get_Instance();
	for (int i = 0; i < m_iNumMesh; ++i)
	{
		HRESULT rt;
		if (fScale == 1.f)
			rt = pGameInstance->Add_Shape(strTag + to_wstring(i), m_Meshes[i]->m_vecVertexPositions, m_Meshes[i]->m_vecIndicies);
		else
		{
			vector<_float3> vecVert;
			vecVert.resize(m_Meshes[i]->m_vecVertexPositions.size());

			for (_uint j = 0; j < vecVert.size(); ++j)
			{
				XMStoreFloat3(&vecVert[j], XMLoadFloat3(&m_Meshes[i]->m_vecVertexPositions[j]) * fScale);
			}

			rt = pGameInstance->Add_Shape(strTag + to_wstring(i), vecVert, m_Meshes[i]->m_vecIndicies);
		}
		if (FAILED(rt))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CModel::Make_PxRigidStatic(const _wstring& strTag, _fmatrix MatTransform, _float fScale)
{
	if (m_pRigidStatic)
	{
		return E_FAIL;
	}
	m_strShapeTag = strTag;

	auto pGameInstance = CGameInstance::Get_Instance();
	auto pShape = pGameInstance->Get_Shape(strTag + to_wstring(0));
	_float4x4  Mat;
	XMStoreFloat4x4(&Mat, MatTransform);
	PxTransform pxTransform(PxVec3{ Mat._41 ,Mat._42,Mat._43});
	//TODO: È¸Àü
	if (pShape)
	{
		pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		m_pRigidStatic = PxCreateStatic(*pGameInstance->Get_Physics(), pxTransform, *pShape);

		for (int i = 1; i < m_iNumMesh; ++i)
		{
			pShape = pGameInstance->Get_Shape(strTag + to_wstring(i));
			if (!pShape)
			{
				return E_FAIL;
			}
			m_pRigidStatic->attachShape(*pShape);
		}
		pGameInstance->Get_PhysxManager()->Add_Actor(m_pRigidStatic);
		return S_OK;
	}
	for (int i = 0; i < m_iNumMesh; ++i)
	{
		Make_Shape(strTag, fScale);
		/*HRESULT rt;
		if(fScale == 1.f)
			rt = pGameInstance->Add_Shape(strTag + to_wstring(i), m_Meshes[i]->m_vecVertexPositions, m_Meshes[i]->m_vecIndicies);
		else
		{
			vector<_float3> vecVert;
			vecVert.resize(m_Meshes[i]->m_vecVertexPositions.size());
			
			for (_uint j = 0;j< vecVert.size();++j)
			{
				XMStoreFloat3(&vecVert[j], XMLoadFloat3(&m_Meshes[i]->m_vecVertexPositions[j]) * fScale);
			}

			rt = pGameInstance->Add_Shape(strTag + to_wstring(i), vecVert, m_Meshes[i]->m_vecIndicies);
		}
		if (FAILED(rt))
		{
			return E_FAIL;
		}*/
		pShape = pGameInstance->Get_Shape(strTag + to_wstring(i));
		if (!pShape)
			return E_FAIL;
		pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		if (i == 0)
		{
			m_pRigidStatic = PxCreateStatic(*pGameInstance->Get_Physics(), pxTransform, *pShape);
		}
		else
		{
			m_pRigidStatic->attachShape(*pShape);
		}
		pGameInstance->Get_PhysxManager()->Add_Actor(m_pRigidStatic);
	}

	return S_OK;
}

void CModel::Remove_Actor()
{
	if (m_pRigidStatic)
	{
		CGameInstance::Get_Instance()->Get_PhysxManager()->Remove_Actor(m_pRigidStatic);
		m_pRigidStatic->release();
		m_pRigidStatic = nullptr;
	}
}

void CModel::Add_Actor(_fmatrix Transform)
{
	if (m_pRigidStatic)
	{
		CGameInstance::Get_Instance()->Get_PhysxManager()->Add_Actor(m_pRigidStatic, Transform);
	}
}

void CModel::Disable_Simulation(bool flag)
{
	if (m_pRigidStatic)
	{
		m_pRigidStatic->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, flag);
	}
}

void CModel::Play_Animation(_float fTimeDelta, _float fFrom, _float fTo, _bool bLerp)
{
	if (fFrom != 0.f)
	{
		if (m_Animations[m_iCurrentAnimIndex]->Get_CurrentTrackPositionRatio() < fFrom)
		{
			m_Animations[m_iCurrentAnimIndex]->Set_TrackPositionRatio(fFrom);
		}
	}

	if(m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrix(fTimeDelta, m_Bones, m_isLoop) && m_isLoop)
	{
		m_iPrevAnimIndex = m_iCurrentAnimIndex;
		m_fPrevTrackPosition = m_Animations[m_iCurrentAnimIndex]->Get_Duration();
	}
	_float fTPR = m_Animations[m_iCurrentAnimIndex]->Get_CurrentTrackPositionRatio();
	if (bLerp && fTPR < 0.05f && m_iCurrentAnimIndex != m_iPrevAnimIndex)
	{
		//Lerp
		m_Animations[m_iPrevAnimIndex]->Lerp_TransformationMatrix(m_Bones, fTPR * 20.f, m_fPrevTrackPosition);
	}
	if (bLerp && fTPR < 0.05f && m_isLoop && m_iCurrentAnimIndex == m_iPrevAnimIndex)
	{
		m_Animations[m_iPrevAnimIndex]->Lerp_TransformationMatrix(m_Bones, fTPR * 20.f, m_fPrevTrackPosition);
	}
	

	if (fTo != 1.f)
	{
		if (fTPR > fTo)
		{
			m_Animations[m_iCurrentAnimIndex]->Set_TrackPositionRatio(fTo);
		}
	}


	for (auto& pBone : m_Bones)
	{
		if(!pBone->m_bRootNode)
			pBone->Update(XMLoadFloat4x4(&m_Bones[pBone->m_iParentIdx]->m_CombinedTransformationMatrix));
		else
		{
			XMStoreFloat4x4(&pBone->m_CombinedTransformationMatrix, XMLoadFloat4x4(&pBone->m_TransformationMatrix) * XMLoadFloat4x4(&m_TransformMatrix));
		}
	}
}

void CModel::Play_Animation_Departed(_float fTimeDelta, _float fFrom, _float fTo, _bool bLerp)
{
	if (fFrom != 0.f)
	{
		if (m_Animations[m_iCurrentAnimIndex]->Get_CurrentTrackPositionRatio() < fFrom)
		{
			m_Animations[m_iCurrentAnimIndex]->Set_TrackPositionRatio(fFrom);
		}
	}

	if (m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrix(fTimeDelta, m_Bones, m_isLoop) && m_isLoop)
	{
		m_iPrevAnimIndex = m_iCurrentAnimIndex;
		m_fPrevTrackPosition = m_Animations[m_iCurrentAnimIndex]->Get_Duration();
	}
	_float fTPR = m_Animations[m_iCurrentAnimIndex]->Get_CurrentTrackPositionRatio();
	if (bLerp && fTPR < 0.05f && m_iCurrentAnimIndex != m_iPrevAnimIndex)
	{
		//Lerp
		m_Animations[m_iPrevAnimIndex]->Lerp_TransformationMatrix(m_Bones, fTPR * 20.f, m_fPrevTrackPosition);
	}
	if (bLerp && fTPR < 0.05f && m_isLoop && m_iCurrentAnimIndex == m_iPrevAnimIndex)
	{
		m_Animations[m_iPrevAnimIndex]->Lerp_TransformationMatrix(m_Bones, fTPR * 20.f, m_fPrevTrackPosition);
	}

	if (fTo != 1.f)
	{
		if (fTPR > fTo)
		{
			m_Animations[m_iCurrentAnimIndex_Departed]->Set_TrackPositionRatio(fTo);
		}
	}

	if (fFrom != 0.f)
	{
		if (m_Animations[m_iCurrentAnimIndex_Departed]->Get_CurrentTrackPositionRatio() < fFrom)
		{
			m_Animations[m_iCurrentAnimIndex_Departed]->Set_TrackPositionRatio(fFrom);
		}
	}

	if (m_Animations[m_iCurrentAnimIndex_Departed]->Update_TransformationMatrix_Departed(fTimeDelta, m_Bones,m_vecDepartedBoneIdx, m_isLoop_Departed) && m_isLoop_Departed)
	{
		m_iPrevAnimIndex_Departed = m_iCurrentAnimIndex_Departed;
		m_fPrevTrackPosition_Departed = m_Animations[m_iCurrentAnimIndex_Departed]->Get_Duration();
	}
	fTPR = m_Animations[m_iCurrentAnimIndex_Departed]->Get_CurrentTrackPositionRatio();
	if (bLerp && fTPR < 0.05f && m_iCurrentAnimIndex_Departed != m_iPrevAnimIndex_Departed)
	{
		//Lerp
		m_Animations[m_iPrevAnimIndex_Departed]->Lerp_TransformationMatrix_Departed(m_Bones, fTPR * 20.f, m_vecDepartedBoneIdx, m_fPrevTrackPosition_Departed);
	}
	if (bLerp && fTPR < 0.05f && m_isLoop_Departed && m_iCurrentAnimIndex_Departed == m_iPrevAnimIndex_Departed)
	{
		m_Animations[m_iPrevAnimIndex_Departed]->Lerp_TransformationMatrix_Departed(m_Bones, fTPR * 20.f, m_vecDepartedBoneIdx,  m_fPrevTrackPosition_Departed);
	}

	if (fTo != 1.f)
	{
		if (fTPR > fTo)
		{
			m_Animations[m_iCurrentAnimIndex_Departed]->Set_TrackPositionRatio(fTo);
		}
	}



	for (auto& pBone : m_Bones)
	{
		if (!pBone->m_bRootNode)
			pBone->Update(XMLoadFloat4x4(&m_Bones[pBone->m_iParentIdx]->m_CombinedTransformationMatrix));
		else
		{
			XMStoreFloat4x4(&pBone->m_CombinedTransformationMatrix, XMLoadFloat4x4(&pBone->m_TransformationMatrix) * XMLoadFloat4x4(&m_TransformMatrix));
		}
	}
}

void CModel::Blend_Animation(_float fTimeDelta, string strTag, _float fRatio)
{
	Blend_Animation(fTimeDelta, m_AnimTagIndex[strTag], fRatio);
}

void CModel::Blend_Animation(_float fTimeDelta, _uint iIdx, _float fRatio)
{
	m_Animations[m_iCurrentAnimIndex]->Blend_TransformationMatrix(fTimeDelta, m_Bones, (1.f - fRatio));
}

void CModel::Reset_Animation(string strTag)
{
	m_Animations[m_AnimTagIndex[strTag]]->Reset();
}

void CModel::Reset_Animation(_uint iIdx)
{
	m_Animations[iIdx]->Reset();
}

void CModel::Bind_BoneMatrices(CShader* pShader, const _char* pConstantName, _uint iMeshIndex)
{
	vector<_float4x4> BoneMatrices;
	
	BoneMatrices.reserve(10);

	_uint iModelBoneIdx = 0;
	for (auto& idx : m_Meshes[iMeshIndex]->m_Bones)
	{

		_float4x4 Matrix = m_Meshes[iMeshIndex]->m_BoneOffset[iModelBoneIdx];
		_matrix Combined = XMLoadFloat4x4(&Matrix);
		XMStoreFloat4x4(&Matrix, Combined * XMLoadFloat4x4(&m_Bones[idx]->m_CombinedTransformationMatrix));
		BoneMatrices.push_back(Matrix);
		++iModelBoneIdx;
	}
	pShader->Bind_Matrices(pConstantName, BoneMatrices.data(), BoneMatrices.size());


}

void CModel::Get_BoneMatrices(vector<_float4x4>& vecOut, _uint iMeshIndex)
{
	_uint iModelBoneIdx = 0;
	for (auto& idx : m_Meshes[iMeshIndex]->m_Bones)
	{

		_float4x4 Matrix = m_Meshes[iMeshIndex]->m_BoneOffset[iModelBoneIdx];
		_matrix Combined = XMLoadFloat4x4(&Matrix);
		XMStoreFloat4x4(&Matrix, Combined * XMLoadFloat4x4(&m_Bones[idx]->m_CombinedTransformationMatrix));
		vecOut.push_back(Matrix);
		++iModelBoneIdx;
	}
}

void CModel::Inverse_Bones()
{
	Inverse_BoneOffsets();
	Inverse_BoneTransform();
}

void CModel::Inverse_BoneOffsets()
{
	for (auto& pMesh : m_Meshes)
	{
		for (auto& matrix : pMesh->m_BoneOffset)
		{
			XMStoreFloat4x4(&matrix, XMMatrixInverse(nullptr, XMLoadFloat4x4(&matrix)));
		}
	}
}

void CModel::Inverse_BoneTransform()
{
	for (auto& pBone : m_Bones)
	{
		auto& matrix = pBone->m_TransformationMatrix;
		XMStoreFloat4x4(&matrix, XMMatrixInverse(nullptr, XMLoadFloat4x4(&matrix)));
	}
}

void CModel::Make_Mesh_Material_Group()
{
	vector<_bool> vecChecked;
	vecChecked.resize(m_vecMaterials.size());
	for (_uint i = 0; i < m_vecMaterials.size(); ++i)
	{
		if (vecChecked[i])
			continue;
		vecChecked[i] = true;
		m_vecMaterialGroup.push_back(vector<_uint>());
		m_vecMaterialGroup.back().push_back(i);
		for (_uint j = 0; j < m_vecMaterials.size(); ++j)
		{
			if (vecChecked[j])
				continue;
			_bool bSame = true;
			if (m_vecMaterials[i].size() != m_vecMaterials[j].size())
				continue;
			for (_uint k = 0; k < m_vecMaterials[i].size(); ++k)
			{
				if (m_vecMaterials[i][k] != m_vecMaterials[j][k])
				{
					bSame = false;
					break;
				}
			}
			if (bSame)
			{
				vecChecked[j] = true;
				m_vecMaterialGroup.back().push_back(j);
			}
		}
	}
	_uint iGroupSize = m_vecMaterialGroup.size();
	m_MeshGroup.resize(iGroupSize);

	for (auto& pMesh : m_Meshes)
	{
		_uint iMatGroup = 0;
		for (auto& MatGroup : m_vecMaterialGroup)
		{
			auto iter = find(MatGroup.begin(), MatGroup.end(), pMesh->Get_Material_Index());
			if (iter != MatGroup.end())
			{
				break;
			}
			++iMatGroup;
		}

		m_MeshGroup[iMatGroup].push_back(pMesh);
	}
}

const vector<_uint>& CModel::Get_MaterialGroup(_uint iGroupIdx)
{
	return m_vecMaterialGroup[iGroupIdx];
}

const vector<CMesh*>& CModel::Get_MeshGroup(_uint iGroupIdx)
{
	return m_MeshGroup[iGroupIdx];
}

_uint CModel::Get_GroupSize()
{
	return m_MeshGroup.size();
}

void CModel::Get_BoneChildren(string strTag, vector<_int>& vecOut)
{
	vecOut = m_Bones[m_BoneTagIndex[strTag]]->Get_Children();
}

void CModel::Add_DepartedBoneTag(string strTag)
{
	m_vecDepartedBoneIdx.push_back(m_BoneTagIndex[strTag]);
}

void CModel::Add_DepartedBoneTag_Child(string strTag)
{
	_uint iIdx = m_BoneTagIndex[strTag];
	m_vecDepartedBoneIdx.push_back(iIdx);
	for (_int iChildIdx : m_Bones[iIdx]->Get_Children())
	{
		Add_DepartedBoneIdx_Child(iChildIdx);
	}
}

void CModel::Add_DepartedBoneIdx_Child(_uint iIdx)
{
	m_vecDepartedBoneIdx.push_back(iIdx);
	for (_int iChildIdx : m_Bones[iIdx]->Get_Children())
	{
		Add_DepartedBoneIdx_Child(iChildIdx);
	}
}



void CModel::Hash_Animation()
{
	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		string name = m_Animations[i]->Get_Name();
		m_AnimTagIndex.emplace(name, i);
	}
}

void CModel::Hash_Bone()
{
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		string name = m_Bones[i]->Get_Name();
		m_BoneTagIndex.emplace(name, i);
	}
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelPath,_fmatrix TransformMatrix, const string& strTextureDirectory)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pModelPath, TransformMatrix, strTextureDirectory)))
	{
		MSG_BOX(TEXT("Failed to Created : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();
	for (auto& pBones : m_Bones)
	{
		Safe_Release(pBones);
	}
	m_Bones.clear();

	for (auto& pMesh : m_Meshes)
	{
		Safe_Release(pMesh);
	}
	for (auto& vecTextures : m_vecMaterials)
	{
		for (auto& pTexture : vecTextures)
		{
			Safe_Release(pTexture);
		}
	}
	for (auto& material : m_mapMaterial)
	{
		Safe_Release(material.second);
	}
	m_Meshes.clear();
	if(m_pRigidStatic)
	{
		CGameInstance::Get_Instance()->Get_PhysxManager()->Remove_Actor(m_pRigidStatic);
		m_pRigidStatic->release();
	}
	for (auto& pBone : m_Bones)
	{
		Safe_Release(pBone);
	}
	m_Bones.clear();
	for (auto& pAnim : m_Animations)
	{
		Safe_Release(pAnim);
	}
	m_Animations.clear();
}

_fmatrix CModel::Get_BoneMatrix(string strTag)
{
	return XMLoadFloat4x4(&m_Bones[m_BoneTagIndex[strTag]]->m_CombinedTransformationMatrix);
}

_fmatrix CModel::Get_BoneMatrix(_uint iIndex)
{
	return XMLoadFloat4x4(&m_Bones[iIndex]->m_CombinedTransformationMatrix);
}

_float4x4* CModel::Get_BoneMatrixPtr(string strTag)
{
	return &m_Bones[m_BoneTagIndex[strTag]]->m_CombinedTransformationMatrix;
}

_float4x4* CModel::Get_BoneMatrixPtr(_uint iIndex)
{
	return &m_Bones[iIndex]->m_CombinedTransformationMatrix;
}

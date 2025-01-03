#include "Room.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Model.h"
#include "Portal.h"
#include "Monster_Base.h"
#include "Player_Manager.h"
#include "Player.h"
#include "RoomManager.h"
#include "Beat_Manager.h"
#include "UI_RoomClear.h"

CRoom::CRoom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)
{
}

CRoom::CRoom(const CRoom& rhs) :
	CGameObject(rhs),
	m_strModelTag(rhs.m_strModelTag),
	m_eLevel(rhs.m_eLevel),m_eRoomType(rhs.m_eRoomType),m_vecMonsterTags(rhs.m_vecMonsterTags), m_vecObjectTags(rhs.m_vecObjectTags), m_vecPortalTags(rhs.m_vecPortalTags)
{
}

HRESULT CRoom::Initialize_Prototype(ROOM_DESC& desc)
{
	__super::Initialize_Prototype();
	
	m_eRoomType = desc.eRoomType;
	m_eLevel = desc.eLevel;
	return Read_Room(desc.strRoomFilePath);
}

HRESULT CRoom::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	if (pArg)
	{
		CLONEING_DESC* pDesc = (CLONEING_DESC*)pArg;
		for(_int i = 0;i<4;++i)
			m_pRoomConnected[i] = pDesc->pRoomConnected[i];
	}
	for (auto& pair : m_vecObjectTags)
	{
		m_pGameInstance->Pool_Object(pair.first);
		m_vecObjects.push_back(m_pGameInstance->Push_Object_Hijack(pair.first, nullptr, &pair.second));
		Safe_AddRef(m_vecObjects.back());
		m_vecObjects.back()->Dying();
	}
	for (auto& pair : m_vecMonsterTags)
	{
		m_pGameInstance->Pool_Object(pair.first);
		m_vecMonsters.push_back((CMonster_Base*)m_pGameInstance->Push_Object_Hijack(pair.first, nullptr, &pair.second));
		Safe_AddRef(m_vecMonsters.back());
		m_vecMonsters.back()->Dying();
		CRoomManager::Get_Instance()->Register_PooledObjectTag(pair.first);
	}
	CPortal::PORTAL_DESC desc;
	for (auto& pair : m_vecPortalTags)
	{
		desc.strModelTag = pair.first;
		desc.Transform = pair.second;
		m_pGameInstance->Pool_Object(L"Prototype_GameObject_Portal", &desc);
		m_vecPortals.push_back((CPortal*)m_pGameInstance->Push_Object_Hijack(L"Prototype_GameObject_Portal"));
		Safe_AddRef(m_vecPortals.back());
		m_vecPortals.back()->Dying();
	}
	_int idx = 0;
	for (_int i = 0; i < 4; ++i)
	{
		if (m_pRoomConnected[i] != -1)
		{
			m_PortalTransform[i] = m_vecPortalTags[idx].second;
			++idx;
		}
	}
	Ready_Components();
	m_pModelCom->Make_Shape(m_strModelTag);

	return S_OK;
}

void CRoom::PriorityTick(_float fTimeDelta)
{
	for (auto& pObj : m_vecObjects)
	{
		pObj->PriorityTick(fTimeDelta);
	}
	_bool bCleared = true;
	for (auto& pObj : m_vecMonsters)
	{
		pObj->PriorityTick(fTimeDelta);
		if (!pObj->Is_Killed())
		{
			bCleared = false;
		}
	}
	if(bCleared && !m_bCleared)
	{
		m_bCleared = true;
		if(m_eRoomType == ROOM_DEFAULT || m_eRoomType == ROOM_BOSS)
			Clear();
	}
	for (auto& pObj : m_vecPortals)
	{
		pObj->PriorityTick(fTimeDelta);
	}
}

void CRoom::Tick(_float fTimeDelta)
{
	for (_uint i = 0; i < m_vecObjects.size(); ++i)
	{
		m_vecObjects[i]->Tick(fTimeDelta);
	}

	for (auto& pObj : m_vecMonsters)
	{
		pObj->Tick(fTimeDelta);
	}
	for (auto& pObj : m_vecPortals)
	{
		pObj->Tick(fTimeDelta);
	}
	if(!Is_Dead() && m_bCleared)
	{
		auto pPM = CPlayer_Manager::Get_Instance();
		auto pPlayer = pPM->Get_Player();
		if (pPlayer)
		{
			auto PlayerPos = XMLoadFloat3(&pPlayer->Get_FootPosition());
			for (_int idx = 0; idx < 4; ++idx)
			{
				if (m_pRoomConnected[idx] != -1)
				{
					_matrix  PortalTransform = XMLoadFloat4x4(&m_PortalTransform[idx]);
					m_fPortalDistance[idx] = XMVectorGetX(XMVector3Length(PortalTransform.r[3] - PlayerPos));
					PortalTransform.r[3] -= XMVector3Normalize(PortalTransform.r[2]) * 3.5f;
					if (3.0f >= XMVectorGetX(XMVector3Length(PlayerPos - PortalTransform.r[3])))
					{
						Dead();
						CRoomManager::Get_Instance()->Get_Room(m_pRoomConnected[idx])->Enter_Room(CRoomManager::Get_Instance()->Get_CurrentRoomIdx());
						CRoomManager::Get_Instance()->Set_CurrentRoomIdx(m_pRoomConnected[idx]);
						break;
					}
				}
			}
		}
	}

}

void CRoom::LateTick(_float fTimeDelta)
{
	{
		auto iter = m_vecObjects.begin();
		for (iter; iter != m_vecObjects.end();)
		{
			if ((*iter)->Is_Dead())
			{
				(*iter)->Dying();
				Safe_Release(*iter);
				iter = m_vecObjects.erase(iter);
			}
			else
			{
				(*iter)->LateTick(fTimeDelta);
				++iter;
			}
		}
	}
	{
		auto iter = m_vecMonsters.begin();
		for (iter; iter != m_vecMonsters.end();)
		{
			if ((*iter)->Is_Dead())
			{
				(*iter)->Dying();
				Safe_Release(*iter);
				iter = m_vecMonsters.erase(iter);
			}
			else
			{
				(*iter)->LateTick(fTimeDelta);
				++iter;
			}
		}
	}
	{
		auto iter = m_vecPortals.begin();
		for (iter; iter != m_vecPortals.end();)
		{
			if ((*iter)->Is_Dead())
			{
				(*iter)->Dying();
				Safe_Release(*iter);
				iter = m_vecPortals.erase(iter);
			}
			else
			{
				(*iter)->LateTick(fTimeDelta);
				++iter;
			}
		}
	}
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CRoom::Render()
{
	//Bind_ShaderResources();
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	_matrix World = m_pTransformCom->Get_WorldMatrix_XMMat();
	m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));

	for (_int i = 0; i < m_pModelCom->Get_GroupSize(); ++i)
	{
		auto& vecMeshes = m_pModelCom->Get_MeshGroup(i);
		if (vecMeshes.size() == 0)
			continue;
		auto vecMat = m_pModelCom->Get_Material(vecMeshes.front()->Get_Material_Index());
		if (vecMat.size())
		{
			auto pTexture = vecMat[0];
			if (pTexture)
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");
			pTexture = vecMat[5];
			if (pTexture)
			{
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
			}
			else
			{
				DEFAULTNORMAL->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
			}
			pTexture = vecMat[14];
			if (pTexture)
			{
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
			}
			else
			{
				DEFAULTORM->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
			}
		}
		m_pShaderCom->Begin(0);
		for (_int j = 0; j < vecMeshes.size(); ++j)
		{
			_vector vMin = XMVectorSetW(XMLoadFloat3(&vecMeshes[j]->Get_Min()), 1.f);
			_vector vMax = XMVectorSetW(XMLoadFloat3(&vecMeshes[j]->Get_Max()), 1.f);
			vMin = XMVector3TransformCoord(vMin, World);
			vMax = XMVector3TransformCoord(vMax, World);
			_vector vRadius = (vMax - vMin) * 0.5f;
			_vector vCenter = vMin + vRadius;
			_float fRadius = XMVectorGetX(XMVector3Length(vRadius));
			if (!m_pGameInstance->isIn_Frustum_WorldSpace(vCenter, fRadius))
				continue;

			vecMeshes[j]->Render();
		}
	}


	//for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	//{
	//	_vector vMin = XMVectorSetW(XMLoadFloat3(&m_pModelCom->Get_Min(i)), 1.f);
	//	_vector vMax = XMVectorSetW(XMLoadFloat3(&m_pModelCom->Get_Max(i)), 1.f);
	//	vMin = XMVector3TransformCoord(vMin, World);
	//	vMax = XMVector3TransformCoord(vMax, World);
	//	_vector vRadius = (vMax - vMin) * 0.5f;
	//	_vector vCenter = vMin + vRadius;
	//	_float fRadius = XMVectorGetX(XMVector3Length(vRadius));
	//	if (!m_pGameInstance->isIn_Frustum_WorldSpace(vCenter, fRadius))
	//		continue;

	//	auto vecMat = m_pModelCom->Get_Material(i);
	//	if (vecMat.size())
	//	{
	//		auto pTexture = vecMat[0];
	//		if (pTexture)
	//			pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");
	//		pTexture = vecMat[5];
	//		if (pTexture)
	//		{
	//			pTexture->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
	//		}
	//		else
	//		{
	//			DEFAULTNORMAL->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
	//		}
	//		pTexture = vecMat[14];
	//		if (pTexture)
	//		{
	//			pTexture->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
	//		}
	//		else
	//		{
	//			DEFAULTORM->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
	//		}
	//	}


	//	m_pShaderCom->Begin(0);
	//	m_pModelCom->Render(i);
	//}
	return S_OK;
}

void CRoom::Dying()
{
	if (m_pModelCom)
	{
		m_pModelCom->Remove_Actor();
	}
	for (auto& pObj : m_vecObjects)
	{
		pObj->Dying();
	}
	for (auto& pObj : m_vecMonsters)
	{
		pObj->Dying();
	}
	for (auto& pObj : m_vecPortals)
	{
		pObj->Dying();
	}
}

void CRoom::Push_From_Pool(void* pArg)
{
	_float4x4 Transform = m_pTransformCom->Get_WorldMatrix();
	_matrix Mat = XMLoadFloat4x4(&Transform);
	if (FAILED(m_pModelCom->Make_PxRigidStatic(m_strModelTag, Mat)))
	{
		m_pModelCom->Add_Actor();
	}
	for (auto& pObj : m_vecObjects)
	{
		pObj->Push_From_Pool(nullptr);
	}
	_int idx = 0;
	for (auto& pObj : m_vecMonsters)
	{
		CMonster_Base::MONSTER_DESC desc;
		desc.Transformation = m_vecMonsterTags[idx].second;
		++idx;
		pObj->Push_From_Pool(&desc);
	}
	for (auto& pObj : m_vecPortals)
	{
		pObj->Push_From_Pool(nullptr);
	}


}

void CRoom::Enter_Room(_int iPrevRoomIdx, _int iCurrentLevel)
{
	Set_Dead(false);
	if (iPrevRoomIdx == -1 && iCurrentLevel == LEVEL_BOSSRUSH)
	{
		auto pPM = CPlayer_Manager::Get_Instance();
		auto pPlayer = pPM->Get_Player();
		if (pPlayer)
		{
			_matrix  PortalTransform = XMLoadFloat4x4(&m_PortalTransform[0]);
			//PortalTransform.r[3] += XMVector3Normalize(PortalTransform.r[2]) * 1.f;
			pPlayer->Set_FootPosition(PortalTransform);
		}
	}
	else
	{
		_int idx = -1;
		for (_int i = 0; i < 4; ++i)
		{
			if (iPrevRoomIdx == m_pRoomConnected[i])
			{
				idx = i;
				break;
			}
		}
		if (idx != -1)
		{
			auto pPM = CPlayer_Manager::Get_Instance();
			auto pPlayer = pPM->Get_Player();
			if (pPlayer)
			{
				_matrix  PortalTransform = XMLoadFloat4x4(&m_PortalTransform[idx]);
				//PortalTransform.r[3] += XMVector3Normalize(PortalTransform.r[2]) * 1.f;
				pPlayer->Set_FootPosition(PortalTransform);
			}
		}
	}
	auto pPM = CPlayer_Manager::Get_Instance();
	auto pPlayer = pPM->Get_Player();
	pPlayer->Cam_Dissolve(0.5f, 0.1f);
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Burn");
	Push_From_Pool(nullptr);
	if(iCurrentLevel != -1)
		m_pGameInstance->Push_Object(iCurrentLevel, L"Room", this);
	else
	{
		m_pGameInstance->Push_Object(m_pGameInstance->Get_Current_LevelID(), L"Room", this);
	}

	if (m_eRoomType == ROOM_BOSS)
	{
		_uint iID = m_pGameInstance->Get_Current_LevelID();
		if (iID == LEVEL_GAMEPLAY)
		{
			CBeat_Manager::Get_Instance()->Queue_Sequence(Asgard_1_Boss);
			_uint iBT = 0;
			m_pGameInstance->Push_Object_From_Pool(TEXT("Prototype_GameObject_UI_BossTitle"), m_pGameInstance->Get_Current_LevelID(), CUIBase::UILayerTag, &iBT);
		}
		if (iID == LEVEL_VANA)
		{
			CBeat_Manager::Get_Instance()->Queue_Sequence(L"Vana_Boss");
			_uint iBT = 5;
			m_pGameInstance->Push_Object_From_Pool(TEXT("Prototype_GameObject_UI_BossTitle"), m_pGameInstance->Get_Current_LevelID(), CUIBase::UILayerTag, &iBT);
		}
		if (iID == LEVEL_HEL)
		{
			CBeat_Manager::Get_Instance()->Queue_Sequence(L"Helheim_Boss");
			_uint iBT = 1;
			m_pGameInstance->Push_Object_From_Pool(TEXT("Prototype_GameObject_UI_BossTitle"), m_pGameInstance->Get_Current_LevelID(), CUIBase::UILayerTag, &iBT);
		}

		if (m_eLevel == LEVEL_BOSSRUSH)
		{
			if (iPrevRoomIdx == -1)
			{
				CBeat_Manager::Get_Instance()->Queue_Sequence(L"Alvis");
				_uint iBT = 2;
				m_pGameInstance->Push_Object_From_Pool(TEXT("Prototype_GameObject_UI_BossTitle"), m_eLevel, CUIBase::UILayerTag, &iBT);
			}
			if (iPrevRoomIdx == 0)
			{
				CBeat_Manager::Get_Instance()->Queue_Sequence(L"Surt");
				_uint iBT = 3;
				m_pGameInstance->Push_Object_From_Pool(TEXT("Prototype_GameObject_UI_BossTitle"), m_eLevel, CUIBase::UILayerTag, &iBT);
			}
			if (iPrevRoomIdx == 1)
			{
				CBeat_Manager::Get_Instance()->Queue_Sequence(L"Gideon_1");
				_uint iBT = 4;
				m_pGameInstance->Push_Object_From_Pool(TEXT("Prototype_GameObject_UI_BossTitle"), m_eLevel, CUIBase::UILayerTag, &iBT);
			}
		}

		if(m_vecMonsters.size())
			pPlayer->Enter_BossRoom(m_vecMonsters[0]);

	}

}

void CRoom::Clear()
{
	
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Room_Clear");
	m_pGameInstance->Push_Object_From_Pool(TEXT("Prototype_GameObject_UI_RoomClear"), m_pGameInstance->Get_Current_LevelID(), CUIBase::UILayerTag);
}

HRESULT CRoom::Ready_Components()
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(m_eLevel, m_strModelTag, L"Model", (CComponent**)&m_pModelCom);

	m_pModelCom->Make_Mesh_Material_Group();

	return S_OK;
}

HRESULT CRoom::Bind_ShaderResources()
{
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Begin(0);

	return S_OK;
}

HRESULT CRoom::Read_Room(const string& strRoomFilePath)
{
	string strTemp;
	ifstream in(strRoomFilePath);
	if (!in.is_open())
		return E_FAIL;
	in >> strTemp >> strTemp;
	m_strModelTag = _wstring{ strTemp.begin(),strTemp.end() };
	_int iNumObj;
	in >> strTemp >> iNumObj;
	m_vecObjectTags.resize(iNumObj - 1);
	for (_int i = 0; i < iNumObj - 1; ++i)
	{
		in >> strTemp;
		_wstring wstTemp = _wstring{ strTemp.begin(),strTemp.end() };
		lstrcpy(m_vecObjectTags[i].first, wstTemp.data());

		in >> m_vecObjectTags[i].second._11 >> m_vecObjectTags[i].second._12 >> m_vecObjectTags[i].second._13 >> m_vecObjectTags[i].second._14
		   >> m_vecObjectTags[i].second._21 >> m_vecObjectTags[i].second._22 >> m_vecObjectTags[i].second._23 >> m_vecObjectTags[i].second._24
		   >> m_vecObjectTags[i].second._31 >> m_vecObjectTags[i].second._32 >> m_vecObjectTags[i].second._33 >> m_vecObjectTags[i].second._34
		   >> m_vecObjectTags[i].second._41 >> m_vecObjectTags[i].second._42 >> m_vecObjectTags[i].second._43 >> m_vecObjectTags[i].second._44;
	}
	in >> strTemp >> iNumObj;
	m_vecPortalTags.resize(iNumObj);
	for (_int i = 0; i < iNumObj; ++i)
	{
		in >> strTemp;
		_wstring wstTemp = _wstring{ strTemp.begin(),strTemp.end() };
		lstrcpy(m_vecPortalTags[i].first, wstTemp.data());
		//m_vecPortalTags[i].first = _wstring{ strTemp.begin(),strTemp.end() };

		in  >> m_vecPortalTags[i].second._11 >> m_vecPortalTags[i].second._12 >> m_vecPortalTags[i].second._13 >> m_vecPortalTags[i].second._14
			>> m_vecPortalTags[i].second._21 >> m_vecPortalTags[i].second._22 >> m_vecPortalTags[i].second._23 >> m_vecPortalTags[i].second._24
			>> m_vecPortalTags[i].second._31 >> m_vecPortalTags[i].second._32 >> m_vecPortalTags[i].second._33 >> m_vecPortalTags[i].second._34
			>> m_vecPortalTags[i].second._41 >> m_vecPortalTags[i].second._42 >> m_vecPortalTags[i].second._43 >> m_vecPortalTags[i].second._44;
	}
	in >> strTemp >> iNumObj;
	m_vecMonsterTags.resize(iNumObj);
	for (_int i = 0; i < iNumObj; ++i)
	{
		in >> strTemp;
		_wstring wstTemp = _wstring{ strTemp.begin(),strTemp.end() };
		lstrcpy(m_vecMonsterTags[i].first, wstTemp.data());

		in  >> m_vecMonsterTags[i].second._11 >> m_vecMonsterTags[i].second._12 >> m_vecMonsterTags[i].second._13 >> m_vecMonsterTags[i].second._14
			>> m_vecMonsterTags[i].second._21 >> m_vecMonsterTags[i].second._22 >> m_vecMonsterTags[i].second._23 >> m_vecMonsterTags[i].second._24
			>> m_vecMonsterTags[i].second._31 >> m_vecMonsterTags[i].second._32 >> m_vecMonsterTags[i].second._33 >> m_vecMonsterTags[i].second._34
			>> m_vecMonsterTags[i].second._41 >> m_vecMonsterTags[i].second._42 >> m_vecMonsterTags[i].second._43 >> m_vecMonsterTags[i].second._44;

	}

	{
		for (auto iterLeft = m_vecPortalTags.begin(); iterLeft != m_vecPortalTags.end(); ++iterLeft)
		{
			for (auto iterRight = m_vecPortalTags.begin(); iterRight != m_vecPortalTags.end(); ++iterRight)
			{
				if (iterLeft == iterRight)
					continue;
				_float fAngleL = atan2(iterLeft->second._43, iterLeft->second._41);
				_float fAngleR = atan2(iterRight->second._43, iterRight->second._41);
				if (fAngleL > fAngleR)
				{
					_float4x4 temp = iterLeft->second;
					iterLeft->second = iterRight->second;
					iterRight->second = temp;
				}
			}
		}
	}


	//sort(m_vecPortalTags.begin(), m_vecPortalTags.end(), [](pair<_tchar[256], _float4x4>& lhs, pair<_tchar[256], _float4x4>& rhs)->bool {
	//	_float fAngleL = atan2(lhs.second._43, lhs.second._41);
	//	_float fAngleR = atan2(rhs.second._43, rhs.second._41);
	//	return fAngleL > fAngleR;
	//	});

	return S_OK;
}

CRoom* CRoom::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ROOM_DESC& desc)
{
	CRoom* pInstance = new CRoom(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(desc)))
	{
		MSG_BOX(TEXT("Failed to Create : CRoom"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRoom::Add_Object(CGameObject* pObj)
{
	m_vecObjects.push_back(pObj);
	Safe_AddRef(pObj);
}

CGameObject* CRoom::Clone(void* pArg)
{
	CRoom* pInstance = new CRoom(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CRoom"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRoom::Free()
{
	auto iter_Obj = m_vecObjects.begin();
	for (iter_Obj; iter_Obj != m_vecObjects.end(); ++iter_Obj)
	{
		Safe_Release(*iter_Obj);
	}
	//for (auto& pObj : m_vecObjects)
	//{
	//	Safe_Release(pObj);
	//}
	//m_vecObjects.clear();
	auto iter_Mon = m_vecMonsters.begin();
	for (iter_Mon; iter_Mon != m_vecMonsters.end(); ++iter_Mon)
	{
		Safe_Release(*iter_Mon);
	}
	//for (auto& pObj : m_vecMonsters)
	//{
	//	Safe_Release(pObj);
	//}
	//m_vecMonsters.clear();
	auto iter_Por = m_vecPortals.begin();
	for (iter_Por; iter_Por != m_vecPortals.end(); ++iter_Por)
	{
		Safe_Release(*iter_Por);
	}

	//for (auto& pObj : m_vecPortals)
	//{
	//	Safe_Release(pObj);
	//}
	//m_vecPortals.clear();
	//m_vecMonsterTags.clear();
	//m_vecObjectTags.clear();
	//m_vecPortalTags.clear();
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}

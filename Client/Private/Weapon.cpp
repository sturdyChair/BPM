#include "Weapon.h"
#include "GameInstance.h"
#include "Shader.h"
#include "Model.h"
#include "Player_Manager.h"
#include "Player.h"
#include "RoomManager.h"
#include "Room.h"
#include "UI_Controller.h"
#include "Effect_LightningArc.h"
#include "Monster_Base.h"

_int CWeapon::iLightIdx = 0;
_wstring CWeapon::strLightTag = L"HitPoint";

CWeapon::CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)
{
}

CWeapon::CWeapon(const CWeapon& rhs) :
	CGameObject(rhs),
	m_strIdle(rhs.m_strIdle), m_iAmo(rhs.m_iAmo),m_iMaxAmo(rhs.m_iMaxAmo), m_bEquipped(false)
{

}

HRESULT CWeapon::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CWeapon::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	Ready_Components();
	_Initialize();

	m_pModelCom->Set_AnimTag(m_strIdle, false);
	m_pPlayerManager = CPlayer_Manager::Get_Instance();

	return S_OK;
}

void CWeapon::PriorityTick(_float fTimeDelta)
{
	m_bRenderOn = false;

}

void CWeapon::Tick(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta);
	if (m_pModelCom->Get_Finished() && m_bToIdle)
	{
		m_pModelCom->Set_AnimTag(m_strIdle, true);
	}
	auto pPlayer = m_pPlayerManager->Get_Player();
	if (pPlayer)
	{
		if (4.f > XMVectorGetX(XMVector3Length(m_pTransformCom->Get_Position() - pPlayer->Get_Transform()->Get_Position())))
		{
			if (!m_bEquipped)
			{
				ITEM_UI_DESC desc;
				desc.eID = SWAP_WEAPON;
				desc.iItemIdx = m_eID;
				CUI_Controller::Get_Instance()->Set_Desc(desc);

				if (m_pGameInstance->Get_KeyDown(DIK_F))
				{
					pPlayer->Equip_Weapon(this);

				}
			}
		}
	}
	XMStoreFloat4x4(&m_MuzzleTransform, XMLoadFloat4x4(&m_MuzzleOffset) * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
}

void CWeapon::LateTick(_float fTimeDelta)
{
	if(m_bRenderOn || !m_bEquipped)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CWeapon::Render()
{
	//Bind_ShaderResources();
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));

	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");

	m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		auto& vecMat = m_pModelCom->Get_Material(i);
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
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrix", i);
		m_pShaderCom->Begin(m_iRenderPass);
		m_pModelCom->Render(i);
	}
	return S_OK;
}

void CWeapon::Dying()
{

}

void CWeapon::Push_From_Pool(void* pArg)
{

}

void CWeapon::UnEquip(const _float4x4& Transform)
{
	m_bEquipped = false;
	m_pTransformCom->Set_WorldMatrix(Transform);
	Set_Dead(false);
	auto pRM = CRoomManager::Get_Instance();
	pRM->Get_Room(pRM->Get_CurrentRoomIdx())->Add_Object(this);
}

void CWeapon::Set_Amo(_uint iAmo)
{
	if (iAmo > m_iMaxAmo)
	{
		m_iAmo = m_iMaxAmo;
	}
	else
	{
		m_iAmo = iAmo;
	}
}

HRESULT CWeapon::Ready_Components()
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Revolver", L"Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

void CWeapon::Hit_Scan(_float fDamage)
{
	m_pGameInstance->Map_Picking_Screen();
	_uint iPickedID = m_pGameInstance->Pick_Screen(g_iWinSizeX / 2, g_iWinSizeY / 2);
	_float4 vHitPoint{};
	_float3 vHP3{};
	auto iter = CGameObject::s_HashedMap.find(iPickedID);
	if (iter != CGameObject::s_HashedMap.end())
	{

		_uint iCD = (*iter).second->Get_CollisionGroup();
		if (iCD == CG_MONSTER || iCD == CG_MONSTER_CONTACT)
		{
			Hit_Point_Effect(g_iWinSizeX / 2, g_iWinSizeY / 2, vHitPoint);
			vHP3.x = vHitPoint.x;
			vHP3.y = vHitPoint.y;
			vHP3.z = vHitPoint.z;
			iter->second->Take_Damage(fDamage, vHP3, DMG_DEFAULT, this);
			m_pGameInstance->Unmap_Picking_Screen();

			return;
		}
	}
	for (_int i = 1; i <= 25; ++i)
	{
		for (_int j = -i; j <= i; ++j)
		{
			iPickedID = m_pGameInstance->Pick_Screen(g_iWinSizeX / 2 + i, g_iWinSizeY / 2 + j);
			iter = CGameObject::s_HashedMap.find(iPickedID);
			if (iter != CGameObject::s_HashedMap.end())
			{

				_uint iCD = (*iter).second->Get_CollisionGroup();
				if (iCD == CG_MONSTER || iCD == CG_MONSTER_CONTACT)
				{
					Hit_Point_Effect(g_iWinSizeX / 2 + i, g_iWinSizeY / 2 + j, vHitPoint);
					vHP3.x = vHitPoint.x;
					vHP3.y = vHitPoint.y;
					vHP3.z = vHitPoint.z;
					iter->second->Take_Damage(fDamage, vHP3, DMG_DEFAULT, this);
					m_pGameInstance->Unmap_Picking_Screen();
					return;
				}
			}
			iPickedID = m_pGameInstance->Pick_Screen(g_iWinSizeX / 2 - i, g_iWinSizeY / 2 + j);
			iter = CGameObject::s_HashedMap.find(iPickedID);
			if (iter != CGameObject::s_HashedMap.end())
			{

				_uint iCD = (*iter).second->Get_CollisionGroup();
				if (iCD == CG_MONSTER || iCD == CG_MONSTER_CONTACT)
				{
					Hit_Point_Effect(g_iWinSizeX / 2 - i, g_iWinSizeY / 2 + j, vHitPoint);
					vHP3.x = vHitPoint.x;
					vHP3.y = vHitPoint.y;
					vHP3.z = vHitPoint.z;
					iter->second->Take_Damage(fDamage, vHP3, DMG_DEFAULT, this);
					m_pGameInstance->Unmap_Picking_Screen();
					return;
				}
			}
			iPickedID = m_pGameInstance->Pick_Screen(g_iWinSizeX / 2 + j, g_iWinSizeY / 2 + i);
			iter = CGameObject::s_HashedMap.find(iPickedID);
			if (iter != CGameObject::s_HashedMap.end())
			{

				_uint iCD = (*iter).second->Get_CollisionGroup();
				if (iCD == CG_MONSTER || iCD == CG_MONSTER_CONTACT)
				{
					Hit_Point_Effect(g_iWinSizeX / 2 + j, g_iWinSizeY / 2 + i, vHitPoint);
					vHP3.x = vHitPoint.x;
					vHP3.y = vHitPoint.y;
					vHP3.z = vHitPoint.z;
					iter->second->Take_Damage(fDamage, vHP3, DMG_DEFAULT, this);
					m_pGameInstance->Unmap_Picking_Screen();

					return;
				}
			}
			iPickedID = m_pGameInstance->Pick_Screen(g_iWinSizeX / 2 + j, g_iWinSizeY / 2 - i);
			iter = CGameObject::s_HashedMap.find(iPickedID);
			if (iter != CGameObject::s_HashedMap.end())
			{

				_uint iCD = (*iter).second->Get_CollisionGroup();
				if (iCD == CG_MONSTER || iCD == CG_MONSTER_CONTACT)
				{
					Hit_Point_Effect(g_iWinSizeX / 2 + j, g_iWinSizeY / 2 - i, vHitPoint);
					vHP3.x = vHitPoint.x;
					vHP3.y = vHitPoint.y;
					vHP3.z = vHitPoint.z;
					iter->second->Take_Damage(fDamage, vHP3, DMG_DEFAULT, this);
					m_pGameInstance->Unmap_Picking_Screen();

					return;
				}
			}
		}
	}
	Hit_Point_Effect(g_iWinSizeX / 2, g_iWinSizeY / 2, vHitPoint);
	_matrix ViewInv = m_pGameInstance->Get_Transform_Matrix(CPipeLine::TS_VIEW_INV);
	ViewInv.r[3] = XMLoadFloat4(&vHitPoint);
	_float4x4 VINV;
	XMStoreFloat4x4(&VINV, ViewInv);
	m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Scorch", m_pGameInstance->Get_Current_LevelID(), L"Effect", nullptr, &VINV);
	m_pGameInstance->Unmap_Picking_Screen();
}

void CWeapon::Hit_Point_Effect(_uint iX, _uint iY, _float4& vOut)
{
	m_pGameInstance->Map_Picking_Position();

	_float4 vWorldPos =  m_pGameInstance->Pick_Position(iX, iY);
	vOut = vWorldPos;
	_matrix ViewInv = m_pGameInstance->Get_Transform_Matrix(CPipeLine::TS_VIEW_INV);
	ViewInv = XMMatrixRotationY(XM_PI) * ViewInv;
	ViewInv.r[3] = XMLoadFloat4(&vWorldPos);
	_float4x4 SparkMat;
	
	XMStoreFloat4x4(&SparkMat, ViewInv);
	m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Particle_Impact", m_pGameInstance->Get_Current_LevelID(),
		L"Effect", nullptr, &SparkMat);

	m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Particle_Lightning_Spark", m_pGameInstance->Get_Current_LevelID(),
		L"Effect", nullptr, &SparkMat);
	_vector MuzzlePos = XMLoadFloat4x4(&m_MuzzleTransform).r[3];
	_vector vDir = ViewInv.r[3] - MuzzlePos;
	CEffect_LightningArc::LIGHRNIG_DESC LTDesc;
	LTDesc.fLifeTime = fRand(0.1f, 0.2f);
	LTDesc.vScale = _float3{ 0.03f, XMVectorGetX(XMVector3Length(vDir)) , 1.f };
	LTDesc.iTex = 8;
	_float4x4 LTTransform;
	_matrix LTMatrix = XMMatrixIdentity();
	LTMatrix.r[3] = MuzzlePos;
	//LTMatrix.r[3] = ViewInv.r[3];
	LTMatrix.r[1] = XMVector3Normalize(vDir);
	LTMatrix.r[0] = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), LTMatrix.r[1]);
	LTMatrix.r[2] = XMVector3Cross(LTMatrix.r[1], LTMatrix.r[0]);
	XMStoreFloat4x4(&LTTransform, LTMatrix);
	m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_LightningArc", m_pGameInstance->Get_Current_LevelID(), L"Effect",
		&LTDesc, &LTTransform);
	//m_pGameInstance->Push_Object_From_Pool();

	m_pGameInstance->Unmap_Picking_Position();
}

void CWeapon::Room_Scan(_float fDamage)
{
	m_pGameInstance->Map_Picking_Position();

	_float4 vFrom = m_pGameInstance->Pick_Position(g_iWinSizeX / 2, g_iWinSizeY / 2);

	m_pGameInstance->Unmap_Picking_Position();
	auto pRM = CRoomManager::Get_Instance();
	auto pCurrRoom = pRM->Get_Room(pRM->Get_CurrentRoomIdx());
	auto& vecMons = pCurrRoom->Get_Monsters();
	for (auto& pMon : vecMons)
	{
		_vector vPos = pMon->Get_Transform()->Get_Position();
		_float3 vfPos;
		XMStoreFloat3(&vfPos, vPos);
		pMon->Take_Damage(fDamage, vfPos, DMG_DEFAULT, this);

		_float4 vWorldPos;
		XMStoreFloat4(&vWorldPos, vPos);
		_matrix ViewInv = m_pGameInstance->Get_Transform_Matrix(CPipeLine::TS_VIEW_INV);
		ViewInv = XMMatrixRotationY(XM_PI) * ViewInv;
		ViewInv.r[3] = XMLoadFloat4(&vWorldPos);
		_float4x4 SparkMat;

		XMStoreFloat4x4(&SparkMat, ViewInv);

		m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Particle_Lightning_Spark", m_pGameInstance->Get_Current_LevelID(),
			L"Effect", nullptr, &SparkMat);
		_vector MuzzlePos = XMLoadFloat4(&vFrom);
		_vector vDir = ViewInv.r[3] - MuzzlePos;
		CEffect_LightningArc::LIGHRNIG_DESC LTDesc;
		LTDesc.fLifeTime = fRand(0.1f, 0.2f);
		LTDesc.vScale = _float3{ 0.03f, XMVectorGetX(XMVector3Length(vDir)) , 1.f };
		LTDesc.iTex = 8;
		_float4x4 LTTransform;
		_matrix LTMatrix = XMMatrixIdentity();
		LTMatrix.r[3] = MuzzlePos;
		//LTMatrix.r[3] = ViewInv.r[3];
		LTMatrix.r[1] = XMVector3Normalize(vDir);
		LTMatrix.r[0] = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), LTMatrix.r[1]);
		LTMatrix.r[2] = XMVector3Cross(LTMatrix.r[1], LTMatrix.r[0]);
		XMStoreFloat4x4(&LTTransform, LTMatrix);
		m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_LightningArc", m_pGameInstance->Get_Current_LevelID(), L"Effect",
			&LTDesc, &LTTransform);
	}

}



CWeapon* CWeapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon* pInstance = new CWeapon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CWeapon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon::Clone(void* pArg)
{
	CWeapon* pInstance = new CWeapon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CWeapon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon::Free()
{

	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}

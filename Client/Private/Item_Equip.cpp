#include "Item_Equip.h"
#include "Player_Manager.h"
#include "Player.h"
#include "RoomManager.h"
#include "Room.h"
#include "GameInstance.h"

CItem_Equip::CItem_Equip(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
	CItem_Base(pDevice,pContext)
{
}

CItem_Equip::CItem_Equip(const CItem_Equip& rhs):
	CItem_Base(rhs),
	m_eSlotID(rhs.m_eSlotID),
	m_strModelTag(rhs.m_strModelTag)
{
}

HRESULT CItem_Equip::Initialize_Prototype(const EQUIP_DESC& desc)
{
	__super::Initialize_Prototype();

	m_eID = desc.eID;
	m_strModelTag = desc.strModelTag;
	m_strName = desc.strName;
	m_strDesc = desc.strDesc;
	m_iPrice = desc.iPrice;
	m_eSlotID = desc.eSlotID;

	m_eSwapID = SWAP_ITEM;

	return S_OK;
}

void CItem_Equip::Dying()
{
}

void CItem_Equip::Push_From_Pool(void* pArg)
{
}

void CItem_Equip::UnEquip(const _float4x4& Transform)
{
	m_bEquipped = false;
	m_pTransformCom->Set_WorldMatrix(Transform);
	Set_Dead(false);
	auto pRM = CRoomManager::Get_Instance();
	pRM->Get_Room(pRM->Get_CurrentRoomIdx())->Add_Object(this);
}

void CItem_Equip::Equip()
{

}

HRESULT CItem_Equip::Ready_Components()
{
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, m_strModelTag, L"Model", (CComponent**)&m_pModelCom);

	return S_OK;
}

void CItem_Equip::_Initialize()
{
}

void CItem_Equip::Interact()
{
	auto pPlayer = m_pPlayerManager->Get_Player();
	if (pPlayer->Get_Coin() < m_iPrice)
		return;
	pPlayer->Reduce_Coin(m_iPrice);
	m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Equipping", m_pGameInstance->Get_Current_LevelID(), L"Effect", nullptr, m_pTransformCom->Get_WorldMatrix_Ptr());
	pPlayer->Equip_Item(this);
	
}

CItem_Equip* CItem_Equip::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const EQUIP_DESC& desc)
{
	CItem_Equip* pInstance = new CItem_Equip(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(desc)))
	{
		MSG_BOX(TEXT("Failed to Create : CItem_Equip"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItem_Equip::Clone(void* pArg)
{
	CItem_Equip* pInstance = new CItem_Equip(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CItem_Equip"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_Equip::Free()
{
	__super::Free();
}

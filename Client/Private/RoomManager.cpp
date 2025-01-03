#include "RoomManager.h"
#include "GameInstance.h"
#include "Room.h"

IMPLEMENT_SINGLETON(CRoomManager)

void Load_Asgard_Model_1(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_T_1",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_T_1.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_T_2",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_T_2.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));
	m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Shop",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Shop.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

}
void Load_Asgard_Model_2(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_I_1",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_I_1.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_I_2",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_I_2.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Boss",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Boss.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

}
void Load_Asgard_Model_3(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Cross_1_a",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Cross_1_a.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Cross_1_b",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Cross_1_b.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Cross_2",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Cross_2.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));


}
void Load_Asgard_Model_4(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_T_3",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_T_3.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_T_4",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_T_4.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Armory",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Armory.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

}
void Load_Asgard_Model_5(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_I_3",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_I_3.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_L_1",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_L_1.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	//m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_L_2",
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_L_2.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_L_3",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_L_3.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

}
void Load_Asgard_Model_6(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Cross_3",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Cross_3.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Cross_4",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Cross_4.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

}
#pragma region HEL
void Load_Hel_Model_1(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_HEL, L"Prototype_Component_Model_Helhime_Armory",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Helhime/Helhime_Armory.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Helhime/Helhime/"));
	m_pGameInstance->Add_Prototype(LEVEL_HEL, L"Prototype_Component_Model_Helhime_Boss",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Helhime/Helhime_Boss.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Helhime/Helhime/"));
}
void Load_Hel_Model_2(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_HEL, L"Prototype_Component_Model_Helhime_Cross_1",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Helhime/Helhime_Cross_1.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Helhime/Helhime/"));
}
void Load_Hel_Model_3(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_HEL, L"Prototype_Component_Model_Helhime_Cross_2",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Helhime/Helhime_Cross_2.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Helhime/Helhime/"));
}
void Load_Hel_Model_4(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_HEL, L"Prototype_Component_Model_Helhime_Cross_3",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Helhime/Helhime_Cross_3.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Helhime/Helhime/"));
}
void Load_Hel_Model_5(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_HEL, L"Prototype_Component_Model_Helhime_Cross_4",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Helhime/Helhime_Cross_4.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Helhime/Helhime/"));
}
void Load_Hel_Model_6(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_HEL, L"Prototype_Component_Model_Helhime_I_1",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Helhime/Helhime_I_1.dat", 
			XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Helhime/Helhime/"));
}
void Load_Hel_Model_7(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_HEL, L"Prototype_Component_Model_Helhime_I_2",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Helhime/Helhime_I_2.dat",
			XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Helhime/Helhime/"));
}
void Load_Hel_Model_8(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_HEL, L"Prototype_Component_Model_Helhime_I_3",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Helhime/Helhime_I_3.dat",
			XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Helhime/Helhime/"));
}
void Load_Hel_Model_9(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_HEL, L"Prototype_Component_Model_Helhime_L_1",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Helhime/Helhime_L_1.dat",
			XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Helhime/Helhime/"));
}
void Load_Hel_Model_10(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_HEL, L"Prototype_Component_Model_Helhime_L_2",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Helhime/Helhime_L_2.dat",
			XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Helhime/Helhime/"));
}
void Load_Hel_Model_11(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_HEL, L"Prototype_Component_Model_Helhime_L_3",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Helhime/Helhime_L_3.dat",
			XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Helhime/Helhime/"));
}
void Load_Hel_Model_12(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_HEL, L"Prototype_Component_Model_Helhime_L_4",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Helhime/Helhime_L_4.dat",
			XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Helhime/Helhime/"));
}
void Load_Hel_Model_13(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_HEL, L"Prototype_Component_Model_Helhime_Shop",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Helhime/Helhime_Shop.dat",
			XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Helhime/Helhime/"));
}
void Load_Hel_Model_14(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_HEL, L"Prototype_Component_Model_Helhime_Start",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Helhime/Helhime_Start.dat",
			XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Helhime/Helhime/"));
}
void Load_Hel_Model_15(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_HEL, L"Prototype_Component_Model_Helhime_T_1",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Helhime/Helhime_T_1.dat",
			XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Helhime/Helhime/"));
}
void Load_Hel_Model_16(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_HEL, L"Prototype_Component_Model_Helhime_Treasure",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Helhime/Helhime_Treasure.dat",
			XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Helhime/Helhime/"));
}
#pragma endregion
void Load_BossRush(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_BOSSRUSH, L"Prototype_Component_Model_BossRush",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/BossRush.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Texture2D/Asgard/"));

}

#pragma region VANA
void Load_Vana_Model_1(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_VANA, L"Prototype_Component_Model_Vana_Boss",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Vana/Vana_Boss.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Vana/Vana/"));
}
void Load_Vana_Model_2(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_VANA, L"Prototype_Component_Model_Vana_Start",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Vana/Vana_Start_Cross.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Vana/Vana/"));
}
void Load_Vana_Model_3(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_VANA, L"Prototype_Component_Model_Vana_Armory",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Vana/Vana_Armory.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Vana/Vana/"));
}
void Load_Vana_Model_4(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_VANA, L"Prototype_Component_Model_Vana_Shop",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Vana/Vana_Shop.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Vana/Vana/"));
}
void Load_Vana_Model_5(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_VANA, L"Prototype_Component_Model_Vana_Treasure",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Vana/Vana_Treasure.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Vana/Vana/"));
}
void Load_Vana_Model_6(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_VANA, L"Prototype_Component_Model_Vana_Cross_1",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Vana/Vana_Cross_1.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Vana/Vana/"));
	m_pGameInstance->Add_Prototype(LEVEL_VANA, L"Prototype_Component_Model_Vana_Cross_2",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Vana/Vana_Cross_2.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Vana/Vana/"));
}
void Load_Vana_Model_7(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_VANA, L"Prototype_Component_Model_Vana_Cross_3",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Vana/Vana_Cross_3.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Vana/Vana/"));
	m_pGameInstance->Add_Prototype(LEVEL_VANA, L"Prototype_Component_Model_Vana_Cross_4",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Vana/Vana_Cross_4.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Vana/Vana/"));
}
void Load_Vana_Model_8(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_VANA, L"Prototype_Component_Model_Vana_I_1",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Vana/Vana_I_1.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Vana/Vana/"));
	m_pGameInstance->Add_Prototype(LEVEL_VANA, L"Prototype_Component_Model_Vana_I_1",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Vana/Vana_I_2.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Vana/Vana/"));
}
void Load_Vana_Model_9(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_VANA, L"Prototype_Component_Model_Vana_L_1",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Vana/Vana_L_1.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Vana/Vana/"));
	m_pGameInstance->Add_Prototype(LEVEL_VANA, L"Prototype_Component_Model_Vana_L_2",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Vana/Vana_L_2.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Vana/Vana/"));
}
void Load_Vana_Model_10(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_VANA, L"Prototype_Component_Model_Vana_L_3",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Vana/Vana_L_3.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Vana/Vana/"));
	m_pGameInstance->Add_Prototype(LEVEL_VANA, L"Prototype_Component_Model_Vana_L_4",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Vana/Vana_L_4.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Vana/Vana/"));
}
#pragma endregion

#pragma region SVART
void Load_Svart_Model_1(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_SVART, L"Prototype_Component_Model_Svart_Choice",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Svart/Svart_Choice.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Svart/Svart/"));
}
void Load_Svart_Model_2(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_SVART, L"Prototype_Component_Model_Svart_Lib",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Svart/Svart_Lib.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Svart/Svart/"));
}
void Load_Svart_Model_3(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_SVART, L"Prototype_Component_Model_Svart_Portal",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Svart/Svart_Portal.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Svart/Svart/"));
}
void Load_Svart_Model_4(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_SVART, L"Prototype_Component_Model_Svart_Start",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Svart/Svart_Start.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Svart/Svart/"));
}
void Load_Svart_Model_5(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_SVART, L"Prototype_Component_Model_Svart_Straight",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Svart/Svart_Straight.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Svart/Svart/"));
}
void Load_Svart_Model_6(CGameInstance* m_pGameInstance, ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext)
{
	m_pGameInstance->Add_Prototype(LEVEL_SVART, L"Prototype_Component_Model_Svart_Treasure",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Svart/Svart_Treasure.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.02f, 0.02f, 0.02f), "../Bin/Resource/Model/Svart/Svart/"));
}

#pragma endregion

CRoomManager::CRoomManager()
{
	m_pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(m_pGameInstance);
}

void CRoomManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice; m_pContext = pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

void CRoomManager::Kill_Instance()
{
	Free();
	Destroy_Instance();
}

void CRoomManager::Purge_Rooms()
{
	for (auto& elem : m_Rooms)
	{
		for (auto& pRoom : elem)
		{
			Safe_Release(pRoom);
		}
		elem.clear();
	}
	for (auto& tag : m_PooledObjectToClear)
	{
		m_pGameInstance->Clear_Pool(tag);
	}
	m_PooledObjectToClear.clear();
	m_Rooms.clear();
}

void CRoomManager::Resize_Room()
{
	Purge_Rooms();
	m_Rooms.resize(m_iSizeX);
	for (auto& elem : m_Rooms)
	{
		elem.resize(m_iSizeY);
	}
}

void CRoomManager::Generate_Room()
{

	Resize_Room();
	vector<vector<_uint>> RandoTron;
	RandoTron.resize(RST_END);
	for (_uint i = 0; i < RST_END; ++i)
	{
		_uint iSize = m_RoomPrototype[i].size();
		RandoTron[i].resize(iSize);
		for (_uint j = 0; j < iSize; ++j)
		{
			RandoTron[i][j] = j;
		}
		for (_uint j = 0; j < iSize * 1000; ++j)
		{
			_uint iSrc = rand() % iSize, iDest = rand() % iSize;
			_uint temp = RandoTron[i][iSrc];
			RandoTron[i][iSrc] = RandoTron[i][iDest];
			RandoTron[i][iDest] = temp;
		}
	}
	vector<_uint> RandoIdx;
	RandoIdx.resize(RST_END);

	CRoom::CLONEING_DESC clD;
	
	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = 3; clD.pRoomConnected[2] = 7; clD.pRoomConnected[3] = -1;
	m_Rooms[0][2] = (CRoom*)m_RoomPrototype[RST_L][RandoTron[RST_L][RandoIdx[RST_L] % RandoTron[RST_L].size()]]->Clone(&clD);
	++RandoIdx[RST_L];

	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = 4; clD.pRoomConnected[2] = 8; clD.pRoomConnected[3] = 2;
	m_Rooms[0][3] = (CRoom*)m_RoomPrototype[RST_T][RandoTron[RST_T][RandoIdx[RST_T] % RandoTron[RST_T].size()]]->Clone(&clD);
	++RandoIdx[RST_T];

	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = 3;
	m_Rooms[0][4] = (CRoom*)m_RoomPrototype[RST_SPECIAL][BOSS]->Clone(&clD);

	
	clD.pRoomConnected[0] = 2; clD.pRoomConnected[1] = 8; clD.pRoomConnected[2] = 12; clD.pRoomConnected[3] = -1;
	m_Rooms[1][2] = (CRoom*)m_RoomPrototype[RST_T][RandoTron[RST_T][RandoIdx[RST_T] % RandoTron[RST_T].size()]]->Clone(&clD);
	++RandoIdx[RST_T];

	clD.pRoomConnected[0] = 3; clD.pRoomConnected[1] = 9; clD.pRoomConnected[2] = 13; clD.pRoomConnected[3] = 7;
	m_Rooms[1][3] = (CRoom*)m_RoomPrototype[RST_CROSS][RandoTron[RST_CROSS][RandoIdx[RST_CROSS] % RandoTron[RST_CROSS].size()]]->Clone(&clD);
	++RandoIdx[RST_CROSS];

	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = 14; clD.pRoomConnected[3] = 8;
	m_Rooms[1][4] = (CRoom*)m_RoomPrototype[RST_L][RandoTron[RST_L][RandoIdx[RST_L] % RandoTron[RST_L].size()]]->Clone(&clD);
	++RandoIdx[RST_L];


	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = 11; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = -1;
	m_Rooms[2][0] = (CRoom*)m_RoomPrototype[RST_SPECIAL][ARMORY]->Clone(&clD);

	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = 12; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = 10;
	m_Rooms[2][1] = (CRoom*)m_RoomPrototype[RST_I][RandoTron[RST_I][RandoIdx[RST_I] % RandoTron[RST_I].size()]]->Clone(&clD);
	++RandoIdx[RST_I];

	clD.pRoomConnected[0] = 7; clD.pRoomConnected[1] = 13; clD.pRoomConnected[2] = 17; clD.pRoomConnected[3] = 11;
	m_Rooms[2][2] = (CRoom*)m_RoomPrototype[RST_SPECIAL][START]->Clone(&clD);

	clD.pRoomConnected[0] = 8; clD.pRoomConnected[1] = 14; clD.pRoomConnected[2] = 18; clD.pRoomConnected[3] = 12;
	m_Rooms[2][3] = (CRoom*)m_RoomPrototype[RST_CROSS][RandoTron[RST_CROSS][RandoIdx[RST_CROSS] % RandoTron[RST_CROSS].size()]]->Clone(&clD);
	++RandoIdx[RST_CROSS];

	clD.pRoomConnected[0] = 9; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = 19; clD.pRoomConnected[3] = 13;
	m_Rooms[2][4] = (CRoom*)m_RoomPrototype[RST_T][RandoTron[RST_T][RandoIdx[RST_T] % RandoTron[RST_T].size()]]->Clone(&clD);
	++RandoIdx[RST_T];


	clD.pRoomConnected[0] = 12; clD.pRoomConnected[1] = 18; clD.pRoomConnected[2] = 22; clD.pRoomConnected[3] = -1;
	m_Rooms[3][2] = (CRoom*)m_RoomPrototype[RST_T][RandoTron[RST_T][RandoIdx[RST_T] % RandoTron[RST_T].size()]]->Clone(&clD);
	++RandoIdx[RST_T];

	clD.pRoomConnected[0] = 13; clD.pRoomConnected[1] = 19; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = 17;
	m_Rooms[3][3] = (CRoom*)m_RoomPrototype[RST_T][RandoTron[RST_T][RandoIdx[RST_T] % RandoTron[RST_T].size()]]->Clone(&clD);
	++RandoIdx[RST_T];

	clD.pRoomConnected[0] = 14; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = 24; clD.pRoomConnected[3] = 18;
	m_Rooms[3][4] = (CRoom*)m_RoomPrototype[RST_T][RandoTron[RST_T][RandoIdx[RST_T] % RandoTron[RST_T].size()]]->Clone(&clD);
	++RandoIdx[RST_T];

	clD.pRoomConnected[0] = 17; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = -1;
	m_Rooms[4][2] = (CRoom*)m_RoomPrototype[RST_SPECIAL][SHOP]->Clone(&clD);

	clD.pRoomConnected[0] = 19; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = -1;
	m_Rooms[4][4] = (CRoom*)m_RoomPrototype[RST_SPECIAL][TREASURE]->Clone(&clD);

	//for (auto& Rooms : m_Rooms)
	//{
	//	for (auto& pRoom : Rooms)
	//	{
	//		Safe_AddRef(pRoom);
	//	}
	//}

	/*
Asgard

0 0 L T B
0 0 T C L
A I R C T
0 0 T T T
0 0 S 0 H

*/
}

void CRoomManager::Generate_Vana()
{
	Resize_Room();
	vector<vector<_uint>> RandoTron;
	RandoTron.resize(RST_END);
	for (_uint i = 0; i < RST_END; ++i)
	{
		_uint iSize = m_RoomPrototype[i].size();
		RandoTron[i].resize(iSize);
		for (_uint j = 0; j < iSize; ++j)
		{
			RandoTron[i][j] = j;
		}
		for (_uint j = 0; j < iSize * 1000; ++j)
		{
			_uint iSrc = rand() % iSize, iDest = rand() % iSize;
			_uint temp = RandoTron[i][iSrc];
			RandoTron[i][iSrc] = RandoTron[i][iDest];
			RandoTron[i][iDest] = temp;
		}
	}
	vector<_uint> RandoIdx;
	RandoIdx.resize(RST_END);

	CRoom::CLONEING_DESC clD;

	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = 7; clD.pRoomConnected[2] = 11; clD.pRoomConnected[3] = -1;
	m_Rooms[1][1] = (CRoom*)m_RoomPrototype[RST_L][RandoTron[RST_L][RandoIdx[RST_L] % RandoTron[RST_L].size()]]->Clone(&clD);
	++RandoIdx[RST_L];

	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = 13; clD.pRoomConnected[3] = 7;
	m_Rooms[1][3] = (CRoom*)m_RoomPrototype[RST_L][RandoTron[RST_L][RandoIdx[RST_L] % RandoTron[RST_L].size()]]->Clone(&clD);
	++RandoIdx[RST_L];

	clD.pRoomConnected[0] = 11; clD.pRoomConnected[1] = 17; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = -1;
	m_Rooms[3][1] = (CRoom*)m_RoomPrototype[RST_L][RandoTron[RST_L][RandoIdx[RST_L] % RandoTron[RST_L].size()]]->Clone(&clD);
	++RandoIdx[RST_L];

	clD.pRoomConnected[0] = 13; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = 17;
	m_Rooms[3][3] = (CRoom*)m_RoomPrototype[RST_L][RandoTron[RST_L][RandoIdx[RST_L] % RandoTron[RST_L].size()]]->Clone(&clD);
	++RandoIdx[RST_L];



	clD.pRoomConnected[0] = 2; clD.pRoomConnected[1] = 8; clD.pRoomConnected[2] = 12; clD.pRoomConnected[3] = 6;
	m_Rooms[1][2] = (CRoom*)m_RoomPrototype[RST_CROSS][RandoTron[RST_CROSS][RandoIdx[RST_CROSS] % RandoTron[RST_CROSS].size()]]->Clone(&clD);
	++RandoIdx[RST_CROSS];

	clD.pRoomConnected[0] = 6; clD.pRoomConnected[1] = 12; clD.pRoomConnected[2] = 16; clD.pRoomConnected[3] = 10;
	m_Rooms[2][1] = (CRoom*)m_RoomPrototype[RST_CROSS][RandoTron[RST_CROSS][RandoIdx[RST_CROSS] % RandoTron[RST_CROSS].size()]]->Clone(&clD);
	++RandoIdx[RST_CROSS];

	clD.pRoomConnected[0] = 8; clD.pRoomConnected[1] = 14; clD.pRoomConnected[2] = 18; clD.pRoomConnected[3] = 12;
	m_Rooms[2][3] = (CRoom*)m_RoomPrototype[RST_CROSS][RandoTron[RST_CROSS][RandoIdx[RST_CROSS] % RandoTron[RST_CROSS].size()]]->Clone(&clD);
	++RandoIdx[RST_CROSS];

	clD.pRoomConnected[0] = 12; clD.pRoomConnected[1] = 18; clD.pRoomConnected[2] = 22; clD.pRoomConnected[3] = 16;
	m_Rooms[3][2] = (CRoom*)m_RoomPrototype[RST_CROSS][RandoTron[RST_CROSS][RandoIdx[RST_CROSS] % RandoTron[RST_CROSS].size()]]->Clone(&clD);
	++RandoIdx[RST_CROSS];


	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = 7; clD.pRoomConnected[3] = -1;
	m_Rooms[0][2] = (CRoom*)m_RoomPrototype[RST_SPECIAL][ARMORY]->Clone(&clD);

	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = 11; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = -1;
	m_Rooms[2][0] = (CRoom*)m_RoomPrototype[RST_SPECIAL][BOSS]->Clone(&clD);

	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = 13;
	m_Rooms[2][4] = (CRoom*)m_RoomPrototype[RST_SPECIAL][TREASURE]->Clone(&clD);

	clD.pRoomConnected[0] = 17; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = -1;
	m_Rooms[4][2] = (CRoom*)m_RoomPrototype[RST_SPECIAL][SHOP]->Clone(&clD);

	clD.pRoomConnected[0] = 7; clD.pRoomConnected[1] = 13; clD.pRoomConnected[2] = 17; clD.pRoomConnected[3] = 11;
	m_Rooms[2][2] = (CRoom*)m_RoomPrototype[RST_SPECIAL][START]->Clone(&clD);
}

void CRoomManager::Generate_Svart()
{
	Resize_Room();

	CRoom::CLONEING_DESC clD;

	clD.pRoomConnected[0] = 7; clD.pRoomConnected[1] = 13; clD.pRoomConnected[2] = 17; clD.pRoomConnected[3] = 11;
	m_Rooms[2][2] = (CRoom*)m_RoomPrototype[0][START_SV]->Clone(&clD);

	clD.pRoomConnected[0] = 2; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = 12; clD.pRoomConnected[3] = -1;
	m_Rooms[1][2] = (CRoom*)m_RoomPrototype[0][STRAIGHT_SV]->Clone(&clD);

	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = 7; clD.pRoomConnected[3] = -1;
	m_Rooms[0][2] = (CRoom*)m_RoomPrototype[0][CHOICE_SV]->Clone(&clD);

	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = 12; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = -1;
	m_Rooms[2][1] = (CRoom*)m_RoomPrototype[0][LIB_SV]->Clone(&clD);

	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = 12;
	m_Rooms[2][3] = (CRoom*)m_RoomPrototype[0][PORTAL_SV]->Clone(&clD);

	clD.pRoomConnected[0] = 12; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = -1;
	m_Rooms[3][2] = (CRoom*)m_RoomPrototype[0][TREASURE_SV]->Clone(&clD);

}

void CRoomManager::Generate_Hel()
{
	Resize_Room();
	vector<vector<_uint>> RandoTron;
	RandoTron.resize(RST_END);
	for (_uint i = 0; i < RST_END; ++i)
	{
		_uint iSize = m_RoomPrototype[i].size();
		RandoTron[i].resize(iSize);
		for (_uint j = 0; j < iSize; ++j)
		{
			RandoTron[i][j] = j;
		}
		for (_uint j = 0; j < iSize * 1000; ++j)
		{
			_uint iSrc = rand() % iSize, iDest = rand() % iSize;
			_uint temp = RandoTron[i][iSrc];
			RandoTron[i][iSrc] = RandoTron[i][iDest];
			RandoTron[i][iDest] = temp;
		}
	}
	vector<_uint> RandoIdx;
	RandoIdx.resize(RST_END);

	CRoom::CLONEING_DESC clD;

	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = 1; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = -1;
	m_Rooms[0][0] = (CRoom*)m_RoomPrototype[RST_SPECIAL][SHOP]->Clone(&clD);

	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = 6; clD.pRoomConnected[3] = 0;
	m_Rooms[0][1] = (CRoom*)m_RoomPrototype[RST_L][RandoTron[RST_L][RandoIdx[RST_L] % RandoTron[RST_L].size()]]->Clone(&clD);
	++RandoIdx[RST_L];

	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = 4; clD.pRoomConnected[2] = 8; clD.pRoomConnected[3] = -1;
	m_Rooms[0][3] = (CRoom*)m_RoomPrototype[RST_L][RandoTron[RST_L][RandoIdx[RST_L] % RandoTron[RST_L].size()]]->Clone(&clD);
	++RandoIdx[RST_L];

	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = 3;
	m_Rooms[0][4] = (CRoom*)m_RoomPrototype[RST_SPECIAL][BOSS]->Clone(&clD);

	clD.pRoomConnected[0] = 1; clD.pRoomConnected[1] = 7; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = -1;
	m_Rooms[1][1] = (CRoom*)m_RoomPrototype[RST_L][RandoTron[RST_L][RandoIdx[RST_L] % RandoTron[RST_L].size()]]->Clone(&clD);
	++RandoIdx[RST_L];

	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = 8; clD.pRoomConnected[2] = 12; clD.pRoomConnected[3] = 6;
	m_Rooms[1][2] = (CRoom*)m_RoomPrototype[RST_T][RandoTron[RST_T][RandoIdx[RST_T] % RandoTron[RST_T].size()]]->Clone(&clD);
	++RandoIdx[RST_T];

	clD.pRoomConnected[0] = 3; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = 13; clD.pRoomConnected[3] = 7;
	m_Rooms[1][3] = (CRoom*)m_RoomPrototype[RST_T][RandoTron[RST_T][RandoIdx[RST_T] % RandoTron[RST_T].size()]]->Clone(&clD);
	++RandoIdx[RST_T];

	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = 11; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = -1;
	m_Rooms[2][0] = (CRoom*)m_RoomPrototype[RST_SPECIAL][ARMORY]->Clone(&clD);

	clD.pRoomConnected[0] = -1; clD.pRoomConnected[1] = 12; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = 10;
	m_Rooms[2][1] = (CRoom*)m_RoomPrototype[RST_I][RandoTron[RST_I][RandoIdx[RST_I] % RandoTron[RST_I].size()]]->Clone(&clD);
	++RandoIdx[RST_I];

	clD.pRoomConnected[0] = 7; clD.pRoomConnected[1] = 13; clD.pRoomConnected[2] = 17; clD.pRoomConnected[3] = 11;
	m_Rooms[2][2] = (CRoom*)m_RoomPrototype[RST_SPECIAL][START]->Clone(&clD);

	clD.pRoomConnected[0] = 8; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = 12;
	m_Rooms[2][3] = (CRoom*)m_RoomPrototype[RST_L][RandoTron[RST_L][RandoIdx[RST_L] % RandoTron[RST_L].size()]]->Clone(&clD);
	++RandoIdx[RST_L];

	clD.pRoomConnected[0] = 12; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = 22; clD.pRoomConnected[3] = -1;
	m_Rooms[3][2] = (CRoom*)m_RoomPrototype[RST_I][RandoTron[RST_I][RandoIdx[RST_I] % RandoTron[RST_I].size()]]->Clone(&clD);
	++RandoIdx[RST_I];

	clD.pRoomConnected[0] = 17; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = -1;
	m_Rooms[4][2] = (CRoom*)m_RoomPrototype[RST_SPECIAL][TREASURE]->Clone(&clD);
	/*
Hel

S L 0 L B
0 L T T 0
A I R L 0
0 0 I 0 0
0 0 H 0 0

*/
}

void CRoomManager::Generate_BossRush()
{
	Resize_Room();

	CRoom::CLONEING_DESC clD;

	clD.pRoomConnected[0] = 1; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = -1;
	m_Rooms[0][0] = (CRoom*)m_RoomPrototype[RST_SPECIAL][0]->Clone(&clD);
	clD.pRoomConnected[0] = 2; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = -1;
	m_Rooms[0][1] = (CRoom*)m_RoomPrototype[RST_SPECIAL][1]->Clone(&clD);
	clD.pRoomConnected[0] = 2; clD.pRoomConnected[1] = -1; clD.pRoomConnected[2] = -1; clD.pRoomConnected[3] = -1;
	m_Rooms[0][2] = (CRoom*)m_RoomPrototype[RST_SPECIAL][2]->Clone(&clD);
}

void CRoomManager::Ready_Rooms_Asgard()
{
#pragma region MODEL
	thread t1(Load_Asgard_Model_1, m_pGameInstance, m_pDevice, m_pContext);
	thread t2(Load_Asgard_Model_2, m_pGameInstance, m_pDevice, m_pContext);
	thread t3(Load_Asgard_Model_3, m_pGameInstance, m_pDevice, m_pContext);
	thread t4(Load_Asgard_Model_4, m_pGameInstance, m_pDevice, m_pContext);
	thread t5(Load_Asgard_Model_5, m_pGameInstance, m_pDevice, m_pContext);
	thread t6(Load_Asgard_Model_6, m_pGameInstance, m_pDevice, m_pContext);

	m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Start_1",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Start_1.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));

	m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Asgard_Treasure",
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/Model/Asgard/Asgard_Treasure.dat", XMMatrixRotationY(XM_PI) * XMMatrixScaling(0.01f, 0.01f, 0.01f), "../Bin/Resource/Model/Texture2D/Asgard/"));
#pragma endregion //MODEL
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	Purge_Prototype();

	CRoom::ROOM_DESC desc;
	desc.eLevel = LEVEL_GAMEPLAY;
	desc.eRoomType = ROOM_DEFAULT;

	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_Cross_1_a.txt";
	m_RoomPrototype[RST_CROSS].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_Cross_1_b.txt";
	m_RoomPrototype[RST_CROSS].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_Cross_2.txt";
	m_RoomPrototype[RST_CROSS].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_Cross_3.txt";
	m_RoomPrototype[RST_CROSS].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_Cross_4.txt";
	m_RoomPrototype[RST_CROSS].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_I_1.txt";
	m_RoomPrototype[RST_I].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_I_2.txt";
	m_RoomPrototype[RST_I].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_I_3.txt";
	m_RoomPrototype[RST_I].push_back(CRoom::Create(m_pDevice, m_pContext, desc));

	//desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_L_1.txt";
	//m_RoomPrototype[RST_L].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	//desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_L_2.txt";
	//m_RoomPrototype[RST_L].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_L_3.txt";
	m_RoomPrototype[RST_L].push_back(CRoom::Create(m_pDevice, m_pContext, desc));

	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_T_1.txt";
	m_RoomPrototype[RST_T].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_T_2.txt";
	m_RoomPrototype[RST_T].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_T_3.txt";
	m_RoomPrototype[RST_T].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_T_4.txt";
	m_RoomPrototype[RST_T].push_back(CRoom::Create(m_pDevice, m_pContext, desc));


	m_RoomPrototype[RST_SPECIAL].resize(SRT_END);
	desc.eRoomType = ROOM_ARMORY;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_Armory.txt";
	m_RoomPrototype[RST_SPECIAL][ARMORY] = (CRoom::Create(m_pDevice, m_pContext, desc));
	desc.eRoomType = ROOM_BOSS;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_Boss.txt";
	m_RoomPrototype[RST_SPECIAL][BOSS] = (CRoom::Create(m_pDevice, m_pContext, desc));
	desc.eRoomType = ROOM_SHOP;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_Shop.txt";
	m_RoomPrototype[RST_SPECIAL][SHOP] = (CRoom::Create(m_pDevice, m_pContext, desc));
	desc.eRoomType = ROOM_START;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_Start.txt";
	m_RoomPrototype[RST_SPECIAL][START] = (CRoom::Create(m_pDevice, m_pContext, desc));
	desc.eRoomType = ROOM_CHSET;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Asgard_Treasure.txt";
	m_RoomPrototype[RST_SPECIAL][TREASURE] = (CRoom::Create(m_pDevice, m_pContext, desc));

}

void CRoomManager::Ready_Rooms_Vana()
{
#pragma region MODEL
	thread t1(Load_Vana_Model_1, m_pGameInstance, m_pDevice, m_pContext);
	thread t2(Load_Vana_Model_2, m_pGameInstance, m_pDevice, m_pContext);
	thread t3(Load_Vana_Model_3, m_pGameInstance, m_pDevice, m_pContext);
	thread t4(Load_Vana_Model_4, m_pGameInstance, m_pDevice, m_pContext);
	thread t5(Load_Vana_Model_5, m_pGameInstance, m_pDevice, m_pContext);
	thread t6(Load_Vana_Model_6, m_pGameInstance, m_pDevice, m_pContext);
	thread t7(Load_Vana_Model_7, m_pGameInstance, m_pDevice, m_pContext);
	//thread t8(Load_Vana_Model_8, m_pGameInstance, m_pDevice, m_pContext);
	thread t9(Load_Vana_Model_9, m_pGameInstance, m_pDevice, m_pContext);
	thread t10(Load_Vana_Model_10, m_pGameInstance, m_pDevice, m_pContext);

#pragma endregion //MODEL
	Purge_Prototype();

	CRoom::ROOM_DESC desc;
	desc.eLevel = LEVEL_VANA;
	desc.eRoomType = ROOM_DEFAULT;

	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Vana_Cross_1.txt";
	m_RoomPrototype[RST_CROSS].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Vana_Cross_2.txt";
	m_RoomPrototype[RST_CROSS].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Vana_Cross_3.txt";
	m_RoomPrototype[RST_CROSS].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Vana_Cross_4.txt";
	m_RoomPrototype[RST_CROSS].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	//
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Vana_I_1.txt";
	m_RoomPrototype[RST_I].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Vana_I_2.txt";
	m_RoomPrototype[RST_I].push_back(CRoom::Create(m_pDevice, m_pContext, desc));

	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Vana_L_1.txt";
	m_RoomPrototype[RST_L].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Vana_L_2.txt";
	m_RoomPrototype[RST_L].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Vana_L_3.txt";
	m_RoomPrototype[RST_L].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Vana_L_4.txt";
	m_RoomPrototype[RST_L].push_back(CRoom::Create(m_pDevice, m_pContext, desc));

	m_RoomPrototype[RST_SPECIAL].resize(SRT_END);
	desc.eRoomType = ROOM_ARMORY;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Vana_Armory.txt";
	m_RoomPrototype[RST_SPECIAL][ARMORY] = (CRoom::Create(m_pDevice, m_pContext, desc));
	desc.eRoomType = ROOM_BOSS;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Vana_Boss.txt";
	m_RoomPrototype[RST_SPECIAL][BOSS] = (CRoom::Create(m_pDevice, m_pContext, desc));
	desc.eRoomType = ROOM_SHOP;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Vana_Shop.txt";
	m_RoomPrototype[RST_SPECIAL][SHOP] = (CRoom::Create(m_pDevice, m_pContext, desc));
	desc.eRoomType = ROOM_START;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Vana_Start.txt";
	m_RoomPrototype[RST_SPECIAL][START] = (CRoom::Create(m_pDevice, m_pContext, desc));
	desc.eRoomType = ROOM_CHSET;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Vana_Treasure.txt";
	m_RoomPrototype[RST_SPECIAL][TREASURE] = (CRoom::Create(m_pDevice, m_pContext, desc));

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	//t8.join();
	t9.join();
	t10.join();
}

void CRoomManager::Ready_Rooms_Svart()
{
	thread t1(Load_Svart_Model_1, m_pGameInstance, m_pDevice, m_pContext);
	thread t2(Load_Svart_Model_2, m_pGameInstance, m_pDevice, m_pContext);
	thread t3(Load_Svart_Model_3, m_pGameInstance, m_pDevice, m_pContext);
	thread t4(Load_Svart_Model_4, m_pGameInstance, m_pDevice, m_pContext);
	thread t5(Load_Svart_Model_5, m_pGameInstance, m_pDevice, m_pContext);
	thread t6(Load_Svart_Model_6, m_pGameInstance, m_pDevice, m_pContext);
	Purge_Prototype();

	CRoom::ROOM_DESC desc;
	desc.eLevel = LEVEL_SVART;
	desc.eRoomType = ROOM_DEFAULT;
	m_RoomPrototype[0].resize(SVART_END);

	desc.eRoomType = ROOM_CHOICE;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Svart_Choice.txt";
	m_RoomPrototype[0][CHOICE_SV] = CRoom::Create(m_pDevice, m_pContext, desc);

	desc.eRoomType = ROOM_LIBRARY;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Svart_Lib.txt";
	m_RoomPrototype[0][LIB_SV] = CRoom::Create(m_pDevice, m_pContext, desc);

	desc.eRoomType = ROOM_CHALLENGE;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Svart_Portal.txt";
	m_RoomPrototype[0][PORTAL_SV] = CRoom::Create(m_pDevice, m_pContext, desc);

	desc.eRoomType = ROOM_DEFAULT;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Svart_Straight.txt";
	m_RoomPrototype[0][STRAIGHT_SV] = CRoom::Create(m_pDevice, m_pContext, desc);

	desc.eRoomType = ROOM_CHSET;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Svart_Treasure.txt";
	m_RoomPrototype[0][TREASURE_SV] = CRoom::Create(m_pDevice, m_pContext, desc);

	desc.eRoomType = ROOM_START;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Svart_Start.txt";
	m_RoomPrototype[0][START_SV] = CRoom::Create(m_pDevice, m_pContext, desc);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
}

void CRoomManager::Ready_Rooms_Hel()
{
	//
#pragma region MODEL
	thread t1(Load_Hel_Model_1, m_pGameInstance, m_pDevice, m_pContext);
	thread t6(Load_Hel_Model_6, m_pGameInstance, m_pDevice, m_pContext);
	thread t7(Load_Hel_Model_7, m_pGameInstance, m_pDevice, m_pContext);
	thread t8(Load_Hel_Model_8, m_pGameInstance, m_pDevice, m_pContext);
	thread t9(Load_Hel_Model_9, m_pGameInstance, m_pDevice, m_pContext);
	thread t10(Load_Hel_Model_10, m_pGameInstance, m_pDevice, m_pContext);
	thread t11(Load_Hel_Model_11, m_pGameInstance, m_pDevice, m_pContext);
	thread t12(Load_Hel_Model_12, m_pGameInstance, m_pDevice, m_pContext);
	thread t13(Load_Hel_Model_13, m_pGameInstance, m_pDevice, m_pContext);
	thread t14(Load_Hel_Model_14, m_pGameInstance, m_pDevice, m_pContext);
	thread t15(Load_Hel_Model_15, m_pGameInstance, m_pDevice, m_pContext);
	thread t16(Load_Hel_Model_16, m_pGameInstance, m_pDevice, m_pContext);
#pragma endregion //MODEL
	Purge_Prototype();

	CRoom::ROOM_DESC desc;
	desc.eLevel = LEVEL_HEL;
	desc.eRoomType = ROOM_DEFAULT;
	
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Helhime_Cross_1.txt";
	m_RoomPrototype[RST_CROSS].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Helhime_Cross_2.txt";
	m_RoomPrototype[RST_CROSS].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Helhime_Cross_3.txt";
	m_RoomPrototype[RST_CROSS].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	//
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Helhime_I_2.txt";
	m_RoomPrototype[RST_I].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Helhime_I_3.txt";
	m_RoomPrototype[RST_I].push_back(CRoom::Create(m_pDevice, m_pContext, desc));

	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Helhime_L_1.txt";
	m_RoomPrototype[RST_L].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Helhime_L_2.txt";
	m_RoomPrototype[RST_L].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Helhime_L_3.txt";
	m_RoomPrototype[RST_L].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Helhime_L_4.txt";
	m_RoomPrototype[RST_L].push_back(CRoom::Create(m_pDevice, m_pContext, desc));
	
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Helhime_T_1.txt";
	m_RoomPrototype[RST_T].push_back(CRoom::Create(m_pDevice, m_pContext, desc));

	m_RoomPrototype[RST_SPECIAL].resize(SRT_END);
	desc.eRoomType = ROOM_ARMORY;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Helhime_Armory.txt";
	m_RoomPrototype[RST_SPECIAL][ARMORY] = (CRoom::Create(m_pDevice, m_pContext, desc));
	desc.eRoomType = ROOM_BOSS;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Helhime_Boss.txt";
	m_RoomPrototype[RST_SPECIAL][BOSS] = (CRoom::Create(m_pDevice, m_pContext, desc));
	desc.eRoomType = ROOM_SHOP;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Helhime_Shop.txt";
	m_RoomPrototype[RST_SPECIAL][SHOP] = (CRoom::Create(m_pDevice, m_pContext, desc));
	desc.eRoomType = ROOM_START;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Helhime_Start.txt";
	m_RoomPrototype[RST_SPECIAL][START] = (CRoom::Create(m_pDevice, m_pContext, desc));
	desc.eRoomType = ROOM_CHSET;
	desc.strRoomFilePath = "../Bin/Resource/Room/Room_Helhime_Treasure.txt";
	m_RoomPrototype[RST_SPECIAL][TREASURE] = (CRoom::Create(m_pDevice, m_pContext, desc));

	t1.join();
	t6.join();
	t7.join();
	t8.join();
	t9.join();
	t10.join();
	t11.join();
	t12.join();
	t13.join();
	t14.join();
	t15.join();
	t16.join();
}

void CRoomManager::Ready_Rooms_BossRush()
{
	thread t1(Load_BossRush, m_pGameInstance, m_pDevice, m_pContext);
	Purge_Prototype();
	CRoom::ROOM_DESC desc;
	desc.eLevel = LEVEL_BOSSRUSH;
	desc.eRoomType = ROOM_BOSS;


	m_RoomPrototype[RST_SPECIAL].resize(5);

	desc.strRoomFilePath = "../Bin/Resource/Room/Room_BossRush_Alvis.txt";
	m_RoomPrototype[RST_SPECIAL][0] = (CRoom::Create(m_pDevice, m_pContext, desc));

	desc.strRoomFilePath = "../Bin/Resource/Room/Room_BossRush_Surt.txt";
	m_RoomPrototype[RST_SPECIAL][1] = (CRoom::Create(m_pDevice, m_pContext, desc));

	desc.strRoomFilePath = "../Bin/Resource/Room/Room_BossRush_Gideon.txt";
	m_RoomPrototype[RST_SPECIAL][2] = (CRoom::Create(m_pDevice, m_pContext, desc));
	t1.join();
}

void CRoomManager::Start(LEVEL eLevel)
{
	if(eLevel != LEVEL_BOSSRUSH)
	{
		m_pCurrentRoom = m_Rooms[2][2];
		m_Rooms[2][2]->Enter_Room(-1, eLevel);
		m_iCurrentRoomIdx = 12;
	}
	else
	{
		m_pCurrentRoom = m_Rooms[0][0];
		m_Rooms[0][0]->Enter_Room(-1, eLevel);
		m_iCurrentRoomIdx = 0;
	}
}

void CRoomManager::Purge_Prototype()
{
	for (auto& vec : m_RoomPrototype)
	{
		for (_uint i = 0; i < vec.size(); ++i)
		{
			Safe_Release(vec[i]);
		}
		//for (auto& pRoom : vec)
		//{
		//	Safe_Release(pRoom);
		//}
		vec.clear();
	}
}

void CRoomManager::Register_PooledObjectTag(_wstring strTag)
{
	m_PooledObjectToClear.insert(strTag);
}

void CRoomManager::Free()
{
	Purge_Prototype();
	Purge_Rooms();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
	__super::Free();
}

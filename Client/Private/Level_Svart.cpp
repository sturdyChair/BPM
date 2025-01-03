
#include "..\Public\Level_Svart.h"
#include "UIBase.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Beat_Manager.h"
#include "UI_Controller.h"
#include "Player.h"
#include "Player_Manager.h"
#include "UI_Include.h"
#include "Portal.h"
#include "Monster_Base.h"
#include "RoomManager.h"
#include "Weapon.h"
#include "EffectObject.h"

CLevel_Svart::CLevel_Svart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{

}

HRESULT CLevel_Svart::Initialize()
{
	m_iLevelID = LEVEL_SVART;
	m_pGameInstance->Stop_Sound(SOUND_CHANNEL::MUSIC, L"Asgard 1_Loading Ambience_001");

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	///* 로딩 레벨에 보여주기위한 객체(배경, 로딩바, 로딩상태폰트)들을 생성한다. */
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
	//	return E_FAIL;


	if (FAILED(Ready_LandObjects()))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(CUIBase::UILayerTag)))
		return E_FAIL;

	CBeat_Manager::Get_Instance()->Queue_Sequence(L"Svart");
	_uint iHel = 2;
	m_pGameInstance->Push_Object_From_Pool(TEXT("Prototype_GameObject_UI_Level_Title"), m_iLevelID, CUIBase::UILayerTag, &iHel);

	//if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Player"))))
	//	return E_FAIL;
	//CPlayer_Manager::Get_Instance()->Register_Player((CPlayer*)(m_pGameInstance->Push_Object_From_Pool(TEXT("Prototype_GameObject_Player"), m_iLevelID, L"Player")));
	//CPlayer_Manager::Get_Instance()->Get_Player()->Equip_Weapon(((CWeapon*)(m_pGameInstance->Push_Object_Hijack(TEXT("Prototype_GameObject_Weapon_Pistol")))));
	auto pPlayer = CPlayer_Manager::Get_Instance()->Get_Player();
	m_pGameInstance->Push_Object(m_iLevelID, L"Player", pPlayer);
	m_pGameInstance->Push_Object(m_iLevelID, L"Layer_Camera", pPlayer->Get_Camera());
	_matrix Transform = XMMatrixIdentity();
	Transform.r[3] = XMVectorSet(-25.f, 5.f, 15.f, 1.f);
	pPlayer->Set_Position(Transform);
	CRoomManager::Get_Instance()->Start(LEVEL_SVART);
	//25 5 15
	CEffectObject::Set_AmbientColor({ 0.9f,0.9f,0.4f,1.f });
	m_pGameInstance->Set_AmbientColor({ 0.1f,0.1f,0.0f,1.f });
	m_pGameInstance->Remove_DirectionalLight(L"DirectionMain_0");
	m_pGameInstance->Remove_DirectionalLight(L"DirectionMain_1");
	DirectionalLight desc{};
	desc.color = { 1.0f,0.3f,0.35f };
	desc.direction = { 0.5f,-0.5f,0.f };
	desc.intensity = 1.f;
	desc.specColor = { 1.0f,0.6f,0.35f };

	m_pGameInstance->Add_Light(L"DirectionMain_0", desc);
	desc.color = { 1.0f,0.3f,0.35f };
	desc.direction = { -0.5f,-0.5f,0.f };
	desc.intensity = 1.f;
	desc.specColor = { 1.0f,0.6f,0.35f };
	m_pGameInstance->Add_Light(L"DirectionMain_1", desc);
	return S_OK;
}

void CLevel_Svart::Tick(_float fTimeDelta)
{
	if (GetFocus())
		SetCursorPos(g_iWinSizeX / 2, g_iWinSizeY / 2);
	_int iFps = (_int)(1.f / fTimeDelta);
	SetWindowText(g_hWnd, to_wstring(iFps).c_str());
}

HRESULT CLevel_Svart::Render()
{

	//SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Svart::Ready_Layer_Camera(const wstring& strLayerTag)
{
	return S_OK;
}

HRESULT CLevel_Svart::Ready_Layer_BackGround(const wstring& strLayerTag)
{

	return S_OK;
}

HRESULT CLevel_Svart::Ready_Layer_Effect(const wstring& strLayerTag)
{


	return S_OK;
}

HRESULT CLevel_Svart::Ready_Layer_UI(const wstring& strLayerTag)
{
	CUI_Controller* pUI_Controller = CUI_Controller::Get_Instance();
	//CUI_Controller::CONTROLLED_UI Desc;

	//Desc.pUI_CrossHair = (CUI_CrossHair*)m_pGameInstance->Add_Clone_Return(m_iLevelID, strLayerTag, TEXT("Prototype_GameObject_UI_CrossHair"));
	//Desc.pUI_Hp = (CUI_Hp*)m_pGameInstance->Add_Clone_Return(m_iLevelID, strLayerTag, TEXT("Prototype_GameObject_UI_Hp"));
	//Desc.pUI_EquipSlot = (CUI_EquipSlot*)m_pGameInstance->Add_Clone_Return(m_iLevelID, strLayerTag, TEXT("Prototype_GameObject_UI_EquipSlot"));
	//Desc.pUI_WeaponSlot = (CUI_WeaponSlot*)m_pGameInstance->Add_Clone_Return(m_iLevelID, strLayerTag, TEXT("Prototype_GameObject_UI_WeaponSlot"));
	//Desc.pUI_AbilitySlot = (CUI_AbilitySlot*)m_pGameInstance->Add_Clone_Return(m_iLevelID, strLayerTag, TEXT("Prototype_GameObject_UI_AbilitySlot"));
	//Desc.pUI_Score = (CUI_Score*)m_pGameInstance->Add_Clone_Return(m_iLevelID, strLayerTag, TEXT("Prototype_GameObject_UI_Score"));
	//Desc.pUI_MiniMap = (CUI_MiniMap*)m_pGameInstance->Add_Clone_Return(m_iLevelID, strLayerTag, TEXT("Prototype_GameObject_UI_MiniMap"));
	//Desc.pUI_SwapItem = (CUI_SwapItem*)m_pGameInstance->Add_Clone_Return(m_iLevelID, strLayerTag, TEXT("Prototype_GameObject_UI_SwapItem"));
	//Desc.pUI_MiniMap->Grid_Initialize();
	pUI_Controller->Get_MiniMap()->Grid_Initialize();
	pUI_Controller->Push_UI(LEVEL_SVART);

	if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_UI_Level_Title"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_UI_RoomClear"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_UI_BossTitle"))))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Svart::Ready_LandObjects()
{

	//CMonster_Base::MONSTER_DESC desc;
	//XMStoreFloat4x4(&desc.Transformation, XMMatrixTranslation(0.f, 20.f, 5.f));

	//m_pGameInstance->Push_Object_From_Pool(TEXT("Prototype_GameObject_Monster_Fly"), m_iLevelID, L"Monster", &desc);

	//_float4x4 Weapon = {1.f,0.f,0.f,0.f,
	//					0.f,1.f,0.f,0.f,
	//					0.f,0.f,1.f,0.f,
	//					5.f,1.f,0.f,1.f};

	//Weapon._41 = 12.f;
	//m_pGameInstance->Push_Object_From_Pool(TEXT("Prototype_GameObject_Weapon_Pistol"), m_iLevelID, L"Weapon", nullptr, &Weapon);
	//Weapon._41 = -4.f;
	//m_pGameInstance->Push_Object_From_Pool(TEXT("Prototype_GameObject_Weapon_Gatling"), m_iLevelID, L"Weapon", nullptr, &Weapon);
	//Weapon._41 = -9.f;
	//m_pGameInstance->Push_Object_From_Pool(TEXT("Prototype_GameObject_Weapon_RocketLauncher"), m_iLevelID, L"Weapon", nullptr, &Weapon);


	return S_OK;
}




CLevel_Svart* CLevel_Svart::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Svart* pInstance = new CLevel_Svart(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Svart"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Svart::Free()
{
	__super::Free();

}

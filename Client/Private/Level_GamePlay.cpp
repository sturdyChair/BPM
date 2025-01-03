
#include "..\Public\Level_GamePlay.h"
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
#include "CharacterController.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext } 
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	m_iLevelID = LEVEL_GAMEPLAY;
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

	CBeat_Manager::Get_Instance()->Queue_Sequence(Asgard_1);
	_uint iAsgard = 0;
	m_pGameInstance->Push_Object_From_Pool(TEXT("Prototype_GameObject_UI_Level_Title"), m_iLevelID, CUIBase::UILayerTag, &iAsgard);
	if(s_ePC != PC_RUN)
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_Player"))))
			return E_FAIL;
		CPlayer_Manager::Get_Instance()->Register_Player((CPlayer*)(m_pGameInstance->Push_Object_From_Pool(TEXT("Prototype_GameObject_Player"), m_iLevelID, L"Player")));
		CPlayer_Manager::Get_Instance()->Get_Player()->Equip_Weapon(((CWeapon*)(m_pGameInstance->Push_Object_Hijack(TEXT("Prototype_GameObject_Weapon_Pistol")))));
	}
	else
	{
		if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_TPS_Player"))))
			return E_FAIL;
		CPlayer_Manager::Get_Instance()->Register_Player((CPlayer*)(m_pGameInstance->Push_Object_From_Pool(TEXT("Prototype_GameObject_TPS_Player"), m_iLevelID, L"Player")));
	}


	CRoomManager::Get_Instance()->Start(LEVEL_GAMEPLAY);

	CEffectObject::Set_AmbientColor({ 1.0f,0.8f,0.5f,1.f });
	m_pGameInstance->Set_AmbientColor({ 0.3f,0.05f,0.05f,1.f });
	m_pGameInstance->Remove_DirectionalLight(L"DirectionMain_0");
	m_pGameInstance->Remove_DirectionalLight(L"DirectionMain_1");
	DirectionalLight desc{};

	desc.color = { 0.9f,0.6f,0.25f };
	desc.direction = { 0.5f,-0.5f,0.f };
	desc.intensity = 1.f;
	desc.specColor = { 0.9f,0.8f,0.5f };
	m_pGameInstance->Add_Light(L"DirectionMain_0", desc);
	desc.color = { 0.7f,0.5f,0.2f };
	desc.direction = { -0.5f,-0.5f,0.f };
	desc.intensity = 1.f;
	desc.specColor = { 0.9f,0.8f,0.5f };
	m_pGameInstance->Add_Light(L"DirectionMain_1", desc);
	//CCharacterController::s_fGlobalGravity = 0.5f;
	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	if (GetFocus())
		SetCursorPos(g_iWinSizeX / 2, g_iWinSizeY / 2);
	_int iFps = (_int)(1.f / fTimeDelta);
	SetWindowText(g_hWnd, to_wstring(iFps).c_str());
}

HRESULT CLevel_GamePlay::Render()
{
	
	//SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const wstring & strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC CFD{};
	CFD.fAspect = (_float)g_iWinSizeX / (_float)g_iWinSizeY;
	CFD.fFar = 300.f;
	CFD.fFovy = XM_PI * 0.33f;
	CFD.fMouseSensor = 0.1f;
	CFD.fNear = 0.1f;
	CFD.fRotationPerSec = XM_PI;
	CFD.fSpeedPerSec = 10.f;
	CFD.strTag = L"DEFAULT";
	CFD.vEye = _float4{0.f, 10.f, -10.f, 1.f};
	CFD.vAt = _float4{ 0.f,0.f,0.f,1.f };
	CFD.strTag = L"FPS";
	if (s_ePC != PC_RUN)
	{
		if (FAILED(m_pGameInstance->Add_Clone(m_iLevelID, strLayerTag, TEXT("Prototype_GameObject_Camera_FPS"), &CFD)))
			return E_FAIL;
		m_pGameInstance->Set_MainCamera(L"FPS");
	}
	else
	{
		CFD.strTag = L"TPS";
		if (FAILED(m_pGameInstance->Add_Clone(m_iLevelID, strLayerTag, TEXT("Prototype_GameObject_Camera_TPS"), &CFD)))
			return E_FAIL;
		m_pGameInstance->Set_MainCamera(L"TPS");
	}


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const wstring & strLayerTag)
{	

	//m_pGameInstance->Push_Object_From_Pool(TEXT("Prototype_GameObject_Terrain"), m_iLevelID, strLayerTag);


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const wstring & strLayerTag)
{


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const wstring& strLayerTag)
{
	CUI_Controller* pUI_Controller = CUI_Controller::Get_Instance();
	CUI_Controller::CONTROLLED_UI Desc;

	Desc.pUI_CrossHair		= (CUI_CrossHair*)	m_pGameInstance->Add_Clone_Return(m_iLevelID, strLayerTag, TEXT("Prototype_GameObject_UI_CrossHair"));
	Desc.pUI_Hp				= (CUI_Hp*)			m_pGameInstance->Add_Clone_Return(m_iLevelID, strLayerTag, TEXT("Prototype_GameObject_UI_Hp"));
	Desc.pUI_EquipSlot		= (CUI_EquipSlot*)	m_pGameInstance->Add_Clone_Return(m_iLevelID, strLayerTag, TEXT("Prototype_GameObject_UI_EquipSlot"));
	Desc.pUI_WeaponSlot		= (CUI_WeaponSlot*)	m_pGameInstance->Add_Clone_Return(m_iLevelID, strLayerTag, TEXT("Prototype_GameObject_UI_WeaponSlot"));
	Desc.pUI_AbilitySlot	= (CUI_AbilitySlot*)m_pGameInstance->Add_Clone_Return(m_iLevelID, strLayerTag, TEXT("Prototype_GameObject_UI_AbilitySlot"));
	Desc.pUI_Score			= (CUI_Score*)		m_pGameInstance->Add_Clone_Return(m_iLevelID, strLayerTag, TEXT("Prototype_GameObject_UI_Score"));
	Desc.pUI_MiniMap		= (CUI_MiniMap*)	m_pGameInstance->Add_Clone_Return(m_iLevelID, strLayerTag, TEXT("Prototype_GameObject_UI_MiniMap"));
	Desc.pUI_SwapItem		= (CUI_SwapItem*)	m_pGameInstance->Add_Clone_Return(m_iLevelID, strLayerTag, TEXT("Prototype_GameObject_UI_SwapItem"));
	Desc.pUI_MiniMap->Grid_Initialize();
	pUI_Controller->Set_UI(&Desc);

	if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_UI_Level_Title"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_UI_RoomClear"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Pool_Object(TEXT("Prototype_GameObject_UI_BossTitle"))))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_LandObjects()
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




CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();

}

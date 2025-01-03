
#include "..\Public\Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "Beat_Manager.h"
#include "Camera_Logo.h"
#include "PlayerSelectModel.h"
#include "UI_Include.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext } 
{

}

HRESULT CLevel_Logo::Initialize()
{
	m_iLevelID = LEVEL_LOGO;
	Ready_Layer_BackGround(L"Background");
	CBeat_Manager::Get_Instance()->Queue_Sequence(L"Menu");
	CCamera::CAMERA_DESC CFD{};
	CFD.fAspect = (_float)g_iWinSizeX / (_float)g_iWinSizeY;
	CFD.fFar = 300.f;
	CFD.fFovy = XM_PI * 0.33f;
	CFD.fNear = 0.1f;
	CFD.fRotationPerSec = XM_PI;
	CFD.fSpeedPerSec = 10.f;
	CFD.vEye = _float4{ 0.f, 0.f, 15.f, 1.f };
	CFD.vAt = _float4{ 0.f,0.f,0.f,1.f };
	CFD.strTag = L"Logo";
	m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Model_Tester", m_iLevelID, L"Room");
	m_pGameInstance->Add_Clone(LEVEL_LOGO, L"Camera", L"Prototype_GameObject_Camera_Logo", &CFD);
	m_pGameInstance->Set_MainCamera(L"Logo");
	m_pCamera = (CCamera_Logo*)m_pGameInstance->Get_Camera(L"Logo");
	Safe_AddRef(m_pCamera);

	_float4x4 PSMmat;
	XMStoreFloat4x4(&PSMmat, XMMatrixRotationY(XM_PIDIV2 * 0.125f)* XMMatrixTranslation(0.f, 1.5f, 11.5f));
	m_pPSM[0] = (CPlayerSelectModel*)m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_PlayerSelectModel_Serath", m_iLevelID, L"PSM", nullptr, &PSMmat);
	XMStoreFloat4x4(&PSMmat, XMMatrixRotationY(XM_PIDIV2) * XMMatrixTranslation(11.5f, 0.f, 0.f));
	m_pPSM[1] = (CPlayerSelectModel*)m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_PlayerSelectModel_Frey", m_iLevelID, L"PSM", nullptr, &PSMmat);
	XMStoreFloat4x4(&PSMmat, XMMatrixRotationY(XM_PI) * XMMatrixTranslation(0.f, 0.f, -12.f));
	m_pPSM[2] = (CPlayerSelectModel*)m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_PlayerSelectModel_Hildr", m_iLevelID, L"PSM", nullptr, &PSMmat);
	XMStoreFloat4x4(&PSMmat, XMMatrixRotationY(-XM_PIDIV2) * XMMatrixTranslation(-11.5f, 0.f, 0.f));
	m_pPSM[3] = (CPlayerSelectModel*)m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_PlayerSelectModel_Run", m_iLevelID, L"PSM", nullptr, &PSMmat);
	for (auto& pPSM : m_pPSM)
	{
		Safe_AddRef(pPSM);
	}


	CUIBase::UI_DESC uiDesc;
	uiDesc.iCurrLevel = m_iLevelID;
	uiDesc.pParentUI = nullptr;
	uiDesc.vSize = _float3(12.f, 15.f, 1.f);
	uiDesc.vPosition = _float3{ 0.f, (_float)g_iWinSizeY * (-0.4f),0.1f };
	m_pTextBox_GameMode = (CUI_TextBox*)m_pGameInstance->Add_Clone_Return(m_iLevelID, CUIBase::UILayerTag, L"Prototype_GameObject_UI_TextBox", &uiDesc);
	m_pTextBox_GameMode->Set_FontSize(_float3(20.f, 25.f, 1.f));
	m_pTextBox_GameMode->Set_Stride(1.5f);
	Safe_AddRef(m_pTextBox_GameMode);

	uiDesc.iCurrLevel = m_iLevelID;
	uiDesc.pParentUI = nullptr;
	uiDesc.vSize = _float3(12.f, 15.f, 1.f);
	uiDesc.vPosition = _float3{ 0.f, (_float)g_iWinSizeY * (-0.34f),0.1f };
	m_pTextBox_Selected = (CUI_TextBox*)m_pGameInstance->Add_Clone_Return(m_iLevelID, CUIBase::UILayerTag, L"Prototype_GameObject_UI_TextBox", &uiDesc);
	m_pTextBox_Selected->Set_FontSize(_float3(20.f, 25.f, 1.f));
	m_pTextBox_Selected->Set_Stride(1.5f);
	Safe_AddRef(m_pTextBox_Selected);


	m_pGameInstance->Set_AmbientColor({ 0.3f,0.05f,0.05f,1.f });
	m_pGameInstance->Remove_DirectionalLight(L"DirectionMain_0");
	m_pGameInstance->Remove_DirectionalLight(L"DirectionMain_1");
	DirectionalLight desc{};

	desc.color = { 0.9f,0.6f,0.25f };
	desc.direction = { 0.5f,-0.5f,0.f };
	desc.intensity = 1.f;
	desc.specColor = { 0.9f,0.6f,0.25f };
	m_pGameInstance->Add_Light(L"DirectionMain_0", desc);
	desc.color = { 0.7f,0.5f,0.2f };
	desc.direction = { -0.5f,-0.5f,0.f };
	desc.intensity = 1.f;

	m_pGameInstance->Add_Light(L"DirectionMain_1", desc);

	return S_OK;
}

void CLevel_Logo::Tick(_float fTimeDelta)
{
	if (m_pGameInstance->Get_KeyDown(DIK_LEFT))
	{
		m_pCamera->Set_TargetAngle(m_pCamera->Get_TargetAngle() - XM_PIDIV2);
		m_pPSM[m_iSelected]->Selected(false);
		--m_iSelected;
		if (m_iSelected < 0)
			m_iSelected = 3;

		m_pPSM[m_iSelected]->Selected(true);
	}
	if (m_pGameInstance->Get_KeyDown(DIK_RIGHT))
	{
		m_pCamera->Set_TargetAngle(m_pCamera->Get_TargetAngle() + XM_PIDIV2);
		m_pPSM[m_iSelected]->Selected(false);
		++m_iSelected;
		m_iSelected %= 4;

		m_pPSM[m_iSelected]->Selected(true);
	}
	if (m_pGameInstance->Get_KeyDown(DIK_UP)|| m_pGameInstance->Get_KeyDown(DIK_DOWN))
	{
		m_iModeSelect += 1;	
		m_iModeSelect %= 2;
	}
	switch (m_iSelected)
	{
	case 0:
		m_pTextBox_Selected->Set_Text(L"Goll");
		break;
	case 1:
		m_pTextBox_Selected->Set_Text(L"Frey");
		break;
	case 2:
		m_pTextBox_Selected->Set_Text(L"Hildr");
		break;
	case 3:
		m_pTextBox_Selected->Set_Text(L"Run");
		break;
	default:
		break;
	}
	if (m_iModeSelect)
	{
		m_pTextBox_GameMode->Set_Text(L"Adventure");
	}
	else
	{
		m_pTextBox_GameMode->Set_Text(L"BossRush");
	}

	if (m_pGameInstance->Get_KeyDown(DIK_SPACE))
	{
		if (m_iSelected == 1)
		{
			s_ePC = PC_FREY;
		}
		else if (m_iSelected == 2)
		{
			s_ePC = PC_HILDR;
		}
		else if(m_iSelected == 3)
		{
			s_ePC = PC_RUN;
		}
		else
		{
			s_ePC = PC_GOLL;
		}
		if(m_iModeSelect)
		{

			if (FAILED(m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
				return;
		}
		else
		{
			if (FAILED(m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_BOSSRUSH))))
				return;
		}
	}
	if (GetKeyState('T') & 0x8000)
	{
		if (FAILED(m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_TOOL))))
			return;
	}

}

HRESULT CLevel_Logo::Render()
{
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_LOGO, strLayerTag, TEXT("Prototype_GameObject_LogoBackGround"))))
		return E_FAIL;

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Logo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	m_pGameInstance->Clear_Pool(L"Prototype_GameObject_PlayerSelectModel_Run");
	m_pGameInstance->Clear_Pool(L"Prototype_GameObject_PlayerSelectModel_Serath");
	m_pGameInstance->Clear_Pool(L"Prototype_GameObject_PlayerSelectModel_Hildr");
	m_pGameInstance->Clear_Pool(L"Prototype_GameObject_PlayerSelectModel_Frey");
	m_pGameInstance->Clear_Pool(L"Prototype_GameObject_Model_Tester");
	__super::Free();
	Safe_Release(m_pCamera);
	for (auto& pPSM : m_pPSM)
	{
		Safe_Release(pPSM);
	}
	Safe_Release(m_pTextBox_GameMode);
	Safe_Release(m_pTextBox_Selected);

}

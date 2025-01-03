
#include "..\Public\Level_Tool.h"
#include "ImGui_Manager.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Beat_Manager.h"
#include "Camera_Free.h"

CLevel_Tool::CLevel_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
	m_pImguiManager =
	CImGui_Manager::Get_Instance();

}

HRESULT CLevel_Tool::Initialize()
{
	m_iLevelID = LEVEL_TOOL;
	CCamera_Free::CAMERA_FREE_DESC CFD{};
	CFD.fAspect = (_float)g_iWinSizeX / (_float)g_iWinSizeY;
	CFD.fFar = 300.f;
	CFD.fFovy = XM_PI * 0.33f;
	CFD.fMouseSensor = 0.1f;
	CFD.fNear = 0.1f;
	CFD.fRotationPerSec = XM_PI;
	CFD.fSpeedPerSec = 10.f;
	CFD.strTag = L"DEFAULT";
	CFD.vEye = _float4{ 0.f, 10.f, -10.f, 1.f };
	CFD.vAt = _float4{ 0.f,0.f,0.f,1.f };
	if (FAILED(m_pGameInstance->Add_Clone(m_iLevelID, L"Camera", TEXT("Prototype_GameObject_Camera_Free"), &CFD)))
		return E_FAIL;
	m_pGameInstance->Set_MainCamera(L"DEFAULT");
	
	Ready_Layer_BackGround(L"Background");
	CBeat_Manager::Get_Instance()->Queue_Sequence(L"Vana_Boss");

	return S_OK;
}

void CLevel_Tool::Tick(_float fTimeDelta)
{

}

HRESULT CLevel_Tool::Render()
{
	m_pImguiManager->Tick();
	m_pImguiManager->Render();
	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_BackGround(const wstring& strLayerTag)
{
	//if (FAILED(m_pGameInstance->Add_Clone(Level_Tool, strLayerTag, TEXT("Prototype_GameObject_BackGround"))))
	//	return E_FAIL;

	return S_OK;
}

CLevel_Tool* CLevel_Tool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Tool* pInstance = new CLevel_Tool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Tool"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Tool::Free()
{

	__super::Free();

}

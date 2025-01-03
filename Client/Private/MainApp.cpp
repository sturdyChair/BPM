
#include "../Public/MainApp.h"
#include "Model.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "Beat_Manager.h"
#include "GameObject.h"
#include "VIBuffer_Terrain.h"
#include "CharacterController.h"
#include "UI_Controller.h"
#include "ImGui_Manager.h"
#include "Player_Manager.h"
#include "RoomManager.h"
#include "Transform.h"
#include "Texture.h"

CMainApp::CMainApp()
	: m_pGameInstance {CGameInstance::Get_Instance()}
{
	Safe_AddRef(m_pGameInstance);

	
}

HRESULT CMainApp::Initialize()
{
	ENGINE_DESC			EngineDesc{};

	EngineDesc.hWnd = g_hWnd;
	EngineDesc.hInstance = g_hInst;
	EngineDesc.isWindowed = true;
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;
	Ready_Prototype_Component();
	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;	
	m_pBeatManager = CBeat_Manager::Get_Instance();
	m_pBeatManager->Initialize();
	m_pBeatManager->Set_Beat(0.340909f);

	m_pUI_Controller = CUI_Controller::Get_Instance();
	m_pUI_Controller->Initialize();
	m_pImguiManager = CImGui_Manager::Get_Instance();
	m_pImguiManager->Initialize(m_pDevice, m_pContext, g_hWnd);

	m_pPlayerManager = CPlayer_Manager::Get_Instance();
	m_pRoomManager = CRoomManager::Get_Instance();
	m_pRoomManager->Initialize(m_pDevice, m_pContext);

	m_pBeatManager->Set_DefaultTexture(CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resource/Texture/DefaultORM.png", 1),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resource/Texture/DefaultNormal.png", 1), CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resource/Texture/Noise.png", 1));
	
	ShowCursor(false);
	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{

	m_pBeatManager->Tick(fTimeDelta);
	m_pUI_Controller->Tick(fTimeDelta);

  	m_pGameInstance->Tick_Engine(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.0f, 0.f, 0.f, 1.f));
	
	m_pGameInstance->Clear_RTVs(_float4(0.0f, 0.f, 0.f, 1.f));

	m_pGameInstance->Clear_DepthStencil_View();

	m_pGameInstance->Draw();

	m_pGameInstance->Present();



	return S_OK;
}



HRESULT CMainApp::Open_Level(LEVEL eLevelID)
{
	if (FAILED(m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pDevice, m_pContext, eLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPosTex",
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxPosTex.hlsl", VTXPOSTEX::Elements, VTXPOSTEX::iNumElements));

	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Shader_VtxNorTex",
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxNorTex.hlsl", VTXNORTEX::Elements, VTXNORTEX::iNumElements));

	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh",
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxMesh.hlsl", VTXMESH::Elements, VTXMESH::iNumElements));

	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh",
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl", VTXANIMMESH::Elements, VTXANIMMESH::iNumElements));

	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Shader_VtxRectInstance",
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxRectInstance.hlsl", VTXRECTINSTANCE::Elements, VTXRECTINSTANCE::iNumElements));

	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Shader_VtxPointInstance",
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxPointInstance.hlsl", VTXPOINTINSTANCE::Elements, VTXPOINTINSTANCE::iNumElements));

	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect",
		CVIBuffer_Rect::Create(m_pDevice, m_pContext));
	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_VIBuffer_RectNorm",
		CVIBuffer_RectNorm::Create(m_pDevice, m_pContext));
	//m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Terrain",
	//	CVIBuffer_Terrain::Create(m_pDevice, m_pContext, L"../Bin/Resource/Texture/Terrain/Height1.bmp"));
	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Parallelogram",
		CVIBuffer_Parallelogram::Create(m_pDevice, m_pContext));

	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Transform",
		CTransform::Create(m_pDevice, m_pContext));


	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_CharacterController",
		CCharacterController::Create(m_pDevice, m_pContext));
	

	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_RigidBody",
		CRigidBody::Create(m_pDevice, m_pContext));

	//m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Collider_AABB",
	//	CCollider::Create(m_pDevice, m_pContext, COLLIDER_TYPE::AABB));

	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Texture_RedScreen",
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Lens/T_LowHealth_Screen2.png"), 1));
	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Texture_White",
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/Lens/White.png"), 1));
	m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Component_Texture_Logo",
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Texture/MenuLogo.png"), 1));
	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	__super::Free();
	m_pBeatManager->Kill_Instance();
	m_pUI_Controller->Kill_Instance();
	m_pPlayerManager->Kill_Instance();
	m_pRoomManager->Kill_Instance();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	Safe_Release(m_pGameInstance);
	m_pImguiManager->Free();
	m_pImguiManager->Destroy_Instance();
	CGameInstance::Release_Engine();
}


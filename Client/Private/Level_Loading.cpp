
#include "..\Public\Level_Loading.h"

#include "GameInstance.h"
#include "Loader.h"
#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include "Beat_Manager.h"
#include "Level_Tool.h"
#include "Level_Hel.h"
#include "Level_BossRush.h"
#include "Level_Ending.h"
#include "Level_Vana.h"
#include "Level_Svart.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext } 
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	m_iLevelID = LEVEL_LOADING;

	m_eNextLevelID = eNextLevelID;
	if(m_eNextLevelID == LEVEL_GAMEPLAY)
		CBeat_Manager::Get_Instance()->Queue_Sequence(L"Asgard_Loading");
	if (m_eNextLevelID == LEVEL_HEL)
		CBeat_Manager::Get_Instance()->Queue_Sequence(L"Asgard_Loading");
	if (m_eNextLevelID == LEVEL_BOSSRUSH)
		CBeat_Manager::Get_Instance()->Queue_Sequence(L"Asgard_Loading");
	if (m_eNextLevelID == LEVEL_VANA)
		CBeat_Manager::Get_Instance()->Queue_Sequence(L"Asgard_Loading");
	if (m_eNextLevelID == LEVEL_SVART)
		CBeat_Manager::Get_Instance()->Queue_Sequence(L"Asgard_Loading");
	/* 로딩 레벨에 보여주기위한 객체(배경, 로딩바, 로딩상태폰트)들을 생성한다. */
	if(m_eNextLevelID != LEVEL_LOGO && m_eNextLevelID != LEVEL_ENDING)
	{
		if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
			return E_FAIL;
	}

	/* 자원로딩을 위한 로더객체를 생성해준다. */
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;	
	m_pGameInstance->Simulate_Physx(false);
	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	if (true == m_pLoader->isFinished())
	{
		//if (GetKeyState(VK_SPACE) & 0x8000)
		//{
			m_pGameInstance->Simulate_Physx();
			CLevel*		pLevel = { nullptr };

			switch (m_eNextLevelID)
			{
			case LEVEL_LOGO:
				pLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_GAMEPLAY:
				pLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_TOOL:
				pLevel = CLevel_Tool::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_HEL:
				pLevel = CLevel_Hel::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_VANA:
				pLevel = CLevel_Vana::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_SVART:
				pLevel = CLevel_Svart::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_BOSSRUSH:
				pLevel = CLevel_BossRush::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_ENDING:
				pLevel = CLevel_Ending::Create(m_pDevice, m_pContext);
				break;
			}

			if (nullptr == pLevel)
				return;

			if (FAILED(m_pGameInstance->Change_Level(pLevel)))
				return;
		//}		
	}	
}

HRESULT CLevel_Loading::Render()
{	
	m_pLoader->Show_LoadingText();
	
	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const wstring & strLayerTag)
{
	LEVEL eLevel = m_eNextLevelID;
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_LOADING, strLayerTag, TEXT("Prototype_GameObject_BackGround"), &eLevel)))
		return E_FAIL;

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Loading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}

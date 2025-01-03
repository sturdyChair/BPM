
#include "..\Public\Level_Ending.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "Beat_Manager.h"

CLevel_Ending::CLevel_Ending(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{

}

HRESULT CLevel_Ending::Initialize()
{
	m_iLevelID = LEVEL_ENDING;
	Ready_Layer_BackGround(L"Background");
	CBeat_Manager::Get_Instance()->Queue_Sequence(L"Menu");
	return S_OK;
}

void CLevel_Ending::Tick(_float fTimeDelta)
{

}

HRESULT CLevel_Ending::Render()
{
	return S_OK;
}

HRESULT CLevel_Ending::Ready_Layer_BackGround(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_ENDING, strLayerTag, TEXT("Prototype_GameObject_LogoBackGround"))))
		return E_FAIL;

	return S_OK;
}

CLevel_Ending* CLevel_Ending::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Ending* pInstance = new CLevel_Ending(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Ending"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Ending::Free()
{
	__super::Free();

}

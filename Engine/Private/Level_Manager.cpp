#include "..\Public\Level_Manager.h"
#include "Level.h"

#include "GameInstance.h"

CLevel_Manager::CLevel_Manager()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CLevel_Manager::Change_Level(CLevel * pNewLevel)
{
	/* ���� ���������� �Ҵ�ƴ� �޸𸮵� �ؼ��ϳ�.  */
	if(nullptr != m_pCurrentLevel)
	{
		m_pGameInstance->Clear(m_pCurrentLevel->Get_LevelID());

	}

	Safe_Release(m_pCurrentLevel);

	m_pCurrentLevel = pNewLevel;	
	
	return S_OK;
}

void CLevel_Manager::Tick(_float fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->Tick(fTimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr == m_pCurrentLevel)
		return E_FAIL;

	m_pCurrentLevel->Render();

	return S_OK;
}

_uint CLevel_Manager::Get_Current_LevelID() const
{
	return m_pCurrentLevel->Get_LevelID();
}

CLevel_Manager * CLevel_Manager::Create()
{
	return new CLevel_Manager();
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pCurrentLevel);
}



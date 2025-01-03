#include "DelayedProcessManager.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Level.h"

CDelayedProcessManager::CDelayedProcessManager()
{}


void CDelayedProcessManager::Initialize()
{
}

void CDelayedProcessManager::Execute(DELAYED _tEvent)
{
	DELAYED_TYPE eType = _tEvent.eType;
	switch (eType)
	{
	case DELAYED_TYPE::CREATE_OBJECT:
		break;
	case DELAYED_TYPE::DELETE_OBJECT:
	{
		CGameObject* pObj = ((CGameObject*)_tEvent.wParam);
		pObj->Set_Dead();
		m_DeadObjList.push_back(pObj);
		Safe_AddRef(pObj);
	}
	break;
	case DELAYED_TYPE::CHANGE_LEVEL_DELAY:
	{
		CGameInstance::Get_Instance()->Change_Level((CLevel*)_tEvent.wParam);
		m_EventVec.clear();
	}
	break;

	default:
		break;
	}
}

void CDelayedProcessManager::Push_Dead_Obj(CGameObject* _pDeadObj)
{
	m_DeadObjList.push_back(_pDeadObj);
}

void CDelayedProcessManager::Tick(_float fTimeDelta)
{
	for (auto& elem : m_DeadObjList)
	{
		Safe_Release(elem);
	}
	m_DeadObjList.clear();
	for (auto& elem : m_EventVec)
	{
		Execute(elem);
	}
	m_EventVec.clear();

}

CDelayedProcessManager* CDelayedProcessManager::Create()
{
	CDelayedProcessManager* pInstance = new CDelayedProcessManager;

	return pInstance;
}

void CDelayedProcessManager::Free()
{
	m_EventVec.clear();
	for (auto& elem : m_DeadObjList)
	{
		Safe_Release(elem);
	}
	m_DeadObjList.clear();
}



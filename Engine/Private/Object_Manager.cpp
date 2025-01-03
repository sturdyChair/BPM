#include "..\Public\Object_Manager.h"

#include "Layer.h"
#include "GameObject.h"

CObject_Manager::CObject_Manager()
{
}

HRESULT CObject_Manager::Initialize(_uint iNumLevels)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_iNumLevels = iNumLevels;

	m_pLayers = new LAYERS[iNumLevels];
	
	return S_OK;
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& Pair : m_pLayers[iLevelIndex])
		Safe_Release(Pair.second);

	m_pLayers[iLevelIndex].clear();
}

HRESULT CObject_Manager::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr != Find_Prototype(strPrototypeTag))
		return E_FAIL;

	m_Prototypes.emplace(strPrototypeTag, pPrototype);

	return S_OK;
}

HRESULT CObject_Manager::Add_Clone(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strPrototypeTag, void * pArg)
{
	CGameObject*	pGameObject = Clone_Object(strPrototypeTag, pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer*			pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	}
	else
		pLayer->Add_GameObject(pGameObject);

	return S_OK;
}

CGameObject* CObject_Manager::Add_Clone_Return(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg)
{
	CGameObject* pObj = Clone_Object(strPrototypeTag, pArg);
	if (!pObj)
		return nullptr;
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);
	if (pLayer)
	{
		pLayer->Add_GameObject(pObj);
	}
	else
	{
		pLayer = CLayer::Create();
		pLayer->Add_GameObject(pObj);
		m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	}

	return pObj;
}

CGameObject* CObject_Manager::Clone_Object(const wstring& strPrototypeTag, void* pArg)
{
	CGameObject* pPrototype = Find_Prototype(strPrototypeTag);

	if (!pPrototype)
	{
		wstring errMsg = L"Clone Object Failed: No Prototype Tag : \"";
		errMsg += strPrototypeTag;
		errMsg += L"\"";
		MSG_BOX(errMsg.c_str());
		return nullptr;
	}

	CGameObject* pObj = pPrototype->Clone(pArg);
	return pObj;
}

HRESULT CObject_Manager::Push_Object(_uint iLevelIndex, const wstring& strLayerTag, CGameObject* pObject)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);
	if (pLayer)
	{
		pLayer->Add_GameObject(pObject);
	}
	else
	{
		pLayer = CLayer::Create();
		pLayer->Add_GameObject(pObject);
		m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	}
	Safe_AddRef(pObject);
	return S_OK;
}

void CObject_Manager::PriorityTick(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->PriorityTick(fTimeDelta);
	}
}

void CObject_Manager::Tick(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Tick(fTimeDelta);
	}
}


void CObject_Manager::LateTick(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->LateTick(fTimeDelta);
	}
}

CComponent * CObject_Manager::Find_Component(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strComponentTag, _uint iIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = m_pLayers[iLevelIndex].find(strLayerTag);

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second->Find_Component(strComponentTag, iIndex);	
}

CGameObject * CObject_Manager::Find_Prototype(const wstring & strPrototypeTag)
{
	auto	iter = m_Prototypes.find(strPrototypeTag);

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;	
}

CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const wstring & strLayerTag)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = m_pLayers[iLevelIndex].find(strLayerTag);
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;	
}

CObject_Manager * CObject_Manager::Create(_uint iNumLevels)
{
	CObject_Manager*	pInstance = new CObject_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX(TEXT("Failed to Created : CObject_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Manager::Free()
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);

		m_pLayers[i].clear();
	}

	Safe_Delete_Array(m_pLayers);

	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);
	m_Prototypes.clear();
}

#include "..\Public\GameObject.h"
#include "GameInstance.h"


const _wstring	CGameObject::m_strTransformTag = TEXT("Com_Transform");
_uint CGameObject::s_iID = 0;
unordered_map<_uint, CGameObject*> CGameObject::s_HashedMap;

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::Get_Instance()}
{
	m_iObjectID = s_iID;
	s_HashedMap.emplace(m_iObjectID, this);
	++s_iID;
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);	
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pDevice{ rhs.m_pDevice }
	, m_pContext{ rhs.m_pContext }
	, m_pGameInstance{ rhs.m_pGameInstance }
{
	m_iObjectID = s_iID;
	s_HashedMap.emplace(m_iObjectID, this);
	++s_iID;
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

HRESULT CGameObject::Initialize_Prototype()
{	

	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	/* ������ �����ϴ� ��ó�� ��������. ������ �ƴϰ�.  */
	/* ������ ��ü�� Ȱ���ϱ����� ���� ����� ���� Ʈ�������� �����. */
	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	/* Ÿ ��ü�� ���� Ʈ�������� �˻��� �� �ֵ��� �ʿ� �߰��ϳ�. */
	if (nullptr != Find_Component(m_strTransformTag))
		return E_FAIL;

	m_Components.emplace(m_strTransformTag, m_pTransformCom);
	Safe_AddRef(m_pTransformCom);

	if (nullptr != pArg)
	{		
		if (FAILED(m_pTransformCom->Initialize(pArg)))
			return E_FAIL;
	}
	
	
	return S_OK;
}

void CGameObject::PriorityTick(_float fTimeDelta)
{
}

void CGameObject::Tick(_float fTimeDelta)
{
}

void CGameObject::LateTick(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}


HRESULT CGameObject::Add_Component(_uint iPrototoypeLevelIndex, const wstring & strPrototypeTag, const wstring & strComponentTag, CComponent** ppOut, void * pArg)
{
	if (nullptr != Find_Component(strComponentTag))
		return E_FAIL;

	CComponent*		pComponent = m_pGameInstance->Clone_Component(iPrototoypeLevelIndex, strPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	return S_OK;
}

CComponent * CGameObject::Find_Component(const wstring & strComponentTag)
{
	auto	iter = m_Components.find(strComponentTag);

	if (iter == m_Components.end())
		return nullptr;	

	return iter->second;
}

void CGameObject::Dead()
{
	DELAYED event{};
	event.eType = DELAYED_TYPE::DELETE_OBJECT;
	event.wParam = (DWORD_PTR)this;
	m_pGameInstance->Add_Delayed(event);
}

void CGameObject::Free()
{

	auto iter = m_Components.begin();
	for(iter; iter != m_Components.end(); ++iter)
	{
		Safe_Release(iter->second);
	}
	//for (auto& Pair : m_Components)
	//	Safe_Release(Pair.second);
	//m_Components.clear();
	if(s_HashedMap.find(m_iObjectID) != s_HashedMap.end())
	s_HashedMap.erase(m_iObjectID);
	Safe_Release(m_pTransformCom);
	if(m_pGameInstance && m_pGameInstance->Get_RefCount() != 0)
		Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}

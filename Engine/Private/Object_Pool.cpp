#include "Object_Pool.h"
#include "GameInstance.h"
#include "GameObject.h"
CObject_Pool::CObject_Pool()
{
    m_pGameInstance = CGameInstance::Get_Instance();
    Safe_AddRef(m_pGameInstance);
}

HRESULT CObject_Pool::Pool_Object(const wstring& strPrototypeTag, void* pArg)
{
    auto pClone = m_pGameInstance->Clone_Object(strPrototypeTag, pArg);
    pClone->Set_Dead(true);
    m_mapPool[strPrototypeTag].push_back(pClone);
    return S_OK;
}

CGameObject* CObject_Pool::Push_Object_From_Pool(const wstring& strPrototypeTag, _uint iLevelIndex, const wstring& strLayerTag, void* pArg, _float4x4* TransformMat)
{
    if (m_mapPool.count(strPrototypeTag) == 0)
    {
        return nullptr;
    }
    vector<CGameObject*>& vecPool = m_mapPool[strPrototypeTag];
    for (int i = 0; i < vecPool.size(); ++i)
    {
        if (vecPool[i]->Is_Dead())
        {
            vecPool[i]->Set_Dead(false);

            if(TransformMat)
                vecPool[i]->Get_Transform()->Set_WorldMatrix(*TransformMat);
            vecPool[i]->Push_From_Pool(pArg);
            m_pGameInstance->Push_Object(iLevelIndex, strLayerTag, vecPool[i]);
            return  vecPool[i];
        }
    }

    return nullptr;
}

CGameObject* CObject_Pool::Push_Object_Hijack(const wstring& strPrototypeTag, void* pArg, _float4x4* TransformMat)
{
    if (m_mapPool.count(strPrototypeTag) == 0)
    {
        return nullptr;
    }
    vector<CGameObject*>& vecPool = m_mapPool[strPrototypeTag];
    for (int i = 0; i < vecPool.size(); ++i)
    {
        if (vecPool[i]->Is_Dead())
        {
            vecPool[i]->Set_Dead(false);
            vecPool[i]->Push_From_Pool(pArg);
            if (TransformMat)
                vecPool[i]->Get_Transform()->Set_WorldMatrix(*TransformMat);
            //m_pGameInstance->Push_Object(iLevelIndex, strLayerTag, vecPool[i]);
            return  vecPool[i];
        }
    }
    return nullptr;
}

void CObject_Pool::Clear_Pool()
{
    for (auto& vec : m_mapPool)
    {
        for (auto& pObject : vec.second)
        {
            Safe_Release(pObject);
        }
        vec.second.clear();
    }
    m_mapPool.clear();
}

void CObject_Pool::Recall_Object()
{
    for (auto& vec : m_mapPool)
    {
        for (auto& pObject : vec.second)
        {
            if (!pObject->Is_Dead())
            {
                pObject->Dead();
            }
        }
    }
}

void CObject_Pool::Recall_Object(const wstring& strPrototypeTag)
{
    if (m_mapPool.count(strPrototypeTag))
    {
        for (auto& pObject : m_mapPool[strPrototypeTag])
        {
            if (!pObject->Is_Dead())
            {
                pObject->Dead();
            }
        }
    }
}

void CObject_Pool::Clear_Pool(_wstring strTag)
{
    auto vec = m_mapPool.find(strTag);
    if(vec != m_mapPool.end())
    {
        for (auto& pObject : vec->second)
        {
            Safe_Release(pObject);
        }
        vec->second.clear();
    }
}

CObject_Pool* CObject_Pool::Create()
{
    return new CObject_Pool;
}

void CObject_Pool::Free()
{
    Clear_Pool();
    if(m_pGameInstance && m_pGameInstance->Get_RefCount() != 0)
        Safe_Release(m_pGameInstance);
}

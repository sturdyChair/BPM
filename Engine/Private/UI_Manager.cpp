//#include "UI_Manager.h"
//#include "UIBase.h"
//#include "GameInstance.h"
//#include "Layer.h"
//
//CUI_Manager::CUI_Manager()
//{
//}
//
//void CUI_Manager::Tick(_float fTimeDelta)
//{
//    auto pGameInstance = CGameInstance::Get_Instance();
//    bool bLButtonTap = pGameInstance->KeyDown(VK_LBUTTON);
//    bool bLButtonHold = pGameInstance->KeyPressing(VK_LBUTTON);
//    bool bLButtonAway = pGameInstance->KeyUp(VK_LBUTTON);
//
//    bool bRButtonTap = pGameInstance->KeyDown(VK_RBUTTON);
//    bool bRButtonHold = pGameInstance->KeyPressing(VK_RBUTTON);
//    bool bRButtonAway = pGameInstance->KeyUp(VK_RBUTTON);
//    //Safe_Release(m_pFocusedUI);
//    m_pFocusedUI = Get_FocusedUI();
//    //Safe_AddRef(m_pFocusedUI);
//    CUIBase* pTargetUI = nullptr;
//    if (m_pFocusedUI && !m_pFocusedUI->Is_Dead())
//        pTargetUI = Get_TargetedUI(m_pFocusedUI);
//
//    if (nullptr != pTargetUI)
//    {
//        pTargetUI->MouseOn();
//        if (bLButtonTap)
//        {
//            pTargetUI->MouseLButtonDown();
//            pTargetUI->m_bLButtonDown = true;
//        }
//        else if (bLButtonAway)
//        {
//            pTargetUI->MouseLButtonUp();
//
//            if (pTargetUI->IsLButtonDown())
//            {
//                pTargetUI->MouseLButtonClicked();
//            }
//
//            pTargetUI->m_bLButtonDown = false;
//        }
//
//        if (bRButtonTap)
//        {
//            pTargetUI->MouseRButtonDown();
//            pTargetUI->m_bRButtonDown = true;
//        }
//        else if (bRButtonAway)
//        {
//            pTargetUI->MouseRButtonUp();
//            if (pTargetUI->IsRButtonDown())
//            {
//                pTargetUI->MouseRButtonClicked();
//            }
//            pTargetUI->m_bRButtonDown = false;
//        }
//    }
//
//}
//
//
//CUIBase* CUI_Manager::Get_TargetedUI(CUIBase* _pParentUI)
//{
//    if (nullptr == _pParentUI) return nullptr;
//    auto pGameInstance = CGameInstance::Get_Instance();
//    CUIBase* pTargetUI = nullptr;
//    CUIBase* pPrevTargetUI = nullptr;
//    CUIBase* pCurrentUI;
//    static std::queue<CUIBase*> qSearch;
//    qSearch.push(_pParentUI);
//    while (!qSearch.empty())
//    {
//        pCurrentUI = qSearch.front();
//        if (pCurrentUI)
//            if (pCurrentUI->IsMouseOn())
//            {
//                pPrevTargetUI = pTargetUI;
//                pTargetUI = pCurrentUI;
//                if (nullptr != pPrevTargetUI)
//                {
//                    if (pGameInstance->KeyUp(VK_LBUTTON))
//                    {
//                        pPrevTargetUI->m_bLButtonDown = false;
//                    }
//                    if (pGameInstance->KeyUp(VK_RBUTTON))
//                    {
//                        pPrevTargetUI->m_bRButtonDown = false;
//                    }
//                }
//            }
//            else if (pGameInstance->KeyUp(VK_LBUTTON))
//            {
//                pCurrentUI->m_bLButtonDown = false;
//                if (pGameInstance->KeyUp(VK_RBUTTON))
//                {
//                    pCurrentUI->m_bRButtonDown = false;
//                }
//            }
//
//        qSearch.pop();
//        if (pCurrentUI)
//            for (auto pChild : pCurrentUI->m_Children)
//            {
//                qSearch.push(pChild);
//            }
//    }
//    return pTargetUI;
//}
//
//CUIBase* CUI_Manager::Get_FocusedUI()
//{
//    auto pGameInstance = CGameInstance::Get_Instance();
//    bool bLButtonTap = pGameInstance->KeyDown(VK_LBUTTON);
//    if (!bLButtonTap) return m_pFocusedUI;
//    auto pCurrentLayer = pGameInstance->Find_Layer(pGameInstance->Get_Current_LevelID(), CUIBase::UILayerTag);
//    if (!pCurrentLayer) return m_pFocusedUI;
//    CGameObject* pFUI = nullptr;
//    auto& UIList = pCurrentLayer->Get_ObjectList();
//
//
//    auto iter = UIList.begin();
//    auto iter_focused = UIList.end();
//    for (; iter != UIList.end(); ++iter)
//    {
//        CUIBase* pUI = dynamic_cast<CUIBase*>(*iter);
//        if (!pUI)
//            throw;
//        if ((pUI)->m_bMouseOn)
//            iter_focused = iter;
//    }
//    if (iter_focused != UIList.end())
//    {
//        pFUI = (*iter_focused);
//        UIList.erase(iter_focused);
//        UIList.push_back(pFUI);
//    }
//
//    return (CUIBase*)pFUI;
//}
//
//void CUI_Manager::Reset_FocusedUI()
//{
//    m_pFocusedUI = nullptr;
//}
//
//void CUI_Manager::Hide_UIs(_bool bIsHiding)
//{
//    m_bIsHiding = bIsHiding;
//
//    auto pGameInstance = CGameInstance::Get_Instance();
//    auto pLayer = pGameInstance->Find_Layer(pGameInstance->Get_Current_LevelID(), CUIBase::UILayerTag);
//    auto& pUIList = pLayer->Get_ObjectList();
//
//    for (auto iter = pUIList.begin(); iter != pUIList.end(); ++iter)
//        static_cast<CUIBase*>(*iter)->Set_Hidden(bIsHiding);
//}
//
//CUI_Manager* CUI_Manager::Create(_uint iLevelNum)
//{
//    return new CUI_Manager;
//}
//
//void CUI_Manager::Free()
//{
//    __super::Free();
//
//    //Safe_Release(m_pFocusedUI);
//}

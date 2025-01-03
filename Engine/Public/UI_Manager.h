//#pragma once
//#include "Base.h"
//#include "UIBase.h"
//
//BEGIN(Engine)
//
//class CUI_Manager final :
//    public CBase
//{
//private:
//    CUI_Manager();
//    ~CUI_Manager() = default;
//
//public:
//    void Tick(_float fTimeDelta);
//    CUIBase* Get_TargetedUI(CUIBase* _pParentUI);
//    CUIBase* Get_FocusedUI();
//    void     Reset_FocusedUI();
//
//    void    Hide_UIs(_bool bIsHiding);
//    _bool   Get_Hiding() const { return m_bIsHiding; }
//
//private:
//    CUIBase* m_pFocusedUI = { nullptr };
//
//    _bool m_bIsHiding = { false };
//
//public:
//    static CUI_Manager* Create(_uint iLevelNum);
//    virtual void Free() override;
//
//};
//
//END
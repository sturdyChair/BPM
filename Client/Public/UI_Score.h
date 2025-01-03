#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)

class CUI_Score :
    public CUIBase
{
private:
    CUI_Score(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Score(const CUI_Score& rhs);
    virtual ~CUI_Score() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

    void    Add_Shake(_float fPower);
    void    Set_Shake(_float fPower);
    void    Shake();
    void    Add_ScoreMultiplier(_float fAmount);
    void    Set_ScoreMultiplier(_float fAmount);
    void    Add_Score(_float fAmount);
    _float  Get_ScoreMultiplier() const { return m_fScoreMultiplier; }

private:
    _float m_fScoreMultiplier = 1.f;
    _float m_fScore = 0.f;
    _float m_fShakeModifier = 0.f;
    _float m_fMaxShake = 20.f;

    class CUI_Font*     m_pScoreMultiplier = nullptr;
    class CUI_Font*     m_pX = nullptr;
    class CUI_TextBox*  m_pScore = nullptr;

public:
    static  CUI_Score* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END
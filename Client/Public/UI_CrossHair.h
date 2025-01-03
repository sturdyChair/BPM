#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)

class CUI_CrossHair :
    public CUIBase
{
private:
    CUI_CrossHair(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_CrossHair(const CUI_CrossHair& rhs);
    virtual ~CUI_CrossHair() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

    void    Set_TextureID(CROSSHAIR_ID eID) { m_iCurrTexIndex = eID; m_fTimer = m_fBeat; }


private:
    CTexture* m_pTextureCom = nullptr;

    _uint m_iCurrTexIndex = 0;
    vector<class CUI_BeatMarker*> m_vecBeatMarkers;
    _float  m_fBeat = 0.f;
    _float  m_fBeatTimer = 0.f;
    _float  m_fTimer = 0.f;
    class CBeat_Manager* m_pBeatManager = nullptr;

public:
    static  CUI_CrossHair* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END
#pragma once
#include "UIBase.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

BEGIN(Client)

class CUI_BeatMarker :
    public CUIBase
{
public:

private:
    CUI_BeatMarker(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_BeatMarker(const CUI_BeatMarker& rhs);
    virtual ~CUI_BeatMarker() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

    void   Set_TextureIndex(BM_TYPE eIndex)
    {
        m_iCurrTexIndex = eIndex;
    }

private:
    CTexture* m_pTextureCom = nullptr;

    _uint m_iCurrTexIndex = 0;

public:
    static  CUI_BeatMarker* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;


};

END
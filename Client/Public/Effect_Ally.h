#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CModel;
class CShader;
class CTexture;


END

BEGIN(Client)
class CPlayer_Manager;
class CBeat_Manager;
class CAfterImage;
class CEffect_Ally :public CEffectObject
{
public:
    enum STATE
    {
        START,
        DYING,
    };
private:
    CEffect_Ally(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Ally(const CEffect_Ally& rhs);
    virtual ~CEffect_Ally() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();
    virtual void Push_From_Pool(void* pArg) override;
    virtual void Dying() override;
    

private:
    CShader* m_pShaderCom = nullptr;
    CModel* m_pModelCom = nullptr;

    CPlayer_Manager* m_pPlayerManager = nullptr;
    CBeat_Manager* m_pBeatManager = nullptr;
    _float    m_fLifeTime = 0.0f;
    _float    m_fLifeTimeMax = 0.0f;
    _float  m_fAnimFrom = 0.f;
    _float  m_fAnimTo = 1.f;

    _float4 m_vStartPos;
    STATE m_eState;
    _uint m_iCurrBeat = 0;
    _uint m_iPrevBeat = 0;
    CAfterImage* m_pAfterImage = nullptr;
    _float  m_fAfterImageTimer = 0.f;
public:
    static  CEffect_Ally* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END

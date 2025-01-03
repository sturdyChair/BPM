

#pragma once
#include "EffectObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_RectNorm;
class CModel;
class CShader;
class CTexture;


END

BEGIN(Client)
class CParticle;
class CPlayer_Manager;
class CTrail;
class CEffect_Sharp_Projectile :public CEffectObject
{
public:
    struct PROJECTILE_DESC
    {
        _float3 vDirection;
        COLLISION_GROUP eCG;
    };

private:
    CEffect_Sharp_Projectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Sharp_Projectile(const CEffect_Sharp_Projectile& rhs);
    virtual ~CEffect_Sharp_Projectile() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype();
    virtual void PriorityTick(_float fTimeDelta);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();
    virtual void Push_From_Pool(void* pArg) override;
    virtual void Dying() override;

    virtual void Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit) override;

private:
    CShader* m_pModelShaderCom = nullptr;
    CModel* m_pModelCom = nullptr;

    CPlayer_Manager* m_pPlayerManager = nullptr;

    _float    m_fLifeTime = 5.0f;
    _float3    m_vDirection;
    CParticle* m_pParticleTail = nullptr;

    _float4x4 m_matDown;
    CTrail* m_pTrail = nullptr;

    _wstring m_strLightTag = L"";
    static _int iLightIdx;
public:
    static  CEffect_Sharp_Projectile* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END

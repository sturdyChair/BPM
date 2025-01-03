
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
class CRoomManager;
class CParticle;
class CTrail;

class CEffect_FireBall :public CEffectObject
{
public:
    struct PROJECTILE_DESC
    {
        _float3 vDestination;
        COLLISION_GROUP eCG;
    };

private:
    CEffect_FireBall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_FireBall(const CEffect_FireBall& rhs);
    virtual ~CEffect_FireBall() = default;
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

    CRoomManager* m_pRoomManager = nullptr;
    
    _float4x4 m_matDown;
    CTrail* m_pTrail = nullptr;

    _float    m_fLifeTime = 5.0f;
    _float3    m_vDirection;
    _float3    m_vDestination;
    _float3    m_vRight;
    _float3    m_vUp;
    _float m_fGravity = 0.f;
    _wstring m_strLightTag = L"";
    static _int iLightIdx;
    CParticle* m_pParticle = nullptr;
    _float m_fLightRange = 1.f;
public:
    static  CEffect_FireBall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END

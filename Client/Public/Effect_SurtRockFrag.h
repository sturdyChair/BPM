


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
class CPlayer_Manager;
class CEffect_SurtRockFrag :public CEffectObject
{
public:
    struct PROJECTILE_DESC
    {
        _float3 vDirection;
        COLLISION_GROUP eCG;
    };

private:
    CEffect_SurtRockFrag(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_SurtRockFrag(const CEffect_SurtRockFrag& rhs);
    virtual ~CEffect_SurtRockFrag() = default;
public:
    virtual HRESULT Initialize(void* Arg);
    virtual HRESULT Initialize_Prototype(_uint iFrag);
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
    _float3    m_vRight;
    _float3    m_vUp;
    _float3     m_vRandTorq{};
    _float m_fGravity = 0.f;
    _int m_iFrag = 0;
public:
    static  CEffect_SurtRockFrag* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iFrag);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END

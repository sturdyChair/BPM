
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
class CMonster_Nidhog;
class CStatue_Gimmick;
class CStatue :public CGameObject
{
private:
    CStatue(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CStatue(const CStatue& rhs);
    virtual ~CStatue() = default;
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

    virtual void Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser) override;

    void Set_Color(_float4& vColor);
    void Set_Color(_fvector vColor);
    void Set_Idx(_uint iIdx) { m_iIdx = iIdx; }
    _float Get_Damage_Timer() const { return m_fDamage_Timer; }

    void Set_Damage_Timer(_float fTimer) { m_fDamage_Timer = fTimer; }
    void Set_Offset(_float fOffset) { m_fOffset = fOffset; }

private:
    CShader* m_pModelShaderCom = nullptr;
    CModel* m_pModelCom = nullptr;

    _uint m_iIdx = 0;

    _float    m_fDamage_Timer = 0.f;
    _float    m_fLifeTime = 5.0f;
    _float3    m_vDirection;
    _float   m_fBeat = 0.f;
    _float4 m_vColor{1.f,1.f,1.f,1.f};
    SHADER_VTXMESH_PASS m_eRenderPass = SHADER_VTXMESH_PASS::Default;
    CStatue_Gimmick* m_pGimmick = nullptr;
    _float m_fOffset=0.f;
    _float m_fHp = 30.f;

public:
    static  CStatue* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END

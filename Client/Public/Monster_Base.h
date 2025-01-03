#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCharacterController;

END

BEGIN(Client)

class CMonster_Base : public CGameObject
{
public:
	class MONSTER_DESC
	{
	public:
		_float4x4 Transformation;
	};

protected:
	CMonster_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Base(const CMonster_Base& rhs);
	virtual ~CMonster_Base() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Dying() override;
	virtual void Push_From_Pool(void* pArg) override;

	virtual void Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit) override;

	virtual void Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser) override;
	_bool Is_Killed()const { return m_bKilled; }

	void Drop_Coin();

protected:


	void Hit_ChangeColor(_float fTimeDelta);

protected:
	_uint m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::Default;
	string m_strIdleTag;
	string m_strDamagedTag;
	class CBeat_Manager* m_pBeatManager = nullptr;
	class CPlayer_Manager* m_pPlayerManager = nullptr;
	_bool m_bKilled = false;
	_float m_fHp = 0.f;
	_float m_fTakeHit = 0.f;
	_float m_fInvincible = 0.f;
	_bool  m_bInFrustum = false;
	_float m_fCoin = 0.5f;

public:
	static CMonster_Base* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
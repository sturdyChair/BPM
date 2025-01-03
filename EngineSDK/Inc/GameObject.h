#pragma once

#include "Transform.h"	

/*클라이언트개발자가 실질적으로 생성해낸 객체타입(Player, Monster)의 부모가 되는 클래스이다. */

/* LPDIRECT3DDEVICE9 == ID3D11Device + ID3D11DeviceContext */

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	static const _wstring		m_strTransformTag;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void PriorityTick(_float fTimeDelta);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDamageType, CGameObject* pCauser) {};

public:
	class CComponent* Find_Component(const wstring& strComponentTag);
	class CTransform* Get_Transform() const { return m_pTransformCom; }
	virtual void Push_From_Pool(void* pArg) {};
	virtual void Set_Dead(bool bDead = true) { m_bIsDead = bDead; }
	void    Dead();
	bool	Is_Dead() const { return m_bIsDead; }
	virtual void Dying() {};

	_uint Get_ObjectID() const { return m_iObjectID; }
	_uint Get_CollisionGroup() const { return m_iCollisionGroup; }
	void Set_CollisionGroup(_uint iGroup) { m_iCollisionGroup = iGroup; }

	virtual void Collision_Enter(CGameObject* pOther,const PxControllerHit& ControllerHit) {};
	virtual void Collision(CGameObject* pOther) {};
	virtual void Collision_Exit(CGameObject* pOther) {};

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };	

protected:
	class CTransform*			m_pTransformCom = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };
	_uint m_iCollisionGroup = 0;
protected:
	bool m_bIsDead = false;
	_uint m_iObjectID = 0;
private:
	static _uint s_iID;
protected:
	map<const wstring, class CComponent*>		m_Components;

protected:
	HRESULT Add_Component(_uint iPrototoypeLevelIndex, const wstring& strPrototypeTag, const wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);
	
public:
	static unordered_map<_uint, CGameObject*> s_HashedMap;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END
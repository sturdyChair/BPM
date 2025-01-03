#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRigidBody : public CComponent
{
public:
	struct RIGID_DESC
	{
		class CTransform*	 pTransform = nullptr;
		PxShape* pShape;
	};

private:
	CRigidBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRigidBody(const CRigidBody& rhs);
	virtual ~CRigidBody() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	void Set_Transform(class CTransform* pTransform);
	void Update(_float fTimeDelta);
	PxRigidDynamic* Get_PxRigidBody() { return m_pPxBody; }

private:
	PxRigidDynamic* m_pPxBody = nullptr;
	//PxShape* m_pShape = nullptr;
	class CTransform* m_pTransform = nullptr;
	class CPhysxManager* m_pPhysxManager = nullptr;

public:
	static CRigidBody* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
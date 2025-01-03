#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCharacterController : public CComponent
{
public:
	struct CCT_DESC : public PxCapsuleControllerDesc
	{
		class CTransform* pTransform = nullptr;
	};

private:
	CCharacterController(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharacterController(const CCharacterController& rhs);
	virtual ~CCharacterController() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	void Set_Transform(class CTransform* pTransform);
	PxController* Get_Controller() { return m_pPxController; }
	void Set_Displacement(const _float3& vDisp) { m_vDisplacement = vDisp; }
	void Set_Displacement(_fvector vDisp) { XMStoreFloat3(&m_vDisplacement, vDisp); }
	void Add_Displacement(const _float3& vDisp);
	void Add_Displacement(_fvector vDisp);

	void Move_Forward(_float fDist);
	void Move_Right(_float fDist);
	void Move_Up(_float fDist);
	void Move_Backward(_float fDist);
	void Move_Left(_float fDist);
	void Move_Down(_float fDist);

	void Move_Land_Forward(_float fDist);
	void Move_Land_Right(_float fDist);
	void Move_Land_Left(_float fDist);
	void Move_Land_Backward(_float fDist);

	void Velocity_Calculation(_float fTimeDelta);

	void Set_Position(_fmatrix Transform);
	void Set_FootPosition(_fmatrix Transform);
	void Update(_float fTimeDelta);

	void Disable(bool bFlag = true);

private:
	PxController* m_pPxController = nullptr;
	class CTransform* m_pTransform = nullptr;
	class CPhysxManager* m_pPhysxManager = nullptr;

public:
	_float3	m_vDisplacement;
	_float3 m_vLook;
	_float3 m_vRight;
	_float3 m_vUp;
	_float m_fHeight = 0.f;
	_float m_fRadius = 0.f;

	bool m_bUseVelocity_Calculation = false;
	_float  m_fMass = { 0.f };
	_float  m_fSpeed = { 0.f };
	_float  m_fMaxSpeed = { 0.f };
	_float  m_fFriction = { 0.f };
	_float  m_fGravity = 40.f;
	bool    m_bGravity = true;
	_float3 m_vVelocity;
	_float3 m_vAccel;
	_float3 m_vImpulse;

	bool m_bActivated = true;
	PxExtendedVec3 m_vPrevPos;

	static _float s_fGlobalGravity;

public:
	static CCharacterController* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
//PxCapsuleControllerDesc desc;
//desc.height = 2.0f;
//desc.radius = 0.5f;
//desc.position = PxExtendedVec3(0.0f, 2.0f, 0.0f);
//desc.material = gPhysics->createMaterial(0.5f, 0.5f, 0.5f);
//gCharacterController = gControllerManager->createController(desc);
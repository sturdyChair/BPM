#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	struct TRANSFORM_DESC
	{
		_float		fSpeedPerSec = {};
		_float		fRotationPerSec = {};
	};

	enum STATE
	{
		STATE_RIGHT,
		STATE_UP,
		STATE_LOOK,
		STATE_POSITION,
		STATE_END
	};

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override; 

	_vector Get_State(STATE eState) const;
	void	Set_State(STATE eState, _fvector vVec);

	_vector Get_Position()const { return Get_State(STATE_POSITION); }
	void Set_Position(_fvector vVec) { Set_State(STATE_POSITION, vVec); }

	_float3 Get_Scale() const;
	void	Set_Scale(const _float3& vScale);
	void	Set_Scale(_fvector vScale);

	void   Go_Straight(_float fTimeDelta);
	void   Go_Backward(_float fTimeDelta);
	void   Go_Left(_float fTimeDelta);
	void   Go_Right(_float fTimeDelta);

	void	Rotation(_fvector vAxis, _float fAngle);
	void	Turn(_fvector vAxis, _float fTimeDelta);

	void	LookAt(_fvector vAt);
	void	LookAt_Horizontal(_fvector vAt);

	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName);

	_float4x4 Get_WorldMatrix() const { return m_WorldMatrix; }
	_float4x4* Get_WorldMatrix_Ptr() { return &m_WorldMatrix; }
	_matrix	  Get_WorldMatrix_XMMat() { return XMLoadFloat4x4(& m_WorldMatrix); }
	_float4x4 Get_WorldMatrix_Inversed() const;
	void	  Set_WorldMatrix(const _float4x4& Matrix) { m_WorldMatrix = Matrix; }
	void	  Set_WorldMatrix(_fmatrix Matrix);

private:
	_float4x4			m_WorldMatrix = {};
	_float				m_fSpeedPerSec = { 0.f };
	_float				m_fRotationPerSec = { 0.f };

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#pragma once

#include "Component.h"
#include "DebugDraw.h"

BEGIN(Engine)


class ENGINE_DLL CCollider : public CComponent
{
private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;


public:
	virtual HRESULT Initialize_Prototype(COLLIDER_TYPE eType);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render() override;

	void Update(_fmatrix WorldMatrix);
	
	class CBounding* Get_Bounding() { return m_pBounding; }
	const COLLIDER_TYPE& Get_Collider_Type() const { return m_eType; }
	_bool Intersect(CCollider* pOther);



private:
	class CBounding* m_pBounding = nullptr;

	COLLIDER_TYPE m_eType = COLLIDER_TYPE::SPHERE;

#ifdef _DEBUG
	PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
	BasicEffect*						 m_pEffect = nullptr;
	ID3D11InputLayout*					 m_pInputLayout = nullptr;

#endif

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, COLLIDER_TYPE eType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
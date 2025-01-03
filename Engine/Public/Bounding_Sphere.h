#pragma once
#include "Bounding.h"

BEGIN(Engine)

class CBounding_Sphere : public CBounding
{
public:
	struct BOUNDING_SPHERE_DESC : public CBounding::BOUNDING_DESC
	{
		_float fRadius;
	};
private:
	CBounding_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_Sphere() = default;


public:
	virtual HRESULT Initialize(CBounding::BOUNDING_DESC* pDesc);
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch) override;

	virtual void Update(_fmatrix WorldMatrix) override;
	virtual _bool Intersect(CBounding* pOther, COLLIDER_TYPE eOthType) override;

	BoundingSphere* Get_Desc() { return m_pBoundingSphere; }

private:
	BoundingSphere* m_pBoundingSphere_Local = nullptr;
	BoundingSphere* m_pBoundingSphere = nullptr;

public:
	static CBounding_Sphere* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CBounding::BOUNDING_DESC* pDesc);
	virtual void Free() override;

};

END
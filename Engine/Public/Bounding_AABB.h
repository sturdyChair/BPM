#pragma once
#include "Bounding.h"

BEGIN(Engine)

class CBounding_AABB : public CBounding
{
public:
	struct BOUNDING_AABB_DESC: public CBounding::BOUNDING_DESC
	{
		_float3 vExtents;
	};
private:
	CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_AABB() = default;


public:
	virtual HRESULT Initialize(CBounding::BOUNDING_DESC* pDesc);
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch) override;

	virtual void Update(_fmatrix WorldMatrix) override;
	virtual _bool Intersect(CBounding* pOther, COLLIDER_TYPE eOthType) override;

	BoundingBox* Get_Desc() { return m_pBoundingBox; }
private:
	BoundingBox* m_pBoundingBox_Local = nullptr;
	BoundingBox* m_pBoundingBox = nullptr;

public:
	static CBounding_AABB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CBounding::BOUNDING_DESC* pDesc);
	virtual void Free() override;

};

END
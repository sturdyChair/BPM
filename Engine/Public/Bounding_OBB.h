
#pragma once
#include "Bounding.h"

BEGIN(Engine)

class CBounding_OBB : public CBounding
{
public:
	struct BOUNDING_OBB_DESC : public CBounding::BOUNDING_DESC
	{
		_float3 vExtents;
		_float3 vRotation;
	};
private:
	CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_OBB() = default;


public:
	virtual HRESULT Initialize(CBounding::BOUNDING_DESC* pDesc);
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch) override;

	virtual void Update(_fmatrix WorldMatrix) override;
	virtual _bool Intersect(CBounding* pOther, COLLIDER_TYPE eOthType) override;

	BoundingOrientedBox* Get_Desc() { return m_pBoundingBox; }
private:
	BoundingOrientedBox* m_pBoundingBox_Local = nullptr;
	BoundingOrientedBox* m_pBoundingBox = nullptr;

public:
	static CBounding_OBB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CBounding::BOUNDING_DESC* pDesc);
	virtual void Free() override;

};

END
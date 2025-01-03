#pragma once
#include "Base.h"
#include "DebugDraw.h"

BEGIN(Engine)

class CBounding: public CBase
{
public:
	struct BOUNDING_DESC
	{
		_float3 vCenter;
	};

protected:
	CBounding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding() = default;


public:
	virtual HRESULT Initialize();
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch);

	virtual void Update(_fmatrix WorldMatrix) = 0;

	virtual _bool Intersect(CBounding* pOther, COLLIDER_TYPE eOthType) = 0;

protected:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	_bool		m_bCollided = false;

public:
	//static CBounding* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END
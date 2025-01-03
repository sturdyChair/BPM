#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectNorm final : public CVIBuffer
{
private:
	CVIBuffer_RectNorm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_RectNorm(const CVIBuffer_RectNorm& rhs);
	virtual ~CVIBuffer_RectNorm() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_RectNorm* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
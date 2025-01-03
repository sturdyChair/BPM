#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Parallelogram final : public CVIBuffer
{
private:
	CVIBuffer_Parallelogram(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Parallelogram(const CVIBuffer_Parallelogram& rhs);
	virtual ~CVIBuffer_Parallelogram() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Parallelogram* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
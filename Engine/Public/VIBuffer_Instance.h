#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instance : public CVIBuffer
{
public:
	struct INSTANCE_DESC
	{
		_float2 vScale{};//MINMAX
		_float2 vSpeed{};//MINMAX
		_float2 vLifeTime{};//MINMAX
		_float3 vTorque{};

		_float3 vCenter{};
		_float3 vRange{};
		_float3 vPivot{};

		_bool bLoop = false;
		_uint iNumInstance = 0;
	};

protected:
	CVIBuffer_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Instance(const CVIBuffer_Instance& rhs);
	virtual ~CVIBuffer_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(const INSTANCE_DESC* desc);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render() override;

	virtual HRESULT Bind_Buffer() override;
	virtual HRESULT Draw() override;
public:
	virtual void Update_Spread(_float fTimeDelta) {};
	virtual void Update_Drop(_float fTimeDelta) {};
	virtual void Update_Fountain(_float fTimeDelta) {};
	virtual void Update_Rising(_float fTimeDelta) {};
	virtual void Update_Cycle(_float fTimeDelta) {};
	virtual void Update_Orbital(_float fTimeDelta) {};

	virtual void Reset();

protected:
	ID3D11Buffer* m_pVBInstance = nullptr;
	INSTANCE_DESC m_tDesc;
	_uint m_iInstanceVertexStride = 0;
	_uint m_iNumIndexPerInstance = 0;
	_float* m_pSpeeds = nullptr;
	_float4* m_vecInitPos = nullptr;
	_float3* m_vecTorque = nullptr;
	

public:
	virtual CComponent* Clone(void* pArg) override = 0;
	virtual void Free() override;

};

END
#pragma once

#include "VIBuffer_instance.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Point_Instance final : public CVIBuffer_Instance
{
private:
	CVIBuffer_Point_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance& rhs);
	virtual ~CVIBuffer_Point_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(const INSTANCE_DESC* pDesc) override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual void Update_Spread(_float fTimeDelta) override;
	virtual void Update_Drop(_float fTimeDelta) override;
	virtual void Update_Fountain(_float fTimeDelta) override;
	virtual void Update_Rising(_float fTimeDelta) override;
	virtual void Update_Cycle(_float fTimeDelta) override;
	virtual void Update_Orbital(_float fTimeDelta) override;
	virtual void Reset() override;

private:
	VTXMATRIX* m_pInstanceVertex = nullptr;

public:
	static CVIBuffer_Point_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const INSTANCE_DESC* pDesc);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
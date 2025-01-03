
#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CPlayer_Manager;

class CWing : public CGameObject
{

private:
	CWing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWing(const CWing& rhs);
	virtual ~CWing() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Dying() override;
	virtual void Push_From_Pool(void* pArg) override;

	virtual void Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser) override;
private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = nullptr;

	_bool m_bOpened = false;
	CPlayer_Manager* m_pPlayerManager = nullptr;
	_wstring m_strLight;

public:
	static CWing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
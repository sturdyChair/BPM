#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CPlayer_Manager;

class CChest : public CGameObject
{
public:
	class CHEST_DESC
	{
	public:
		_wstring strModelTag;
		_wstring strContainTag;
	};
private:
	CChest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChest(const CChest& rhs);
	virtual ~CChest() = default;

public:
	virtual HRESULT Initialize_Prototype(CHEST_DESC& desc);
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Dying() override;
	virtual void Push_From_Pool(void* pArg) override;

	void Open();

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = nullptr;
	CGameObject* m_pContain = nullptr;
	CHEST_DESC m_ContainDesc;
	_bool m_bOpened = false;
	CPlayer_Manager* m_pPlayerManager = nullptr;

public:
	static CChest* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CHEST_DESC& desc);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
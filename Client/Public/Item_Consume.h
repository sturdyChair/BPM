#pragma once
#include "Client_Defines.h"
#include "Item_Base.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CPlayer_Manager;

class CItem_Consume : public CItem_Base
{
public:
	class CONSUME_DESC
	{
	public:
		ITEM_TEXURE_ID eID;
		_wstring strModelTag;
		_wstring strDesc = L"";
		_wstring strName = L"";
		_int iPrice = 0;
	};

private:
	CItem_Consume(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem_Consume(const CItem_Consume& rhs);
	virtual ~CItem_Consume() = default;

public:
	HRESULT Initialize_Prototype(const CONSUME_DESC& desc);

	virtual void Dying() override;
	virtual void Push_From_Pool(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;

private:
	virtual HRESULT Ready_Components() override;

	virtual void _Initialize() override;

	virtual void Interact() override;


private:
	_bool m_bConsumed = false;
	ITEM_TEXURE_ID m_eID = ITEM_CONSUME_HEAL_25;
	_wstring m_strModelTag;

public:
	static CItem_Consume* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CONSUME_DESC& desc);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};


END
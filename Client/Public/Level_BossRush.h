
#pragma once

#include "Client_Defines.h"
#include "Level.h"


BEGIN(Client)

class CLevel_BossRush final : public CLevel
{
private:
	CLevel_BossRush(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_BossRush() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:

	HRESULT Ready_Layer_Camera(const wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const wstring& strLayerTag);
	HRESULT Ready_Layer_Effect(const wstring& strLayerTag);
	HRESULT Ready_Layer_UI(const wstring& strLayerTag);

	HRESULT Ready_LandObjects();

	/* ���� �����÷��� ȭ���� ����� �����ϴµ� �־� �ʿ��� ��ü���� ��Ƴ��� ���̾ �����Ѵ�. */


public:
	static CLevel_BossRush* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
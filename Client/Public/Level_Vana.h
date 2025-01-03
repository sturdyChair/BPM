
#pragma once

#include "Client_Defines.h"
#include "Level.h"


BEGIN(Client)

class CLevel_Vana final : public CLevel
{
private:
	CLevel_Vana(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Vana() = default;

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



public:
	static CLevel_Vana* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

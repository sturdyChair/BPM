#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CCamera_Logo;
class CPlayerSelectModel;
class CUI_TextBox;
class CLevel_Logo final : public CLevel
{
private:
	CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Logo() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_BackGround(const wstring& strLayerTag);
	CCamera_Logo* m_pCamera = nullptr;

	_int m_iSelected = 0;
	_int m_iModeSelect = 0;
	CPlayerSelectModel* m_pPSM[PLAYER_CHARACTER::PC_END];
	CUI_TextBox* m_pTextBox_GameMode = nullptr;
	CUI_TextBox* m_pTextBox_Selected = nullptr;
public:
	static CLevel_Logo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
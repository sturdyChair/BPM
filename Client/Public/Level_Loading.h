#pragma once

#include "Client_Defines.h"
#include "Level.h"

/* 1. 로딩 레벨을 보여주기위한 객체들을 생성하는 작업을 수행한다. */
/* 2. 자원로딩을 위한 로더객체를 생성해준다. */
/* Level_Loading생성하는 작업 : 메인스레드 */



BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:
	CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT Initialize(LEVEL eNextLevelID);
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	LEVEL						m_eNextLevelID = { LEVEL_END };
	class CLoader*				m_pLoader = { nullptr };

private:
	/* 로딩화면의 배경을 구성하는데 있어 필요한 객체들을 모아놓은 레이어를 생성한다. */
	HRESULT Ready_Layer_BackGround(const wstring& strLayerTag);


public:
	static CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID);
	virtual void Free() override;
};

END
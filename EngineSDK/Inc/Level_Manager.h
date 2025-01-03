#pragma once

#include "Base.h"

/* 1. ���� �Ҵ�� ������ �����ϳ�.*/
/* 1_1. ���� ��ü ��, ���� ������ �ı��ϴ� ��Ȱ. ���� ���������� �Ҵ�ƴ� �޸𸮵� �ؼ��ϳ�.  */
/* 2. Ȱ��ȭ�� ������ Tick Render�� ������ ȣ���Ѵ�. */


BEGIN(Engine)

class CLevel_Manager final : public CBase
{
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT Change_Level(class CLevel* pNewLevel);
	void Tick(_float fTimeDelta);
	HRESULT Render();
	_uint	Get_Current_LevelID() const;

private:
	class CLevel*			m_pCurrentLevel = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };

public:
	static CLevel_Manager* Create();
	virtual void Free() override;
};

END
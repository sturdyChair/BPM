#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer final : public CBase
{
private:
	CTimer();
	virtual ~CTimer() = default;

public:
	float			Get_TimeDelta() const {	
		return m_fTimeDelta * m_fBulletTimeMod;	}

	void Set_Bullet_Time_Modifier(float _fMod, float _fDuration)
	{
		m_fBulletTimeMod = _fMod;
		m_fBulletTimeDuration = _fDuration;
	}

	_float Get_Bullet_Time_Modifier() const { return m_fBulletTimeMod; }

public:
	HRESULT			Initialize();
	void			Update();

private:
	LARGE_INTEGER			m_CurTime;
	LARGE_INTEGER			m_OldTime;
	LARGE_INTEGER			m_OriginTime;
	LARGE_INTEGER			m_CpuTick;

	float					m_fTimeDelta;
	float					m_fBulletTimeMod = 1.f;
	float					m_fBulletTimeDuration = 0.f;

public:
	static CTimer* Create();
	virtual void Free() override;
};


END
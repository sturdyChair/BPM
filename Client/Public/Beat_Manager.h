
#pragma once

#include "Base.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CGameInstance;
class CTexture;
END

BEGIN(Client)

class CBeat_Manager final : public CBase
{
	DECLARE_SINGLETON(CBeat_Manager)
private:
	CBeat_Manager();
	virtual ~CBeat_Manager() = default;

public:
	void Tick(_float fTimeDelta);

	void Set_Beat(_float fBeat) { m_fBeat = fBeat; }
	void Reset()
	{
		m_fTimer = 0.f; m_iBeatCount = 0;
	}

	const _float& Get_Beat() const { return m_fBeat; }
	const _uint& Get_BeatCount() const { return m_iBeatCount; }
	const _float& Get_Timer() const { return m_fTimer; }

	void Initialize();
	void Kill_Instance();

	void Queue_Sequence(const _wstring& strSeqTag)
	{
		m_bSequenceStoped = false;
		m_bSequenceChange = true;
		m_strNextSequenceTag = strSeqTag;
	}
	void Stop_Sequence();

	void Start_Sequence();
	void Proceed();
	const _float& Get_N_Beats(_uint iN) { return m_fNextBeats[iN]; }
	BM_TYPE Get_N_BeatType(_uint iN) { return m_eNextBeatTypes[iN]; }
	void Set_N_BeatType(_uint iN, BM_TYPE eType);

	void Set_DefaultTexture(CTexture* pORM, CTexture* pNormal, CTexture* pNoise) {
		m_pDefaultORM = pORM; m_pDefaultNormal = pNormal; m_pNoise
			= pNoise;
	}

	CTexture* Get_DefaultORM() { return m_pDefaultORM; }
	CTexture* Get_DefaultNormal() { return m_pDefaultNormal; }
	CTexture* Get_DefaultNoise() { return m_pNoise; }

private:
	_float m_fBeat = 0.f;
	_float m_fTimer = 0.f;
	_uint  m_iBeatCount = 0;
	_float m_fNextBeats[8]{};
	BM_TYPE m_eNextBeatTypes[8]{};

	_bool m_bSequenceStoped = false;
	bool   m_bSequenceChange = false;
	_wstring m_strNextSequenceTag;
	BGM_SEQUENCE* m_pCurrSequence = nullptr;
	map<_wstring, BGM_SEQUENCE*> m_mapBGM;
	vector<BGM_SEQUENCE> m_vecBGMSequence;
	CGameInstance* m_pGameInstance = nullptr;
	CTexture* m_pDefaultNormal = nullptr;
	CTexture* m_pDefaultORM = nullptr;
	CTexture* m_pNoise = nullptr;
	CTexture* m_pLinearUV = nullptr;

public:
	virtual void Free() override;
};

END
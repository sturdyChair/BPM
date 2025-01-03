#pragma once
#include "Base.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CGameInstance;

END


BEGIN(Client)
class CStatue;
class CBeat_Manager;

class CStatue_Gimmick : public CBase
{
private:
	CStatue_Gimmick();
	virtual ~CStatue_Gimmick() = default;

public:
	HRESULT Initialize(class CMonster_Nidhog* pNidHog);

	void PriorityTick(_float fTimeDelta);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	
	void Statue_Damaged(_uint iIdx);
	void End() { m_bGimmickEnd = true; }
private:
	void Beat_Moved();

private:
	CGameInstance* m_pGameInstance = nullptr;
	CStatue* m_pStatues[3]{ nullptr,nullptr ,nullptr };
	vector<vector<_uint>> m_vecPattern;
	_uint m_iCurrPattern = 0;
	_uint m_iCurrPatternIndicator = 0;
	CBeat_Manager* m_pBeatManager = nullptr;
	class CMonster_Nidhog* m_pNidhog = nullptr;

	_uint m_iCurrBC = 0;
	_uint m_iPrevBC = 0;
	_bool m_bDamaged = false;
	_uint m_iDamageCount = 0;

	_float4 m_vPositions[3];
	_bool m_bGimmickEnd = false;
public:
	static CStatue_Gimmick* Create(class CMonster_Nidhog* pNidHog);

	virtual void Free() override;

};

END
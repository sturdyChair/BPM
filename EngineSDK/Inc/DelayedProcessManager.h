
#pragma once
#include "Base.h"

BEGIN(Engine)

class CDelayedProcessManager : public CBase
{
private:
	CDelayedProcessManager();
	~CDelayedProcessManager() = default;

public:
	void Initialize();

	void Add_Delayed(DELAYED _tEvent)
	{
		m_EventVec.push_back(_tEvent);
	}
	void Execute(DELAYED _tEvent);

	void Push_Dead_Obj(class CGameObject* _pDeadObj);

	void Tick(_float fTimeDelta);


private:
	list<class CGameObject*> m_DeadObjList;
	vector<DELAYED> m_EventVec;

public:
	static CDelayedProcessManager* Create();
	virtual void Free() override;
};

END
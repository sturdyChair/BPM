#pragma once

#include "Base.h"

/* �纻��ü���� ��Ƽ� �����Ѵ�. */
/* 4. �����ϰ��ִ� �纻��ü���� Tick���� ȣ���Ѵ�. */
/* 5. �����ϰ��ִ� �纻��ü���� Render (x : �׸��� ������ �����ؾ� �� �ʿ䰡 �־. ) */

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	void PriorityTick(_float fTimeDelta);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	list<class CGameObject*>& Get_ObjectList() { return m_GameObjects; }

public:
	class CComponent* Find_Component(const wstring& strComponentTag, _uint iIndex);

private:
	list<class CGameObject*>			m_GameObjects;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END
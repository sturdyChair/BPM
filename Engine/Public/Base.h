#pragma once

#include "Engine_Defines.h"

/* 1. ������ ���� �����ϴ� ��� Ŭ������ �θ� �Ǵ� Ŭ������.(SuperClass) */
/* 2. ���۷���ī��Ʈ�� �����Ѵ�. */
/* ���۷���ī��Ʈ : �������� */

/* abstract �߻�Ŭ���� : ����(ȥ�ڼ�)������ ��üȭ �� �� ���� Ŭ����. �θ�! */

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;
public:
	unsigned int AddRef();

	/* ���������� ���ҽ�Ų�� or �����Ѵ�. */
	unsigned int Release();

	unsigned int Get_RefCount() { return m_iRefCnt; }
private:
	unsigned int			m_iRefCnt = { 0 };

public:
	virtual void Free();
};

END
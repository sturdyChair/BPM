#pragma once
#include "Base.h"

BEGIN(Engine)

class CObject_Pool :
    public CBase
{
private:
	CObject_Pool();
	~CObject_Pool() = default;

public:
	HRESULT Pool_Object(const wstring& strPrototypeTag, void* pArg = nullptr);
	class CGameObject* Push_Object_From_Pool(const wstring& strPrototypeTag, _uint iLevelIndex, const wstring& strLayerTag, void* pArg, _float4x4* TransformMat);
	class CGameObject* Push_Object_Hijack(const wstring& strPrototypeTag, void* pArg, _float4x4* TransformMat);
	void	Clear_Pool();
	void	Recall_Object();
	void	Recall_Object(const wstring& strPrototypeTag);
	void	Clear_Pool(_wstring strTag);
public:
	static CObject_Pool* Create();
	virtual void Free() override;

private:
	class CGameInstance* m_pGameInstance = nullptr;
	map< const wstring, vector<CGameObject*>> m_mapPool;
};

END
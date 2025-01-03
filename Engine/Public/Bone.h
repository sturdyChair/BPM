#pragma once
#include "Base.h"

BEGIN(Engine)

class CBone : public CBase
{
public:
	struct BONE_DESC
	{
		string strName;
		string strParentName;
		_float4x4 Transformation;
	};

private:
	CBone();
	virtual ~CBone() override;

public:
	HRESULT Initialize(BONE_DESC& desc);

	void	Update(_fmatrix ParentMatrix);
	const string& Get_Name() const { return m_strName; }

	void Set_TransformationMatrix(_fmatrix matrix);
	_matrix Get_TransformationMatrix() const { return XMLoadFloat4x4(&m_TransformationMatrix); }
	void Add_Child(_int iIdx);
	const vector<_int>& Get_Children() { return m_iChildrenIdx; }
private:
	string m_strName;
	string m_strParentName;
	_float4x4 m_TransformationMatrix; //부모 적용 안된 행렬
	_float4x4 m_CombinedTransformationMatrix; //부모 적용 이후 행렬

	bool  m_bRootNode = false;
	_int m_iParentIdx = -1;
	vector<_int> m_iChildrenIdx;

public:
	static CBone* Create(BONE_DESC& desc);
	CBone* Clone();
	virtual void Free() override;

	friend class CModel;
};

END
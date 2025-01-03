#pragma once
#include "Base.h"


BEGIN(Engine)

class CBone;

class CChannel : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;
public:
	HRESULT Initialize(ifstream& in, const vector<CBone*>& Bones);
	HRESULT Initialize_Bin(ifstream& in, const vector<CBone*>& Bones);

	void Update(_float fCurrentTrackPosition, const vector<CBone*>& Bones, _uint& iCurrentKeyFrameIndex);
	void Lerp(_float fPosition,_float fLerp ,const vector<CBone*>& Bones);

	void FrameInfo(vector<KEYFRAME>& BoneFrame, _float fTrackPosition);
	_uint Get_BoneIdx() { return m_iBoneIndex; }

private:
	string m_strName;
	vector<KEYFRAME> m_KeyFrames;
	_uint m_iNumKeyFrame = 0;

	_uint m_iBoneIndex = 0;

public:
	static CChannel* Create(ifstream& in, const vector<CBone*>& Bones);
	static CChannel* Create(ifstream& in, const vector<CBone*>& Bones, bool bBinFlag);
	virtual void Free() override;

};

END
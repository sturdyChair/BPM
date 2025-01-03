#pragma once
#include "Base.h"


BEGIN(Engine)

class CBone;

class CAnimation : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(ifstream& in, const vector<CBone*>& Bones);
	HRESULT Initialize_Bin(ifstream& in, const vector<CBone*>& Bones);
	_bool Update_TransformationMatrix(_float fTimeDelta, const vector<CBone*>& Bones, _bool isLoop = false);
	_bool Update_TransformationMatrix_Departed(_float fTimeDelta, const vector<CBone*>& Bones, const vector<_int>& vecIdx, _bool isLoop = false);

	void Lerp_TransformationMatrix(const vector<CBone*>& Bones, _float fLerp, _float fTrackPosition);
	void Lerp_TransformationMatrix_Departed(const vector<CBone*>& Bones, _float fLerp, const vector<_int>& vecIdx, _float fTrackPosition);


	_bool Blend_TransformationMatrix(_float fTimeDelta, const vector<CBone*>& Bones, _float fLerp, _bool isLoop = false);
	_bool Get_Finished() const {
		return m_isFinished;
	}
	void Reset();
	const string& Get_Name()const { return m_strName; }
	const _float& Get_CurrentTrackPosition() const { return m_fCurrentTrackPosition; }
	_float Get_CurrentTrackPositionRatio() const { return m_fCurrentTrackPosition / m_fDuration; }
	void Set_TrackPositionRatio(_float fPerOne);
	void FrameInfo(vector<KEYFRAME>& BoneFrame, _float fTrackPosition);

	_float Get_TickPerSec()const {
		return m_fTickPerSecond;
	}
	void Set_TickPerSec(_float fTPS) {
		m_fTickPerSecond = fTPS;
	}


	_float Get_Duration()const { return m_fDuration; }

private:
	string m_strName;
	_float m_fDuration = 0.f;
	_float m_fTickPerSecond = 0.f;
	_float m_fCurrentTrackPosition = 0.f;

	_uint m_iNumChannels = 0;
	vector<class CChannel*> m_Channels;
	vector<_uint> m_CurrentKeyFrameIndices;
	_bool m_isFinished = false;

public:
	static CAnimation* Create(ifstream& in, const vector<CBone*>& Bones);
	static CAnimation* Create(ifstream& in, const vector<CBone*>& Bones, bool bBinFlag);
	CAnimation* Clone();
	virtual void Free() override;
	
};

END
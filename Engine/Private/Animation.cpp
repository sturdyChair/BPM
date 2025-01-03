#include "Animation.h"
#include "Channel.h"

CAnimation::CAnimation():
	CBase()
{
}

CAnimation::CAnimation(const CAnimation& rhs):
	m_strName(rhs.m_strName),
	m_fDuration(rhs.m_fDuration), m_fTickPerSecond(rhs.m_fTickPerSecond), m_fCurrentTrackPosition(rhs.m_fCurrentTrackPosition),
	m_iNumChannels(rhs.m_iNumChannels), m_Channels(rhs.m_Channels)
{
	for (auto& pChannels: m_Channels)
	{
		Safe_AddRef(pChannels);
	}
	m_CurrentKeyFrameIndices.resize(m_iNumChannels);
}

HRESULT CAnimation::Initialize(ifstream& in, const vector<CBone*>& Bones)
{
	string strTemp;
	in >> strTemp >> m_strName;
	in >> strTemp >> m_iNumChannels;
	m_Channels.resize(m_iNumChannels);
	in >> strTemp >> m_fDuration;
	in >> strTemp >> m_fTickPerSecond;
	for (_int i = 0; i < m_iNumChannels; ++i)
	{
		m_Channels[i] = CChannel::Create(in, Bones);
	}
	m_CurrentKeyFrameIndices.resize(m_iNumChannels);

	return S_OK;
}
HRESULT CAnimation::Initialize_Bin(ifstream& in, const vector<CBone*>& Bones)
{
	_uint iLength = 0;
	_char szBuffer[256];
	string strTemp;
	read_data(in, iLength);
	//in >> iLength;
	in.read(szBuffer, iLength);
	szBuffer[iLength] = '\0';
	m_strName = szBuffer;
	read_data(in, m_iNumChannels);
	//in >> m_iNumChannels;
	m_Channels.resize(m_iNumChannels);
	read_data(in, m_fDuration);
	read_data(in, m_fTickPerSecond);
	//in >> m_fDuration;
	in >> m_fTickPerSecond;
	for (_int i = 0; i < m_iNumChannels; ++i)
	{
		m_Channels[i] = CChannel::Create(in, Bones, true);
	}
	m_CurrentKeyFrameIndices.resize(m_iNumChannels);

	return S_OK;
}

_bool CAnimation::Update_TransformationMatrix(_float fTimeDelta, const vector<CBone*>& Bones, _bool isLoop)
{
	_bool rt = false;
	m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta;

	if (m_fCurrentTrackPosition >= m_fDuration)
	{
		m_isFinished = true;
		rt = true;
		if (true == isLoop)
		{
			m_isFinished = false;
			m_fCurrentTrackPosition = 0.f;
			for (auto& idx: m_CurrentKeyFrameIndices)
			{
				idx = 0;
			}
		}

		if (true == m_isFinished)
		{
			return rt;
		}
	}

	_uint idx = 0;
	for (auto& pChannel : m_Channels)
	{
		pChannel->Update(m_fCurrentTrackPosition, Bones, m_CurrentKeyFrameIndices[idx]);
		++idx;
	}
	return rt;
}

_bool CAnimation::Update_TransformationMatrix_Departed(_float fTimeDelta, const vector<CBone*>& Bones, const vector<_int>& vecIdx, _bool isLoop)
{
	_bool rt = false;
	m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta;

	if (m_fCurrentTrackPosition >= m_fDuration)
	{
		m_isFinished = true;
		rt = true;
		if (true == isLoop)
		{
			m_isFinished = false;
			m_fCurrentTrackPosition = 0.f;
			for (auto& idx : m_CurrentKeyFrameIndices)
			{
				idx = 0;
			}
		}

		if (true == m_isFinished)
		{
			return rt;
		}
	}

	_uint idx = 0;

	for (auto& pChannel : m_Channels)
	{
		if(binary_search(vecIdx.begin(), vecIdx.end(), pChannel->Get_BoneIdx()))
			pChannel->Update(m_fCurrentTrackPosition, Bones, m_CurrentKeyFrameIndices[idx]);
		++idx;
	}
	return rt;
}

void CAnimation::Lerp_TransformationMatrix(const vector<CBone*>& Bones, _float fLerp, _float fTrackPosition)
{
	_uint idx = 0;
	for (auto& pChannel : m_Channels)
	{
		pChannel->Lerp(fTrackPosition, fLerp, Bones);
		++idx;
	}
}

void CAnimation::Lerp_TransformationMatrix_Departed(const vector<CBone*>& Bones, _float fLerp, const vector<_int>& vecIdx, _float fTrackPosition)
{
	_uint idx = 0;
	for (auto& pChannel : m_Channels)
	{
		if (binary_search(vecIdx.begin(), vecIdx.end(), pChannel->Get_BoneIdx()))
			pChannel->Lerp(fTrackPosition, fLerp, Bones);
		++idx;
	}
}

_bool CAnimation::Blend_TransformationMatrix(_float fTimeDelta, const vector<CBone*>& Bones, _float fLerp, _bool isLoop)
{
	m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta;

	if (m_fCurrentTrackPosition >= m_fDuration)
	{
		m_isFinished = true;

		if (true == isLoop)
		{
			m_isFinished = false;
			m_fCurrentTrackPosition = 0.f;
			for (auto& idx : m_CurrentKeyFrameIndices)
			{
				idx = 0;
			}
		}

		if (true == m_isFinished)
			return true;
	}
	_uint idx = 0;
	for (auto& pChannel : m_Channels)
	{
		pChannel->Lerp(m_fCurrentTrackPosition, fLerp, Bones);
		++idx;
	}
	return false;
}

void CAnimation::Reset()
{
	m_isFinished = false;
	m_fCurrentTrackPosition = 0.f;

	for (_uint i = 0; i < m_iNumChannels; ++i)
		m_CurrentKeyFrameIndices[i] = 0;
}

void CAnimation::Set_TrackPositionRatio(_float fPerOne)
{
	m_fCurrentTrackPosition = m_fDuration * fPerOne;
	if (m_fCurrentTrackPosition > m_fDuration)
	{
		m_fCurrentTrackPosition = m_fDuration;
	}
}

void CAnimation::FrameInfo(vector<KEYFRAME>& BoneFrame, _float fTrackPosition)
{
	for (auto& pChannel : m_Channels)
	{
		pChannel->FrameInfo(BoneFrame, fTrackPosition);
	}
}


CAnimation* CAnimation::Create(ifstream& in, const vector<CBone*>& Bones)
{
	CAnimation* pInstance = new CAnimation;
	if (FAILED(pInstance->Initialize(in, Bones)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAnimation* CAnimation::Create(ifstream& in, const vector<CBone*>& Bones, bool bBinFlag)
{
	CAnimation* pInstance = new CAnimation;
	if (FAILED(pInstance->Initialize_Bin(in, Bones)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
	{
		Safe_Release(pChannel);
	}
	m_Channels.clear();
	__super::Free();
}

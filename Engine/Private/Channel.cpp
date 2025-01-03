#include "Channel.h"
#include "Bone.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(ifstream& in, const vector<CBone*>& Bones)
{
	string strTemp;
	in >> strTemp >> m_strName;
	_uint iNumScaleFrame = 0;
	_uint iNumRotationFrame = 0;
	_uint iNumTranslationFrame = 0;
	in >> strTemp >> iNumScaleFrame;
	in >> strTemp >> iNumRotationFrame;
	in >> strTemp >> iNumTranslationFrame;

	auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone)->_bool
		{
			if (m_strName == pBone->Get_Name())
				return true;

			++m_iBoneIndex;

			return false;
		});

	m_iNumKeyFrame = max(iNumScaleFrame, max(iNumRotationFrame, iNumTranslationFrame));
	m_KeyFrames.resize(m_iNumKeyFrame);
	in >> strTemp;
	for (_uint i = 0; i < m_iNumKeyFrame; ++i)
	{
		if(i < iNumScaleFrame)
		{
			in >> m_KeyFrames[i].vScale.x >> m_KeyFrames[i].vScale.y >> m_KeyFrames[i].vScale.z;
		}
		else
		{
			m_KeyFrames[i].vScale = m_KeyFrames[i - 1].vScale;
		}
	}
	in >> strTemp;
	for (_uint i = 0; i < m_iNumKeyFrame; ++i)
	{
		if (i < iNumRotationFrame)
		{
			in >> m_KeyFrames[i].vRotation.x >> m_KeyFrames[i].vRotation.y >> m_KeyFrames[i].vRotation.z >> m_KeyFrames[i].vRotation.w;
		}
		else
		{
			m_KeyFrames[i].vRotation = m_KeyFrames[i - 1].vRotation;
		}
	}
	in >> strTemp;
	for (_uint i = 0; i < m_iNumKeyFrame; ++i)
	{
		if (i < iNumTranslationFrame)
		{
			in >> m_KeyFrames[i].vTranslation.x >> m_KeyFrames[i].vTranslation.y >> m_KeyFrames[i].vTranslation.z;
		}
		else
		{
			m_KeyFrames[i].vTranslation = m_KeyFrames[i - 1].vTranslation;
		}
	}
	in >> strTemp;
	for (_uint i = 0; i < m_iNumKeyFrame; ++i)
	{
		in >> m_KeyFrames[i].fTrackPosition;
	}

	return S_OK;
}

HRESULT CChannel::Initialize_Bin(ifstream& in, const vector<CBone*>& Bones)
{
	string strTemp;
	_uint iLength = 0;
	_char szBuffer[256];
	read_data(in, iLength);
	//in >> iLength;
	in.read(szBuffer, iLength);
	szBuffer[iLength] = '\0';
	m_strName = szBuffer;
	_uint iNumScaleFrame = 0;
	_uint iNumRotationFrame = 0;
	_uint iNumTranslationFrame = 0;
	read_data(in, iNumScaleFrame);
	read_data(in, iNumRotationFrame);
	read_data(in, iNumTranslationFrame);
	//in >> iNumScaleFrame;
	//in >> iNumRotationFrame;
	//in >> iNumTranslationFrame;

	auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone)->_bool
		{
			if (m_strName == pBone->Get_Name())
				return true;

			++m_iBoneIndex;

			return false;
		});

	m_iNumKeyFrame = max(iNumScaleFrame, max(iNumRotationFrame, iNumTranslationFrame));
	m_KeyFrames.resize(m_iNumKeyFrame);

	for (_uint i = 0; i < m_iNumKeyFrame; ++i)
	{
		if (i < iNumScaleFrame)
		{
			read_data(in, m_KeyFrames[i].vScale);
			//in >> m_KeyFrames[i].vScale.x >> m_KeyFrames[i].vScale.y >> m_KeyFrames[i].vScale.z;
		}
		else
		{
			m_KeyFrames[i].vScale = m_KeyFrames[i - 1].vScale;
		}
	}

	for (_uint i = 0; i < m_iNumKeyFrame; ++i)
	{
		if (i < iNumRotationFrame)
		{
			read_data(in, m_KeyFrames[i].vRotation);
			//in >> m_KeyFrames[i].vRotation.x >> m_KeyFrames[i].vRotation.y >> m_KeyFrames[i].vRotation.z >> m_KeyFrames[i].vRotation.w;
		}
		else
		{
			m_KeyFrames[i].vRotation = m_KeyFrames[i - 1].vRotation;
		}
	}

	for (_uint i = 0; i < m_iNumKeyFrame; ++i)
	{
		if (i < iNumTranslationFrame)
		{
			read_data(in, m_KeyFrames[i].vTranslation);
			//in >> m_KeyFrames[i].vTranslation.x >> m_KeyFrames[i].vTranslation.y >> m_KeyFrames[i].vTranslation.z;
		}
		else
		{
			m_KeyFrames[i].vTranslation = m_KeyFrames[i - 1].vTranslation;
		}
	}

	for (_uint i = 0; i < m_iNumKeyFrame; ++i)
	{
		read_data(in, m_KeyFrames[i].fTrackPosition);
		//in >> m_KeyFrames[i].fTrackPosition;
	}

	return S_OK;
}

void CChannel::Update(_float fCurrentTrackPosition, const vector<CBone*>& Bones, _uint& iCurrentKeyFrameIndex)
{
	KEYFRAME tLastFrame = m_KeyFrames.back();

	_matrix Transformation = XMMatrixIdentity();

	_vector vScale, vRotation, vTranslation;

	if (fCurrentTrackPosition >= tLastFrame.fTrackPosition)
	{
		vScale = XMLoadFloat3(&tLastFrame.vScale);
		vRotation = XMLoadFloat4(&tLastFrame.vRotation);
		vTranslation = XMVectorSetW(XMLoadFloat3(&tLastFrame.vTranslation), 1.f);
	}
	else
	{
		while (fCurrentTrackPosition >= m_KeyFrames[iCurrentKeyFrameIndex + 1].fTrackPosition)
			++iCurrentKeyFrameIndex;

		_float		fRatio = (fCurrentTrackPosition - m_KeyFrames[iCurrentKeyFrameIndex].fTrackPosition) /
			(m_KeyFrames[iCurrentKeyFrameIndex + 1].fTrackPosition - m_KeyFrames[iCurrentKeyFrameIndex].fTrackPosition);

		vScale = XMVectorLerp(XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrameIndex].vScale), XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrameIndex + 1].vScale), fRatio);
		vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_KeyFrames[iCurrentKeyFrameIndex].vRotation), XMLoadFloat4(&m_KeyFrames[iCurrentKeyFrameIndex + 1].vRotation), fRatio);
		vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrameIndex].vTranslation), XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrameIndex + 1].vTranslation), fRatio), 1.f);
	}

	Transformation = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
	Bones[m_iBoneIndex]->Set_TransformationMatrix(Transformation);

}

void CChannel::Lerp(_float fPosition, _float fLerp, const vector<CBone*>& Bones)
{
	KEYFRAME tLastFrame = m_KeyFrames.back();

	_matrix Transformation = XMMatrixIdentity();

	_vector vScale, vRotation, vTranslation;
	_uint iCurrentKeyFrameIndex = 0;

	if (fPosition >= tLastFrame.fTrackPosition)
	{
		vScale = XMLoadFloat3(&tLastFrame.vScale);
		vRotation = XMLoadFloat4(&tLastFrame.vRotation);
		vTranslation = XMVectorSetW(XMLoadFloat3(&tLastFrame.vTranslation), 1.f);
	}
	else
	{
		while (fPosition >= m_KeyFrames[iCurrentKeyFrameIndex + 1].fTrackPosition)
			++iCurrentKeyFrameIndex;

		_float		fRatio = (fPosition - m_KeyFrames[iCurrentKeyFrameIndex].fTrackPosition) /
			(m_KeyFrames[iCurrentKeyFrameIndex + 1].fTrackPosition - m_KeyFrames[iCurrentKeyFrameIndex].fTrackPosition);

		vScale = XMVectorLerp(XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrameIndex].vScale), XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrameIndex + 1].vScale), fRatio);
		vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_KeyFrames[iCurrentKeyFrameIndex].vRotation), XMLoadFloat4(&m_KeyFrames[iCurrentKeyFrameIndex + 1].vRotation), fRatio);
		vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrameIndex].vTranslation), XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrameIndex + 1].vTranslation), fRatio), 1.f);
	}

	Transformation = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
	Bones[m_iBoneIndex]->Set_TransformationMatrix(Bones[m_iBoneIndex]->Get_TransformationMatrix() * (fLerp) + Transformation * (1.f - fLerp));
}

void CChannel::FrameInfo(vector<KEYFRAME>& BoneFrame, _float fTrackPosition)
{
	KEYFRAME rtFrame = m_KeyFrames.back();

	_matrix Transformation = XMMatrixIdentity();

	_vector vScale, vRotation, vTranslation;
	_uint iCurrentKeyFrameIndex = 0;
	if (fTrackPosition >= rtFrame.fTrackPosition)
	{
		vScale = XMLoadFloat3(&rtFrame.vScale);
		vRotation = XMLoadFloat4(&rtFrame.vRotation);
		vTranslation = XMVectorSetW(XMLoadFloat3(&rtFrame.vTranslation), 1.f);
	}
	else
	{
		while (fTrackPosition >= m_KeyFrames[iCurrentKeyFrameIndex + 1].fTrackPosition)
			++iCurrentKeyFrameIndex;

		_float		fRatio = (fTrackPosition - m_KeyFrames[iCurrentKeyFrameIndex].fTrackPosition) /
			(m_KeyFrames[iCurrentKeyFrameIndex + 1].fTrackPosition - m_KeyFrames[iCurrentKeyFrameIndex].fTrackPosition);

		vScale = XMVectorLerp(XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrameIndex].vScale), XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrameIndex + 1].vScale), fRatio);
		vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_KeyFrames[iCurrentKeyFrameIndex].vRotation), XMLoadFloat4(&m_KeyFrames[iCurrentKeyFrameIndex + 1].vRotation), fRatio);
		vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrameIndex].vTranslation), XMLoadFloat3(&m_KeyFrames[iCurrentKeyFrameIndex + 1].vTranslation), fRatio), 1.f);
	}
	XMStoreFloat3(&rtFrame.vScale, vScale);
	XMStoreFloat4(&rtFrame.vRotation, vRotation);
	XMStoreFloat3(&rtFrame.vTranslation, vTranslation);

	BoneFrame[m_iBoneIndex] = rtFrame;
}

CChannel* CChannel::Create(ifstream& in, const vector<CBone*>& Bones)
{
	CChannel* pInstance = new CChannel;
	if (FAILED(pInstance->Initialize(in, Bones)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CChannel* CChannel::Create(ifstream& in, const vector<CBone*>& Bones, bool bBinFlag)
{
	CChannel* pInstance = new CChannel;
	if (FAILED(pInstance->Initialize_Bin(in, Bones)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free()
{
	__super::Free();
}

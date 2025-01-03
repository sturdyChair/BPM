#include "Statue_Gimmick.h"
#include "GameInstance.h"
#include "Statue.h"
#include "Beat_Manager.h"
#include "Monster_Nidhog.h"
#include "Effect_StatueFragment.h"
CStatue_Gimmick::CStatue_Gimmick():
	CBase()
{
	m_pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(m_pGameInstance);
	m_pBeatManager = CBeat_Manager::Get_Instance();
}

HRESULT CStatue_Gimmick::Initialize(CMonster_Nidhog* pNidHog)
{
	for (auto& pStatue : m_pStatues)
	{
		pStatue = (CStatue*)m_pGameInstance->Push_Object_Hijack(L"Prototype_GameObject_Statue", this);
		Safe_AddRef(pStatue);
	}
	_matrix World = XMMatrixIdentity();
	World.r[3] = XMVectorSet(-9.f, -10.f, -15.f, 1.f);
	m_pStatues[0]->Get_Transform()->Set_WorldMatrix(World);
	m_pStatues[0]->Set_Idx(0);
	World.r[3] = XMVectorSet(0.f, -10.f, -15.f, 1.f);
	m_pStatues[1]->Get_Transform()->Set_WorldMatrix(World);
	m_pStatues[1]->Set_Idx(1);
	World.r[3] = XMVectorSet(9.f, -10.f, -15.f, 1.f);
	m_pStatues[2]->Get_Transform()->Set_WorldMatrix(World);
	m_pStatues[2]->Set_Idx(2);

	m_vecPattern.resize(3);
	m_vecPattern[0].resize(3);
	m_vecPattern[0][0] = 0;
	m_vecPattern[0][1] = 1;
	m_vecPattern[0][2] = 2;

	m_vecPattern[1].resize(3);
	m_vecPattern[1][0] = 0;
	m_vecPattern[1][1] = 2;
	m_vecPattern[1][2] = 1;

	m_vecPattern[2].resize(6);
	m_vecPattern[2][0] = 0;
	m_vecPattern[2][1] = 2;
	m_vecPattern[2][2] = 1;
	m_vecPattern[2][3] = 2;
	m_vecPattern[2][4] = 1;
	m_vecPattern[2][5] = 2;

	m_iCurrBC = m_pBeatManager->Get_BeatCount();
	m_iPrevBC = m_iCurrBC;

	m_pNidhog = pNidHog;

	return S_OK;
}

void CStatue_Gimmick::PriorityTick(_float fTimeDelta)
{
	m_iPrevBC = m_iCurrBC;
	m_iCurrBC = m_pBeatManager->Get_BeatCount();
	if (m_iPrevBC != m_iCurrBC)
	{
		Beat_Moved();
	}
	m_bDamaged = false;
	for (auto& pStatue : m_pStatues)
	{
		pStatue->PriorityTick(fTimeDelta);
		if (pStatue->Get_Damage_Timer() > 0.01f)
		{
			m_bDamaged = true;
			pStatue->Set_Color(XMVectorSet(2.f, 0.7f, 0.7f, 1.f));
			m_iCurrPatternIndicator = 0;
		}
		else
		{
			pStatue->Set_Color(XMVectorSet(1.f, 1.f, 1.f, 1.f));
		}
	}
}

void CStatue_Gimmick::Tick(_float fTimeDelta)
{
	for (_uint i = 0;i<3;++i)
	{
		auto pStatue = m_pStatues[i];
		CTransform* pTransform = pStatue->Get_Transform();

		_vector vPos = pTransform->Get_Position();
		pTransform->LookAt(m_pNidhog->Get_Transform()->Get_Position());

		_float y = XMVectorGetY(vPos);
		if (y < 0.f)
		{	

			vPos = XMVectorSetY(vPos, y + 20.f * fTimeDelta);
			pTransform->Set_Position(vPos);

		}
		pStatue->Tick(fTimeDelta);
	}
	if (!m_bDamaged)
	{
		if (m_iCurrPattern < m_vecPattern.size())
		{
			_uint iCurrStatueIdx = m_vecPattern[m_iCurrPattern][m_iCurrPatternIndicator];

			for (_uint i = 0; i < 3; ++i)
			{
				if(i == iCurrStatueIdx)
				{
					if (m_iCurrPatternIndicator == 0)
					{
						m_pStatues[iCurrStatueIdx]->Set_Color(XMVectorSet(0.7f, 2.0f, 0.7f, 1.f));
					}
					else
					{
						m_pStatues[iCurrStatueIdx]->Set_Color(XMVectorSet(0.7f, 0.7f, 2.0f, 1.f));
					}
					CTransform* pTransform = m_pStatues[iCurrStatueIdx]->Get_Transform();
					_vector vPos = pTransform->Get_Position();
					pTransform->LookAt(m_pNidhog->Get_Transform()->Get_Position());

					_float y = XMVectorGetY(vPos);
					m_pStatues[iCurrStatueIdx]->Set_Offset(2.f);
				}
				else
				{
					m_pStatues[i]->Set_Color(XMVectorSet(1.f, 1.f, 1.f, 1.f));
					m_pStatues[i]->Set_Offset(0.f);
				}
			}
		}
	}
}

void CStatue_Gimmick::LateTick(_float fTimeDelta)
{
	for (auto& pStatue : m_pStatues)
	{
		pStatue->LateTick(fTimeDelta);
	}
}

void CStatue_Gimmick::Statue_Damaged(_uint iIdx)
{
	if (m_vecPattern[m_iCurrPattern][m_iDamageCount] == iIdx)
	{
		++m_iDamageCount;
		if (m_iDamageCount >= m_vecPattern[m_iCurrPattern].size())
		{
			m_iDamageCount = 0;
			++m_iCurrPattern;
			m_pStatues[0]->Set_Damage_Timer(0.f);
			m_pStatues[1]->Set_Damage_Timer(0.f);
			m_pStatues[2]->Set_Damage_Timer(0.f);
		}
		else
		{
			_float fBeat = m_pBeatManager->Get_Beat();
			fBeat *= 3.1f;
			for (_uint i = 0; i < m_iDamageCount;++i)
			{
				m_pStatues[m_vecPattern[m_iCurrPattern][i]]->Set_Damage_Timer(fBeat);
			}
		}
		if (m_iCurrPattern >= m_vecPattern.size())
		{
			//Kill Statue
			m_pNidhog->Kill_Statue();
			_uint iLevelIdx = m_pGameInstance->Get_Current_LevelID();
			CEffect_StatueFragment::PROJECTILE_DESC desc;
			desc.eCG = CG_MONSTER_PROJECTILE;
		
			for (_uint i = 0; i < 3; ++i)
			{
				for (_uint j = 0; j < 4; ++j)
				{
					_float fRandSpeed = fRand(20.f, 50.f);
					desc.vDirection = _float3{fRand(-fRandSpeed,fRandSpeed),fRand(-fRandSpeed,fRandSpeed),fRand(-fRandSpeed,fRandSpeed) };
					_wstring strTag = _wstring(L"Prototype_GameObject_Effect_StatueFragment") + to_wstring(j);
					m_pGameInstance->Push_Object_From_Pool(strTag, iLevelIdx, L"Effect", &desc, m_pStatues[i]->Get_Transform()->Get_WorldMatrix_Ptr());
				}
				//CMonster_Base::MONSTER_DESC monDesc;
				//monDesc.Transformation = m_pStatues[i]->Get_Transform()->Get_WorldMatrix();
				//m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Monster_Reaper", iLevelIdx, L"Monster", &monDesc);
			}

		

			return;
		}
	}
	else
	{
		m_iDamageCount = 0;
		m_pStatues[0]->Set_Damage_Timer(0.f);
		m_pStatues[1]->Set_Damage_Timer(0.f);
		m_pStatues[2]->Set_Damage_Timer(0.f);
		if (m_bGimmickEnd)
		{
			//Kill Statue
			m_pNidhog->Kill_Statue();
			_uint iLevelIdx = m_pGameInstance->Get_Current_LevelID();
			CEffect_StatueFragment::PROJECTILE_DESC desc;
			desc.eCG = CG_MONSTER_PROJECTILE;

			for (_uint i = 0; i < 3; ++i)
			{
				for (_uint j = 0; j < 4; ++j)
				{
					_float fRandSpeed = fRand(20.f, 50.f);
					desc.vDirection = _float3{ fRand(-fRandSpeed,fRandSpeed),fRand(-fRandSpeed,fRandSpeed),fRand(-fRandSpeed,fRandSpeed) };
					_wstring strTag = _wstring(L"Prototype_GameObject_Effect_StatueFragment") + to_wstring(j);
					m_pGameInstance->Push_Object_From_Pool(strTag, iLevelIdx, L"Effect", &desc, m_pStatues[i]->Get_Transform()->Get_WorldMatrix_Ptr());
				}
			}


			return;
		}
	}
}

void CStatue_Gimmick::Beat_Moved()
{
	++m_iCurrPatternIndicator;
	if(m_iCurrPattern <= 2)
	{
		if (m_vecPattern[m_iCurrPattern].size() <= m_iCurrPatternIndicator)
		{
			m_iCurrPatternIndicator = 0;
		}
	}
}

CStatue_Gimmick* CStatue_Gimmick::Create(CMonster_Nidhog* pNidHog)
{
	CStatue_Gimmick* pInstance = new CStatue_Gimmick;
	auto hr = pInstance->Initialize(pNidHog);
	if (FAILED(hr))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatue_Gimmick::Free()
{
	__super::Free();
	Safe_Release(m_pGameInstance);
	for (auto& pStatue : m_pStatues)
	{
		Safe_Release(pStatue);
	}
}

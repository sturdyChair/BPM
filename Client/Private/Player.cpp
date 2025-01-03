#include "..\Public\Player.h"
#include "CharacterController.h"
#include "GameInstance.h"
#include "Camera_FPS.h"
#include "UI_Controller.h"
#include "UI_Include.h"
#include "Beat_Manager.h"
#include "Shader.h"
#include "Model.h"
#include "Weapon.h"
#include "Item_Equip.h"
#include "Effect_Hex_Shield.h"
#include "Effect_RuneField.h"
#include "Effect_FireBall.h"
#include "Effect_ThreeRune.h"
#include "Effect_Emission.h"
#include "Effect_ThunderClap.h"
#include "Effect_FireWall.h"
#include "Level_Loading.h"
#include "Particle.h"
#include "AfterImage.h"
#include "Effect_Sonar.h"
#include "Effect_BlockLight.h"
#include "SpotLight.h"
#include "SkeletonKey.h"

const string CPlayer::strWeapon_r = "weapon_R";
const string CPlayer::strWeapon_l = "weapon_L";
const string CPlayer::strMegaShot = "FP_player.ao|FP_player_Mega_Shot_02";
const string CPlayer::strThunderClap = "FP_player.ao|FP_player_Make_It_Rain_01";

const string CPlayer::strHealOverTime = "FP_player.ao|FP_player_Regen_health_over_time_01";
const string CPlayer::strBuff = "FP_player.ao|FP_player_Speed_Empower_01";
const string CPlayer::strFireWall = "FP_player.ao|FP_fireball_LH";
const string CPlayer::strSummonAlly = "FP_player.ao|FP_player_Spawn_Ally_01";
//
CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	: CGameObject(rhs)
{
	m_pUIController = CUI_Controller::Get_Instance();
	Safe_AddRef(m_pUIController);
	m_pBeatManager = CBeat_Manager::Get_Instance();
	m_iCollisionGroup = CG_PLAYER;
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	m_fMouseSensor = 0.7f;
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	if (FAILED(__super::Initialize(&desc)))
		return E_FAIL;
	CCharacterController::CCT_DESC cctDesc;
	cctDesc.pTransform = m_pTransformCom;
	cctDesc.height = 2.0f;
	cctDesc.radius = 0.5f;
	cctDesc.position = PxExtendedVec3(0.0f, 10.0f, 0.0f);
	cctDesc.material = m_pGameInstance->Get_Physics()->createMaterial(0.5f, 0.5f, 0.5f);
	cctDesc.userData = this;
	m_pTransformCom->Set_Position(XMVectorSet(0.f, 10.f, 0.f, 1.f));
	Add_Component(LEVEL_STATIC, L"Prototype_Component_CharacterController", L"Com_CCT", (CComponent**)&m_pCCTCom, &cctDesc);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Transform", L"Com_CamTrans", (CComponent**)&m_pCamTransformCom);
	m_pCamera = static_cast<CCamera_FPS*>(m_pGameInstance->Get_Camera(L"FPS"));
	m_pCamera->Set_Target(m_pCamTransformCom);
	m_pCamera->Set_Offset(XMVectorSet(0.f, 1.f, 0.f, 0.f));
	Safe_AddRef(m_pCamera);
	m_pCCTCom->m_bGravity = true;
	m_pCCTCom->m_bUseVelocity_Calculation = true;
	m_pCCTCom->m_fFriction = 5.f;


	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Player", L"Model", (CComponent**)&m_pModelCom);

	m_pModelCom->Set_AnimIndex(0, true);

	m_fFov = XM_PI * 0.33f;
	m_fFovOri = XM_PI * 0.33f;
	XMStoreFloat4x4(&m_ModelTransform,
		XMMatrixAffineTransformation(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f), XMVectorZero(),
			XMQuaternionRotationRollPitchYawFromVector(XMVectorSet(XM_PI * 0.5f, XM_PI, 0.f, 0.f)), XMVectorSet(0.f, -1.4f, -0.18f, 1.f)));

	XMStoreFloat4x4(&m_ModelTransformSecond,
		XMMatrixAffineTransformation(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f), XMVectorZero(),
			XMQuaternionRotationRollPitchYawFromVector(XMVectorSet(XM_PI *1.0f, XM_PI, 0.f, 0.f)), XMVectorSet(0.f, -1.4f, -0.18f, 1.f)));
	

	m_pHex_Shield = (CEffect_Hex_Shield*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_Hex_Shield", m_pCamTransformCom->Get_WorldMatrix_Ptr());
	//m_pThreeRune = (CEffect_ThreeRune*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_ThreeRune", m_pCamTransformCom->Get_WorldMatrix_Ptr());
	m_pThreeRune = (CEffect_ThreeRune*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_ThreeRune", &m_MatHandBone);

	m_pHex_Shield->Get_Transform()->Set_Position(XMVectorSet(0.f, 1.0f, 2.f, 1.f));
	//m_pThreeRune->Get_Transform()->Set_Position(XMVectorSet(0.f, 1.0f, 2.0f, 1.f));
	m_pHex_Shield->Get_Transform()->Rotation(XMVectorSet(1.f,0.f,0.f,0.f), XM_PIDIV2);
	m_vTranslation = _float3{ 0.f,0.f,0.f };
	m_pModelCom->Set_AnimTPS(strMegaShot, m_pModelCom->Get_AnimTPS(strMegaShot) * 1.2f);

	auto pAbilitySlot = m_pUIController->Get_AbilitySlot();
	pAbilitySlot->Set_Ability_ID(CUI_AbilitySlot::AS_ULT, m_eUlt);
	pAbilitySlot->Set_Ability_ID(CUI_AbilitySlot::AS_SEC, m_eSec);
	m_pParticleBuff = (CParticle*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_Particle_Buff_Rising", nullptr);
	m_pParticleHeal = (CParticle*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_Particle_Heal_Rising", nullptr);

	// m_pAfterImage = (CAfterImage*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_AfterImage_Anim_0.03sec_10", m_pModelCom);
	m_matCamOffset = Identity;

	SpotLight spotLight;
	spotLight.color = _float3{ 1.f,0.3f,0.3f };
	spotLight.direction = _float3{ 0.f,0.f,1.f };
	spotLight.innerConeCos = cos(XM_PI * 0.07f);
	spotLight.intensity = 5.f;
	spotLight.outerConeCos = cos(XM_PI * 0.14f);
	spotLight.range = 30.f;
	spotLight.position = _float3{ 0.f,0.f,0.f };
	spotLight.specColor = _float3{ 1.f,1.f,1.f };
	m_pGameInstance->Add_Light(m_strSpotLight, spotLight);
	m_iCoin = 28;
	m_iKey = 3;
	return S_OK;
}

void CPlayer::PriorityTick(_float fTimeDelta)
{
	m_iPrevBeatCount = m_iCurBeatCount;
	m_iCurBeatCount = m_pBeatManager->Get_BeatCount();
	m_fInvincible -= fTimeDelta;
#pragma region CamOffset
	if(!m_bDash)
	{
		if (m_matCamOffset._41 > 0.01f)
		{
			m_matCamOffset._41 -= fTimeDelta * 2.f;
		}
		else if (m_matCamOffset._41 < -0.01f)
		{
			m_matCamOffset._41 += fTimeDelta * 2.f;
		}
		else
		{
			m_matCamOffset._41 = 0.f;
		}
		if (m_matCamOffset._42 > 0.01f)
		{
			m_matCamOffset._42 -= fTimeDelta * 2.f;
		}
		else if (m_matCamOffset._42 < -0.01f)
		{
			m_matCamOffset._42 += fTimeDelta * 2.f;
		}
		else
		{
			m_matCamOffset._42 = 0.f;
		}
		if (m_matCamOffset._43 > 0.01f)
		{
			m_matCamOffset._43 -= fTimeDelta * 2.f;
		}
		else if (m_matCamOffset._43 < -0.01f)
		{
			m_matCamOffset._43 += fTimeDelta * 2.f;
		}
		else
		{
			m_matCamOffset._43 = 0.f;
		}
	}
#pragma endregion


	if (m_fInvincible < 0.f)
	{
		m_fInvincible = 0.f;
	}
	if (m_pBeatManager->Get_Timer() > 0.7f * m_pBeatManager->Get_Beat() && m_pBeatManager->Get_Timer() - fTimeDelta < 0.7f)
	{
		m_bBeatMoved = false;
		if (m_ArrItem[ITEM_GLOVE_SIXSHOOTER])
		{
			m_iMoveLeft = 1;
		}
	}
	else if (m_pBeatManager->Get_Timer() > 0.3f * m_pBeatManager->Get_Beat())
	{
		m_bBeatMoved = true;
		m_iMovedBeat = m_iCurBeatCount;
	}
	
	if (m_pWeapon)
	{
		auto pSlot = m_pUIController->Get_WeaponSlot();
		pSlot->Set_CurrAmo(m_pWeapon->Get_Amo());
		pSlot->Set_MaxAmo(m_pWeapon->Get_MaxAmo());
		pSlot->Set_Weapon(m_pWeapon->Get_Weapon_ID());
		m_pWeapon->PriorityTick(fTimeDelta);
	}
	auto pHpUI = m_pUIController->Get_Hp();
	pHpUI->Set_MaxHp(m_fMaxHp);
	pHpUI->Set_Hp(m_fHp);
	pHpUI->Set_Shield(m_fShield);
	pHpUI->Set_Coin(m_iCoin);
	pHpUI->Set_Key(m_iKey);
	for (_uint i = 0; i < SLOT_END; ++i)
	{
		if (m_ArrEquip[i])
		{
			auto pSlotUI = m_pUIController->Get_EquipSlot();
			pSlotUI->Set_SlotID((SLOT_ID)i, m_ArrEquip[i]->Get_ID());
		}
		else
		{
			auto pSlotUI = m_pUIController->Get_EquipSlot();
			SLOT_ID eSlot = (SLOT_ID)i;
			switch (eSlot)
			{
			case SLOT_ARM:
			{
				pSlotUI->Set_SlotID(eSlot, ITEM_EMPTY_ARMS);
			}
			break;
			case SLOT_BOOTS:
			{
				pSlotUI->Set_SlotID(eSlot, ITEM_EMPTY_FEET);
			}
			break;
			case SLOT_HEAD:
			{
				pSlotUI->Set_SlotID(eSlot, ITEM_EMPTY_HEAD);
			}
			break;
			case SLOT_CHEST:
			{
				pSlotUI->Set_SlotID(eSlot, ITEM_EMPTY_SHEILD);
			}
			break;
			default:
				break;
			}

		}
	}
	m_bFrameEquipped = false;
	m_pHex_Shield->PriorityTick(fTimeDelta);
	if (m_pModelCom->Get_Current_Animation_Tag() == strMegaShot)
	{
		XMStoreFloat4x4(&m_MatHandBone, m_pModelCom->Get_BoneMatrix(strWeapon_l) * m_pTransformCom->Get_WorldMatrix_XMMat());
		m_pThreeRune->PriorityTick(fTimeDelta);
	}

	if (m_fStompTimer > 0.f)
	{
		m_fStompTimer -= fTimeDelta;
	}
	if (m_fHealTimer > 0.f)
	{
		m_fHealTimer -= fTimeDelta;
		m_pParticleHeal->PriorityTick(fTimeDelta);
		if (m_iPrevBeatCount != m_iCurBeatCount)
		{
			Heal(5.f);
		}
	}
	if (m_fBuffTimer > 0.f)
	{
		m_fBuffTimer -= fTimeDelta;
		m_pParticleBuff->PriorityTick(fTimeDelta);
	}
	//m_pAfterImage->PriorityTick(fTimeDelta);
	m_bNoMiss = m_bNoMissNextFrame;
	m_bNoMissNextFrame = false;
	if (m_pGameInstance->Get_KeyDown(DIK_1))
	{
		m_bDebugNoMiss = !m_bDebugNoMiss;
	}
	if (m_bDebugNoMiss)
	{
		m_bNoMiss = true;
	}
}

void CPlayer::Tick(_float fTimeDelta)
{
	PxControllerState pCS;
	m_pCCTCom->Get_Controller()->getState(pCS);
	if (pCS.collisionFlags & PxControllerCollisionFlag::eCOLLISION_DOWN)
	{
		m_pCCTCom->m_vVelocity.y = 0.f;
		m_iJumpCount = 2;

		if (m_bDash && m_vDashVelocity.y < -1.0f && m_fStompTimer < 0.01f)
		{
			_matrix ImpactWorld = XMMatrixIdentity();
			ImpactWorld.r[3] = m_pTransformCom->Get_Position();
			_float4x4 Impact;
			XMStoreFloat4x4(&Impact, ImpactWorld);
			m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_DiscImpact", m_pGameInstance->Get_Current_LevelID(), L"Effect", nullptr,&Impact);

			Cam_Shake(0.2f, 0.2f);
			m_fStompTimer = m_pBeatManager->Get_Beat();


		}
	}
	if(m_bInControl)
	{
		if (!m_bDash)
		{
			_bool bMoved = false;
			if (m_pGameInstance->Get_KeyPressing(DIK_A))
			{
				m_pCCTCom->Move_Land_Left(m_fSpeed);
				if (m_vTranslation.x < 0.05f)
				{
					m_vTranslation.x += fTimeDelta * 0.5f;
				}
				else if (m_vTranslation.x > 0.06f)
				{
					m_vTranslation.x -= fTimeDelta * 1.5f;
				}
				else
				{
					m_vTranslation.x = 0.05f;
				}
				bMoved = true;
			}
			else
			{
				if (m_vTranslation.x > 0.01f)
				{
					m_vTranslation.x -= fTimeDelta;
				}
			}
			if (m_pGameInstance->Get_KeyPressing(DIK_D))
			{
				m_pCCTCom->Move_Land_Right(m_fSpeed);
				if (m_vTranslation.x > -0.05f)
				{
					m_vTranslation.x -= fTimeDelta * 0.5f;
				}
				else if (m_vTranslation.x < -0.06f)
				{
					m_vTranslation.x += fTimeDelta * 1.5f;
				}
				else
				{
					m_vTranslation.x = -0.05f;
				}
				bMoved = true;
			}
			else
			{
				if (m_vTranslation.x < -0.01f)
				{
					m_vTranslation.x += fTimeDelta;
				}
			}
			if (m_pGameInstance->Get_KeyPressing(DIK_W))
			{
				m_pCCTCom->Move_Land_Forward(m_fSpeed);
				if (m_vTranslation.z > -0.05f)
				{
					m_vTranslation.z -= fTimeDelta * 0.5f;
				}
				else if (m_vTranslation.z < -0.06f)
				{
					m_vTranslation.z += fTimeDelta * 1.5f;
				}
				else
				{
					m_vTranslation.z = -0.05f;
				}
				bMoved = true;
			}
			else
			{
				if (m_vTranslation.z < -0.01f)
				{
					m_vTranslation.z += fTimeDelta;
				}
			}
			if (m_pGameInstance->Get_KeyPressing(DIK_S))
			{
				m_pCCTCom->Move_Land_Backward(m_fSpeed);
				if (m_vTranslation.z < 0.05f)
				{
					m_vTranslation.z += fTimeDelta * 0.5f;
				}
				else if (m_vTranslation.z > 0.06f)
				{
					m_vTranslation.z -= fTimeDelta * 1.5f;
				}
				else
				{
					m_vTranslation.z = 0.05f;
				}
				bMoved = true;
			}
			else
			{
				if (m_vTranslation.z > 0.01f)
				{
					m_vTranslation.z -= fTimeDelta;
				}
			}
			if (bMoved)
			{
				m_fYTrans += fTimeDelta * XM_2PI * 2;
				m_vTranslation.y = cos(m_fYTrans) * 0.01f;
			}
			else
			{
				m_vTranslation.y = 0.f;
				m_fYTrans = 0.f;
			}

			if (m_pGameInstance->Get_KeyDown(DIK_G))
			{
				_matrix world = XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix());
				world.r[3] += XMVector3Normalize(world.r[2]) * 10.f;
				m_pCCTCom->Set_Position(world);
			}


			if (m_iJumpCount > 0)
			{
				if (m_pGameInstance->Get_KeyDown(DIK_SPACE))
				{
					_int iRand = rand() % 3;
					_wstring strTag = _wstring(L"Player_Jump") + to_wstring(iRand);
					m_pCCTCom->m_vVelocity.y = 15.f;
					if (m_fStompTimer > 0.01f)
					{
						m_fStompTimer = 0.f;
						m_pCCTCom->m_vVelocity.y = 30.0f;
					}
					--m_iJumpCount;
					if (m_iJumpCount <= 0 && m_fRollTime <= 0.f && m_ArrItem[ITEM_FOOT_SANDALS])
					{
						m_fRollTime += 0.5f;
						XMStoreFloat3(&m_vRollAxis, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
						m_WorldBeforeRoll = m_pTransformCom->Get_WorldMatrix();
					}
				}
			}
			if (m_pModelCom->Get_Current_Animation_Tag() == strMegaShot)
			{
				_float fTR = m_pModelCom->Get_Current_Animation_TrackRatio();
				if (fTR >= 0.1f + 0.05f * (8 - m_iMegaShotCounter) && m_iMegaShotCounter > 0)
				{
					CEffect_FireBall::PROJECTILE_DESC desc;
					desc.eCG = CG_PLAYER_PROJECTILE;

					//m_pGameInstance->Map_Picking_Position();
					//_float4 vPos = m_pGameInstance->Pick_Position(g_iWinSizeX / 2 + 20, g_iWinSizeY / 2 - 80);
					//desc.vDestination.x = vPos.x;
					//desc.vDestination.y = vPos.y;
					//desc.vDestination.z = vPos.z;
					//m_pGameInstance->Unmap_Picking_Position();
					_vector vDest = m_pTransformCom->Get_Position() + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 3.f;
					XMStoreFloat3(&desc.vDestination, vDest);
					_float4x4 Mat;
					_matrix Temp = m_pTransformCom->Get_WorldMatrix_XMMat();
					Temp.r[3] = vDest - m_pTransformCom->Get_State(CTransform::STATE_LOOK);
					XMStoreFloat4x4(&Mat, Temp);
					m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_FireBall", m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, &Mat);
					--m_iMegaShotCounter;
					m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_CastFireball");
					m_pThreeRune->Set_ColorBoost(1.f);
				}
			}
			else
			{
				if (m_pGameInstance->Get_MouseDown(DIMK_LBUTTON))
				{
					Shoot(fTimeDelta);
				}
				if (m_pGameInstance->Get_MousePressing(DIMK_LBUTTON))
				{
					if (m_pWeapon && m_pWeapon->Get_Weapon_ID() == WEAPON_MINIGUN)
					{
						Shoot(fTimeDelta);
					}
				}
				else
				{
					m_iGatlingStreak = 0;
					m_fGatlingTimer = 0.f;
				}
				if (m_pGameInstance->Get_MouseDown(DIMK_RBUTTON))
				{
					Reload(fTimeDelta);
				}
			}
			if (m_pGameInstance->Get_KeyDown(DIK_LSHIFT) && m_iDashCoolDown <= 0)
			{
				Dash(fTimeDelta);
			}
			if (m_pGameInstance->Get_KeyDown(DIK_LCONTROL) && m_iDashCoolDown <= 0 && m_iJumpCount != 2)
			{
				Stomp(fTimeDelta);
			}
			if (m_pGameInstance->Get_KeyDown(DIK_R))
			{
				Parry(fTimeDelta);
			}
			if (m_pGameInstance->Get_KeyDown(DIK_Q))
			{
				MegaShoot(fTimeDelta);
			}
			if (m_pGameInstance->Get_KeyDown(DIK_T))
			{
				Clap(fTimeDelta);
			}
			if (m_pGameInstance->Get_KeyDown(DIK_Z))
			{
				Buff(fTimeDelta);
			}
			if (m_pGameInstance->Get_KeyDown(DIK_X))
			{
				HealOverTime(fTimeDelta);

			}
			if (m_pGameInstance->Get_KeyDown(DIK_C))
			{
				FireWall();
				//{
				//	CEffect_BlockLight::BLOCK_DESC bdesc;
				//	bdesc.bOdd = true;
				//	bdesc.fBlockSize = 5.f;
				//	bdesc.fLifeTime = m_pBeatManager->Get_Beat() * 2.f;
				//	bdesc.vColor = _float4{ 1.f,0.5f,0.2f,1.f };
				//	XMStoreFloat4(&bdesc.vOrigin, m_pTransformCom->Get_Position());
				//	m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_BlockLight", m_pGameInstance->Get_Current_LevelID(), L"Effect", &bdesc);
				//}
			}
			if (m_pGameInstance->Get_KeyDown(DIK_V))
			{
				SummonAlly(fTimeDelta);
				//{
				//	CEffect_BlockLight::BLOCK_DESC bdesc;
				//	bdesc.bOdd = false;
				//	bdesc.fBlockSize = 5.f;
				//	bdesc.fLifeTime = m_pBeatManager->Get_Beat() * 2.f;
				//	bdesc.vColor = _float4{ 1.f,0.5f,0.2f,1.f };
				//	XMStoreFloat4(&bdesc.vOrigin, m_pTransformCom->Get_Position());
				//	m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_BlockLight", m_pGameInstance->Get_Current_LevelID(), L"Effect", &bdesc);
				//}
			}
			m_fFov = m_fFovOri;
			auto iterPush = m_vecPushVector.begin();
			for (; iterPush != m_vecPushVector.end();)
			{
				if (iterPush->second <= 0.f)
				{
					iterPush = m_vecPushVector.erase(iterPush);
				}
				else
				{
					m_pCCTCom->m_vDisplacement.x += iterPush->first.x;
					m_pCCTCom->m_vDisplacement.y += iterPush->first.y;
					m_pCCTCom->m_vDisplacement.z += iterPush->first.z;
					iterPush->second -= fTimeDelta;
					++iterPush;
				}
			}
		}
		else
		{
			m_fDashTimer -= fTimeDelta;
			m_fFov = m_fFovOri * (1.f + 0.33f * m_fDashTimer / m_pBeatManager->Get_Beat() * 0.5f);

			m_pCCTCom->Set_Displacement(m_vDashVelocity);
			if (m_fDashTimer <= 0.f)
			{
				m_fDashTimer = 0.f;
				m_vDashVelocity = _float3{ 0.f,0.f,0.f };
				m_pCCTCom->m_bGravity = true;
				m_bDash = false;
			}

		}
	}
	if (m_iCurBeatCount != m_iPrevBeatCount)
	{
		m_iDashCoolDown -= 1;
		if (m_iDashCoolDown <= 0)
		{
			m_iDashCoolDown = 0;
		}
	}
	ThunderClap(fTimeDelta);
	//static _float fov = 0.33f;
	//if (m_pGameInstance->Get_KeyPressing(DIK_R))
	//{
	//	fov += fTimeDelta;
	//}
	//if (m_pGameInstance->Get_KeyPressing(DIK_F))
	//{
	//	fov -= fTimeDelta;
	//}
	//
	//m_pCamera->Set_Fov(fov * XM_PI);



	if (!m_bBossIntro)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * m_pGameInstance->Get_DIMouseMove(DIMM_X) * m_fMouseSensor);
		_float fYMovement = m_pGameInstance->Get_DIMouseMove(DIMM_Y);
		_float fDot = XMVectorGetX(XMVector3Dot(XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK)), XMVectorSet(0.f, -1.f, 0.f, 0.f)));

		if (fYMovement > 0.f && fDot < 0.95f)
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * m_pGameInstance->Get_DIMouseMove(DIMM_Y) * m_fMouseSensor);
		}
		if (fYMovement < 0.f && fDot > -0.95f)
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * m_pGameInstance->Get_DIMouseMove(DIMM_Y) * m_fMouseSensor);
		}
	}

	m_pCCTCom->Update(fTimeDelta);



	//static _float fPitch = 0.5f, fYaw = 1.f;

	//if (m_pGameInstance->Get_KeyPressing(DIK_H))
	//{
	//	fPitch += fTimeDelta;
	//}
	//if (m_pGameInstance->Get_KeyPressing(DIK_Y))
	//{
	//	fPitch -= fTimeDelta;
	//}
	//if (m_pGameInstance->Get_KeyPressing(DIK_U))
	//{
	//	fYaw += fTimeDelta;
	//}
	//if (m_pGameInstance->Get_KeyPressing(DIK_J))
	//{
	//	fYaw -= fTimeDelta;
	//}

	//static _float fY = -1.4f, fZ = -0.18f;
	//if (m_pGameInstance->Get_KeyPressing(DIK_I))
	//{
	//	fY += fTimeDelta;
	//}
	//if (m_pGameInstance->Get_KeyPressing(DIK_K))
	//{
	//	fY -= fTimeDelta;
	//}
	//if (m_pGameInstance->Get_KeyPressing(DIK_O))
	//{
	//	fZ += fTimeDelta;
	//}
	//if (m_pGameInstance->Get_KeyPressing(DIK_L))
	//{
	//	fZ -= fTimeDelta;
	//}

	//XMStoreFloat4x4(&m_ModelTransform,
	//	XMMatrixAffineTransformation(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f), XMVectorZero(),
	//		XMQuaternionRotationRollPitchYawFromVector(XMVectorSet(XM_PI * fPitch, XM_PI * fYaw, 0.f, 0.f)), XMVectorSet(0.f, fY, fZ, 1.f)));

	//

	m_pModelCom->Play_Animation(fTimeDelta, m_fAnimFrom, m_fAnimTo, (m_pWeapon->Get_Weapon_ID() != WEAPON_MINIGUN));

	if (m_pModelCom->Get_Finished())
	{
		m_fAnimFrom = 0.f;
		m_fAnimTo	= 1.f;
		if (m_pWeapon)
		{
			string strIdle;
			m_pWeapon->Idle(strIdle);
			m_pModelCom->Set_AnimTag(strIdle, true);
		}
		if (m_pModelCom->Get_Current_Animation_Tag() == strMegaShot)
		{
			m_pThreeRune->Dying();
		}
	}

	if (m_pModelCom->Get_Current_Animation_Tag() == "FP_player.ao|Hand_MiniGun_reload"
		|| m_pModelCom->Get_Current_Animation_Tag() == "FP_player.ao|Hand_MiniGun_Fire_Loop"
		|| m_pModelCom->Get_Current_Animation_Tag() == "FP_player.ao|Hand_MiniGun_Flourish")
	{
		m_TransformMat = m_ModelTransformSecond;
	}
	else
	{
		m_TransformMat = m_ModelTransform;
	}
	if (m_ArrItem[ITEM_CHEST_TALLSHIELD])
	{
		if(m_fShield < 25.f)
			m_fShield += fTimeDelta * 2.f;
	}
	XMStoreFloat4x4(&m_TransformMat, XMLoadFloat4x4(&m_TransformMat) * 
		XMMatrixTranslation(m_vTranslation.x, m_vTranslation.y, m_vTranslation.z)
		* m_pGameInstance->Get_Transform_Matrix(CPipeLine::TS_VIEW_INV) );


	if (m_bDash)
	{

		//m_pAfterImage->SetUp_Matrices(m_TransformMat);

	}
	//m_pAfterImage->Tick(fTimeDelta);
	if (m_pWeapon)
	{
		if (m_ArrItem[ITEM_CHEST_INFINITE])
		{
			m_pWeapon->Set_Amo(m_pWeapon->Get_MaxAmo());
		}
		m_pWeapon->Get_Transform()->Set_WorldMatrix(m_pModelCom->Get_BoneMatrix("ik_hand_gun") * XMLoadFloat4x4(&m_TransformMat));
		m_pWeapon->Get_Transform()->Set_Scale(_float3(1.f, 1.f, 1.f));
		m_pWeapon->Tick(fTimeDelta);
	}
	m_pHex_Shield->Tick(fTimeDelta);

	if (m_pModelCom->Get_Current_Animation_Tag() == strMegaShot)
	{
		m_pThreeRune->Get_Transform()->Set_WorldMatrix(m_pModelCom->Get_BoneMatrix("ik_hand_gun") * XMLoadFloat4x4(&m_TransformMat));
		m_pThreeRune->Get_Transform()->Set_Scale(_float3(0.3f, 0.3f, 0.3f));
		m_pThreeRune->Tick(fTimeDelta);

	}
	else if (m_pModelCom->Get_Current_Animation_Tag() == strHealOverTime)
	{

	}
	else if (m_pModelCom->Get_Current_Animation_Tag() == strBuff)
	{

	}
	else
	{
		m_pWeapon->Render_On();
	}

	if (m_bBossIntro)
	{
		if (!m_bInitIntro)
		{
			m_pCamTransformCom->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix_XMMat());


			m_bInitIntro = true;
		}
		m_fBossIntroTimer -= fTimeDelta;
		_float fBeat = m_pBeatManager->Get_Beat();
		if (m_fBossIntroTimer >= fBeat * 6.f)
		{
			if (m_fBossIntroTimer >= fBeat * 7.f)
			{
				m_pGameInstance->Blur_On(_float2{ 0.5f,0.5f }, (m_fBossIntroTimer - fBeat * 7.f) * 10.f);
			}
			m_fFov *= 0.5f;
		}
		else if (m_fBossIntroTimer >= fBeat * 4.f)
		{
			if (m_fBossIntroTimer >= fBeat * 5.f)
			{
				m_pGameInstance->Blur_On(_float2{ 0.5f,0.5f }, (m_fBossIntroTimer - fBeat * 5.f) * 10.f);
			}
			m_fFov *= 0.3f;
		}
		else
		{
			if (m_fBossIntroTimer >= fBeat * 3.f)
			{
				m_pGameInstance->Blur_On(_float2{ 0.5f,0.5f }, (m_fBossIntroTimer - fBeat * 3.f) * 10.f);
			}
			m_fFov *= 0.2f;
		}
		if (m_fBossIntroTimer <= 0.f)
		{
			m_bBossIntro = false;
			m_bInitIntro = false;
		}
	
	}
	else
	{
		m_pCamTransformCom->Set_WorldMatrix(/*XMLoadFloat4x4(&m_matCamOffset)*/ m_pTransformCom->Get_WorldMatrix_XMMat());
	}
	if (m_fHealTimer > 0.f)
	{
		m_pParticleHeal->Tick(fTimeDelta);
		m_pParticleHeal->Get_Transform()->Set_WorldMatrix(m_pCamTransformCom->Get_WorldMatrix());
	}
	if (m_fBuffTimer > 0.f)
	{
		m_pParticleBuff->Tick(fTimeDelta);
		m_pParticleBuff->Get_Transform()->Set_WorldMatrix(m_pCamTransformCom->Get_WorldMatrix());
	}
	m_pCamera->Set_Fov(m_fFov);
	auto pSpotLight = m_pGameInstance->Find_SpotLight(m_strSpotLight);
	if(pSpotLight)
	{
		pSpotLight->Set_Direction(XMVector3Normalize(m_pCamera->Get_Transform()->Get_State(CTransform::STATE_LOOK)));
		pSpotLight->Set_Position(m_pCamera->Get_Transform()->Get_Position());
		pSpotLight->Set_Color(XMLoadFloat4(&CEffectObject::s_vAmbientColor));
	}
}

void CPlayer::LateTick(_float fTimeDelta)
{
	if (m_bCleared)
	{
		m_fClearTimer -= fTimeDelta;
		if (m_fClearTimer <= 0.f)
		{
			DELAYED event{};
			event.eType = DELAYED_TYPE::CHANGE_LEVEL_DELAY;
			event.wParam = (DWORD_PTR)CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_ENDING);
			m_pGameInstance->Add_Delayed(event);
		}
	}
	if(m_fRollTime > 0.f)
	{
		Do_A_Barrel_Roll(fTimeDelta);
		m_fRollTime -= fTimeDelta;
		if (m_fRollTime <= 0.f)
		{
			_vector vPos = m_pTransformCom->Get_Position();
			m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&m_WorldBeforeRoll));
			m_pTransformCom->Set_Position(vPos);
		}
	}
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
	if (m_pWeapon)
	{
		m_pWeapon->LateTick(fTimeDelta);
	}
	m_pHex_Shield->LateTick(fTimeDelta);
	if (m_pHex_Shield && m_fParring >= 0.01f)
	{
		m_fParring -= fTimeDelta;
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_DISTORTION, m_pHex_Shield);
	}
	if (m_pModelCom->Get_Current_Animation_Tag() == strMegaShot)
	{
		m_pThreeRune->LateTick(fTimeDelta);
	}
	if (m_fHealTimer > 0.f)
	{
		m_pParticleHeal->LateTick(fTimeDelta);
	}
	if (m_fBuffTimer > 0.f)
	{
		m_pParticleBuff->LateTick(fTimeDelta);
	}
	//m_pAfterImage->LateTick(fTimeDelta);
}

HRESULT CPlayer::Render()
{
	//Normal : 5, ORM : 14
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));

	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_TransformMat);
	m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
	for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
	{
		auto& vecMat = m_pModelCom->Get_Material(i);
		if (vecMat.size())
		{
			auto pTexture = vecMat[0];
			if (pTexture)
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_Texture");
			pTexture = vecMat[5];
			if (pTexture)
			{
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
			}
			else
			{
				DEFAULTNORMAL->Bind_ShaderResource(m_pShaderCom, "g_Normal_Texture");
			}
			pTexture = vecMat[14];
			if (pTexture)
			{
				pTexture->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
			}
			else
			{
				DEFAULTORM->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture");
			}
		}
		
		//_float4x4 World = m_pTransformCom->Get_WorldMatrix();


		//m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");

		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrix", i);
		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}
	return S_OK;
}

void CPlayer::Set_Position(_fmatrix Transform)
{
	m_pCCTCom->Set_Position(Transform);
	m_pTransformCom->Set_WorldMatrix(Transform);
}

void CPlayer::Set_FootPosition(_fmatrix Transform)
{
	m_pCCTCom->Set_FootPosition(Transform);
	m_pTransformCom->Set_WorldMatrix(Transform);
}

_float3 CPlayer::Get_FootPosition()
{
	auto PxvFootPos = m_pCCTCom->Get_Controller()->getFootPosition();
	
	return _float3(PxvFootPos.x, PxvFootPos.y, PxvFootPos.z);
}

void CPlayer::Equip_Weapon(CWeapon* pWeapon)
{
	if (m_bFrameEquipped)
		return;
	if (m_pWeapon)
	{
		m_pWeapon->UnEquip(pWeapon->Get_Transform()->Get_WorldMatrix());
	}
	Safe_Release(m_pWeapon);
	m_pWeapon = pWeapon;
	Safe_AddRef(m_pWeapon);
	if (m_pWeapon)
	{
		string strTag;
		m_pWeapon->Equip(strTag);
		m_pWeapon->Dead();
		m_pModelCom->Set_AnimTag(strTag, false);
	}
	m_bFrameEquipped = true;
}

void CPlayer::Equip_Item(CItem_Equip* pEquip)
{
	if (m_bFrameEquipped)
		return;
	SLOT_ID eSlot =pEquip->Get_Slot();
	if (m_ArrEquip[eSlot])
	{
		m_ArrItem[m_ArrEquip[eSlot]->Get_ID()] = false;
		m_ArrEquip[eSlot]->UnEquip(pEquip->Get_Transform()->Get_WorldMatrix());
	}
	Safe_Release(m_ArrEquip[eSlot]);
	m_ArrEquip[eSlot] = pEquip;
	Safe_AddRef(m_ArrEquip[eSlot]);
	if (m_ArrEquip[eSlot])
	{
		m_ArrEquip[eSlot]->Equip();
		m_ArrItem[m_ArrEquip[eSlot]->Get_ID()] = true;
		m_ArrEquip[eSlot]->Dead();
	}
	m_bFrameEquipped = true;
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Equip_Item");
}

void CPlayer::Heal(_float fAmount)
{
	m_fHp += fAmount;
	if (m_fHp > m_fMaxHp)
	{
		m_fHp = m_fMaxHp;
	}
}

void CPlayer::Increase_Max_Hp(_float fAmount)
{
	m_fMaxHp += fAmount;
}

void CPlayer::Add_Shield(_float fAmount)
{
	m_fShield += fAmount;
}

void CPlayer::Add_Coin(_int iAmount)
{
	m_iCoin += iAmount;
}

void CPlayer::Add_Key(_int iAmount)
{
	m_iKey += iAmount;
}

void CPlayer::Collision_Enter(CGameObject* pOther, const PxControllerHit& ControllerHit)
{

}

void CPlayer::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
{
	if (m_fParring > 0.01f && m_fInvincible < 0.05f)
	{
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Block");
		if (m_pHex_Shield)
			m_pHex_Shield->Block();
		m_fInvincible = 0.5f;
		return;
	}

	if (m_fInvincible < 0.05f && m_fRollTime < 0.05f)
	{
		_int iRand = rand() % 3;
		_wstring strTag = _wstring(L"Player_Hurt") + to_wstring(iRand);
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, strTag);

		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT,  L"Player_TakeDamage");
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT,  L"Player_TakeDamage0");
		_float fShielded = m_fShield - fAmount;
		m_fHp += min(fShielded, 0.f);
		m_fShield = max(fShielded, 0.f);
		m_pCamera->Clear_LensEffect();
		m_pCamera->RedScreen(0.5f, _float4(1.f, 1.f, 1.f, 1.0f));
		if (m_fHp <= 0.f)
		{
			m_fHp = m_fMaxHp;
		}
		if(m_fHp <= 25.f)
			m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT,  L"Player_LowHealth");
		m_fInvincible = 0.5f;
		Cam_Shake(0.1f, 0.2f);
	}
}

void CPlayer::Cam_Shake(_float fDuration, _float fPower)
{
	m_pCamera->Shake(fDuration, fPower);
}

void CPlayer::Cam_Dissolve(_float fDuration, _float fRange)
{
	m_pCamera->Dissolve(fDuration, fRange);
}

void CPlayer::Enter_BossRoom(CGameObject* pTarget)
{
	m_bBossIntro = true;
	m_bInitIntro = false;
	m_fBossIntroTimer = m_pBeatManager->Get_Beat() * 8.f;
	_vector vTargetPos = 
	pTarget->Get_Transform()->Get_Position();
	vTargetPos += XMVectorSet(0.f, 1.5f, 0.f, 0.f);
	m_pTransformCom->LookAt(vTargetPos);
	
}

void CPlayer::Add_PushVector(const _float3& vVector, _float fDuration)
{
	m_vecPushVector.push_back({ vVector, fDuration });
}

void CPlayer::Clear_PushVector()
{
	m_vecPushVector.clear();
}

void CPlayer::WhiteOut()
{
	for(_uint i = 1;i <=40;++i)
	{
		m_pCamera->ColorScreen(m_pBeatManager->Get_Beat() * 0.25f, _float4{ 1.f,1.f,1.f,0.025f *(_float)i});
	}
	m_pCamera->ColorScreen(m_pBeatManager->Get_Beat() * 2.f, _float4{ 1.f,1.f,1.f,1.f});
	m_fInvincible = 60.f;
	m_fClearTimer = m_pBeatManager->Get_Beat() * 15.f;
	m_bCleared = true;
}

void CPlayer::Set_SpotLight_Color(const _float3& vColor)
{
	auto pSpotLight = m_pGameInstance->Find_SpotLight(m_strSpotLight);
	if (pSpotLight)
	{
		pSpotLight->Set_Color(vColor);
	}
}

void CPlayer::Set_SpotLight_Color(_fvector vColor)
{
	auto pSpotLight = m_pGameInstance->Find_SpotLight(m_strSpotLight);
	if (pSpotLight)
	{
		pSpotLight->Set_Color(vColor);
	}
}

void CPlayer::Jump_Able()
{
	m_iJumpCount = 2;
}

void CPlayer::Dash(_float fTimeDelta)
{
	if (m_bBeatMoved && m_iMovedBeat == m_iCurBeatCount && !m_bNoMiss)
	{
		Bad_Beat();
		return;
	}
	Clear_PushVector();
	m_pUIController->Dash();
	m_fDashTimer = m_pBeatManager->Get_Beat() * 0.5f;
	m_bDash = true;
	m_iDashCoolDown = 2;
	if (m_pGameInstance->Get_KeyPressing(DIK_A))
	{
		XMStoreFloat3(&m_vDashVelocity,
			XMLoadFloat3(&m_vDashVelocity) + XMVector3Normalize(XMVectorSet(m_pCCTCom->m_vRight.x, 0.f, m_pCCTCom->m_vRight.z, 0.f)) * -4.f * m_fSpeed);
		_uint iDirection = 4;
		m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_PlayerDash", m_pGameInstance->Get_Current_LevelID(), L"Effect", &iDirection);
		//m_matCamOffset._41 = -0.1f;
		m_vTranslation.x = 0.1f;
	}
	if (m_pGameInstance->Get_KeyPressing(DIK_D))
	{
		XMStoreFloat3(&m_vDashVelocity,
			XMLoadFloat3(&m_vDashVelocity) + XMVector3Normalize(XMVectorSet(m_pCCTCom->m_vRight.x, 0.f, m_pCCTCom->m_vRight.z, 0.f)) * 4.f * m_fSpeed);
		_uint iDirection = 3;
		m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_PlayerDash", m_pGameInstance->Get_Current_LevelID(), L"Effect", &iDirection);
		//m_matCamOffset._41 = 0.1f;
		m_vTranslation.x = -0.1f;
	}
	if (m_pGameInstance->Get_KeyPressing(DIK_W))
	{
		XMStoreFloat3(&m_vDashVelocity,
			XMLoadFloat3(&m_vDashVelocity) + XMVector3Normalize(XMVectorSet(m_pCCTCom->m_vLook.x, 0.f, m_pCCTCom->m_vLook.z, 0.f)) * 4.f * m_fSpeed);
		_uint iDirection = 1;
		m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_PlayerDash", m_pGameInstance->Get_Current_LevelID(), L"Effect", &iDirection);
		//m_matCamOffset._43 = 0.1f;
		m_vTranslation.z = -0.2f;
	}
	if (m_pGameInstance->Get_KeyPressing(DIK_S))
	{
		XMStoreFloat3(&m_vDashVelocity,
			XMLoadFloat3(&m_vDashVelocity) + XMVector3Normalize(XMVectorSet(m_pCCTCom->m_vLook.x, 0.f, m_pCCTCom->m_vLook.z, 0.f)) * -4.f * m_fSpeed);
		_uint iDirection = 2;
		m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_PlayerDash", m_pGameInstance->Get_Current_LevelID(), L"Effect", &iDirection);
		//m_matCamOffset._43 = -0.1f;
	}
	m_pCCTCom->m_bGravity = false;
	m_pUIController->Set_PosScale(1.1f);
	Beat_Move();
	//m_bBeatMoved = true;
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Blink");
}

void CPlayer::Stomp(_float fTimeDelta)
{
	if (m_bBeatMoved && m_iMovedBeat == m_iCurBeatCount && !m_bNoMiss)
	{
		Bad_Beat();
		return;
	}
	m_pUIController->Dash();
	m_fDashTimer = m_pBeatManager->Get_Beat() * 0.5f;
	m_bDash = true;
	m_iDashCoolDown = 2;
	XMStoreFloat3(&m_vDashVelocity,
		XMLoadFloat3(&m_vDashVelocity) + (XMVectorSet(0.f, -4.f * m_fSpeed, 0.f, 0.f)) );
	m_pCCTCom->m_bGravity = false;
	m_pUIController->Set_PosScale(1.1f);
	Beat_Move();
	//m_bBeatMoved = true;
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Blink");
}

void CPlayer::Shoot(_float fTimeDelta)
{
	if (m_pWeapon && m_pWeapon->Get_Weapon_ID() == WEAPON_MINIGUN)
	{
		m_fGatlingTimer += fTimeDelta;
		if (m_pWeapon->Get_Amo() <= 0)
		{
			m_iGatlingStreak = 0;
		}
		if (m_iGatlingStreak < 4)
		{
			if(m_fGatlingTimer >= m_pBeatManager->Get_Beat())
			{
				string strAnimation;
				m_pWeapon->Shoot(strAnimation);
				m_pModelCom->Set_AnimTag(strAnimation, false);
				m_bReloading = false;
				++m_iGatlingStreak;
				m_fGatlingTimer = m_pBeatManager->Get_Timer();
				m_bBeatMoved = true;
				m_iMovedBeat = m_iCurBeatCount;
				m_pUIController->Set_PosScale(1.05f);
			}
		}
		else if (m_iGatlingStreak < 12)
		{
			if (m_fGatlingTimer >= m_pBeatManager->Get_Beat() * 0.5f)
			{
				string strAnimation;
				m_pWeapon->Shoot(strAnimation);
				m_pModelCom->Set_AnimTag(strAnimation, false);
				m_bReloading = false;
				++m_iGatlingStreak;
				m_fGatlingTimer -= m_pBeatManager->Get_Beat() * 0.5f;
				m_bBeatMoved = true;
				m_iMovedBeat = m_iCurBeatCount;
				m_pUIController->Set_PosScale(1.05f);
			}
		}
		else
		{
			if (m_fGatlingTimer >= m_pBeatManager->Get_Beat() * 0.25f)
			{
				string strAnimation;
				m_pWeapon->Shoot(strAnimation);
				m_pModelCom->Set_AnimTag(strAnimation, false);
				m_bReloading = false;
				++m_iGatlingStreak;
				m_fGatlingTimer -= m_pBeatManager->Get_Beat() * 0.25f;
				m_bBeatMoved = true;
				m_iMovedBeat = m_iCurBeatCount;
				m_pUIController->Set_PosScale(1.05f);
			}
		}
		if (m_fBuffTimer > 0.f)
		{
			m_pWeapon->Room_Scan(10.f);
		}

		return;
	}
	if (m_bBeatMoved && m_iMovedBeat == m_iCurBeatCount && !m_bNoMiss)
	{

		Bad_Beat();
		return;
	}
	if (m_pWeapon)
	{
		string strAnimation;
		m_pWeapon->Shoot(strAnimation);
		m_pModelCom->Set_AnimTag(strAnimation, false);
		m_bReloading = false;
	}
	if (m_fBuffTimer > 0.f)
	{
		m_pWeapon->Room_Scan(10.f);
	}
	Beat_Move();
	m_pUIController->Set_PosScale(1.05f);
}

void CPlayer::Reload(_float fTimeDelta)
{
	if (m_bBeatMoved && m_iMovedBeat == m_iCurBeatCount && !m_bNoMiss)
	{
		Bad_Beat();
		return;
	}
	if (m_pWeapon)
	{
		string strAnimation;
		if (!m_bReloading)
		{
			m_pWeapon->Reload_Start(strAnimation);
			m_bReloading = true;
			if (m_pWeapon->Get_Weapon_ID() == WEAPON_MINIGUN)
			{
				m_fAnimFrom = 0.f;
				m_fAnimTo = 0.33f;
			}
		}
		else
		{
			if (m_pWeapon->Reload(strAnimation))
			{
				m_bReloading = false;
				if (m_pWeapon->Get_Weapon_ID() == WEAPON_MINIGUN)
				{
					m_fAnimFrom = 0.66f;
					m_fAnimTo = 1.0f;
				}
			}
			else
			{
				if (m_pWeapon->Get_Weapon_ID() == WEAPON_MINIGUN)
				{
					m_fAnimFrom = 0.33f;
					m_fAnimTo = 0.66f;
				}
			}
		}
		m_pModelCom->Set_AnimTag(strAnimation, false);
	}
	Beat_Move();
}

void CPlayer::Parry(_float fTimeDelta)
{
	if (m_bBeatMoved && m_iMovedBeat == m_iCurBeatCount && !m_bNoMiss)
	{
		Bad_Beat();
		return;
	}
	m_pModelCom->Set_AnimTag("FP_player.ao|FP_player_Invulnerable_01", false);
	m_fParring += m_pBeatManager->Get_Beat() * 4.f;
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Block_Start");
	m_pUIController->Get_AbilitySlot()->Sec();
	Beat_Move();
}

void CPlayer::MegaShoot(_float fTimeDelta)
{
	if (m_bBeatMoved && m_iMovedBeat == m_iCurBeatCount && !m_bNoMiss)
	{
		Bad_Beat();
		return;
	}
	m_iMegaShotCounter = 8;
	m_pModelCom->Set_AnimTag(strMegaShot, false);
	m_pThreeRune->Push_From_Pool(nullptr);
	//CEffect_Emission::EMIT_DESC EDESC;
	//EDESC.fLifeTime = m_pBeatManager->Get_Beat() * 9.f;
	//EDESC.pTransform = m_pThreeRune->Get_Transform()->Get_WorldMatrix_Ptr();
	//EDESC.vColor = { 1.0f,1.0f,1.0f,0.5f };
	//m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Emission_Corona", m_pGameInstance->Get_Current_LevelID(),L"Effect", &EDESC);
	m_pUIController->Get_AbilitySlot()->Ult();
	Beat_Move();
}

void CPlayer::HealOverTime(_float fTimeDelta)
{
	if (m_bBeatMoved && m_iMovedBeat == m_iCurBeatCount && !m_bNoMiss)
	{
		Bad_Beat();
		return;
	}
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_CastMagic");
	m_pModelCom->Set_AnimTag(strHealOverTime, false);
	m_fHealTimer = m_pBeatManager->Get_Beat() * 6.f;
	m_pUIController->Get_AbilitySlot()->Ult();
	CEffect_Sonar::SONAR_DESC sonDesc{};
	sonDesc.fLifeTime = 5.f;
	sonDesc.fSonarInterval = 100.f;
	sonDesc.fSonarSpeed = 50.f;
	sonDesc.fSonarThickness = 3.0f;
	sonDesc.vColor = { 0.1f,0.7f,0.4f, 1.f };
	XMStoreFloat4(&sonDesc.vOrigin, m_pTransformCom->Get_Position());
	m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Sonar", m_pGameInstance->Get_Current_LevelID(), L"DEffect", &sonDesc, nullptr);
	Beat_Move();
}

void CPlayer::Buff(_float fTimeDelta)
{
	if (m_bBeatMoved && m_iMovedBeat == m_iCurBeatCount && !m_bNoMiss)
	{
		Bad_Beat();
		return;
	}
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_CastMagic");
	m_pModelCom->Set_AnimTag(strBuff, false);
	m_fBuffTimer = m_pBeatManager->Get_Beat() * 6.f;
	m_pUIController->Get_AbilitySlot()->Ult();
	CEffect_Sonar::SONAR_DESC sonDesc{};
	sonDesc.fLifeTime = 5.f;
	sonDesc.fSonarInterval = 100.f;
	sonDesc.fSonarSpeed = 50.f;
	sonDesc.fSonarThickness = 3.0f;
	sonDesc.vColor = { 0.7f,0.2f,0.4f, 1.f };
	XMStoreFloat4(&sonDesc.vOrigin, m_pTransformCom->Get_Position());
	m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Sonar", m_pGameInstance->Get_Current_LevelID(), L"DEffect", &sonDesc, nullptr);
	Beat_Move();
}

void CPlayer::Clap(_float fTimeDelta)
{
	if (m_bBeatMoved && m_iMovedBeat == m_iCurBeatCount && !m_bNoMiss)
	{
		Bad_Beat();
		return;
	}
	m_iThunderClapCounter = 12;
	m_pModelCom->Set_AnimTag(strThunderClap, false);

	m_pUIController->Get_AbilitySlot()->Ult();
	Beat_Move();
}

void CPlayer::ThunderClap(_float fTimeDelta)
{
	if(m_iThunderClapCounter > 0 && m_iPrevBeatCount != m_iCurBeatCount)
	{
		CEffect_ThunderClap::LIGHRNIG_DESC LTDesc{};
		LTDesc.fLifeTime = 0.2f;
		LTDesc.vScale = _float3{ 12.f, 48.f , 1.f };
		_matrix MatWorld = XMMatrixIdentity();
		MatWorld.r[3] = XMVectorSetW(XMLoadFloat3(&Get_FootPosition()), 1.f);
		for(_int i = -1;i<2;++i)
		{
			for(_int j = -1; j<2;++j)
			{
				if (i == 0 && j == 0)
					continue;
				_vector vDist = XMVector3Normalize(XMVectorSet((_float)i, 0.f, (_float)j, 0.f)) * 10.f;
				_float4x4 Mat;
				_matrix MatTemp = MatWorld;
				MatTemp.r[3] += vDist;
				XMStoreFloat4x4(&Mat, MatTemp);

				m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Particle_Lightning_Spark_Spread", m_pGameInstance->Get_Current_LevelID(), L"Particle",
					nullptr, &Mat);
				m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_ThunderClap", m_pGameInstance->Get_Current_LevelID(), L"Effect",
					&LTDesc, &Mat);
			}
		}
		//_wstring strTC = L"ThunderGuitar";
		//strTC += to_wstring(4 - m_iThunderClapCounter / 2);
		//m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, strTC);
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"ThunderClap");
		--m_iThunderClapCounter;
	}
}

void CPlayer::SummonAlly(_float fTimeDelta)
{
	if (m_bBeatMoved && m_iMovedBeat == m_iCurBeatCount && !m_bNoMiss)
	{
		Bad_Beat();
		return;
	}

	_vector vPosition = XMVectorSetW(XMLoadFloat3(&Get_FootPosition()), 1.f);

	_vector vDir = XMVector3Normalize(XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_LOOK), 0.f));
	_float4x4 SpikeMat;
	XMStoreFloat4x4(&SpikeMat, XMMatrixTranslationFromVector(vPosition + vDir * 12.f));
	m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Ally", m_pGameInstance->Get_Current_LevelID(), L"Effect",
		nullptr, &SpikeMat);

	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_CastMagic");
	m_pUIController->Get_AbilitySlot()->Ult();
	m_pModelCom->Set_AnimTag(strSummonAlly, false);
	Beat_Move();
}

void CPlayer::FireWall()
{
	if (m_bBeatMoved && m_iMovedBeat == m_iCurBeatCount && !m_bNoMiss)
	{
		Bad_Beat();
		return;
	}
	CEffect_FireWall::PROJECTILE_DESC RRDESC;
	RRDESC.eCG = CG_PLAYER_PROJECTILE;
	RRDESC.fLifeTime = m_pBeatManager->Get_Beat() * 2.f;
	_vector vPosition = XMVectorSetW(XMLoadFloat3(&Get_FootPosition()), 1.f);

	_vector vDir = XMVector3Normalize(XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_LOOK),0.f));
	XMStoreFloat3(&RRDESC.vDirection, vDir * 6.f);
	_float4x4 SpikeMat;
	XMStoreFloat4x4(&SpikeMat, XMMatrixScaling(6, 0.1f, 6.f) * XMMatrixTranslationFromVector(vPosition + vDir * 12.f));
	m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_FireWall", m_pGameInstance->Get_Current_LevelID(), L"Effect", &RRDESC, &SpikeMat);

	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_CastMagic");
	m_pUIController->Get_AbilitySlot()->Sec();
	m_pModelCom->Set_AnimTag(strFireWall, false);
	Beat_Move();
}

void CPlayer::Beat_Move()
{
	if (!m_bNoMiss)
	{
		if (m_pBeatManager->Get_Timer() > 0.8f * m_pBeatManager->Get_Beat())
		{
			m_iMovedBeat = m_iCurBeatCount + 1;
		}
		else
		{
			m_iMovedBeat = m_iCurBeatCount;
		}

		if (m_iMoveLeft <= 1)
		{
			m_bBeatMoved = true;
			m_iMoveLeft = 0;
		}
		else
		{
			m_iMoveLeft -= 1;
		}
	}
	auto pScore = m_pUIController->Get_Score();
	pScore->Add_ScoreMultiplier(0.05f);
	pScore->Add_Score(10.f);
	pScore->Shake();
}

void CPlayer::Bad_Beat()
{
	m_pUIController->Bad_Beat();
	m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_MissedBeat");
}

void CPlayer::Do_A_Barrel_Roll(_float fTimeDelta)
{
	//m_fRollAngle += fTimeDelta * XM_2PI * 4.f;
	m_pTransformCom->Turn(XMLoadFloat3(&m_vRollAxis), fTimeDelta * XM_2PI * 4.f);
}

void CPlayer::Pick()
{
	m_iPickedMonster = -1;
	m_iPickedItem = -1;

	_uint iX = g_iWinSizeX / 2;
	_uint iY = g_iWinSizeY / 2;
	//vector<_uint> vOut;
	//vOut.reserve(1600);
	//m_pGameInstance->Pick_Screen(iX, iY, 40, vOut);
	_uint iID = m_pGameInstance->Pick_Screen(iX, iY);
	_bool bMonster = false;
	_bool bItem = false;
	auto iter = s_HashedMap.find(iID);
	if (iter != s_HashedMap.end())
	{
		_uint iCD = (*iter).second->Get_CollisionGroup();
		if (iCD == CG_MONSTER || iCD == CG_MONSTER_CONTACT)
		{
			bMonster = true;
			m_iPickedMonster = iCD;
		}
		if (iCD == CG_ITEM || iCD == CG_WEAPON)
		{
			bItem = true;
			m_iPickedItem = iCD;
		}
	}

	for (_int i = 1; i < 41; ++i)
	{
		for (_int j = -i; j <= i; ++j)
		{
			iID = m_pGameInstance->Pick_Screen(iX + i, iY + j);
			auto iter = s_HashedMap.find(iID);
			if (iter != s_HashedMap.end())
			{
				_uint iCD = (*iter).second->Get_CollisionGroup();
				if(!bMonster)
				if(iCD == CG_MONSTER || iCD == CG_MONSTER_CONTACT)
				{
					bMonster = true;
					m_iPickedMonster = iCD;
				}
				if(!bItem)
				if (iCD == CG_ITEM || iCD == CG_WEAPON)
				{
					bItem = true;
					m_iPickedItem = iCD;
				}
			}
			iID = m_pGameInstance->Pick_Screen(iX - i, iY + j);
			iter = s_HashedMap.find(iID);
			if (iter != s_HashedMap.end())
			{
				_uint iCD = (*iter).second->Get_CollisionGroup();
				if (!bMonster)
					if (iCD == CG_MONSTER || iCD == CG_MONSTER_CONTACT)
					{
						bMonster = true;
						m_iPickedMonster = iCD;
					}
				if (!bItem)
					if (iCD == CG_ITEM || iCD == CG_WEAPON)
					{
						bItem = true;
						m_iPickedItem = iCD;
					}
			}
			iID = m_pGameInstance->Pick_Screen(iX + j, iY + i);
			iter = s_HashedMap.find(iID);
			if (iter != s_HashedMap.end())
			{
				_uint iCD = (*iter).second->Get_CollisionGroup();
				if (!bMonster)
					if (iCD == CG_MONSTER || iCD == CG_MONSTER_CONTACT)
					{
						bMonster = true;
						m_iPickedMonster = iCD;
					}
				if (!bItem)
					if (iCD == CG_ITEM || iCD == CG_WEAPON)
					{
						bItem = true;
						m_iPickedItem = iCD;
					}
			}
			iID = m_pGameInstance->Pick_Screen(iX + j, iY - i);
			iter = s_HashedMap.find(iID);
			if (iter != s_HashedMap.end())
			{
				_uint iCD = (*iter).second->Get_CollisionGroup();
				if (!bMonster)
					if (iCD == CG_MONSTER || iCD == CG_MONSTER_CONTACT)
					{
						bMonster = true;
						m_iPickedMonster = iCD;
					}
				if (!bItem)
					if (iCD == CG_ITEM || iCD == CG_WEAPON)
					{
						bItem = true;
						m_iPickedItem = iCD;
					}
			}
			if (bMonster && bItem)
				break;
		}
		if (bMonster && bItem)
			break;
	}

}

void CPlayer::PickUp_SkeletonKey(CSkeletonKey* pKey)
{
	for (_uint i = 0; i < 4; ++i)
	{
		if (!m_pSkeletonKey[i])
		{
			m_pSkeletonKey[i] = pKey;
			Safe_AddRef(pKey);
			++m_iSkelKey;
			return;
		}
	}
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CPlayer::Free()
{
	Safe_Release(m_pCamera);
	Safe_Release(m_pCCTCom);
	Safe_Release(m_pUIController);

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pWeapon);
	Safe_Release(m_pHex_Shield);
	Safe_Release(m_pThreeRune);
	Safe_Release(m_pParticleBuff);
	Safe_Release(m_pParticleHeal);
	Safe_Release(m_pCamTransformCom);
	for (auto& pItem : m_ArrEquip)
	{
		Safe_Release(pItem);
	}
	for (auto& pKey : m_pSkeletonKey)
	{
		Safe_Release(pKey);
	}
	//Safe_Release(m_pAfterImage);
	__super::Free();

}

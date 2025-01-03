#include "TPS_Player.h"
#include "CharacterController.h"
#include "GameInstance.h"
#include "Camera_FPS.h"
#include "Camera_TPS.h"
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
#include "Effect_DashAura.h"
#include "Effect_DashIndicator.h"
#include "Effect_ShockImpact.h"
#include "SkeletonKey.h"
#include "Texture.h"
CTPS_Player::CTPS_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
	CPlayer(pDevice, pContext)
{
}

CTPS_Player::CTPS_Player(const CTPS_Player& rhs):
	CPlayer(rhs)
{
}

HRESULT CTPS_Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTPS_Player::Initialize(void* pArg)
{
	m_fMouseSensor = 0.7f;
	CTransform::TRANSFORM_DESC desc;
	desc.fRotationPerSec = 1.f;
	desc.fSpeedPerSec = 1.f;
	if (FAILED(CGameObject::Initialize(&desc)))
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

	m_pTPSCam = static_cast<CCamera_TPS*>(m_pGameInstance->Get_Camera(L"TPS"));
	m_pTPSCam->Set_Target(m_pCamTransformCom);
	m_pTPSCam->Set_Offset(_float3{ 0.f,1.5f,0.f });
	m_pTPSCam->Set_Distance(8.f);
	Safe_AddRef(m_pTPSCam);
	m_pCCTCom->m_bGravity = true;
	m_pCCTCom->m_bUseVelocity_Calculation = true;
	m_pCCTCom->m_fFriction = 5.f;


	Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);
	Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Monster_Gideon", L"Model", (CComponent**)&m_pModelCom);

	Add_Component(LEVEL_STATIC, L"Prototype_Component_Texture_LinearUV", L"FlowTexture", (CComponent**)&m_pLinearUV);

	m_fFov = XM_PI * 0.33f;
	m_fFovOri = XM_PI * 0.33f;


	m_pHex_Shield = (CEffect_Hex_Shield*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_Hex_Shield", m_pCamTransformCom->Get_WorldMatrix_Ptr());
	//m_pThreeRune = (CEffect_ThreeRune*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_ThreeRune", m_pCamTransformCom->Get_WorldMatrix_Ptr());
	m_pThreeRune = (CEffect_ThreeRune*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_ThreeRune", &m_MatHandBone);

	m_pHex_Shield->Get_Transform()->Set_Position(XMVectorSet(0.f, 1.0f, 2.f, 1.f));
	//m_pThreeRune->Get_Transform()->Set_Position(XMVectorSet(0.f, 1.0f, 2.0f, 1.f));
	m_pHex_Shield->Get_Transform()->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XM_PIDIV2);
	m_vTranslation = _float3{ 0.f,0.f,0.f };
	m_pModelCom->Set_AnimTPS(strMegaShot, m_pModelCom->Get_AnimTPS(strMegaShot) * 1.2f);

	auto pAbilitySlot = m_pUIController->Get_AbilitySlot();
	pAbilitySlot->Set_Ability_ID(CUI_AbilitySlot::AS_ULT, m_eUlt);
	pAbilitySlot->Set_Ability_ID(CUI_AbilitySlot::AS_SEC, m_eSec);
	m_pParticleBuff = (CParticle*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_Particle_Buff_Rising", nullptr);
	m_pParticleHeal = (CParticle*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_Particle_Heal_Rising", nullptr);

	m_pAfterImage = (CAfterImage*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_AfterImage_Anim_1sec_10", m_pModelCom);
	m_pAfterImage->Set_Color(_float4{ 1.f, 0.5f, 0.6f,1.f });
	m_pAfterImage->Set_LifeTime(0.5f);
	
	m_matCamOffset = Identity;
	m_matCamOffset._42 = 3.0f;
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

	m_pDashIndicator = (CEffect_DashIndicator*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_DashIndicator", nullptr);

	m_strAnimTag[IDLE] = "Gideon_Undertow.ao|Idle";
	m_fAnimSpeed[IDLE] = 1.5f;

	m_strAnimTag[UPPER_IDLE] = "Gideon_Undertow.ao|TravelMode_Hover_Idle";
	m_fAnimSpeed[UPPER_IDLE] = 1.5f;
	
	m_strAnimTag[START] = "Gideon_Undertow.ao|Emote_Master_Levitate_T3";
	m_fAnimSpeed[START] = 3.0f;
	m_strAnimTag[MOVE] = "Gideon_Undertow.ao|Jog_Fwd";
	m_fAnimSpeed[MOVE] = 1.5f;
	m_strAnimTag[MOVE_BWD] = "Gideon_Undertow.ao|Jog_Bwd";
	m_fAnimSpeed[MOVE_BWD] = 1.5f;
	m_strAnimTag[MOVE_RIGHT] = "Gideon_Undertow.ao|Jog_Right";
	m_fAnimSpeed[MOVE_RIGHT] = 4.5f;
	m_strAnimTag[DASH_RIGHT] = "Gideon_Undertow.ao|Jog_Right";
	m_fAnimSpeed[DASH_RIGHT] = 4.5f;
	m_strAnimTag[MOVE_LEFT] = "Gideon_Undertow.ao|Jog_Left";
	m_fAnimSpeed[MOVE_LEFT] = 4.5f;
	m_strAnimTag[DASH_LEFT] = "Gideon_Undertow.ao|Jog_Left";
	m_fAnimSpeed[DASH_LEFT] = 4.5f;

	m_strAnimTag[MOVE_END] = "Gideon_Undertow.ao|Jump_In_Place_Recover";
	m_fAnimSpeed[MOVE_END] = 1.5f;

	m_strAnimTag[PHASE_2_START] = "Gideon_Undertow.ao|LevelStart"; //0.46666
	m_fAnimSpeed[PHASE_2_START] = 1.5f;

	m_strAnimTag[STRAIGHT] = "Gideon_Undertow.ao|PortalBlast_Start";
	m_fAnimSpeed[STRAIGHT] = 1.5f;
	m_strAnimTag[STRAIGHT_LOOP] = "Gideon_Undertow.ao|PortalBlast_Loop";
	m_fAnimSpeed[STRAIGHT_LOOP] = 1.5f;
	m_strAnimTag[STRAIGHT_THROW] = "Gideon_Undertow.ao|portal_blast_medium";//0.2f
	m_fAnimSpeed[STRAIGHT_THROW] = 1.5f;

	m_strAnimTag[TELEPORT] = "Gideon_Undertow.ao|Torn_Space_v2";//0.33f
	m_fAnimSpeed[TELEPORT] = 1.5f;

	m_strAnimTag[UPPER_START] = "Gideon_Undertow.ao|RMB_Intro";
	m_fAnimSpeed[UPPER_START] = 1.5f;
	m_strAnimTag[UPPER] = "Gideon_Undertow.ao|RMB_Cast";//0.4f
	m_fAnimSpeed[UPPER] = 1.5f;

	m_strAnimTag[ZAP_1] = "Gideon_Undertow.ao|Primary_Attack_A_Medium";  //0.18f
	m_fAnimSpeed[ZAP_1] = 1.5f;
	m_strAnimTag[ZAP_2] = "Gideon_Undertow.ao|Primary_Attack_B_Medium";  //0.18f
	m_fAnimSpeed[ZAP_2] = 1.5f;
	m_strAnimTag[ZAP_3] = "Gideon_Undertow.ao|Primary_Attack_C_Medium";  //0.18f
	m_fAnimSpeed[ZAP_3] = 1.5f;

	m_strAnimTag[KIYATHOU] = "Gideon_Undertow.ao|Burden_Start";
	m_fAnimSpeed[KIYATHOU] = 1.5f;
	m_strAnimTag[KIYATHOU_LOOP] = "Gideon_Undertow.ao|Burden_Loop";
	m_fAnimSpeed[KIYATHOU_LOOP] = 1.5f;
	//m_strAnimTag[KIYATHOU_END] = "Gideon_Undertow.ao|Jump_In_Place_Land";
	m_strAnimTag[KIYATHOU_END] = "Gideon_Undertow.ao|Jump_In_Place_Recover";
	m_fAnimSpeed[KIYATHOU_END] = 1.5f;

	m_strAnimTag[RIFT_START] = "Gideon_Undertow.ao|CosmicRift_Start";
	m_fAnimSpeed[RIFT_START] = 1.5f;
	m_strAnimTag[RIFT] = "Gideon_Undertow.ao|CosmicRift";
	m_fAnimSpeed[RIFT] = 1.5f;

	m_strAnimTag[LAST_DITCH_START] = "Gideon_Undertow.ao|Blackhole_Start";
	m_fAnimSpeed[LAST_DITCH_START] = 1.5f;
	m_strAnimTag[LAST_DITCH] = "Gideon_Undertow.ao|BlackHole_Loop";
	m_fAnimSpeed[LAST_DITCH] = 1.5f;


	m_strAnimTag[DYING] = "Gideon_Undertow.ao|Knock_Fwd";
	m_fAnimSpeed[DYING] = 1.5f;
	m_pModelCom->Add_DepartedBoneTag_Child("thigh_l");
	m_pModelCom->Add_DepartedBoneTag_Child("thigh_r");
	m_pModelCom->Add_DepartedBoneTag_Child("ik_foot_root");
	m_eUpperState = UPPER_IDLE;
	m_pModelCom->Set_AnimTag(m_strAnimTag[UPPER_IDLE], true);
	m_pModelCom->Set_DepartedAnimTag(m_strAnimTag[IDLE], true);
	return S_OK;
}

void CTPS_Player::PriorityTick(_float fTimeDelta)
{
	m_bDisintegration = false;
	if (m_fDisintegrationTarget > 0.f)
	{
		m_bDisintegration = true;
		m_fDisintegrationTarget -= fTimeDelta * 2.f;
		m_fDisintegration += fTimeDelta * 2.f;
	}
	else if (m_fDisintegration > 0.f)
	{
		m_bDisintegration = true;
		m_fDisintegration -= fTimeDelta * 2.f;
		if (m_fDisintegration <= 0.f)
		{
			m_bDisintegration = false;
		}
	}
	m_ePrevState = m_eState;
	m_iPrevBeatCount = m_iCurBeatCount;
	m_iCurBeatCount = m_pBeatManager->Get_BeatCount();
	m_fInvincible -= fTimeDelta;
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
	if (m_pGameInstance->Get_MousePressing(DIMK_RBUTTON))
	{
		m_pDashIndicator->PriorityTick(fTimeDelta);
	}
	m_pAfterImage->PriorityTick(fTimeDelta);
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
	for (auto& pKey : m_pSkeletonKey)
	{
		if (pKey)
		{
			pKey->PriorityTick(fTimeDelta);
		}
	}

}

void CTPS_Player::Tick(_float fTimeDelta)
{
	XMStoreFloat4x4(&m_MatLeftHand, m_pModelCom->Get_BoneMatrix("hand_l") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
	XMStoreFloat4x4(&m_MatRightHand, m_pModelCom->Get_BoneMatrix("hand_r") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
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
			m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_DiscImpact", m_pGameInstance->Get_Current_LevelID(), L"Effect", nullptr, &Impact);

			Cam_Shake(0.2f, 0.2f);
			m_fStompTimer = m_pBeatManager->Get_Beat();


		}
	}
	_bool bMoved = false;
	if (m_bInControl)
	{
		if (!m_bDash)
		{

			if (m_pGameInstance->Get_KeyPressing(DIK_A))
			{
				m_pCCTCom->Move_Land_Left(m_fSpeed);
				m_eState = MOVE_LEFT;
				bMoved = true;
			}
			if (m_pGameInstance->Get_KeyPressing(DIK_D))
			{
				m_pCCTCom->Move_Land_Right(m_fSpeed);
				m_eState = MOVE_RIGHT;
				bMoved = true;
			}

			if (m_pGameInstance->Get_KeyPressing(DIK_W))
			{
				m_pCCTCom->Move_Land_Forward(m_fSpeed);
				m_eState = MOVE;
				bMoved = true;
			}

			if (m_pGameInstance->Get_KeyPressing(DIK_S))
			{
				m_pCCTCom->Move_Land_Backward(m_fSpeed);
				m_eState = MOVE_BWD;
				bMoved = true;
			}
			if (bMoved)
			{
			}
			else
			{
				m_eState = IDLE;
			}

			if (m_pGameInstance->Get_KeyDown(DIK_G))
			{
				_matrix world = XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix());
				world.r[3] += XMVector3Normalize(world.r[2]) * 10.f;
				m_pCCTCom->Set_Position(world);
			}

			m_iJumpCount = 2;
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
				if (m_pGameInstance->Get_MouseDown(DIMK_LBUTTON) && m_eUpperState != ZAP_3)
				{
					_bool bRight = false;
					if (m_eUpperState == UPPER_IDLE)
					{
						m_eUpperState = ZAP_1;
						bRight = true;
					}
					else if(m_eUpperState == ZAP_1)
					{
						m_eUpperState = ZAP_2;
					}
					else if (m_eUpperState == ZAP_2)
					{
						m_eUpperState = ZAP_3;
						bRight = true;
					}

					CEffect_FireBall::PROJECTILE_DESC desc;
					desc.eCG = CG_PLAYER_PROJECTILE;
					_matrix Temp;
					if (bRight)
					{
						Temp = XMLoadFloat4x4(&m_MatRightHand);
					}
					else
					{
						Temp = XMLoadFloat4x4(&m_MatLeftHand);
					}
					_vector vDest = Temp.r[3] + m_pTPSCam->Get_Transform()->Get_State(CTransform::STATE_LOOK) * 3.f;
					XMStoreFloat3(&desc.vDestination, vDest);

					_float4x4 Mat;
	
	
					Temp.r[3] = vDest - m_pTPSCam->Get_Transform()->Get_State(CTransform::STATE_LOOK);
					XMStoreFloat4x4(&Mat, Temp);
					m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_FireBall", m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, &Mat);

					m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_CastFireball");
					//m_pThreeRune->Set_ColorBoost(1.f);
					m_pModelCom->Set_AnimTag(m_strAnimTag[m_eUpperState], false);
				}

			}
			if (m_pGameInstance->Get_MouseDown(DIMK_RBUTTON))
			{
				m_eUpperState = STRAIGHT;
				m_pModelCom->Set_AnimTag(m_strAnimTag[m_eUpperState], false);

				_vector vDashDir = XMVector3Normalize(XMVectorSet(m_pCCTCom->m_vLook.x, 0.f, m_pCCTCom->m_vLook.z, 0.f));
				_float fDashDist = 4.f * m_fSpeed * m_pBeatManager->Get_Beat();
				_float3 vfPos;
				XMStoreFloat3(&vfPos, m_pTransformCom->Get_Position()  + vDashDir * 2.f);
				PxSweepBuffer Hit;
				PxTransform Start{ PxVec3{vfPos.x,vfPos.y,vfPos.z} };

				_float3 vDirection;

				XMStoreFloat3(&vDirection, vDashDir);
				PxVec3 pvDist = PxVec3{ vDirection.x,vDirection.y,vDirection.z };
				pvDist.normalize();
				if (m_pGameInstance->PxSweep(Start, pvDist, fDashDist, 1.f, Hit))
				{
					_float fDistance = Hit.block.distance;
					vDashDir = vDashDir * (fDistance + 2.f);
				}
				else
				{
					vDashDir = vDashDir * fDashDist;
				}

				_matrix DashIndicator = XMMatrixScaling(3.f, 3.f, 3.f) * XMMatrixTranslationFromVector(m_pTransformCom->Get_Position() + vDashDir);
				XMStoreFloat4x4(&m_matDashIndicator, DashIndicator);
				m_pDashIndicator->Push_From_Pool(&m_matDashIndicator);
			}
			if (m_pGameInstance->Get_MousePressing(DIMK_RBUTTON))
			{
				_vector vDashDir = XMVector3Normalize(XMVectorSet(m_pCCTCom->m_vLook.x, 0.f, m_pCCTCom->m_vLook.z, 0.f));
				_float fDashDist = 4.f * m_fSpeed * m_pBeatManager->Get_Beat();
				_float3 vfPos;
				XMStoreFloat3(&vfPos, m_pTransformCom->Get_Position() + XMVectorSet(0.f,1.5f,0.f,0.f) + vDashDir * 2.f);
				PxSweepBuffer Hit;
				PxTransform Start{ PxVec3{vfPos.x,vfPos.y,vfPos.z} };

				_float3 vDirection;

				XMStoreFloat3(&vDirection, vDashDir);
				PxVec3 pvDist = PxVec3{ vDirection.x,vDirection.y,vDirection.z };
				pvDist.normalize();
				if (m_pGameInstance->PxSweep(Start, pvDist, fDashDist, 1.f, Hit))
				{
					_float fDistance = Hit.block.distance;
					vDashDir = vDashDir * (fDistance + 2.f);
				}
				else
				{
					vDashDir = vDashDir * fDashDist;
				}

				_matrix DashIndicator = XMMatrixScaling(3.f, 3.f, 3.f) * XMMatrixTranslationFromVector(m_pTransformCom->Get_Position() + vDashDir);
				XMStoreFloat4x4(&m_matDashIndicator, DashIndicator);
				m_pDashIndicator->Tick(fTimeDelta);
			}
			if (m_pGameInstance->Get_MouseUp(DIMK_RBUTTON)/* && (m_eUpperState == STRAIGHT_LOOP || m_eUpperState == STRAIGHT)*/)
			{
				m_pDashIndicator->Dying();
				m_eUpperState = STRAIGHT_THROW;
				m_pModelCom->Set_AnimTag(m_strAnimTag[m_eUpperState], false);
				{
					Clear_PushVector();
					m_pUIController->Dash();
					m_fDashTimer = m_pBeatManager->Get_Beat();
					m_bDash = true;
					m_iDashCoolDown = 2;

					XMStoreFloat3(&m_vDashVelocity,
						XMLoadFloat3(&m_vDashVelocity) + XMVector3Normalize(XMVectorSet(m_pCCTCom->m_vLook.x, 0.f, m_pCCTCom->m_vLook.z, 0.f)) * 4.f * m_fSpeed);
					_uint iDirection = 1;
					m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_PlayerDash", m_pGameInstance->Get_Current_LevelID(), L"Effect", &iDirection);
					//m_matCamOffset._43 = 0.1f;
					m_vTranslation.z = -0.2f;

					m_pCCTCom->m_bGravity = false;
					m_pUIController->Set_PosScale(1.1f);
					m_bZapDash = true;
					m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Blink");
					CEffect_DashAura::PROJECTILE_DESC dashDesc;
					dashDesc.fLifeTime = m_fDashTimer;
					dashDesc.pParent = m_pTransformCom->Get_WorldMatrix_Ptr();
					XMStoreFloat3(&dashDesc.vDirection, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
					m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_DashAura", m_pGameInstance->Get_Current_LevelID(), L"Effect", &dashDesc, m_pTransformCom->Get_WorldMatrix_Ptr());
				}

			}
			if (m_pGameInstance->Get_KeyDown(DIK_Q))
			{
				m_eUpperState = KIYATHOU;


				//m_pThreeRune->Set_ColorBoost(1.f);
				m_pModelCom->Set_AnimTag(m_strAnimTag[m_eUpperState], false);
				//KIYATHOU
			}
			if (m_pGameInstance->Get_KeyDown(DIK_V))
			{
				m_fDisintegrationTarget = 4.f;
			}

			if (m_pGameInstance->Get_KeyDown(DIK_LSHIFT) && m_iDashCoolDown <= 0)
			{
				Dash(fTimeDelta);
				if (m_pGameInstance->Get_KeyPressing(DIK_A))
				{
					m_eState = MOVE_LEFT;
					bMoved = true;
				}
				if (m_pGameInstance->Get_KeyPressing(DIK_D))
				{
					m_eState = MOVE_RIGHT;
					bMoved = true;
				}

				if (m_pGameInstance->Get_KeyPressing(DIK_W))
				{
					m_eState = MOVE;
					bMoved = true;
				}

				if (m_pGameInstance->Get_KeyPressing(DIK_S))
				{
					m_eState = MOVE_BWD;
					bMoved = true;
				}

				if (!bMoved)
				{
					m_eState = IDLE;
				}
			}
			if (m_pGameInstance->Get_KeyDown(DIK_LCONTROL) && m_iDashCoolDown <= 0 && m_iJumpCount != 2)
			{
				Stomp(fTimeDelta);
			}
			if (m_pGameInstance->Get_KeyDown(DIK_R))
			{
				//Parry(fTimeDelta);
			}
			if (m_pGameInstance->Get_KeyDown(DIK_Q))
			{
				//MegaShoot(fTimeDelta);
			}
			if (m_pGameInstance->Get_KeyDown(DIK_T))
			{
				//Clap(fTimeDelta);
			}
			if (m_pGameInstance->Get_KeyDown(DIK_Z))
			{
				//Buff(fTimeDelta);
			}
			if (m_pGameInstance->Get_KeyDown(DIK_X))
			{
				//HealOverTime(fTimeDelta);

			}
			if (m_pGameInstance->Get_KeyDown(DIK_C))
			{
				//FireWall();
			}
			if (m_pGameInstance->Get_KeyDown(DIK_V))
			{
				//SummonAlly(fTimeDelta);
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
				m_bZapDash = false;
			}
			m_fAfterImageTimer += fTimeDelta;
			if (m_fAfterImageTimer >= 0.05f)
			{
				_float4x4 Matrix = m_pTransformCom->Get_WorldMatrix();
				m_pAfterImage->SetUp_Matrices(Matrix);
				m_fAfterImageTimer -= 0.05f;
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
	m_pAfterImage->Tick(fTimeDelta);
	m_pTPSCam->MouseInput(fTimeDelta);
	_vector vLook = m_pTPSCam->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	if (XMVectorGetX(XMVector3Length(vLook)) > 0.03f)
	{
		m_pTransformCom->LookAt_Horizontal(m_pTransformCom->Get_Position() + vLook);
	}


	m_pCCTCom->Update(fTimeDelta);
	auto vxPos = m_pCCTCom->Get_Controller()->getFootPosition();
	_float4 vPos = _float4( vxPos.x,vxPos.y,vxPos.z,1.f );
	m_pTransformCom->Set_Position(XMLoadFloat4(&vPos));

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

	if (bMoved)
	{
		if (m_eState != m_ePrevState)
		{
			m_pModelCom->Set_DepartedAnimTag(m_strAnimTag[m_eState], false);
		}
	}
	else
	{
		if (m_eState != m_ePrevState)
			m_pModelCom->Set_DepartedAnimTag(m_strAnimTag[m_eState], true);
	}

	m_pModelCom->Play_Animation_Departed(fTimeDelta, m_fAnimFrom, m_fAnimTo, true);

	if (m_pModelCom->Get_Finished())
	{
		Upper_End();
	}
	if (m_pModelCom->Get_Departed_Finished())
	{
		m_eState = IDLE;
		m_pModelCom->Set_DepartedAnimTag(m_strAnimTag[m_eState], true);
	}
	if (m_ArrItem[ITEM_CHEST_TALLSHIELD])
	{
		if (m_fShield < 25.f)
			m_fShield += fTimeDelta * 2.f;
	}

	
	m_pHex_Shield->Tick(fTimeDelta);
	{
		m_pCamTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&m_matCamOffset) * m_pTransformCom->Get_WorldMatrix_XMMat());
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
	m_pTPSCam->Set_Fov(m_fFov);
	auto pSpotLight = m_pGameInstance->Find_SpotLight(m_strSpotLight);
	if (pSpotLight)
	{
		pSpotLight->Set_Direction(XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK)));
		pSpotLight->Set_Position(m_pTransformCom->Get_Position() + XMVectorSet(0.f,1.5f,0.f,0.f));
		pSpotLight->Set_Color(XMLoadFloat4(&CEffectObject::s_vAmbientColor));
	}
	m_fKeyAngle += fTimeDelta * XM_PI;
	_uint iKeyIdx = 0;
	_matrix KeyMat = XMMatrixTranslation(0.f, 2.5f, 3.f);
	for (auto& pKey : m_pSkeletonKey)
	{
		if (pKey)
		{
			pKey->Get_Transform()->Set_WorldMatrix(KeyMat * XMMatrixRotationY(m_fKeyAngle + 0.25f * XM_PI * (_float)iKeyIdx) * m_pTransformCom->Get_WorldMatrix_XMMat());
			pKey->Tick(fTimeDelta);
			++iKeyIdx;
		}
	}

}

void CTPS_Player::LateTick(_float fTimeDelta)
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
	if (m_fRollTime > 0.f)
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
	if(!m_bDisintegration)
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
	else
		m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND_NON_LIT, this);

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
	m_pAfterImage->LateTick(fTimeDelta);
	if (m_pGameInstance->Get_MousePressing(DIMK_RBUTTON))
	{
		m_pDashIndicator->LateTick(fTimeDelta);
	}
	for (auto& pKey : m_pSkeletonKey)
	{
		if (pKey)
		{
			pKey->LateTick(fTimeDelta);
		}
	}
}

HRESULT CTPS_Player::Render()
{

	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
	m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	if (m_bDisintegration)
	{
		m_pLinearUV->Bind_ShaderResource(m_pShaderCom, "g_Flow_Texture");
		m_pShaderCom->Bind_Float("FlowSpeed", 1.f);
		m_pShaderCom->Bind_Float("Time", m_fDisintegration);
		m_pShaderCom->Bind_Float("DisintegrationAmount", m_fDisintegration);
		DEFAULTNOISE->Bind_ShaderResource(m_pShaderCom, "g_Noise_Texture");
	}

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
		if (m_bDisintegration)
		{
			m_pShaderCom->Begin((_uint)SHADER_VTXANIMMESH_PASS::Disintegration);
		}
		else
		{
			m_pShaderCom->Begin(0);
		}

		m_pModelCom->Render(i);
	}
	return S_OK;
}

void CTPS_Player::Cam_Shake(_float fDuration, _float fPower)
{
	m_pTPSCam->Shake(fDuration, fPower);
}

void CTPS_Player::Cam_Dissolve(_float fDuration, _float fRange)
{
	m_pTPSCam->Dissolve(fDuration, fRange);
}

void CTPS_Player::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
{
	if (m_fDisintegration >= 0.01f)
	{
		return;
	}

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

		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_TakeDamage");
		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_TakeDamage0");
		_float fShielded = m_fShield - fAmount;
		m_fHp += min(fShielded, 0.f);
		m_fShield = max(fShielded, 0.f);
		m_pTPSCam->Clear_LensEffect();
		m_pTPSCam->RedScreen(0.5f, _float4(1.f, 1.f, 1.f, 1.0f));
		if (m_fHp <= 0.f)
		{
			m_fHp = m_fMaxHp;
		}
		if (m_fHp <= 25.f)
			m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_LowHealth");
		m_fInvincible = 0.5f;
		Cam_Shake(0.1f, 0.2f);
	}
}

void CTPS_Player::Upper_End()
{
	m_fAnimFrom = 0.f;
	m_fAnimTo = 1.f;
	if (m_eUpperState == STRAIGHT)
	{
		m_eUpperState = STRAIGHT_LOOP;
		m_pModelCom->Set_AnimTag(m_strAnimTag[m_eUpperState], true);
	}
	else if (m_eUpperState == KIYATHOU)
	{
		CEffect_ShockImpact::PROJECTILE_DESC desc;
		_matrix Temp;
		Temp = XMLoadFloat4x4(&m_MatRightHand);

		_vector vDest = m_pTPSCam->Get_Transform()->Get_State(CTransform::STATE_LOOK);
		XMStoreFloat3(&desc.vDirection, vDest * 15.f);

		_float4x4 Mat;


		Temp.r[3] += vDest * 2.f;
		XMStoreFloat4x4(&Mat, Temp);
		desc.fLifeTime = 2.f;
		m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_ShockImpact", m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, &Mat);

		m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_CastFireball");
		m_eUpperState = UPPER_IDLE;
		m_pModelCom->Set_AnimTag(m_strAnimTag[m_eUpperState], true);
	}
	else
	{
		m_eUpperState = UPPER_IDLE;
		m_pModelCom->Set_AnimTag(m_strAnimTag[m_eUpperState], true);
	}
}

CTPS_Player* CTPS_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTPS_Player* pInstance = new CTPS_Player(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTPS_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTPS_Player::Clone(void* pArg)
{
	CTPS_Player* pInstance = new CTPS_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTPS_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTPS_Player::Free()
{
	__super::Free();
	Safe_Release(m_pTPSCam);
	Safe_Release(m_pAfterImage);
	Safe_Release(m_pDashIndicator);
	Safe_Release(m_pLinearUV);
}

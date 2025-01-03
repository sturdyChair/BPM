#include "Monster_Gideon.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
#include "CharacterController.h"
#include "Beat_Manager.h"
#include "Player.h"
#include "Player_Manager.h"
#include "Effect_Charge.h"
#include "Effect_Sweep.h"
#include "Effect_Spike.h"
#include "Level_Loading.h"
#include "Particle.h"
#include "UI_BossHP.h"
#include "Effect_AlvisRock.h"
#include "Effect_RuneField.h"
#include "Effect_SurtLaser_Warning.h"
#include "Effect_SurtRune.h"
#include "Effect_SurtRock.h"
#include "Effect_LightningArc.h"
#include "Effect_VortexFire.h"
#include "Effect_GideonField.h"
#include "Effect_GideonSpear.h"
#include "PointLight.h"
#include "Effect_GideonPortal.h"
#include "Effect_GideonBeam.h"
#include "Effect_FlowerBurst.h"
#include "Effect_GideonRune.h"
#include "AfterImage.h"
#include "RoomManager.h"
#include "Room.h"
#include "Effect_GideonSlowProjectile.h"
//

CMonster_Gideon::CMonster_Gideon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CMonster_Base(pDevice, pContext)
{
}

CMonster_Gideon::CMonster_Gideon(const CMonster_Gideon& rhs) :
    CMonster_Base(rhs)
{
}

HRESULT CMonster_Gideon::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CMonster_Gideon::Initialize(void* pArg)
{
    CTransform::TRANSFORM_DESC transDesc;
    transDesc.fRotationPerSec = XM_PI;
    __super::Initialize(pArg);
    Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Monster_Gideon", L"Model", (CComponent**)&m_pModelCom);
    Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom);

    CCharacterController::CCT_DESC cctDesc;
    if (m_pGameInstance->Get_Current_LevelID() != LEVEL_TOOL)
    {
        cctDesc.pTransform = m_pTransformCom;
    }
    else
    {
        cctDesc.pTransform = nullptr;
    }
    cctDesc.height = 2.5f;
    cctDesc.radius = 1.5f;
    cctDesc.userData = this;
    _float3 vPos;
    XMStoreFloat3(&vPos, m_pTransformCom->Get_Position());
    cctDesc.position = PxExtendedVec3(vPos.x, vPos.y, vPos.z);
    cctDesc.material = m_pGameInstance->Get_Physics()->createMaterial(0.5f, 0.5f, 0.5f);
    Add_Component(LEVEL_STATIC, L"Prototype_Component_CharacterController", L"Com_CCT", (CComponent**)&m_pCCTCom, &cctDesc);
    m_pCCTCom->m_bGravity = true;
    m_pCCTCom->m_bUseVelocity_Calculation = true;
    m_pCCTCom->m_fFriction = 50.f;
    m_pCCTCom->m_fGravity = 80.f;


    m_strAnimTag[IDLE] = "Gideon_Undertow.ao|TravelMode_Hover_Idle";
    m_fAnimSpeed[IDLE] = 1.5f;
    m_strAnimTag[START] = "Gideon_Undertow.ao|Emote_Master_Levitate_T3";
    m_fAnimSpeed[START] = 3.0f;
    m_strAnimTag[MOVE] = "Gideon_Undertow.ao|TravelMode_Hover_Fwd_2";
    m_fAnimSpeed[MOVE] = 1.5f;
    m_strAnimTag[MOVE_BWD] = "Gideon_Undertow.ao|TravelMode_Hover_Bwd_2";
    m_fAnimSpeed[MOVE_BWD] = 1.5f;
    m_strAnimTag[MOVE_RIGHT] = "Gideon_Undertow.ao|TravelMode_Hover_Right_2";
    m_fAnimSpeed[MOVE_RIGHT] = 4.5f;
    m_strAnimTag[DASH_RIGHT] = "Gideon_Undertow.ao|TravelMode_Hover_Right_2";
    m_fAnimSpeed[DASH_RIGHT] = 4.5f;
    m_strAnimTag[MOVE_LEFT] = "Gideon_Undertow.ao|TravelMode_Hover_Left_2";
    m_fAnimSpeed[MOVE_LEFT] = 4.5f;
    m_strAnimTag[DASH_LEFT] = "Gideon_Undertow.ao|TravelMode_Hover_Left_2";
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


    m_eState = START;
    m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    m_fTimer = m_pBeatManager->Get_Beat();
    m_ProjectileDesc.eCG = CG_MONSTER_PROJECTILE;
    m_fHp = 5000.f;
    m_fMaxHp = m_fHp;
    m_pHpBar = (CUI_BossHP*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_BossHP", nullptr);
    m_pHpBar->Set_MaxHp(m_fMaxHp);
    m_pHpBar->Set_Hp(m_fHp);
    m_fRuneTimer = m_pBeatManager->Get_Beat() * 8.f;

    m_pRuneL = (CEffect_GideonRune*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_GideonRune", nullptr);
    m_pRuneL->Push_From_Pool(&m_MatLeftHand);
    m_pRuneR = (CEffect_GideonRune*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_GideonRune", nullptr);
    m_pRuneR->Push_From_Pool(&m_MatRightHand);
    m_pAfterImage = (CAfterImage*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_AfterImage_Anim_1sec_10", m_pModelCom);
    return S_OK;
}

void CMonster_Gideon::PriorityTick(_float fTimeDelta)
{
    m_pHpBar->PriorityTick(fTimeDelta);
    if (m_fInvincible > 0.001f)
    {
        m_fInvincible -= fTimeDelta;
    }
    m_iPrevBitCount = m_iCurrBitCount;
    m_iCurrBitCount = m_pBeatManager->Get_BeatCount();
    m_pRuneL->PriorityTick(fTimeDelta);
    m_pRuneR->PriorityTick(fTimeDelta);
    m_pAfterImage->PriorityTick(fTimeDelta);
}

void CMonster_Gideon::Tick(_float fTimeDelta)
{
    m_pModelCom->Play_Animation(fTimeDelta * m_fAnimSpeed[m_eState], m_fAnimFrom, m_fAnimTo);
    _float fCurRatio = m_pModelCom->Get_Current_Animation_TrackRatio();
    auto pPlayer = m_pPlayerManager->Get_Player();
    XMStoreFloat4(&m_vPrevHandLPos, XMLoadFloat4x4(&m_MatLeft).r[3]);
    XMStoreFloat4(&m_vPrevHandRPos, XMLoadFloat4x4(&m_MatRight).r[3]);

    XMStoreFloat4x4(&m_MatLeft, m_pModelCom->Get_BoneMatrix("FX_Trail_L_02") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
    XMStoreFloat4x4(&m_MatRight, m_pModelCom->Get_BoneMatrix("FX_Trail_R_02") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
    XMStoreFloat4x4(&m_MatLeftHand, m_pModelCom->Get_BoneMatrix("hand_l") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
    XMStoreFloat4x4(&m_MatRightHand, m_pModelCom->Get_BoneMatrix("hand_r") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));

    m_pRuneL->Set_Tex(0);
    m_pRuneR->Set_Tex(0);

    if (pPlayer)
    {
        if (m_pModelCom->Get_Finished())
        {
            Anim_Finished();
            fCurRatio = 0.f;
        }

        switch (m_eState)
        {
        case Client::CMonster_Gideon::START:
        {
            m_bRune = true;
            XMStoreFloat3(&m_vBlackHolePos, m_pTransformCom->Get_Position());
        }
            break;
        case Client::CMonster_Gideon::IDLE:
        {
            m_fTimer -= fTimeDelta;
            if (m_iPrevBitCount != m_iCurrBitCount)
            {
                State_Change();
            }
        }
            break;
        case Client::CMonster_Gideon::MOVE:
        {
            _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
            _vector vPosition = m_pTransformCom->Get_Position();
            _float fDist = XMVectorGetX(XMVector3Length(XMVectorSetY(vPlayerPosition - vPosition, 0.f)));
            if (fDist > 30.f)
            {
                XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(XMVectorSetY(vPlayerPosition - vPosition, 0.f)) * 20.f);
            }
            else if (fDist < 15.f)
            {
                m_eState = MOVE_BWD;
                m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE_BWD], false);
            }
            else
            {
                if (rand() % 2)
                {
                    m_eState = MOVE_RIGHT;
                }
                else
                {
                    m_eState = MOVE_LEFT;
                }
                m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
            }
            if (m_iPrevBitCount != m_iCurrBitCount)
            {
                ++m_iMoveCounter;
                if (m_iMoveCounter >= 4)
                {
                    Anim_Finished();
                }
            }
        }
            break;
        case Client::CMonster_Gideon::MOVE_BWD:
        {
            _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
            _vector vPosition = m_pTransformCom->Get_Position();
            _float fDist = XMVectorGetX(XMVector3Length(XMVectorSetY(vPlayerPosition - vPosition, 0.f)));
            if (fDist < 25.f)
            {
                XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(XMVectorSetY(vPlayerPosition - vPosition, 0.f)) * -20.f);
            }
            else if (fDist > 30.f)
            {
                m_eState = MOVE;
                m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE], false);
            }
            else
            {
                if(rand()%2)
                {
                    m_eState = MOVE_RIGHT;
                }
                else
                {
                    m_eState = MOVE_LEFT;
                }
                m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
            }
            if (m_iPrevBitCount != m_iCurrBitCount)
            {
                ++m_iMoveCounter;
                if (m_iMoveCounter >= 4)
                {
                    Anim_Finished();
                }
            }
        }
            break;
        case Client::CMonster_Gideon::MOVE_RIGHT:
        {
            _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
            _vector vPosition = m_pTransformCom->Get_Position();
            _float fDist = XMVectorGetX(XMVector3Length(XMVectorSetY(vPlayerPosition - vPosition, 0.f)));
            if (fDist < 15.f)
            {
                m_eState = MOVE_BWD;
                m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE_BWD], false);
            }
            else if (fDist > 30.f)
            {
                m_eState = MOVE;
                m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE], false);
            }
            XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMLoadFloat3(&m_pCCTCom->m_vRight) * 30.f);
            if (m_iPrevBitCount != m_iCurrBitCount)
            {
                ++m_iMoveCounter;
                if (m_iMoveCounter >= 4)
                {
                    Anim_Finished();
                }
            }
        }
            break;
        case Client::CMonster_Gideon::MOVE_LEFT:
        {
            _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
            _vector vPosition = m_pTransformCom->Get_Position();
            _float fDist = XMVectorGetX(XMVector3Length(XMVectorSetY(vPlayerPosition - vPosition, 0.f)));
            if (fDist < 15.f)
            {
                m_eState = MOVE_BWD;
                m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE_BWD], false);
            }
            else if (fDist > 30.f)
            {
                m_eState = MOVE;
                m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE], false);
            }
            XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMLoadFloat3(&m_pCCTCom->m_vRight) * -30.f);
            if (m_iPrevBitCount != m_iCurrBitCount)
            {
                ++m_iMoveCounter;
                if (m_iMoveCounter >= 4)
                {
                    Anim_Finished();
                }
            }
        }
            break;
        case Client::CMonster_Gideon::DASH_RIGHT:
        {
            XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMLoadFloat3(&m_pCCTCom->m_vRight) * 80.f);
            if (m_iPrevBitCount != m_iCurrBitCount)
            {
                ++m_iMoveCounter;
                if (m_iMoveCounter >= 5)
                {
                    Anim_Finished();
                }
            }
            m_fAfterImageTimer += fTimeDelta;
            if(m_fAfterImageTimer >= 0.05f)
            {
                _float4x4 Matrix = m_pTransformCom->Get_WorldMatrix();
                m_pAfterImage->SetUp_Matrices(Matrix);
                m_fAfterImageTimer -= 0.05f;
                CEffect_GideonSlowProjectile::PROJECTILE_DESC spdesc;
                spdesc.eCG = CG_MONSTER_PROJECTILE;
                Matrix._42 += 1.5f;
                m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_GideonSlowProjectile", m_pGameInstance->Get_Current_LevelID(), L"Effect", &spdesc,&Matrix);
            }
        }
        break;
        case Client::CMonster_Gideon::DASH_LEFT:
        {
            XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMLoadFloat3(&m_pCCTCom->m_vRight) * -80.f);
            if (m_iPrevBitCount != m_iCurrBitCount)
            {
                ++m_iMoveCounter;
                if (m_iMoveCounter >= 5)
                {
                    Anim_Finished();
                }
            }
            m_fAfterImageTimer += fTimeDelta;
            if (m_fAfterImageTimer >= 0.05f)
            {
                _float4x4 Matrix = m_pTransformCom->Get_WorldMatrix();
                m_pAfterImage->SetUp_Matrices(Matrix);
                m_fAfterImageTimer -= 0.05f;
                CEffect_GideonSlowProjectile::PROJECTILE_DESC spdesc;
                spdesc.eCG = CG_MONSTER_PROJECTILE;
                Matrix._42 += 1.5f;
                m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_GideonSlowProjectile", m_pGameInstance->Get_Current_LevelID(), L"Effect", &spdesc, &Matrix);

            }
        }
        break;
        case Client::CMonster_Gideon::MOVE_END:
        {
            m_pCCTCom->m_vVelocity.x = 0.f;
            m_pCCTCom->m_vVelocity.y = 0.f;
            m_pCCTCom->m_vVelocity.z = 0.f;
            m_pCCTCom->m_bGravity = true;
        }
            break;
        case Client::CMonster_Gideon::PHASE_2_START:
        {
            m_bPhase2 = true;
            if (!m_bPush && fCurRatio >= 0.4666f)
            {
                m_bRune = true;
                m_bPush = true;
                m_pRuneL->Set_Tex(1);
                m_pRuneR->Set_Tex(1);
                CEffect_VortexFire::PROJECTILE_DESC RRDESC;
                RRDESC.eCG = CG_MONSTER_PROJECTILE;
                RRDESC.fLifeTime = m_pBeatManager->Get_Beat() * 2.f;
                RRDESC.iSpikeLeft = 0;
                _vector vPosition = m_pTransformCom->Get_Position();
                for(_int i = -1; i <=1 ;++i)
                {
                    for (_int j = -1; j <= 1; ++j)
                    {
                        _vector vDir = XMVector3Normalize(XMVectorSet(i, 0.f, j ,0.f));
                        XMStoreFloat3(&RRDESC.vDirection, vDir * 4.f);
                        _float4x4 SpikeMat;
                        XMStoreFloat4x4(&SpikeMat, XMMatrixScaling(2.f, 0.1f, 2.f) * XMMatrixTranslationFromVector(vPosition + vDir * 2.f));
                        m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_VortexFire", m_pGameInstance->Get_Current_LevelID(), L"Effect", &RRDESC, &SpikeMat);
                    }
                }

               //MONSTER_DESC MonDesc{};
               //XMStoreFloat4x4(&MonDesc.Transformation, XMMatrixTranslation(m_vBlackHolePos.x, m_vBlackHolePos.y, m_vBlackHolePos.z));
               
               //auto pDraugr = (CMonster_Base*)m_pGameInstance->Push_Object_Hijack(L"Prototype_GameObject_Monster_Draugr", &MonDesc);
               //Safe_AddRef(pDraugr);
               //CRoomManager::Get_Instance()->Get_Room(CRoomManager::Get_Instance()->Get_CurrentRoomIdx())->Get_Monsters().push_back(pDraugr);
              
            }
        }
            break;
        case Client::CMonster_Gideon::STRAIGHT:
        {
            m_iCounter = 4;
        }
            break;
        case Client::CMonster_Gideon::STRAIGHT_LOOP:
        {
            m_pRuneL->Set_Tex(1);
            m_pRuneR->Set_Tex(1);
            if (m_iPrevBitCount != m_iCurrBitCount)
            {
                if (m_iCounter == 4)
                {
                    m_pBeatManager->Set_N_BeatType(2, BM_DODGE);

                }
                --m_iCounter;
                if (m_iCounter == 3)
                {
                    m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Gideon_Warn");
                }
                if (m_iCounter <= 0)
                {
                    m_eState = STRAIGHT_THROW;
                    m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
                }
            }
        }
            break;
        case Client::CMonster_Gideon::STRAIGHT_THROW:
        {
            if (!m_bPush)
            {
                m_bPush = true;
                CEffect_GideonBeam::PROJECTILE_DESC desc;
                desc.eCG = CG_MONSTER_PROJECTILE;
                _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                _vector vPosition = m_pTransformCom->Get_Position();
                _vector vDir = vPlayerPosition - vPosition;
                _vector vNormDir = XMVector3Normalize(vDir);
                XMStoreFloat3(&desc.vDirection, vNormDir * 10.f);
                //
                m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_GideonBeam", m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, &m_MatLeft);
                if(m_bPhase2)
                {
  
                    XMStoreFloat3(&desc.vDirection, XMVector3Rotate(vNormDir,XMQuaternionRotationRollPitchYaw(0.f,XM_PIDIV2 * 0.5f, 0.f))*10.f);
                    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_GideonBeam", m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, &m_MatLeft);
                    XMStoreFloat3(&desc.vDirection, XMVector3Rotate(vNormDir, XMQuaternionRotationRollPitchYaw(0.f, -XM_PIDIV2 * 0.5f, 0.f)) * 10.f);
                    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_GideonBeam", m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, &m_MatLeft);
                }
            }
        }
            break;
        case Client::CMonster_Gideon::TELEPORT:
        {
            if (!m_bPush && fCurRatio >= 0.33f)
            {
                //m_pCCTCom->Set_FootPosition();플레이어 앞까지?
            }
        }
            break;
        case Client::CMonster_Gideon::UPPER_START:
        {
            m_pRuneL->Set_Tex(1);
            m_pRuneR->Set_Tex(1);
        }
            break;
        case Client::CMonster_Gideon::UPPER:
        {
            m_pRuneL->Set_Tex(1);
            m_pRuneR->Set_Tex(1);
            if (!m_bPush && fCurRatio >= 0.4f)
            {
                m_bPush = true;
                //Prototype_GameObject_Effect_VortexFire
                _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                _vector vPosition = m_pTransformCom->Get_Position();
                CEffect_VortexFire::PROJECTILE_DESC RRDESC;
                RRDESC.eCG = CG_MONSTER_PROJECTILE;
                RRDESC.fLifeTime = m_pBeatManager->Get_Beat() * 2.f;
                RRDESC.iSpikeLeft = 20;
                _vector vDir = XMVector3Normalize(XMVectorSetY(vPlayerPosition - vPosition, 0.f));
                XMStoreFloat3(&RRDESC.vDirection, vDir * 4.f);
                _float4x4 SpikeMat;
                if (m_bPhase2)
                {
                    XMStoreFloat4x4(&SpikeMat, XMMatrixScaling(4.f, 0.2f, 4.f)* XMMatrixTranslationFromVector(vPosition + vDir * 2.f));
                }
                else
                    XMStoreFloat4x4(&SpikeMat, XMMatrixScaling(2.f,0.1f,2.f)*XMMatrixTranslationFromVector(vPosition + vDir * 2.f));
                m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_VortexFire", m_pGameInstance->Get_Current_LevelID(), L"Effect", &RRDESC, &SpikeMat);

            }
        }
            break;
        case Client::CMonster_Gideon::ZAP_1:
        {
            m_pRuneL->Set_Tex(1);
            m_pRuneR->Set_Tex(1);
            if (fCurRatio >= 0.18f)
            {
                if(m_bPhase2)
                {
                    m_eState = ZAP_2;
                    m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
                    _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                    _vector vPosition = m_pTransformCom->Get_Position();
                    CEffect_FlowerBurst::PROJECTILE_DESC RRDESC;
                    RRDESC.eCG = CG_MONSTER_PROJECTILE;
                    _vector vDir = XMVector3Normalize(XMVectorSetY(vPlayerPosition - vPosition, 0.f));
                    XMStoreFloat3(&RRDESC.vDirection, vDir * 40.f);

                    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_FlowerBurst", m_pGameInstance->Get_Current_LevelID(), L"Effect", &RRDESC, &m_MatLeftHand);
                }
                else
                {
                    if (!m_bPush)
                    {
                        m_bPush = true;
                        _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                        _vector vPosition = m_pTransformCom->Get_Position();
                        CEffect_FlowerBurst::PROJECTILE_DESC RRDESC;
                        RRDESC.eCG = CG_MONSTER_PROJECTILE;
                        _vector vDir = XMVector3Normalize(XMVectorSetY(vPlayerPosition - vPosition, 0.f));
                        XMStoreFloat3(&RRDESC.vDirection, vDir * 40.f);

                        m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_FlowerBurst", m_pGameInstance->Get_Current_LevelID(), L"Effect", &RRDESC, &m_MatLeftHand);
                    }
                }
            }
        }
            break;
        case Client::CMonster_Gideon::ZAP_2:
        {
            m_pRuneL->Set_Tex(1);
            m_pRuneR->Set_Tex(1);
            if (fCurRatio >= 0.18f)
            {
                m_eState = ZAP_3;
                m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
                _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                _vector vPosition = m_pTransformCom->Get_Position();
                CEffect_FlowerBurst::PROJECTILE_DESC RRDESC;
                RRDESC.eCG = CG_MONSTER_PROJECTILE;
                _vector vDir = XMVector3Normalize(XMVectorSetY(vPlayerPosition - vPosition, 0.f));
                XMStoreFloat3(&RRDESC.vDirection, vDir * 40.f);

                m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_FlowerBurst", m_pGameInstance->Get_Current_LevelID(), L"Effect", &RRDESC, &m_MatRightHand);
            }
        }
            break;
        case Client::CMonster_Gideon::ZAP_3:
        {
            m_pRuneL->Set_Tex(1);
            m_pRuneR->Set_Tex(1);
            if (fCurRatio >= 0.18f)
            {
                if (!m_bPush)
                {
                    m_bPush = true;
                    _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                    _vector vPosition = m_pTransformCom->Get_Position();
                    CEffect_FlowerBurst::PROJECTILE_DESC RRDESC;
                    RRDESC.eCG = CG_MONSTER_PROJECTILE;
                    _vector vDir = XMVector3Normalize(XMVectorSetY(vPlayerPosition - vPosition, 0.f));
                    XMStoreFloat3(&RRDESC.vDirection, vDir * 40.f);

                    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_FlowerBurst", m_pGameInstance->Get_Current_LevelID(), L"Effect", &RRDESC, &m_MatLeftHand);
                }
            }
        }
            break;
        case Client::CMonster_Gideon::KIYATHOU:
        {
            m_pRuneL->Set_Tex(1);
            m_pRuneR->Set_Tex(1);
            if (!m_bPush)
            {
                m_bPush = true;
                _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                CEffect_GideonField::RF_DESC rdesc;
                _float4x4 EffectMat;
                _matrix MatTemp = XMMatrixIdentity();
                rdesc.fLifeTime = (4.f) * m_pBeatManager->Get_Beat();
                rdesc.bPhase2 = m_bPhase2;
                _matrix MatEffect = MatTemp;
                MatEffect.r[3] = vPlayerPosition;
                XMStoreFloat4x4(&EffectMat, MatEffect);
                m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_GideonField", m_pGameInstance->Get_Current_LevelID(), L"Effect", &rdesc, &EffectMat);
            }
        }
            break;
        case Client::CMonster_Gideon::KIYATHOU_LOOP:
        {
            if (m_iPrevBitCount != m_iCurrBitCount)
            {
                --m_iCounter;
                if (m_iCounter <= 0)
                {
                    Anim_Finished();
                }
            }
        }
        break;
        case Client::CMonster_Gideon::KIYATHOU_END:
        {

        }
            break;
        case Client::CMonster_Gideon::RIFT_START:
        {
            m_pRuneL->Set_Tex(1);
            m_pRuneR->Set_Tex(1);
            if (!m_bPush)
            {
                m_bPush = true;
                _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                CEffect_GideonPortal::RF_DESC rdesc;
                _float4x4 EffectMat;
                _matrix MatTemp = XMMatrixIdentity();
                rdesc.fLifeTime = (4.f) * m_pBeatManager->Get_Beat();
                rdesc.bPhase2 = m_bPhase2;
                _matrix MatEffect = MatTemp;
                MatEffect.r[3] = vPlayerPosition + XMVectorSet(0.f,10.f,0.f,0.f);
                XMStoreFloat4x4(&EffectMat, MatEffect);
                m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_GideonPortal", m_pGameInstance->Get_Current_LevelID(), L"Effect", &rdesc, &EffectMat);
            }
            if (m_iPrevBitCount != m_iCurrBitCount)
            {
                if (m_bPhase2)
                {
                    _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                    _vector vPosition = m_pTransformCom->Get_Position();
                    CEffect_FlowerBurst::PROJECTILE_DESC RRDESC;
                    RRDESC.eCG = CG_MONSTER_PROJECTILE;
                    _vector vDir = XMVector3Normalize(XMVectorSetY(vPlayerPosition - vPosition, 0.f));
                    XMStoreFloat3(&RRDESC.vDirection, vDir * 40.f);
                    m_bLeft = !m_bLeft;
                    if(m_bLeft)
                        m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_FlowerBurst", m_pGameInstance->Get_Current_LevelID(), L"Effect", &RRDESC, &m_MatLeftHand);
                    else
                        m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_FlowerBurst", m_pGameInstance->Get_Current_LevelID(), L"Effect", &RRDESC, &m_MatRightHand);
                }
            }
        }
            break;
        case Client::CMonster_Gideon::RIFT:
        {
            if (m_iPrevBitCount != m_iCurrBitCount)
            {
                if (m_bPhase2)
                {
                    _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                    _vector vPosition = m_pTransformCom->Get_Position();
                    CEffect_FlowerBurst::PROJECTILE_DESC RRDESC;
                    RRDESC.eCG = CG_MONSTER_PROJECTILE;
                    _vector vDir = XMVector3Normalize(XMVectorSetY(vPlayerPosition - vPosition, 0.f));
                    XMStoreFloat3(&RRDESC.vDirection, vDir * 40.f);
                    m_bLeft = !m_bLeft;
                    if (m_bLeft)
                        m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_FlowerBurst", m_pGameInstance->Get_Current_LevelID(), L"Effect", &RRDESC, &m_MatLeftHand);
                    else
                        m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_FlowerBurst", m_pGameInstance->Get_Current_LevelID(), L"Effect", &RRDESC, &m_MatRightHand);
                }
            }
        }
            break;
        case Client::CMonster_Gideon::LAST_DITCH_START:
        {
            m_iCounter = 20;
            m_bPush = false;
        }
            break;
        case Client::CMonster_Gideon::LAST_DITCH:
        {
            if (m_iPrevBitCount != m_iCurrBitCount)
            {
                if (!m_bPush)
                {
                    m_bPush = true;
                    _float4x4 Mat;
                    XMStoreFloat4x4(&Mat, XMMatrixTranslation(m_vBlackHolePos.x, m_vBlackHolePos.y + 3.f, m_vBlackHolePos.z + 40.f));
                    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_GideonBlackHole", m_pGameInstance->Get_Current_LevelID(), L"Effect"
                       , nullptr, &Mat);
                }
                --m_iCounter;
                if (m_iCounter <= 0)
                {
                    m_bDitching = false;
                    Take_Damage(1.f, { 0.f,0.f,0.f }, DMG_DEFAULT, this);
                }
            }
        }
            break;
        case Client::CMonster_Gideon::DYING:
        {
            m_bRune = false;
            pPlayer->NoMiss(true);
        }
            break;
        case Client::CMonster_Gideon::STATE_END:
            break;
        default:
            break;
        }

        m_fTakeHit -= fTimeDelta;
        if (m_fTakeHit <= 0.f)
        {
            m_fTakeHit = 0.f;
        }
        if (m_fTakeHit >= 0.01f)
        {
            m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::TakeHit;
        }
        else
        {
            m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::Rim;
        }
        if (m_bKilled)
        {
            m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::Dissolve;
        }
        //if (m_eState != DYING)
        //{
        //    m_fRuneTimer -= fTimeDelta;
        //}
        //if (m_fRuneTimer <= 0.f)
        //{
        //    CEffect_RuneField::RF_DESC rdesc;
        //    _float4x4 EffectMat;
        //    _matrix MatTemp = XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr());
        //    _uint iRand = rand() % 3 + 1;
        //    for (_uint i = 0; i < iRand; ++i)
        //    {
        //        rdesc.fLifeTime = (4.f + (_float)i) * m_pBeatManager->Get_Beat();
        //        _matrix MatEffect = MatTemp;
        //        MatEffect.r[3] += XMVectorSet(fRand(-30.f, 30.f), 0.f, fRand(-30.f, 30.f), 0.f);
        //        XMStoreFloat4x4(&EffectMat, MatEffect);
        //        m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_RuneField", m_pGameInstance->Get_Current_LevelID(), L"Effect", &rdesc, &EffectMat);

        //    }
        //    m_fRuneTimer = m_pBeatManager->Get_Beat() * 8.f;
        //}


        _vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
        _vector vTargetPos = pPlayer->Get_Transform()->Get_Position();
        m_pTransformCom->LookAt_Horizontal(vTargetPos);


        m_pCCTCom->Update(fTimeDelta);
        auto pRawController = m_pCCTCom->Get_Controller();
        if (pRawController)
        {
            auto pxvFP = pRawController->getFootPosition();
            m_pTransformCom->Set_Position(XMVectorSet(pxvFP.x, pxvFP.y, pxvFP.z, 1.f));
        }
        m_pHpBar->Set_Hp(m_fHp);
        m_pHpBar->Tick(fTimeDelta);
        m_pGameInstance->Find_PointLight(m_strLightTag)->Set_Position(m_pTransformCom->Get_Position() + XMVectorSet(0.f,3.f,0.f,0.f) + m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
        m_pGameInstance->Find_PointLight(m_strLightTag2)->Set_Position(m_pTransformCom->Get_Position() + XMVectorSet(0.f,2.f,0.f,0.f) - m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
        m_pRuneL->Tick(fTimeDelta);
        m_pRuneR->Tick(fTimeDelta);
    }
    m_pAfterImage->Tick(fTimeDelta);
}

void CMonster_Gideon::LateTick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
    m_pHpBar->LateTick(fTimeDelta);
    if (m_bRune)
    {
        m_pRuneL->LateTick(fTimeDelta);
        m_pRuneR->LateTick(fTimeDelta);
    }
    m_pAfterImage->LateTick(fTimeDelta);
}

HRESULT CMonster_Gideon::Render()
{
    m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
    _float4 vColor = { 0.f,0.6f,1.f,1.f };
    m_pShaderCom->Bind_Vector4("g_vColor", &vColor);
    m_pShaderCom->Bind_Vector4("g_vCamPos", &m_pGameInstance->Get_CamPosition_Float4());
    m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
    m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
    if (m_iRenderPass == (_uint)SHADER_VTXANIMMESH_PASS::Dissolve)
    {
        m_pShaderCom->Bind_Float("g_Dissolve", m_pModelCom->Get_Current_Animation_TrackRatio());
        DEFAULTNOISE->Bind_ShaderResource(m_pShaderCom, "g_Noise_Texture");
    }
    for (int i = 0; i < m_pModelCom->Get_NumMesh(); ++i)
    {
        if (m_pModelCom->Get_Material(i).size())
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
        }
        m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrix", i);
        m_pShaderCom->Begin(m_iRenderPass);
        m_pModelCom->Render(i);
    }
    return S_OK;
}

void CMonster_Gideon::Dying()
{
    if (m_pCCTCom)
        m_pCCTCom->Disable();
    m_pGameInstance->Remove_PointLight(m_strLightTag);
    m_pGameInstance->Remove_PointLight(m_strLightTag2);
}

void CMonster_Gideon::Push_From_Pool(void* pArg)
{
    if (pArg)
    {
        MONSTER_DESC* pDesc = (MONSTER_DESC*)pArg;
        m_pCCTCom->Set_FootPosition(XMLoadFloat4x4(&pDesc->Transformation));
        m_pTransformCom->Set_WorldMatrix(pDesc->Transformation);
    }
    m_iPrevBitCount = m_iCurrBitCount;
    m_iCurrBitCount = m_pBeatManager->Get_BeatCount();
    PointLight pLight;
    pLight.color = { 0.3f,1.f,0.3f };
    pLight.intensity = 5.f;
    pLight.range = 20.f;
    XMStoreFloat3(&pLight.position, m_pTransformCom->Get_Position());
    pLight.specColor = { 0.3f,1.f,0.3f };
    m_pGameInstance->Add_Light(m_strLightTag, pLight);
    m_pGameInstance->Add_Light(m_strLightTag2, pLight);
}

void CMonster_Gideon::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
{
    if (m_bKilled)
        return;
    if (m_bDitching)
        return;
    if (iDmgType == DMG_EXPLODE)
    {
        if (m_fInvincible >= 0.01f)
        {
            return;
        }
        else
        {
            m_fInvincible = m_pBeatManager->Get_Beat();
        }
    }
    m_fTakeHit = 0.2f;
    if (m_fHp <= 0.f && !m_bKilled)
    {
        if(!m_bPhase2)
        {
            _wstring Chord = L"Surt_Chord";
            m_pGameInstance->Stop_Sound(SOUND_CHANNEL::MUSIC, Chord + to_wstring(m_iFinish));
            ++m_iFinish;
            m_pGameInstance->Play_Sound(SOUND_CHANNEL::MUSIC, Chord + to_wstring(m_iFinish));

            m_pModelCom->Set_AnimTag(m_strAnimTag[DYING], true);
            if (m_iFinish >= 4)
            {
                //m_bKilled = true;
                m_pModelCom->Set_AnimTag(m_strAnimTag[DYING], false);
            }
            return;
        }
        else
        {
            _wstring Chord = L"Nidhog_Chord";
            m_pGameInstance->Stop_Sound(SOUND_CHANNEL::MUSIC, Chord + to_wstring(m_iFinish));
            ++m_iFinish;
            m_pGameInstance->Play_Sound(SOUND_CHANNEL::MUSIC, Chord + to_wstring(m_iFinish));

            m_pModelCom->Set_AnimTag(m_strAnimTag[DYING], true);
            if (m_iFinish >= 8)
            {
                m_bKilled = true;
                m_pModelCom->Set_AnimTag(m_strAnimTag[DYING], false);
                m_pPlayerManager->Get_Player()->WhiteOut();
            }
            return;
        }
    }
    m_fHp -= fAmount;
    if (m_fHp <= 0.f)
    {
        m_pModelCom->Set_AnimTag(m_strAnimTag[DYING], true);
        m_eState = DYING;

        if(!m_bPhase2)
        {
            m_pGameInstance->Play_Sound(SOUND_CHANNEL::MUSIC, L"Surt_Chord0");
            m_pBeatManager->Queue_Sequence(L"Asgard_CoolDown");
        }
        else
        {
            if (!m_bDitch)
            {
                m_bDitch = true;
                m_bDitching = true;
                m_fHp = 1.f;
                m_eState = LAST_DITCH_START;
                m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
         
            }
            else
            {
                m_pGameInstance->Play_Sound(SOUND_CHANNEL::MUSIC, L"Nidhog_Chord0");
                m_pBeatManager->Queue_Sequence(L"Asgard_CoolDown");
            }
        }
    }

}

void CMonster_Gideon::Moving(_float fTimeDelta)
{
}

void CMonster_Gideon::To_Idle()
{
    m_eState = IDLE;
    m_pModelCom->Set_AnimTag(m_strAnimTag[IDLE], false);
    m_fTimer = m_pBeatManager->Get_Beat();
    m_fAnimFrom = 0.f;
    m_fAnimTo = 1.f;
}

void CMonster_Gideon::State_Change()
{
    _bool bP2Dodge = false;
    if (!m_ComboQue.empty())
    {
        if (m_bPhase2)
        {
            m_eNextState = m_ComboQue.front();
            if (rand() % 2)
            {
                m_eState = DASH_RIGHT;
            }
            else
                m_eState = DASH_LEFT;
            m_iMoveCounter = 4;
            bP2Dodge = true;
        }
        else
            m_eState = m_ComboQue.front();
        m_ComboQue.pop();
    }
    else
    {
        _int iRand = rand() % 5;
        if (iRand == 0)
        {
            m_eNextState = STRAIGHT;
        }
        if (iRand == 1)
        {
            m_eNextState = UPPER_START;
        }
        if (iRand == 2)
        {
            m_eNextState = ZAP_1;

        }
        if (iRand == 3)
        {
            m_eNextState = KIYATHOU;
        }
        if (iRand == 4)
        {
            m_eNextState = RIFT_START;
        }
        _int iPrev = iRand;
        while(iPrev == iRand)
        {
            iRand = rand() % 5;
        }
        if (iRand == 0)
        {
            m_ComboQue.push(STRAIGHT);
        }
        if (iRand == 1)
        {
            m_ComboQue.push(UPPER_START);
        }
        if (iRand == 2)
        {
            m_ComboQue.push(ZAP_1);

        }
        if (iRand == 3)
        {
            m_ComboQue.push(KIYATHOU);
        }
        if (iRand == 4)
        {
            m_ComboQue.push(RIFT_START);
        }
        m_eState = MOVE;
    }
    m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);

    m_iPrevBitCount = m_pBeatManager->Get_BeatCount();
    m_iAttackCount = 0;
    if(!bP2Dodge)
        m_iMoveCounter = 0;
    m_bChargeFlag = false;
    m_bPush = false;
    m_bFlag = false;
}

void CMonster_Gideon::Anim_Finished()
{
    string strCurTag = m_pModelCom->Get_Current_Animation_Tag();

    switch (m_eState)
    {
    case Client::CMonster_Gideon::START:
    {
        To_Idle();
    }
        break;
    case Client::CMonster_Gideon::IDLE:
    {
        State_Change();
    }
        break;
    case Client::CMonster_Gideon::MOVE:
    {
        if (!m_bPhase2)
        {
            m_eState = MOVE_END;
        }
        else
        {
            if (rand() % 2)
            {
                m_eState = DASH_RIGHT;
                m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Blink");
            }
            else
            {
                m_eState = DASH_LEFT;
                m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Blink");
            }
        }
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
        break;
    case Client::CMonster_Gideon::MOVE_BWD:
    {
        if (!m_bPhase2)
        {
            m_eState = MOVE_END;
        }
        else
        {
            if (rand() % 2)
            {
                m_eState = DASH_RIGHT;
                m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Blink");
            }
            else
            {
                m_eState = DASH_LEFT;
                m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Blink");
            }
        }
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
        break;
    case Client::CMonster_Gideon::MOVE_RIGHT:
    {
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.y = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;
        if (!m_bPhase2)
        {
            m_eState = MOVE_END;
        }
        else
        {
            if (rand() % 2)
            {
                m_eState = DASH_RIGHT;

            }
            else
            {
                m_eState = DASH_LEFT;
            }
            m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Blink");
        }
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
        break;
    case Client::CMonster_Gideon::DASH_RIGHT:
    {
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.y = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;

        if (0 == (rand() % 3))
        {
            m_iMoveCounter = 3;
            m_eState = DASH_LEFT;
            m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Blink");
        }

        m_eState = MOVE_END;

        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
        break;
    case Client::CMonster_Gideon::MOVE_LEFT:
    {
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.y = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;
        if (!m_bPhase2)
        {
            m_eState = MOVE_END;
        }
        else
        {
            if (rand() % 2)
            {
                m_eState = DASH_RIGHT;
            }
            else
            {
                m_eState = DASH_LEFT;
            }
            m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Blink");
        }
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
        break;
    case Client::CMonster_Gideon::DASH_LEFT:
    {
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.y = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;
        if (0 == (rand() % 3))
        {
            m_iMoveCounter = 3;
            m_eState = DASH_RIGHT;
            m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Player_Blink");
        }
        m_eState = MOVE_END;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
        break;
    case Client::CMonster_Gideon::MOVE_END:
    {
        m_eState = m_eNextState;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
        break;
    case Client::CMonster_Gideon::PHASE_2_START:
    {
        State_Change();
    }
        break;
    case Client::CMonster_Gideon::STRAIGHT:
    {
        m_iCounter = 4;
        m_eState = STRAIGHT_LOOP;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], true);
    }
        break;
    case Client::CMonster_Gideon::STRAIGHT_LOOP:
    {
        m_eState = STRAIGHT_THROW;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
        break;
    case Client::CMonster_Gideon::STRAIGHT_THROW:
    {
        To_Idle();
    }
        break;
    case Client::CMonster_Gideon::TELEPORT:
    {
        To_Idle();
    }
        break;
    case Client::CMonster_Gideon::UPPER_START:
    {
        m_eState = UPPER;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
        break;
    case Client::CMonster_Gideon::UPPER:
    {
        To_Idle();
    }
        break;
    case Client::CMonster_Gideon::ZAP_1:
    {
        To_Idle();
    }
        break;
    case Client::CMonster_Gideon::ZAP_2:
    {
        To_Idle();
    }
        break;
    case Client::CMonster_Gideon::ZAP_3:
    {
        To_Idle();
    }
        break;
    case Client::CMonster_Gideon::KIYATHOU:
    {
        m_eState = KIYATHOU_LOOP;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], true);
        m_iCounter = 3;
    }
        break;
    case Client::CMonster_Gideon::KIYATHOU_LOOP:
    {
        m_eState = KIYATHOU_END;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
    break;
    case Client::CMonster_Gideon::KIYATHOU_END:
    {
        To_Idle();
    }
        break;
    case Client::CMonster_Gideon::RIFT_START:
    {
        m_eState = RIFT;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
        break;
    case Client::CMonster_Gideon::RIFT:
    {
        To_Idle();
    }
        break;
    case Client::CMonster_Gideon::LAST_DITCH_START:
    {
        m_eState = LAST_DITCH;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], true);
    }
        break;
    case Client::CMonster_Gideon::LAST_DITCH:
    {
        m_bDitching = false;
        Take_Damage(1.f, _float3{ 0.f,0.f,0.f }, DMG_DEFAULT, this);
    }
        break;
    case Client::CMonster_Gideon::DYING:
    {
        if (m_bPhase2)
        {
            Dead();
        }
        else
        {
            m_bPhase2 = true;
            m_bPush = false;
            m_eState = PHASE_2_START;
            m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
            m_fHp = m_fMaxHp;
            m_pBeatManager->Queue_Sequence(L"Gideon_2");
            m_bKilled = false;
            m_iFinish = 0;
            auto pPM = CPlayer_Manager::Get_Instance();
            auto pPlayer = pPM->Get_Player();
            pPlayer->Enter_BossRoom(this);
        }
    }
        break;
    case Client::CMonster_Gideon::STATE_END:
        break;
    default:
        break;
    }
}

CMonster_Gideon* CMonster_Gideon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster_Gideon* pInstance = new CMonster_Gideon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CMonster_Gideon"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMonster_Gideon::Clone(void* pArg)
{
    CMonster_Gideon* pInstance = new CMonster_Gideon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CMonster_Gideon"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster_Gideon::Free()
{
    Safe_Release(m_pCCTCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pHpBar);
    Safe_Release(m_pRuneR);
    Safe_Release(m_pRuneL);
    Safe_Release(m_pAfterImage);
    __super::Free();
}

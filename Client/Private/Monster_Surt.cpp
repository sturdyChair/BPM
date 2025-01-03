#include "Monster_Surt.h"
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
#include "Trail.h"
#include "AfterImage.h"

//

CMonster_Surt::CMonster_Surt(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CMonster_Base(pDevice, pContext)
{
}

CMonster_Surt::CMonster_Surt(const CMonster_Surt& rhs) :
    CMonster_Base(rhs)
{
}

HRESULT CMonster_Surt::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CMonster_Surt::Initialize(void* pArg)
{
    CTransform::TRANSFORM_DESC transDesc;
    transDesc.fRotationPerSec = XM_PI;
    __super::Initialize(pArg);
    Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Monster_Surt", L"Model", (CComponent**)&m_pModelCom);
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
    cctDesc.height = 2.f;
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


    m_strAnimTag[IDLE] = "GruxMolten.ao|Idle";
    m_fAnimSpeed[IDLE] = 1.5f;
    m_strAnimTag[START] = "GruxMolten.ao|Emote_Taunt_FootSlide_T1";
    m_fAnimSpeed[START] = 2.0f;
    m_strAnimTag[MOVE_START] = "GruxMolten.ao|Run_Fwd_Start";
    m_fAnimSpeed[MOVE_START] = 1.5f;
    m_strAnimTag[MOVE] = "GruxMolten.ao|Run_Fwd";
    m_fAnimSpeed[MOVE] = 1.5f;

    m_strAnimTag[GROUND_SMASH_START] = "GruxMolten.ao|Jump_Start";
    m_fAnimSpeed[GROUND_SMASH_START] = 1.5f;
    m_strAnimTag[GROUND_SMASH] = "GruxMolten.ao|LevelStart";
    m_fAnimSpeed[GROUND_SMASH] = 1.5f;
    
    m_strAnimTag[SWING_LEFT] = "GruxMolten.ao|PrimaryAttack_LA_Fast";
    m_fAnimSpeed[SWING_LEFT] = 8.0f;

    m_strAnimTag[SWING_RIGHT] = "GruxMolten.ao|PrimaryAttack_RA_Fast";
    m_fAnimSpeed[SWING_RIGHT] = 8.0f;

    m_strAnimTag[CHARGE] = "GruxMolten.ao|Stampede";
    m_fAnimSpeed[CHARGE] = 1.5f;
    m_strAnimTag[SWING_UPPER] = "GruxMolten.ao|Stampede_Knockup";
    m_fAnimSpeed[SWING_UPPER] = 1.5f;

    m_strAnimTag[CLAP_ROAR] = "GruxMolten.ao|SelectScreen_Emote";
    m_fAnimSpeed[CLAP_ROAR] = 1.5f;

    m_strAnimTag[ROAR] = "GruxMolten.ao|Ultimate_Roar";
    m_fAnimSpeed[ROAR] = 1.0f;

    m_strAnimTag[DYING] = "GruxMolten.ao|Death_A";
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

    CTrail::TRAIL_DESC desc{};
    desc.bTextureAnim = false;
    desc.iMaxRect = 12;
    desc.iRenderPass = (_uint)SHADER_VTXPOS_PASS::Trail;
    desc.pDownTransform = &m_MatLeftHand;
    desc.pUpperTransform = &m_MatLeft;
    desc.strTextureTag = L"Prototype_Component_Texture_FireTrail";

    m_pLeftTrail = CTrail::Create(m_pDevice, m_pContext, desc);
    desc.pDownTransform = &m_MatRightHand;
    desc.pUpperTransform = &m_MatRight;
    m_pRightTrail = CTrail::Create(m_pDevice, m_pContext, desc);

    m_pAfterImage = (CAfterImage*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_AfterImage_Anim_1sec_10", m_pModelCom);
    m_pAfterImage->Set_Color(_float4{ 1.f,0.2f,0.2f,1.f });
    return S_OK;
}

void CMonster_Surt::PriorityTick(_float fTimeDelta)
{
    m_pHpBar->PriorityTick(fTimeDelta);
    if (m_fInvincible > 0.001f)
    {
        m_fInvincible -= fTimeDelta;
    }
    m_iPrevBitCount = m_iCurrBitCount;
    m_iCurrBitCount = m_pBeatManager->Get_BeatCount();
    if (m_fLeftTrailTimer >= 0.f)
    {
        m_fLeftTrailTimer -= fTimeDelta;
        m_pLeftTrail->PriorityTick(fTimeDelta);
    }
    if (m_fRightTrailTimer >= 0.f)
    {
        m_fRightTrailTimer -= fTimeDelta;
        m_pRightTrail->PriorityTick(fTimeDelta);
    }
    m_pAfterImage->PriorityTick(fTimeDelta);
}

void CMonster_Surt::Tick(_float fTimeDelta)
{
    m_pModelCom->Play_Animation(fTimeDelta * m_fAnimSpeed[m_eState], m_fAnimFrom, m_fAnimTo);
    _float fCurRatio = m_pModelCom->Get_Current_Animation_TrackRatio();
    auto pPlayer = m_pPlayerManager->Get_Player();
    XMStoreFloat4(&m_vPrevHandLPos, XMLoadFloat4x4(&m_MatLeft).r[3]);
    XMStoreFloat4(&m_vPrevHandRPos, XMLoadFloat4x4(&m_MatRight).r[3]);

    XMStoreFloat4x4(&m_MatLeft, m_pModelCom->Get_BoneMatrix("FX_Trail_L_02") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
    XMStoreFloat4x4(&m_MatLeftHand, m_pModelCom->Get_BoneMatrix("hand_l") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
    XMStoreFloat4x4(&m_MatRight, m_pModelCom->Get_BoneMatrix("FX_Trail_R_02") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
    XMStoreFloat4x4(&m_MatRightHand, m_pModelCom->Get_BoneMatrix("hand_r") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));

    if (pPlayer)
    {
        if (m_pModelCom->Get_Finished())
        {
            Anim_Finished();
            fCurRatio = 0.f;
        }

        switch (m_eState)
        {
        case Client::CMonster_Surt::START:
            break;
        case Client::CMonster_Surt::IDLE:
        {
            m_fTimer -= fTimeDelta;
            if (m_iPrevBitCount != m_iCurrBitCount)
            {
                State_Change();
            }
        }
            break;
        case Client::CMonster_Surt::MOVE_START:
        {
            auto pPlayer = m_pPlayerManager->Get_Player();
            _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
            _vector vPosition = m_pTransformCom->Get_Position();
            if (XMVectorGetX(XMVector3Length(XMVectorSetY(vPlayerPosition - vPosition, 0.f))) > 10.f)
            {
                XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(XMVectorSetY(vPlayerPosition - vPosition, 0.f)) * 25.f);
            }
            else
            {
                Anim_Finished();
            }
        }
            break;
        case Client::CMonster_Surt::MOVE:
        {
            auto pPlayer = m_pPlayerManager->Get_Player();
            _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
            _vector vPosition = m_pTransformCom->Get_Position();
            if (XMVectorGetX(XMVector3Length(XMVectorSetY(vPlayerPosition - vPosition, 0.f))) > 10.f)
            {
                XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(XMVectorSetY(vPlayerPosition - vPosition, 0.f)) * 25.f);
            }
            else
            {
                Anim_Finished();
            }
        }
            break;
        case Client::CMonster_Surt::GROUND_SMASH_START:
            break;
        case Client::CMonster_Surt::GROUND_SMASH:
        {   
            if (!m_bPush)
            {
                m_bPush = true;
                auto pPlayer = m_pPlayerManager->Get_Player();
                _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                _vector vPosition = m_pTransformCom->Get_Position();
                XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(vPlayerPosition - vPosition) * 20.f);
                m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Alvis_Impact0");
                CEffect_SurtRock::PROJECTILE_DESC RRDESC;
                RRDESC.eCG = CG_MONSTER_PROJECTILE;
                RRDESC.fLifeTime = m_pBeatManager->Get_Beat() * 2.f;
                RRDESC.iSpikeLeft = 20;
                _vector vDir = XMVector3Normalize(XMVectorSetY(vPlayerPosition - vPosition, 0.f));
                XMStoreFloat3(&RRDESC.vDirection, vDir * 4.f);
                _float4x4 SpikeMat;
                XMStoreFloat4x4(&SpikeMat, XMMatrixTranslationFromVector(vPosition + vDir * 2.f));
                m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_SurtRock", m_pGameInstance->Get_Current_LevelID(), L"Effect", &RRDESC, &SpikeMat);
                if (m_fHp <= m_fMaxHp * 0.5f)
                {
                    m_iSmashCount = 3;
                    m_fLeftTrailTimer = 1.0f;
                    m_fRightTrailTimer = 1.0f;
                    m_pLeftTrail->Push_From_Pool(nullptr);
                    m_pRightTrail->Push_From_Pool(nullptr);
                }
                else
                {
                    m_iSmashCount = 0;
                    m_fLeftTrailTimer = 0.5f;
                    m_fRightTrailTimer = 0.5f;
                    m_pLeftTrail->Push_From_Pool(nullptr);
                    m_pRightTrail->Push_From_Pool(nullptr);
                }
            }
            if (m_iSmashCount > 0 && fCurRatio >= 0.07f)
            {
                --m_iSmashCount;
                m_fAnimFrom = 0.01f;
                m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
                m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Alvis_Impact0");
                CEffect_SurtRock::PROJECTILE_DESC RRDESC;
                RRDESC.eCG = CG_MONSTER_PROJECTILE;
                RRDESC.fLifeTime = m_pBeatManager->Get_Beat() * 2.f;
                RRDESC.iSpikeLeft = 20;
                auto pPlayer = m_pPlayerManager->Get_Player();
                _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                _vector vPosition = m_pTransformCom->Get_Position();
                _vector vDir = XMVector3Normalize(XMVectorSetY(vPlayerPosition - vPosition, 0.f));
                XMStoreFloat3(&RRDESC.vDirection, vDir * 4.f);
                _float4x4 SpikeMat;
                XMStoreFloat4x4(&SpikeMat, XMMatrixTranslationFromVector(vPosition + vDir * 2.f));
                m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_SurtRock", m_pGameInstance->Get_Current_LevelID(), L"Effect", &RRDESC, &SpikeMat);
            }
            if(fCurRatio <= 0.2f)
            m_fAfterImageTimer += fTimeDelta;
            if (m_fAfterImageTimer >= 0.05f)
            {
                m_pAfterImage->SetUp_Matrices(m_pTransformCom->Get_WorldMatrix());
                m_fAfterImageTimer -= 0.05f;
            }
        }
            break;
        case Client::CMonster_Surt::SWING_LEFT:
        {
            if (!m_bPush)
            {
                m_bPush = true;
                m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Surt_Slice");
                //0.6f;


                _uint iRand = rand() % 4;
                if (iRand == 0)
                {
                    m_ComboQueue.push(SWING_RIGHT);
                    m_fLeftTrailTimer = 1.0f;
                    m_fRightTrailTimer = 1.0f;
                }
                if (iRand == 1)
                {
                    m_ComboQueue.push(SWING_RIGHT);
                    m_ComboQueue.push(SWING_UPPER);
                    m_fLeftTrailTimer = 1.5f;
                    m_fRightTrailTimer = 1.5f;
                }
                if (iRand == 2)
                {
                    m_ComboQueue.push(SWING_RIGHT);
                    m_ComboQueue.push(SWING_LEFT);
                    m_ComboQueue.push(SWING_RIGHT);
                    m_fLeftTrailTimer = 2.5f;
                    m_fRightTrailTimer = 2.5f;
                }
                if (iRand == 3)
                {
                    m_ComboQueue.push(SWING_RIGHT);
                    m_ComboQueue.push(GROUND_SMASH_START);
                    m_fLeftTrailTimer = 1.0f;
                    m_fRightTrailTimer = 1.0f;
                }
                m_pLeftTrail->Push_From_Pool(nullptr);
                m_pRightTrail->Push_From_Pool(nullptr);
            }
            if (fCurRatio > 0.6f)
            {
                if (!m_ComboQueue.empty())
                {
                    m_eState = m_ComboQueue.front();
                    m_ComboQueue.pop();
                    m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
                    if (m_eState == GROUND_SMASH_START)
                    {
                        m_bPush = false;
                    }
                    else
                    {
                        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Surt_Slice");
                        auto pPlayer = m_pPlayerManager->Get_Player();
                        _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                        _vector vPosition = m_pTransformCom->Get_Position();
                        _vector vDir = XMVectorSetY(vPlayerPosition - vPosition, 0.f);
                        if (XMVectorGetX(XMVector3Length(vDir)) >= 8.f)
                            XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(vDir) * 70.f);
                    }
                }
                else
                {
                    To_Idle();
                }
            }
            else if (fCurRatio >= 0.2f)
            {
                m_pCCTCom->m_vVelocity.x = 0.f;
                m_pCCTCom->m_vVelocity.z = 0.f;
            }

            _vector vTargetPos = pPlayer->Get_Transform()->Get_Position();
            _matrix HandMat = XMLoadFloat4x4(&m_MatRight);
            if (XMVectorGetX(XMVector4Length(vTargetPos - HandMat.r[3])) < 4.f)
            {
                _float3 vHitPoint;
                XMStoreFloat3(&vHitPoint, m_pTransformCom->Get_Position());
                pPlayer->Take_Damage(25.f, vHitPoint, DAMAGE_TYPE::DMG_DEFAULT, this);
                _vector vPrevHandPos = XMLoadFloat4(&m_vPrevHandRPos);
                pPlayer->Clear_PushVector();
                _float3 vPush;
                XMStoreFloat3(&vPush, XMVector3Normalize(HandMat.r[3] - vPrevHandPos) * 30.f);

                pPlayer->Add_PushVector(vPush, 1.0f);
            }
        }
            break;
        case Client::CMonster_Surt::SWING_RIGHT:
        {
            _vector vTargetPos = pPlayer->Get_Transform()->Get_Position();
            _matrix HandMat = XMLoadFloat4x4(&m_MatRight);
            if (XMVectorGetX(XMVector4Length(vTargetPos - HandMat.r[3])) < 4.f)
            {
                _float3 vHitPoint;
                XMStoreFloat3(&vHitPoint, m_pTransformCom->Get_Position());
                pPlayer->Take_Damage(25.f, vHitPoint, DAMAGE_TYPE::DMG_DEFAULT, this);
                _vector vPrevHandPos = XMLoadFloat4(&m_vPrevHandRPos);
                pPlayer->Clear_PushVector();
                _float3 vPush;
                XMStoreFloat3(&vPush, XMVector3Normalize(HandMat.r[3] - vPrevHandPos) * 30.f);

                pPlayer->Add_PushVector(vPush, 1.0f);
            }
            if (fCurRatio > 0.6f)
            {
                if (!m_ComboQueue.empty())
                {
                    m_eState = m_ComboQueue.front();
                    m_ComboQueue.pop();
                    m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
                    if (m_eState == GROUND_SMASH_START)
                    {
                        m_bPush = false;
                    }
                    else
                    {
                        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Surt_Slice");
                        auto pPlayer = m_pPlayerManager->Get_Player();
                        _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                        _vector vPosition = m_pTransformCom->Get_Position();
                        _vector vDir = XMVectorSetY(vPlayerPosition - vPosition, 0.f);
                        if (XMVectorGetX(XMVector3Length(vDir)) >= 8.f)
                            XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(vDir) * 70.f);
                    }
                }
                else
                {
                    To_Idle();
                }
            }
            else if (fCurRatio >= 0.2f)
            {
                m_pCCTCom->m_vVelocity.x = 0.f;
                m_pCCTCom->m_vVelocity.z = 0.f;
            }

        }
            break;
        case Client::CMonster_Surt::CHARGE:
        {
            auto pPlayer = m_pPlayerManager->Get_Player();
            _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
            _vector vPosition = m_pTransformCom->Get_Position();
            if (XMVectorGetX(XMVector3Length(XMVectorSetY(vPlayerPosition - vPosition, 0.f))) > 5.f)
            {
                XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(XMVectorSetY(vPlayerPosition - vPosition, 0.f)) * 35.f);
            }
            else
            {
                Anim_Finished();
            }
        }
            break;
        case Client::CMonster_Surt::SWING_UPPER:
        {
            _vector vTargetPos = pPlayer->Get_Transform()->Get_Position();
            _matrix HandMat = XMLoadFloat4x4(&m_MatRight);
            if (XMVectorGetX(XMVector4Length(vTargetPos - HandMat.r[3])) < 4.f)
            {
                _float3 vHitPoint;
                XMStoreFloat3(&vHitPoint, m_pTransformCom->Get_Position());
                pPlayer->Take_Damage(25.f, vHitPoint, DAMAGE_TYPE::DMG_DEFAULT, this);
                _vector vPrevHandPos = XMLoadFloat4(&m_vPrevHandRPos);
                pPlayer->Clear_PushVector();
                _float3 vPush;
                XMStoreFloat3(&vPush, XMVector3Normalize(HandMat.r[3] - vPrevHandPos) * 30.f);

                pPlayer->Add_PushVector(vPush, 1.0f);
            }
            if(!m_bFlag)
            {
                m_bFlag = true;
                CEffect_AlvisRock::PROJECTILE_DESC desc;
                desc.eCG = CG_MONSTER_PROJECTILE;
                _matrix ImpactWorld = XMMatrixIdentity();
                ImpactWorld.r[3] = m_pTransformCom->Get_Position();
                _float4x4 Impact;
                XMStoreFloat4x4(&Impact, ImpactWorld);
                for (_uint i = 0; i < 5; ++i)
                {
                    _wstring strTag = L"Prototype_GameObject_Effect_AlvisRock_Fragment_";
                    strTag += to_wstring(i);
                    for (_uint j = 0; j < 3; ++j)
                    {
                        XMStoreFloat3(&desc.vDirection, XMVector3Normalize(XMVectorSet(fRand(-1.f, 1.f), fRand(0.5f, 1.f), fRand(-1.f, 1.f), 0.f)) * fRand(10.f, 15.f));
                        m_pGameInstance->Push_Object_From_Pool(strTag, m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, &Impact);
                    }
                }
                m_fLeftTrailTimer = 1.0f;
                m_fRightTrailTimer = 1.0f;
                m_pLeftTrail->Push_From_Pool(nullptr);
                m_pRightTrail->Push_From_Pool(nullptr);
            }
            if (fCurRatio >= 0.2f)
            {
                m_pCCTCom->m_vVelocity.x = 0.f;
                m_pCCTCom->m_vVelocity.z = 0.f;
            }
        }
            break;
        case Client::CMonster_Surt::CLAP_ROAR:
        {
            m_MatRune = m_pTransformCom->Get_WorldMatrix();
            m_MatRune._42 += 3.f;
            _float4 vLook;
            XMStoreFloat4(&vLook, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
            m_MatRune._41 += vLook.x * 1.5f;
            m_MatRune._43 += vLook.z * 1.5f;
            if (!m_bPush)
            {
                m_bPush = true;
                CEffect_SurtLaser_Warning::LASER_DESC lDesc{};
                lDesc.fLifeTime = m_pBeatManager->Get_Beat() * 8.f;
                lDesc.pTarget = m_pPlayerManager->Get_Player()->Get_Transform()->Get_WorldMatrix_Ptr();
                _float4x4 Effect = m_pTransformCom->Get_WorldMatrix();
                Effect._42 += 2.7f;
                m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_SurtLaser_Warning", m_pGameInstance->Get_Current_LevelID(), L"Effect", &lDesc, &Effect);

                //m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_SurtRune", m_pGameInstance->Get_Current_LevelID(), L"Effect", &m_MatRune, &m_MatRune);

            }//0.4 ~ 0.64
            m_fAfterImageTimer += fTimeDelta;
            if (m_fAfterImageTimer >= 0.05f)
            {
                m_pAfterImage->SetUp_Matrices(m_pTransformCom->Get_WorldMatrix());
                m_fAfterImageTimer -= 0.05f;
            }
        }
            break;
        case Client::CMonster_Surt::ROAR:
        {
            if (!m_bPush)
            {
                m_bPush = true;
                m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Surt_Roar");
            }
        }
            break;
        case Client::CMonster_Surt::DYING:
        {
            pPlayer->NoMiss(true);
        }
            break;
        case Client::CMonster_Surt::STATE_END:
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
        if(m_eState != DYING)
        {
            m_fRuneTimer -= fTimeDelta;
        }
        if(m_fRuneTimer <= 0.f)
        {
            CEffect_RuneField::RF_DESC rdesc;
            _float4x4 EffectMat;
            _matrix MatTemp = XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr());
            _uint iRand = rand() % 3 + 1;
            rdesc.bLight = true;
            for (_uint i = 0; i < iRand; ++i)
            {
                rdesc.fLifeTime = (4.f + (_float)i) * m_pBeatManager->Get_Beat();
                _matrix MatEffect = MatTemp;
                MatEffect.r[3] += XMVectorSet(fRand(-30.f, 30.f), 0.f, fRand(-30.f, 30.f), 0.f);
                XMStoreFloat4x4(&EffectMat, MatEffect);
                m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_RuneField", m_pGameInstance->Get_Current_LevelID(), L"Effect", &rdesc, &EffectMat);
   
            }
            m_fRuneTimer = m_pBeatManager->Get_Beat() * 8.f;
        }


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
        if (m_fLeftTrailTimer >= 0.f)
        {
            m_pLeftTrail->Tick(fTimeDelta);
        }
        if (m_fRightTrailTimer >= 0.f)
        {
            m_pRightTrail->Tick(fTimeDelta);
        }
    }
    m_pAfterImage->Tick(fTimeDelta);
}

void CMonster_Surt::LateTick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
    m_pHpBar->LateTick(fTimeDelta);
    if (m_fLeftTrailTimer >= 0.f)
    {
        m_pLeftTrail->LateTick(fTimeDelta);
    }
    if (m_fRightTrailTimer >= 0.f)
    {
        m_pRightTrail->LateTick(fTimeDelta);
    }
    m_pAfterImage->LateTick(fTimeDelta);
}

HRESULT CMonster_Surt::Render()
{
    m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
    m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
    m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
    _float4 vColor = { 0.8f,0.3f,0.4f,1.f };
    m_pShaderCom->Bind_Vector4("g_vColor", &vColor);
    m_pShaderCom->Bind_Vector4("g_vCamPos", &m_pGameInstance->Get_CamPosition_Float4());
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

void CMonster_Surt::Dying()
{
    if (m_pCCTCom)
        m_pCCTCom->Disable();
}

void CMonster_Surt::Push_From_Pool(void* pArg)
{
    if (pArg)
    {
        MONSTER_DESC* pDesc = (MONSTER_DESC*)pArg;
        m_pCCTCom->Set_FootPosition(XMLoadFloat4x4(&pDesc->Transformation));
        m_pTransformCom->Set_WorldMatrix(pDesc->Transformation);
    }
    m_iPrevBitCount = m_iCurrBitCount;
    m_iCurrBitCount = m_pBeatManager->Get_BeatCount();
}

void CMonster_Surt::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
{
    if (m_bKilled)
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
        _wstring Chord = L"Surt_Chord";
        m_pGameInstance->Stop_Sound(SOUND_CHANNEL::MUSIC, Chord + to_wstring(m_iFinish));
        ++m_iFinish;
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::MUSIC, Chord + to_wstring(m_iFinish));
        m_fAnimFrom = 0.2f * (_float)m_iFinish;
        m_fAnimTo = m_fAnimFrom + 0.2f;
        m_pModelCom->Set_AnimTag(m_strAnimTag[DYING], false);
        if (m_iFinish >= 4)
        {
            m_bKilled = true;
        }
        return;
    }
    m_fHp -= fAmount;
    if (m_fHp <= 0.f)
    {
        m_pModelCom->Set_AnimTag(m_strAnimTag[DYING], true);
        m_fAnimFrom = 0.f;
        m_fAnimTo = 0.2f;
        m_eState = DYING;
        m_pBeatManager->Queue_Sequence(L"Asgard_CoolDown");
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::MUSIC, L"Surt_Chord0");
    }

}

void CMonster_Surt::To_Idle()
{
    m_eState = IDLE;
    m_pModelCom->Set_AnimTag(m_strAnimTag[IDLE], true);
    m_fTimer = m_pBeatManager->Get_Beat();
    m_fAnimFrom = 0.f;
    m_fAnimTo = 1.f;
}

void CMonster_Surt::State_Change()
{
    _int iRand = rand() % 4;
    if(iRand == 0)
    {
        m_eState = GROUND_SMASH_START;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    }
    if (iRand == 1)
    {
        m_eNextState = SWING_LEFT;
        m_eState = MOVE_START;
        m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE_START], false);
    }
    if (iRand == 2)
    {
        m_eState = CHARGE;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);

    }
    if (iRand == 3)
    {
        m_eState = CLAP_ROAR;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Surt_Laser");
    }

    m_iPrevBitCount = m_pBeatManager->Get_BeatCount();
    m_iAttackCount = 0;
    m_bChargeFlag = false;
    m_bPush = false;
    m_bFlag = false;
}

void CMonster_Surt::Anim_Finished()
{
    string strCurTag = m_pModelCom->Get_Current_Animation_Tag();

    switch (m_eState)
    {
    case Client::CMonster_Surt::START:
    {
        To_Idle();
    }
    break;
    case Client::CMonster_Surt::IDLE:
        break;
    case Client::CMonster_Surt::MOVE_START:
    {
        m_eState = MOVE;
        m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE], false);
    }
    break;
    case Client::CMonster_Surt::MOVE:
    {
        m_eState = m_eNextState;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;
    }
    break;
    case Client::CMonster_Surt::GROUND_SMASH_START:
    {
        m_eState = GROUND_SMASH;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;
    }
    break;
    case Client::CMonster_Surt::GROUND_SMASH:
    {
        To_Idle();
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;
    }
    break;
    case Client::CMonster_Surt::SWING_LEFT:
    {
        To_Idle();
    }
    break;
    case Client::CMonster_Surt::SWING_RIGHT:
    {
        To_Idle();
    }
    break;
    case Client::CMonster_Surt::CHARGE:
    {
        m_eState = SWING_UPPER;
        m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
        m_pCCTCom->m_vVelocity.x = 0.f;
        m_pCCTCom->m_vVelocity.z = 0.f;
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Surt_Slice");
    }
    break;
    case Client::CMonster_Surt::SWING_UPPER:
    {
        To_Idle();
    }
    break;
    case Client::CMonster_Surt::CLAP_ROAR:
    {
        To_Idle();
    }
    break;
    case Client::CMonster_Surt::ROAR:
    {
        To_Idle();
    }
    break;
    case Client::CMonster_Surt::DYING:
    {
        Dead();
    }
    break;
    default:
        break;
    }

}

CMonster_Surt* CMonster_Surt::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster_Surt* pInstance = new CMonster_Surt(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CMonster_Surt"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMonster_Surt::Clone(void* pArg)
{
    CMonster_Surt* pInstance = new CMonster_Surt(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CMonster_Surt"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster_Surt::Free()
{
    Safe_Release(m_pCCTCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pHpBar);
    Safe_Release(m_pRightTrail);
    Safe_Release(m_pLeftTrail);
    Safe_Release(m_pAfterImage);
    __super::Free();
}

#include "Monster_Draugr.h"
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

CMonster_Draugr::CMonster_Draugr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CMonster_Base(pDevice, pContext)
{
}

CMonster_Draugr::CMonster_Draugr(const CMonster_Draugr& rhs) :
    CMonster_Base(rhs)
{
}

HRESULT CMonster_Draugr::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CMonster_Draugr::Initialize(void* pArg)
{
    CTransform::TRANSFORM_DESC transDesc;
    transDesc.fRotationPerSec = XM_PI;
    __super::Initialize(pArg);
    Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Monster_Draugr", L"Model", (CComponent**)&m_pModelCom);
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
    cctDesc.radius = 0.5f;
    cctDesc.userData = this;
    _float3 vPos;
    XMStoreFloat3(&vPos, m_pTransformCom->Get_Position());
    cctDesc.position = PxExtendedVec3(vPos.x, vPos.y, vPos.z);
    cctDesc.material = m_pGameInstance->Get_Physics()->createMaterial(0.5f, 0.5f, 0.5f);
    Add_Component(LEVEL_STATIC, L"Prototype_Component_CharacterController", L"Com_CCT", (CComponent**)&m_pCCTCom, &cctDesc);
    m_pCCTCom->m_bGravity = false;
    m_pCCTCom->m_bUseVelocity_Calculation = true;
    m_pCCTCom->m_fFriction = 20.f;


    m_strAnimTag[DANCE] = "Sevarog_MaskedReaper_GDC.ao|Emote_Thriller";
    m_strAnimTag[CLAP] = "Sevarog_MaskedReaper_GDC.ao|Emote_Clap_T1";
    m_strAnimTag[IDLE] = "Sevarog_MaskedReaper_GDC.ao|Idle";
    m_strAnimTag[MOVE] = "Sevarog_MaskedReaper_GDC.ao|LaunchPad";
    m_strAnimTag[START] = "Sevarog_MaskedReaper_GDC.ao|LevelStart";
    m_strAnimTag[RAPID] = "Sevarog_MaskedReaper_GDC.ao|Recall";
    m_strAnimTag[SWING] = "Sevarog_MaskedReaper_GDC.ao|Ultimate_Swing_120fps";
    m_strAnimTag[DYING] = "Sevarog_MaskedReaper_GDC.ao|Knock_back_bwd";

    m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
    m_fTimer = m_pBeatManager->Get_Beat();
    m_ProjectileDesc.eCG = CG_MONSTER_PROJECTILE;
    m_fHp = 2000.f;
    m_fMaxHp = m_fHp;  
    m_pParticle = (CParticle*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_Effect_Particle_Plasma_Orbit", nullptr);
    m_pHpBar = (CUI_BossHP*)m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_BossHP", nullptr);
    m_pHpBar->Set_MaxHp(m_fMaxHp);
    m_pHpBar->Set_Hp(m_fHp);
    return S_OK;
}

void CMonster_Draugr::PriorityTick(_float fTimeDelta)
{
    m_pParticle->PriorityTick(fTimeDelta);
    m_pHpBar->PriorityTick(fTimeDelta);
    if (m_fInvincible > 0.001f)
    {
        m_fInvincible -= fTimeDelta;
    }
}

void CMonster_Draugr::Tick(_float fTimeDelta)
{
    string strCurTag = m_pModelCom->Get_Current_Animation_Tag();
    if(strCurTag != m_strAnimTag[SWING])
        m_pModelCom->Play_Animation(fTimeDelta * 1.5f, m_fAnimFrom, m_fAnimTo);
    else
    {
        m_pModelCom->Play_Animation(fTimeDelta * 4.0f, m_fAnimFrom, m_fAnimTo);
    }
    if (m_pModelCom->Get_Finished())
    {
        if(strCurTag != m_strAnimTag[MOVE])
        {
            if(m_eState != DYING)
                To_Idle();
            else
            {
                _float4 vDest = { 0.f,0.f,24.f,1.f };
                _float4x4 PortWorld;
                XMStoreFloat4x4(&PortWorld, XMMatrixTranslation(0.f, -10.f, 24.f));
                m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Boss_Portal", m_pGameInstance->Get_Current_LevelID(), L"Portal", &vDest, &PortWorld);
                //0, 0, 24
                Dead();
            }
        }
        else
        {
            m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
            if (m_eState == DANCE)
            {
                m_fAnimFrom = 0.f;
                m_fAnimTo = 1.f / 7.f;
                m_pBeatManager->Set_N_BeatType(4, BM_DODGE);
                m_iAttackDelay = 4;
                m_iPrevBitCount = m_pBeatManager->Get_BeatCount();

                m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Dance_Start");
            }
        }
    }
    if (m_eState == IDLE)
    {
        m_fTimer -= fTimeDelta;
        if (m_fTimer <= 0.f)
        {
            State_Change();
        }
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
    XMStoreFloat4x4(&m_MatLeft, m_pModelCom->Get_BoneMatrix("ik_hand_l") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
    XMStoreFloat4x4(&m_MatRight, m_pModelCom->Get_BoneMatrix("ik_hand_r") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()));
    if (m_eState != MOVE)
    {
        _vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
        _vector vTargetPos = m_pPlayerManager->Get_Player()->Get_Transform()->Get_Position();
        m_pTransformCom->LookAt_Horizontal(vTargetPos);
        //_vector vDir = vTargetPos - vLook;
        //vLook = XMVector3Normalize( XMVectorSetY(vLook, 0.f));
        //vDir = XMVector3Normalize(XMVectorSetY(vDir, 0.f));
        //
        //if (XMVectorGetX(XMVector3Dot(vLook, vDir)) >= 0.02f)
        //{
        //    m_pTransformCom->Turn(XMVector3Cross(vLook, vDir), fTimeDelta);
        //
        //}
    }
    if(m_pModelCom->Get_Current_Animation_Tag() != m_strAnimTag[MOVE])
        {
            switch (m_eState)
            {
            case Client::CMonster_Draugr::START:
                break;
            case Client::CMonster_Draugr::IDLE:
                break;
            case Client::CMonster_Draugr::CLAP:
            {
                Clap(fTimeDelta);
            }
                break;
            case Client::CMonster_Draugr::MOVE:
                break;
            case Client::CMonster_Draugr::DANCE:
            {
                Dance_Attack(fTimeDelta);
            }
                break;
            case Client::CMonster_Draugr::RAPID:
            {
                Rapid(fTimeDelta);
            }
                break;
            case Client::CMonster_Draugr::SWING:
            {
                Swing(fTimeDelta);
            }
                break;
            case Client::CMonster_Draugr::DYING:
            {
                m_pPlayerManager->Get_Player()->NoMiss(true);
            }
                break;
            default:
                break;
            }
        }

    m_pCCTCom->Update(fTimeDelta);

    auto pxvFP = m_pCCTCom->Get_Controller()->getFootPosition();
    m_pTransformCom->Set_Position(XMVectorSet(pxvFP.x, pxvFP.y, pxvFP.z, 1.f));
    _matrix ParticleMat = m_pTransformCom->Get_WorldMatrix_XMMat();
    ParticleMat.r[3] = XMVectorSetY(ParticleMat.r[3], XMVectorGetY(ParticleMat.r[3]) + 1.5f);
    m_pParticle->Get_Transform()->Set_WorldMatrix(ParticleMat);
    m_pParticle->Tick(fTimeDelta);
    m_pHpBar->Set_Hp(m_fHp);
    m_pHpBar->Tick(fTimeDelta);
}

void CMonster_Draugr::LateTick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
    m_pParticle->LateTick(fTimeDelta);
    m_pHpBar->LateTick(fTimeDelta);
}

HRESULT CMonster_Draugr::Render()
{
    m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
    m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
    m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
    _float4 vColor = { 0.2f,0.8f,0.4f,1.f };
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

void CMonster_Draugr::Dying()
{
    if (m_pCCTCom)
        m_pCCTCom->Disable();
}

void CMonster_Draugr::Push_From_Pool(void* pArg)
{
    if (pArg)
    {
        MONSTER_DESC* pDesc = (MONSTER_DESC*)pArg;
        m_pCCTCom->Set_FootPosition(XMLoadFloat4x4(&pDesc->Transformation));
        m_pTransformCom->Set_WorldMatrix(pDesc->Transformation);
    }
}

void CMonster_Draugr::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
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
        _wstring Chord = L"Draugr_Chord";
        m_pGameInstance->Stop_Sound(SOUND_CHANNEL::MUSIC, Chord + to_wstring(m_iFinish));
        ++m_iFinish;
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::MUSIC, Chord + to_wstring(m_iFinish));
        if(m_iFinish < 4)
            m_pModelCom->Set_AnimTag("Sevarog_MaskedReaper_GDC.ao|Knock_back_bwd", true);
        if (m_iFinish >= 4)
        {
            m_pModelCom->Set_AnimTag("Sevarog_MaskedReaper_GDC.ao|Knock_back_bwd", false);
            m_bKilled = true;
        }
        return;
    }
    if (m_bKilled)
        return;
    m_fHp -= fAmount;
    if (m_fHp <= 0.f)
    {
        if (m_iFinish < 4)
            m_pModelCom->Set_AnimTag("Sevarog_MaskedReaper_GDC.ao|Knock_back_bwd", true);
        else
            m_pModelCom->Set_AnimTag("Sevarog_MaskedReaper_GDC.ao|Knock_back_bwd", false);
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Death");
        m_eState = DYING;
        m_pBeatManager->Queue_Sequence(L"Asgard_CoolDown");
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::MUSIC, L"Draugr_Chord0");
    }

}

void CMonster_Draugr::To_Idle()
{
    m_eState = IDLE;
    m_pModelCom->Set_AnimTag(m_strAnimTag[IDLE], true);
    m_fTimer = m_pBeatManager->Get_Beat() * 4.f;
    m_fAnimFrom = 0.f;
    m_fAnimTo = 1.f;
}

void CMonster_Draugr::Dance_Attack(_float fTimeDelta)
{
    if (!m_bChargeFlag)
    {
        &m_MatRight;
        m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_DraugrRune", m_pGameInstance->Get_Current_LevelID(), L"Effect", &m_MatRight);
        &m_MatLeft;
        m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_DraugrRune", m_pGameInstance->Get_Current_LevelID(), L"Effect", &m_MatLeft);
        m_bChargeFlag = true;
    }

    if (m_pBeatManager->Get_BeatCount() - m_iPrevBitCount >= m_iAttackDelay)
    {
        m_iAttackDelay = 4;
        m_iPrevBitCount = m_pBeatManager->Get_BeatCount();
        _float4x4 EffectMatrix;
        _vector vTargetPos = m_pPlayerManager->Get_Player()->Get_Transform()->Get_Position();
      
        _vector vPos = (m_pModelCom->Get_BoneMatrix("ik_hand_l") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix())).r[3];
        _vector vDir = vTargetPos - vPos;
        vDir = XMVector3Normalize(XMVectorSetY(vDir, 0.f));
        _vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

        _float3 vfDir;
        XMStoreFloat3(&vfDir, vDir);
        _float fAngle = atan2(vfDir.z, vfDir.x);
        if (m_iAttackCount % 2)
        {//Left
            _vector vLook = XMVector3Cross(vDir, vUp);
            _vector vRight = XMVector3Cross(vUp, vLook);
            _matrix WallWorld = XMMatrixIdentity();
            WallWorld.r[0] = vRight;
            WallWorld.r[1] = vUp;
            WallWorld.r[2] = vLook;
            WallWorld.r[3] = vPos;
            XMStoreFloat4x4(&EffectMatrix, XMMatrixScaling(300.f, 300.f, 1.f) * WallWorld);
        }
        else
        {//Right
            _vector vLook = XMVector3Cross(vUp, vDir);
            _vector vRight = XMVector3Cross(vUp, vLook);
            _matrix WallWorld = XMMatrixIdentity();
            WallWorld.r[0] = vRight;
            WallWorld.r[1] = vUp;
            WallWorld.r[2] = vLook;
            WallWorld.r[3] = vPos;
            XMStoreFloat4x4(&EffectMatrix, XMMatrixScaling(300.f, 300.f, 1.f) * WallWorld);
        }



        m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Wall", m_pGameInstance->Get_Current_LevelID(), L"Effect", nullptr, &EffectMatrix);
        ++m_iAttackCount;
        if (m_iAttackCount >= 4)
        {
            To_Idle();
        }
        else
        {
            if(m_iAttackCount % 2)
            {
                m_fAnimFrom = 1.f /7.f;
                m_fAnimTo = 2.f / 7.f;
                m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
                //Left
                m_pBeatManager->Set_N_BeatType(3, BM_DODGE);
            }
            else
            {
                m_fAnimFrom = 2.f / 7.f;
                m_fAnimTo = 0.38f;
                m_pModelCom->Set_AnimTag(m_strAnimTag[m_eState], false);
                //Right
                m_pBeatManager->Set_N_BeatType(3, BM_DODGE);
            }
        }
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Dance");
    }

}

void CMonster_Draugr::Clap(_float fTimeDelta)
{
    if (!m_bChargeFlag)
    {
        CEffect_Charge::CHARGE_DESC cDesc;
        cDesc.fLifeTime = 6.f;
        cDesc.pParent = &m_MatRight;
        m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Charge", m_pGameInstance->Get_Current_LevelID(), L"Effect", &cDesc);
        cDesc.pParent = &m_MatLeft;
        m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Charge", m_pGameInstance->Get_Current_LevelID(), L"Effect", &cDesc);
        m_bChargeFlag = true;
    }
    if (m_iAttackCount == 0 && m_pModelCom->Get_Current_Animation_TrackRatio() >= 0.3101604f)
    {//58/187
        ++m_iAttackCount;
        Projectile();
    }
    else if (m_iAttackCount == 1 && m_pModelCom->Get_Current_Animation_TrackRatio() >= 0.4919786f)
    {//92/187
        ++m_iAttackCount;
        Projectile();
    }
    else if (m_iAttackCount == 2 && m_pModelCom->Get_Current_Animation_TrackRatio() >= 0.6631016f)
    {//124/187
        ++m_iAttackCount;
        Projectile();
    }


        
}

void CMonster_Draugr::Rapid(_float fTimeDelta)
{
    //180/241부터 시작
    _float fTR = m_pModelCom->Get_Current_Animation_TrackRatio();

    if (!m_bChargeFlag)
    {
        CEffect_Charge::CHARGE_DESC cDesc;
        cDesc.fLifeTime = 8.f;
        cDesc.pParent = &m_MatRight;
        m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Charge", m_pGameInstance->Get_Current_LevelID(), L"Effect", &cDesc);
        cDesc.pParent = &m_MatLeft;
        m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Charge", m_pGameInstance->Get_Current_LevelID(), L"Effect", &cDesc);
        m_bChargeFlag = true;

    }
    if (fTR >= 0.29f && !m_bPush)
    {
        m_bPush = true;
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Hammer");
        auto pPlayer = m_pPlayerManager->Get_Player();
        _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
        _vector vPos = m_pTransformCom->Get_Position();
        _vector vDir = vPlayerPosition - vPos;
        if (XMVectorGetX(XMVector3Length(vDir)) <= 8.f)
        {
            XMStoreFloat3(&pPlayer->Get_CCT()->m_vImpulse, XMVector3Normalize(vDir) * 15.f);
        }
    }

    if(fTR >= 0.75f)
    {
        for(_int i = 0;i< 10;++i)
        {
            if(fTR >= 0.75f + i * 0.025f && m_iAttackCount == i)
            {
                auto pPlayer = m_pPlayerManager->Get_Player();
                _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                string strBone;
                if (i % 2)
                {
                    strBone = "ik_hand_l";
                }
                else
                {
                    strBone = "ik_hand_r";
                }
                _matrix ProjectileMat = m_pModelCom->Get_BoneMatrix(strBone) * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix());
                _vector vPosition = ProjectileMat.r[3];
                XMStoreFloat3(&m_ProjectileDesc.vDirection, XMVector3Normalize(vPlayerPosition - vPosition) * 26.f);
                //ProjectileMat.r[3] += XMLoadFloat3(&m_ProjectileDesc.vDirection) * 0.1f;
                _float4x4 ProjMat;
                XMStoreFloat4x4(&ProjMat, ProjectileMat);
                m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Projectile", m_pGameInstance->Get_Current_LevelID(), L"Effect",
                    &m_ProjectileDesc, &ProjMat);
                ++m_iAttackCount;
                m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Projectile");
                break;
            }
        }
    }

}

void CMonster_Draugr::Swing(_float fTimeDelta)
{
    _float fTR = m_pModelCom->Get_Current_Animation_TrackRatio();
    if (fTR >= 0.25f && !m_bPush)
    {
        m_bPush = true;
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Hammer");
        CEffect_Sweep::SWEEP_DESC cDesc;
        cDesc.fLifeTime = 1.f;
        cDesc.pParent = m_pTransformCom->Get_WorldMatrix_Ptr();
        m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Sweep", m_pGameInstance->Get_Current_LevelID(), L"Effect", &cDesc);
        auto pPlayer = m_pPlayerManager->Get_Player();
        _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
        _vector vPos = m_pTransformCom->Get_Position();
        _vector vDir = vPlayerPosition - vPos;
        if (XMVectorGetX(XMVector3Length(vDir)) <= 6.f)
        {
            XMStoreFloat3(&pPlayer->Get_CCT()->m_vImpulse, XMVector3Normalize(vDir) * 15.f);
            _float3 vHit;
            XMStoreFloat3(&vHit, vPlayerPosition);
            pPlayer->Take_Damage(25.f, vHit, DMG_EXPLODE, this);
        }
        CEffect_Spike::PROJECTILE_DESC desc;
        desc.eCG = CG_MONSTER_PROJECTILE;
        desc.fLifeTime = m_pBeatManager->Get_Beat() * 2.f;
        desc.iSpikeLeft = 20;
        XMStoreFloat3(&desc.vDirection, XMVector3Normalize(vDir) * 2.f);
        _float4x4 SpikeMat ;
        XMStoreFloat4x4(&SpikeMat, XMMatrixTranslationFromVector(vPos + XMVector3Normalize(vDir)));
        m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Spike", m_pGameInstance->Get_Current_LevelID(), L"Effect", &desc, &SpikeMat);
    }
}

void CMonster_Draugr::Projectile()
{
    auto pPlayer = m_pPlayerManager->Get_Player();

    _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
    _matrix ProjectileMat = m_pModelCom->Get_BoneMatrix("ik_hand_l") * XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix());
    _vector vPosition = ProjectileMat.r[3];
    XMStoreFloat3(&m_ProjectileDesc.vDirection, XMVector3Normalize(vPlayerPosition - vPosition) * 13.f);
    //ProjectileMat.r[3] += XMLoadFloat3(&m_ProjectileDesc.vDirection) * 0.1f;
    _float4x4 ProjMat;
    XMStoreFloat4x4(&ProjMat, ProjectileMat);
    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Projectile", m_pGameInstance->Get_Current_LevelID(), L"Effect",
        &m_ProjectileDesc, &ProjMat);

    XMStoreFloat3(&m_ProjectileDesc.vDirection, XMVector3TransformNormal(XMVector3Normalize(vPlayerPosition - vPosition), XMMatrixRotationY(XM_PI * 0.33f)) * 13.f);
    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Projectile", m_pGameInstance->Get_Current_LevelID(), L"Effect",
        &m_ProjectileDesc, &ProjMat);
    XMStoreFloat3(&m_ProjectileDesc.vDirection, XMVector3TransformNormal(XMVector3Normalize(vPlayerPosition - vPosition), XMMatrixRotationY(-XM_PI * 0.33f)) * 13.f);
    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Projectile", m_pGameInstance->Get_Current_LevelID(), L"Effect",
        &m_ProjectileDesc, &ProjMat);
    m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Projectile");
}

void CMonster_Draugr::State_Change()
{
    _int iRand = rand() % (_int)STATE_END;
    while (iRand == STATE::MOVE || iRand == STATE::IDLE || iRand == STATE::DYING || iRand == START)
    {
        iRand = rand() % (_int)STATE_END;
    }
    m_eState = (STATE)iRand;
    m_pModelCom->Set_AnimTag(m_strAnimTag[MOVE], false);
    auto pPlayer = m_pPlayerManager->Get_Player();
    _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
    _vector vPosition = m_pTransformCom->Get_Position();
    XMStoreFloat3(& m_pCCTCom->m_vVelocity, XMVector3Normalize(vPlayerPosition - vPosition) * 20.f);
    m_iPrevBitCount = m_pBeatManager->Get_BeatCount();
    m_iAttackCount = 0;
    m_bChargeFlag = false;
    m_bPush = false;
}

CMonster_Draugr* CMonster_Draugr::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster_Draugr* pInstance = new CMonster_Draugr(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CMonster_Draugr"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMonster_Draugr::Clone(void* pArg)
{
    CMonster_Draugr* pInstance = new CMonster_Draugr(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CMonster_Draugr"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster_Draugr::Free()
{
    Safe_Release(m_pCCTCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pParticle);
    Safe_Release(m_pHpBar);
    __super::Free();
}

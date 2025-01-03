#include "Monster_Luminous.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
#include "CharacterController.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Beat_Manager.h"
#include "Effect_Teardrop.h"

CMonster_Luminous::CMonster_Luminous(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CMonster_Base(pDevice, pContext)
{
}

CMonster_Luminous::CMonster_Luminous(const CMonster_Luminous& rhs) :
    CMonster_Base(rhs)
{
    m_iCollisionGroup = CG_MONSTER_CONTACT;
}

HRESULT CMonster_Luminous::Initialize_Prototype()
{
    __super::Initialize_Prototype();
    return S_OK;
}

HRESULT CMonster_Luminous::Initialize(void* pArg)
{
    __super::Initialize(pArg);
    Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Luminous", L"Model", (CComponent**)&m_pModelCom);
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
    cctDesc.height = 0.5f;
    cctDesc.radius = 0.5f;
    cctDesc.userData = this;
    _float3 vPos;
    XMStoreFloat3(&vPos, m_pTransformCom->Get_Position());
    cctDesc.position = PxExtendedVec3(vPos.x, vPos.y, vPos.z);
    cctDesc.material = m_pGameInstance->Get_Physics()->createMaterial(0.5f, 0.5f, 0.5f);
    Add_Component(LEVEL_STATIC, L"Prototype_Component_CharacterController", L"Com_CCT", (CComponent**)&m_pCCTCom, &cctDesc);
    m_pCCTCom->m_bGravity = false;
    m_pCCTCom->m_bUseVelocity_Calculation = true;
    m_pCCTCom->m_fFriction = 30.f;

    m_fHp = 350.f;

    m_pModelCom->Set_AnimTag(m_strIdle, true);
    return S_OK;
}

void CMonster_Luminous::PriorityTick(_float fTimeDelta)
{
    if (m_fInvincible > 0.001f)
    {
        m_fInvincible -= fTimeDelta;
    }
}

void CMonster_Luminous::Tick(_float fTimeDelta)
{
    m_pModelCom->Play_Animation(fTimeDelta);
    auto pPlayer = m_pPlayerManager->Get_Player();
    if (m_pModelCom->Get_Current_Animation_Tag() == m_strAttack)
    {
        if (m_pModelCom->Get_Current_Animation_TrackRatio() >= 0.5f && !m_bAttacking)
        {

            if (pPlayer)
            {
                _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
                _vector vPosition = m_pTransformCom->Get_Position();
                XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(vPlayerPosition - vPosition) * (30.f + rand() % 10 - 5));
                m_bAttacking = true;
                m_iPrevBitCount = m_pBeatManager->Get_BeatCount();
            }
        }
    }
    if (!m_fHp <= 0.f && !m_bAttacking && pPlayer)
        m_pTransformCom->LookAt_Horizontal(pPlayer->Get_Transform()->Get_Position());

    if (m_pModelCom->Get_Finished())
    {
        if (m_fHp <= 0.f)
        {
            m_bKilled = true;
            Drop_Coin();
            Dead();
        }
        else
        {
            m_pModelCom->Set_AnimTag(m_strIdleTag, true);
        }
    }
    if (m_pBeatManager->Get_BeatCount() - m_iPrevBitCount >= m_iAttackDelay)
    {
        Attack();
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
        m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::Emission;
    }
    if (m_fHp <= 0.f)
    {
        m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::Dissolve;
    }
    m_pCCTCom->Update(fTimeDelta);
    auto pRawController = m_pCCTCom->Get_Controller();
    if (pRawController)
    {
        auto pxvFP = pRawController->getFootPosition();
        m_pTransformCom->Set_Position(XMVectorSet(pxvFP.x, pxvFP.y, pxvFP.z, 1.f));
    }
}

void CMonster_Luminous::LateTick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CMonster_Luminous::Render()
{
    m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_VIEW));
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(D3DTS_PROJ));
    m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix");
    m_pShaderCom->Bind_RawValue("g_iObjectID", &m_iObjectID, sizeof(_uint));
    if (m_iRenderPass == (_uint)SHADER_VTXANIMMESH_PASS::Dissolve)
    {
        m_pShaderCom->Bind_Float("g_Dissolve", m_pModelCom->Get_Current_Animation_TrackRatio());
        DEFAULTNOISE->Bind_ShaderResource(m_pShaderCom, "g_Noise_Texture");
    }
    if (m_iRenderPass == (_uint)SHADER_VTXANIMMESH_PASS::ColorBoost)
    {
        m_pShaderCom->Bind_Float("g_fMagicNumber", 3.f);
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
                pTexture = vecMat[3];
                if (pTexture)
                    pTexture->Bind_ShaderResource(m_pShaderCom, "g_Emission_Texture");
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

void CMonster_Luminous::Dying()
{
    __super::Dying();
    if (m_pCCTCom)
        m_pCCTCom->Disable();
}

void CMonster_Luminous::Push_From_Pool(void* pArg)
{
    if (pArg)
    {
        MONSTER_DESC* pDesc = (MONSTER_DESC*)pArg;
        m_pCCTCom->Set_FootPosition(XMLoadFloat4x4(&pDesc->Transformation));
        m_pTransformCom->Set_WorldMatrix(pDesc->Transformation);
    }
}

void CMonster_Luminous::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
{
    if (m_fHp <= 0.f)
    {
        return;
    }
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
    m_fHp -= fAmount;
    m_fTakeHit = 0.2f;
    if (m_fHp <= 0.f)
    {
        m_bKilled = true;
        m_pCCTCom->m_bGravity = true;
        m_pCCTCom->m_vVelocity.y -= 5.f;
        m_pModelCom->Set_AnimTag("SK_Luminous_Monster.ao|Anim_Luminous_Monster_Death", false);

        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Luminous_Death");
        //m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Spider_Death");
        if (iDmgType == DMG_EXPLODE)
        {
            XMStoreFloat3(&m_pCCTCom->m_vImpulse, XMLoadFloat3(&m_pCCTCom->m_vImpulse) + XMVector3Normalize(m_pTransformCom->Get_Position() - XMLoadFloat3(&vHitPoint)) * fAmount * 0.5f);
        }
    }
    else
    {
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Luminous_Hurt");
        //m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Spider_Attacked");
    }
}

void CMonster_Luminous::Move_Action()
{
    if (m_fHp <= 0.f)
    {
        return;
    }
    m_iAttackCount = 4;
    auto pPlayer = m_pPlayerManager->Get_Player();
    _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
    _vector vPosition = m_pTransformCom->Get_Position();

    CEffect_Teardrop::PROJECTILE_DESC PDESC;
    PDESC.eCG = CG_MONSTER_PROJECTILE;
    _vector vDir = XMVector3Normalize(vPlayerPosition - vPosition);
    XMStoreFloat3(&PDESC.vDirection, vDir * 16.f);
    _matrix ProjectileMat = XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix());
    ProjectileMat.r[3] += XMLoadFloat3(&PDESC.vDirection) * 0.1f;
    _float4x4 ProjMat;
    XMStoreFloat4x4(&ProjMat, XMMatrixScaling(0.5f, 0.5f, 0.5f) * ProjectileMat);
    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Teardrop", m_pGameInstance->Get_Current_LevelID(), L"Effect",
        &PDESC, &ProjMat);


    vPlayerPosition += XMVectorSet(rand() % 20 - 10.f, 0.f, rand() % 20 - 10.f, 0.f);
    vPosition = XMVectorSetY(vPosition, 0.f);
    XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(vPlayerPosition - vPosition) * 30.f);
    m_iPrevBitCount = m_pBeatManager->Get_BeatCount();
    m_iAttackDelay = 5 + rand() % 5;
}

void CMonster_Luminous::Attack()
{
    if (m_fHp <= 0.f)
        return;
    m_bAttacking = false;
    m_iPrevBitCount = m_pBeatManager->Get_BeatCount();

    m_pModelCom->Set_AnimTag(m_strAttack, false);

    m_iAttackDelay = 10 + rand() % 4;
}

CMonster_Luminous* CMonster_Luminous::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster_Luminous* pInstance = new CMonster_Luminous(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CMonster_Luminous"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMonster_Luminous::Clone(void* pArg)
{
    CMonster_Luminous* pInstance = new CMonster_Luminous(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CMonster_Luminous"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster_Luminous::Free()
{
    Safe_Release(m_pCCTCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    __super::Free();
}

#include "Monster_Fly.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
#include "CharacterController.h"
#include "Player_Manager.h"
#include "Player.h"
#include "Beat_Manager.h"
#include "Effect_Projectile.h"

CMonster_Fly::CMonster_Fly(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
    CMonster_Base(pDevice,pContext)
{
}

CMonster_Fly::CMonster_Fly(const CMonster_Fly& rhs):
    CMonster_Base(rhs)
{
}

HRESULT CMonster_Fly::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CMonster_Fly::Initialize(void* pArg)
{
	__super::Initialize(pArg);
    Add_Component(LEVEL_STATIC, L"Prototype_Component_Model_Monster_Fly", L"Model", (CComponent**)&m_pModelCom);
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

    m_fHp = 175.f;

    m_pModelCom->Set_AnimTag(m_strIdle, true);
	return S_OK;
}

void CMonster_Fly::PriorityTick(_float fTimeDelta)
{
    if (m_fInvincible > 0.001f)
    {
        m_fInvincible -= fTimeDelta;
    }
}

void CMonster_Fly::Tick(_float fTimeDelta)
{
    m_pModelCom->Play_Animation(fTimeDelta);
    auto pPlayer = m_pPlayerManager->Get_Player();
    if(pPlayer)
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
            m_pModelCom->Set_AnimTag(m_strIdle, true);
        }
    }
    if (m_pBeatManager->Get_BeatCount() - m_iPrevBitCount >= m_iAttackDelay && pPlayer)
    {
        Move_Action();
    }

    Hit_ChangeColor(fTimeDelta);
    if (m_fHp <= 0.f)
    {
        m_iRenderPass = (_uint)SHADER_VTXANIMMESH_PASS::Dissolve;
    }
    m_pCCTCom->Update(fTimeDelta);
    auto pRawController =  m_pCCTCom->Get_Controller();
    if(pRawController)
    {
        auto pxvFP = pRawController->getFootPosition();
        m_pTransformCom->Set_Position(XMVectorSet(pxvFP.x, pxvFP.y, pxvFP.z, 1.f));
    }
}

void CMonster_Fly::LateTick(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CMonster_Fly::Render()
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

void CMonster_Fly::Dying()
{
    __super::Dying();
    if(m_pCCTCom)
        m_pCCTCom->Disable();
}

void CMonster_Fly::Push_From_Pool(void* pArg)
{
    if (pArg)
    {
        MONSTER_DESC* pDesc = (MONSTER_DESC*)pArg;
        m_pCCTCom->Set_FootPosition(XMLoadFloat4x4(&pDesc->Transformation));
        m_pTransformCom->Set_WorldMatrix(pDesc->Transformation);
    }
}

void CMonster_Fly::Take_Damage(_float fAmount, const _float3& vHitPoint, _uint iDmgType, CGameObject* pCauser)
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
        m_pModelCom->Set_AnimTag("SK_Insect.ao|Anim_Insect_Death", false);
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Spider_Death");
        if (iDmgType == DMG_EXPLODE)
        {
            XMStoreFloat3(&m_pCCTCom->m_vImpulse, XMLoadFloat3(&m_pCCTCom->m_vImpulse) + XMVector3Normalize(m_pTransformCom->Get_Position() - XMLoadFloat3(&vHitPoint)) * fAmount * 0.5f);
        }
    }
    else
    {
        m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Spider_Attacked");
    }
}

void CMonster_Fly::Move_Action()
{
    if (m_fHp <= 0.f)
    {
        return;
    }
    auto pPlayer = m_pPlayerManager->Get_Player();
    _vector vPlayerPosition = pPlayer->Get_Transform()->Get_Position();
    _vector vPosition = m_pTransformCom->Get_Position();
    CEffect_Projectile::PROJECTILE_DESC PDESC;
    PDESC.eCG = CG_MONSTER_PROJECTILE;
    XMStoreFloat3(&PDESC.vDirection, XMVector3Normalize(vPlayerPosition - vPosition) * 13.f);

    _matrix ProjectileMat = XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix());
    ProjectileMat.r[3] += XMLoadFloat3(&PDESC.vDirection) * 0.1f;
    _float4x4 ProjMat;
    XMStoreFloat4x4(&ProjMat, XMMatrixScaling(0.5f,0.5f,0.5f) * ProjectileMat);
    m_pGameInstance->Push_Object_From_Pool(L"Prototype_GameObject_Effect_Projectile", m_pGameInstance->Get_Current_LevelID(), L"Effect",
        &PDESC, &ProjMat);


    vPlayerPosition += XMVectorSet(rand() % 20 - 10.f, 0.f, rand() % 20 - 10.f, 0.f);
    vPosition = XMVectorSetY(vPosition, 0.f);
    XMStoreFloat3(&m_pCCTCom->m_vVelocity, XMVector3Normalize(vPlayerPosition - vPosition) * 23.f);
    m_iPrevBitCount = m_pBeatManager->Get_BeatCount();
    m_pModelCom->Set_AnimTag(m_strAttack, false);
    m_pGameInstance->Play_Sound(SOUND_CHANNEL::PLAYER_SOUND_EFFECT, L"Draugr_Projectile");
    m_iPrevBitCount = m_pBeatManager->Get_BeatCount();
    m_iAttackDelay = 5 + rand() % 5;
}

CMonster_Fly* CMonster_Fly::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster_Fly* pInstance = new CMonster_Fly(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CMonster_Fly"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMonster_Fly::Clone(void* pArg)
{
    CMonster_Fly* pInstance = new CMonster_Fly(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CMonster_Fly"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMonster_Fly::Free()
{
    Safe_Release(m_pCCTCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    __super::Free();
}
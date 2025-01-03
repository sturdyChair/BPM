

#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)
class CPlayer_Manager;

class CMagmaField : public CGameObject
{

private:
	CMagmaField(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMagmaField(const CMagmaField& rhs);
	virtual ~CMagmaField() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Dying() override;
	virtual void Push_From_Pool(void* pArg) override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = nullptr;
	CTexture* m_pWaterFlow = nullptr;
	//
	CPlayer_Manager* m_pPlayerManager = nullptr;
	_float m_fTimer = 0.f;
public:
	static CMagmaField* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
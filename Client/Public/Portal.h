#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CPortal : public CGameObject
{
public:
	class PORTAL_DESC
	{
	public:
		_wstring strModelTag;
		_float4x4 Transform;
		_bool	  bFrame;
	};


private:
	CPortal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPortal(const CPortal& rhs);
	virtual ~CPortal() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Dying() override;
	virtual void Push_From_Pool(void* pArg) override;

	void Open();

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = nullptr;
	bool	m_bCleared = false;
	_float  m_fOpenAngle = 0.f;
	_float  m_fOriginalAngle = 0.f;
	_wstring m_strModelTag;
	_wstring m_strArchModelTag;
	_float4 m_vDoorOffset = _float4{ 1.f,0.f,0.f,0.f };
	_float4x4 m_DoorTransformation;
	_float4x4 m_DoorMirrorTransformation;

public:
	static CPortal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight_Manager final : public CBase
{
private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	//const LIGHT_DESC* Get_LightDesc(_uint iIndex, LIGHT_DESC::TYPE eType) const;
	class CPointLight*		 Find_PointLight(const _wstring& strTag);
	class CSpotLight*		 Find_SpotLight(const _wstring& strTag);
	class CDirectionalLight* Find_DirectionalLight(const _wstring& strTag);

	HRESULT Remove_PointLight(const _wstring& strTag);
	HRESULT Remove_SpotLight(const _wstring& strTag);
	HRESULT Remove_DirectionalLight(const _wstring& strTag);
	HRESULT Add_Light(const _wstring& strTag, const PointLight& LightDesc);
	HRESULT Add_Light(const _wstring& strTag, const DirectionalLight& LightDesc);
	HRESULT Add_Light(const _wstring& strTag, const SpotLight& LightDesc);
	HRESULT Bind_Lights(class CShader* pShader);
	void	Set_AmbientColor(const _float4& vColor) { m_vAmbientColor = vColor; }

public:
	HRESULT Initialize();


private:
	map<_wstring, class CPointLight*>			m_PointLights;
	map<_wstring, class CSpotLight*>				m_SpotLights;
	map<_wstring, class CDirectionalLight*>		m_DirectionalLights;


	vector<PointLight>			m_pointLights;
	vector<DirectionalLight>	m_directionalLights;
	vector<SpotLight>			m_spotLights;
	_float4						m_vAmbientColor{};
								 
public:
	static CLight_Manager* Create();
	virtual void Free() override;

};

END
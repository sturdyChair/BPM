#include "..\Public\Light_Manager.h"
#include "Light.h"
#include "Shader.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"

CLight_Manager::CLight_Manager()
{
}

CPointLight* CLight_Manager::Find_PointLight(const _wstring& strTag)
{
	auto iter =
		m_PointLights.find(strTag);
	if (iter == m_PointLights.end())
	{
		return nullptr;
	}
	return iter->second;
}

CSpotLight* CLight_Manager::Find_SpotLight(const _wstring& strTag)
{
	auto iter =
		m_SpotLights.find(strTag);
	if (iter == m_SpotLights.end())
	{
		return nullptr;
	}
	return iter->second;
}

CDirectionalLight* CLight_Manager::Find_DirectionalLight(const _wstring& strTag)
{
	auto iter =
		m_DirectionalLights.find(strTag);
	if (iter == m_DirectionalLights.end())
	{
		return nullptr;
	}
	return iter->second;
}

HRESULT CLight_Manager::Remove_PointLight(const _wstring& strTag)
{
	auto iter =
		m_PointLights.find(strTag);
	if (iter == m_PointLights.end())
	{
		return E_FAIL;
	}
	Safe_Release(iter->second);
	m_PointLights.erase(iter);
	return S_OK;
}

HRESULT CLight_Manager::Remove_SpotLight(const _wstring& strTag)
{
	auto iter =
		m_SpotLights.find(strTag);
	if (iter == m_SpotLights.end())
	{
		return E_FAIL;
	}
	Safe_Release(iter->second);
	m_SpotLights.erase(iter);
	return S_OK;
}


HRESULT CLight_Manager::Remove_DirectionalLight(const _wstring& strTag)
{
	auto iter =
		m_DirectionalLights.find(strTag);
	if (iter == m_DirectionalLights.end())
	{
		return E_FAIL;
	}
	Safe_Release(iter->second);
	m_DirectionalLights.erase(iter);
	return S_OK;
}

//const LIGHT_DESC * CLight_Manager::Get_LightDesc(_uint iIndex, LIGHT_DESC::TYPE eType) const
//{
//	auto	iter = m_Lights[eType].begin();
//
//	for (size_t i = 0; i < iIndex; i++)
//		++iter;
//
//	if (iter == m_Lights[eType].end())
//		return nullptr;
//
//	return (*iter)->Get_LightDesc();
//}

HRESULT CLight_Manager::Initialize()
{
	m_vAmbientColor = { 1.f,1.f,1.f,1.f };
	return S_OK;
}

HRESULT CLight_Manager::Add_Light(const _wstring& strTag, const PointLight& LightDesc)
{
	if (Find_PointLight(strTag))
		return E_FAIL;

	m_PointLights.emplace(strTag, CPointLight::Create(LightDesc));
	return S_OK;
}

HRESULT CLight_Manager::Add_Light(const _wstring& strTag, const DirectionalLight& LightDesc)
{
	if (Find_DirectionalLight(strTag))
		return E_FAIL;
	m_DirectionalLights.emplace(strTag, CDirectionalLight::Create(LightDesc));
	return S_OK;
}

HRESULT CLight_Manager::Add_Light(const _wstring& strTag, const SpotLight& LightDesc)
{
	if (Find_SpotLight(strTag))
		return E_FAIL;
	m_SpotLights.emplace(strTag, CSpotLight::Create(LightDesc));
	return S_OK;
}


HRESULT CLight_Manager::Bind_Lights(CShader* pShader)
{
	m_pointLights.clear();
	m_directionalLights.clear();
	m_spotLights.clear();

	for (auto& Light:m_PointLights)
	{
		if (Light.second && Light.second->Is_Active())
		{
			m_pointLights.push_back(Light.second->Get_Light());
		}
	}
	for (auto& Light : m_DirectionalLights)
	{
		if (Light.second && Light.second->Is_Active())
		{
			m_directionalLights.push_back(Light.second->Get_Light());
		}
	}
	for (auto& Light : m_SpotLights)
	{
		if (Light.second && Light.second->Is_Active())
		{
			m_spotLights.push_back(Light.second->Get_Light());
		}
	}

	_int iNumLight = m_pointLights.size();
	if (iNumLight > 32)
		iNumLight = 32;
	pShader->Bind_RawValue("pointLights", m_pointLights.data(), iNumLight * sizeof(PointLight));
	pShader->Bind_RawValue("iNumPoint", &iNumLight, sizeof(_int));

	iNumLight = m_directionalLights.size();
	if (iNumLight > 32)
		iNumLight = 32;
	pShader->Bind_RawValue("directionalLights", m_directionalLights.data(), iNumLight * sizeof(DirectionalLight));
	pShader->Bind_RawValue("iNumDirectional", &iNumLight, sizeof(_int));

	iNumLight = m_spotLights.size();
	if (iNumLight > 32)
		iNumLight = 32;
	pShader->Bind_RawValue("spotLights", m_spotLights.data(), iNumLight * sizeof(SpotLight));
	pShader->Bind_RawValue("iNumSpot", &iNumLight, sizeof(_int));

	pShader->Bind_Vector4("ambientLight", &m_vAmbientColor);

	return S_OK;
}

CLight_Manager * CLight_Manager::Create()
{
	CLight_Manager*		pInstance = new CLight_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLight_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLight_Manager::Free()
{
	for (auto& pair : m_DirectionalLights)
	{
		Safe_Release(pair.second);
	}
	for (auto& pair : m_PointLights)
	{
		Safe_Release(pair.second);
	}
	for (auto& pair : m_SpotLights)
	{
		Safe_Release(pair.second);
	}


	__super::Free();


}


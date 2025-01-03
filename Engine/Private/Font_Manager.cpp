#include "Font_Manager.h"
#include "CustomFont.h"

CFont_Manager::CFont_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
	m_pDevice(pDevice),m_pContext(pContext)
{
	Safe_AddRef(m_pDevice); Safe_AddRef(m_pContext);
}

HRESULT CFont_Manager::Add_Font(const _wstring& strTag, const _wstring& strFile)
{
	auto pFont = Find_Font(strTag);
	if (pFont)
	{
		return E_FAIL;
	}
	pFont = CCustomFont::Create(m_pDevice, m_pContext, strFile);
	m_Fonts.emplace(strTag, pFont);
	return S_OK;
}

CCustomFont* CFont_Manager::Find_Font(const _wstring& strTag)
{
	auto iter = m_Fonts.find(strTag);
	if (iter != m_Fonts.end())
	{
		return iter->second;
	}

	return nullptr;
}

HRESULT CFont_Manager::Render(const _wstring& strTag, const _wstring& strText, const _float2& vPos, _fvector vColor, _float fRotation, const _float2& vOrigin, _float fScale)
{
	auto pFont = Find_Font(strTag);
	if (!pFont)
	{
		return E_FAIL;
	}
	pFont->Render(strText, vPos, vColor, fRotation, vOrigin, fScale);

	return S_OK;
}

CFont_Manager* CFont_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFont_Manager* pInstance = new CFont_Manager(pDevice, pContext);

	return pInstance;
}

void CFont_Manager::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	for (auto& pair : m_Fonts)
	{
		Safe_Release(pair.second);
	}
	m_Fonts.clear();
}

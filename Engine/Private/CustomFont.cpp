#include "CustomFont.h"

CCustomFont::CCustomFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext):
	m_pDevice(pDevice),m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}


HRESULT CCustomFont::Initialize(const _wstring& strFilePath)
{
	m_pBatch = new SpriteBatch(m_pContext);
	m_pFont = new SpriteFont(m_pDevice, strFilePath.c_str());

	return S_OK;
}

HRESULT CCustomFont::Render(const _wstring& strText, const _float2& vPos, _fvector vColor, _float fRotation, const _float2& vOrigin, _float fScale)
{
	m_pFont->DrawString(m_pBatch, strText.c_str(), vPos, vColor, fRotation, vOrigin, fScale);

	return S_OK;
}

CCustomFont* CCustomFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _wstring& strFilePath)
{
	CCustomFont* pInstance = new CCustomFont(pDevice, pContext);
	if (FAILED(pInstance->Initialize(strFilePath)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCustomFont::Free()
{
	delete m_pBatch;
	delete m_pFont;
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

}

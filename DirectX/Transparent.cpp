#include "pch.h"
#include "Transparent.h"
#include "Texture.h"

Transparent::Transparent(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::wstring& pathShader) :
	Material(pDevice, pDeviceContext, pathShader),
	m_pDiffuseMap{ m_pShader->GetVariableBySemantic("DiffuseMap")->AsShaderResource() }
{
	if (!m_pDiffuseMap) std::cout << "Effect Constructor couldn't find diffuse map from effect file" << std::endl;
}

Transparent::~Transparent()
{
	m_pDiffuseMap->Release();
}

void Transparent::SetDiffuseMap(Texture* pTexture)
{
	m_pDiffuseMap->SetResource(pTexture->GetShaderResourceView());
}
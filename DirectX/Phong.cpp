#include "pch.h"
#include "Phong.h"
#include "Texture.h"

Phong::Phong(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::wstring& pathShader) :
	Material(pDevice, pDeviceContext, pathShader),
	m_pWorldMatrix{ m_pShader->GetVariableBySemantic("WorldMatrix")->AsMatrix() },
	m_pCameraPosition{ m_pShader->GetVariableBySemantic("CameraPosition")->AsVector() },
	m_pDiffuseMap{ m_pShader->GetVariableBySemantic("DiffuseMap")->AsShaderResource() },
	m_pNormalMap{ m_pShader->GetVariableBySemantic("NormalMap")->AsShaderResource() },
	m_pSpecularMap{ m_pShader->GetVariableBySemantic("SpecularMap")->AsShaderResource() },
	m_pGlossinessMap{ m_pShader->GetVariableBySemantic("GlossinessMap")->AsShaderResource() },
	m_pUseNormalMap{ m_pShader->GetVariableBySemantic("UseNormalMap")->AsScalar() }
	
{
	if (!m_pWorldMatrix) std::cout << "Effect Constructor couldn't find world matrix from effect file" << std::endl;
	if (!m_pCameraPosition) std::cout << "Effect Constructor couldn't find camera position from effect file" << std::endl;
	if (!m_pDiffuseMap) std::cout << "Effect Constructor couldn't find diffuse map from effect file" << std::endl;
	if (!m_pNormalMap) std::cout << "Effect Constructor couldn't find normal map from effect file" << std::endl;
	if (!m_pSpecularMap) std::cout << "Effect Constructor couldn't find specular map from effect file" << std::endl;
	if (!m_pGlossinessMap) std::cout << "Effect Constructor couldn't find glossiness map from effect file" << std::endl;
}

Phong::~Phong()
{
	m_pGlossinessMap->Release();
	m_pSpecularMap->Release();
	m_pNormalMap->Release();
	m_pDiffuseMap->Release();
	m_pCameraPosition->Release();
	m_pWorldMatrix->Release();
}

void Phong::SetWorldMatrix(const dae::Matrix& matrix)
{
	float* pData
	{
		new float[16]
		{
			matrix[0].x, matrix[0].y, matrix[0].z, matrix[0].w,
			matrix[1].x, matrix[1].y, matrix[1].z, matrix[1].w,
			matrix[2].x, matrix[2].y, matrix[2].z, matrix[2].w,
			matrix[3].x, matrix[3].y, matrix[3].z, matrix[3].w
		}
	};

	m_pWorldMatrix->SetMatrix(pData);

	delete[] pData;
}

void Phong::SetCameraPosition(const dae::Vector3 position)
{
	float* pData
	{
		new float[4]
		{
			position.x, position.y, position.z, 0.0f
		}
	};

	m_pCameraPosition->SetFloatVector(pData);

	delete[] pData;
}

void Phong::SetDiffuseMap(Texture* pTexture)
{
	m_pDiffuseMap->SetResource(pTexture->GetShaderResourceView());
}

void Phong::SetNormalMap(Texture* pTexture)
{
	m_pNormalMap->SetResource(pTexture->GetShaderResourceView());
}

void Phong::SetSpecularMap(Texture* pTexture)
{
	m_pSpecularMap->SetResource(pTexture->GetShaderResourceView());
}

void Phong::SetGlossinessMap(Texture* pTexture)
{
	m_pGlossinessMap->SetResource(pTexture->GetShaderResourceView());
}

void Phong::ToggleUseNormalMap()
{
	bool* pCurrentToggle{ new bool };
	m_pUseNormalMap->GetBool(pCurrentToggle);
	m_pUseNormalMap->SetBool(!(*pCurrentToggle));
	std::cout << ((*pCurrentToggle) ? "Not using" : "Using") << " the normal map" << std::endl;
	delete pCurrentToggle;
}
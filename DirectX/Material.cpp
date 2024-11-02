#include "pch.h"
#include "Material.h"
#include "DataTypes.h"
#include "Texture.h"

using namespace dae;

Material::Material(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::wstring& pathShader) :
	m_pDevice{ pDevice },
	m_pDeviceContext{ pDeviceContext },
	m_pShader{ LoadShader(pathShader) },
	m_pInputLayout{ CreateInputLayout() },
	m_pSamplerState{ m_pShader->GetVariableBySemantic("Sampler")->AsSampler() },
	m_pWorldViewProjectionMatrix{ m_pShader->GetVariableBySemantic("WorldViewProjectionMatrix")->AsMatrix() }

{
	if (!m_pDevice) std::cout << "Effect Constructor got nullptr to device" << std::endl;
	if (!m_pDeviceContext) std::cout << "Effect Constructor got nullptr to device context" << std::endl;
	if (!m_pShader) std::cout << "Effect Constructor got incorrect effect path: " << std::endl;
	if (!m_pInputLayout) std::cout << "Effect Constructor got no input layouts" << std::endl;
	if (!m_pSamplerState) std::cout << "Effect Constructor couldn't find sampler state from effect file" << std::endl;
	if (!m_pWorldViewProjectionMatrix) std::cout << "Effect Constructor couldn't find world view projection matrix from effect file" << std::endl;
}

Material::~Material()
{
	m_pWorldViewProjectionMatrix->Release();
	m_pSamplerState->Release();
	m_pInputLayout->Release();
	m_pShader->Release();
}

ID3DX11Effect* Material::GetShader() const
{
	return m_pShader;
}

ID3D11InputLayout* Material::GetInputLayout() const
{
	return m_pInputLayout;
}

HRESULT Material::SetSamplerState(ID3D11SamplerState* pSamplerState)
{
	return m_pSamplerState->SetSampler(0, pSamplerState);
}

HRESULT Material::SetWorldViewProjectionMatrix(const Matrix& matrix)
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

	HRESULT result{ m_pWorldViewProjectionMatrix->SetMatrix(pData) };
	delete[] pData;

	return result;
}

ID3DX11Effect* Material::LoadShader(const std::wstring& path) const
{
	// Variables needed in order to create effect
	HRESULT result{};
	ID3D10Blob* pErrorBlob{};
	ID3DX11Effect* pEffect;
	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags = shaderFlags | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	// Create effect
	result = D3DX11CompileEffectFromFile(path.c_str(), nullptr, nullptr, shaderFlags, 0, m_pDevice, &pEffect, &pErrorBlob);

	// Some debuging when we failed to create the effect object
	if (FAILED(result))
	{
		if (pErrorBlob)
		{
			const char* pErrors{ static_cast<char*>(pErrorBlob->GetBufferPointer()) };
			for (int i{}; i < pErrorBlob->GetBufferSize(); ++i) std::wcout << pErrors[i];

			pErrorBlob->Release();
		}
		else
		{
			std::wcout << "Effect::LoadEffect(const std::wstring& path) failed to create effect object with path: " << path << std::endl;
		}

		pEffect = nullptr;
	}

	return pEffect;
}

ID3D11InputLayout* Material::CreateInputLayout() const
{
	D3DX11_PASS_DESC passDesc{};
	m_pShader->GetTechniqueByIndex(0)->GetPassByIndex(0)->GetDesc(&passDesc);

	ID3D11InputLayout* pInputLayout{ nullptr };
	m_pDevice->CreateInputLayout(g_VertexDescription, g_NumberOfVertexDescriptions, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &pInputLayout);

	return pInputLayout;
}
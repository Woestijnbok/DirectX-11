#include "pch.h"
#include "Texture.h"

Texture::Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& path) :
	m_pDevice{ pDevice },
	m_pDeviceContext{ pDeviceContext },
	m_pTexture{ CreateTexture(path) },
	m_pShaderResourceView{ CreateShaderResourceView() }
{
	if (!m_pDevice) std::cout << "Texture Constructor got nullptr to device" << std::endl;
	if (!m_pDeviceContext) std::cout << "Texture Constructor got nullptr to device context" << std::endl;
	if (!m_pTexture) std::cout << "Texture Constructor got invalid path" << std::endl;
	if (!m_pShaderResourceView) std::cout << "Texture Constructor got invalid ID3D11Resource* member variable" << std::endl;
}

Texture::~Texture()
{
	m_pShaderResourceView->Release();
	m_pTexture->Release();
}

ID3D11ShaderResourceView* Texture::GetShaderResourceView() const
{
	return m_pShaderResourceView;
}

ID3D11Texture2D* Texture::CreateTexture(const std::string& path) const
{
	SDL_Surface* pSurface{ IMG_Load(path.c_str()) };

	const D3D11_TEXTURE2D_DESC textureDesc
	{
		UINT(pSurface->w),								// Width
		UINT(pSurface->h),								// Height
		1,												// MipLevels
		1,												// ArraySize
		DXGI_FORMAT_R8G8B8A8_UNORM,						// Format
		DXGI_SAMPLE_DESC								// SampleDesc
		{
			1,			// Count
			0			// Quality
		},
		D3D11_USAGE_DEFAULT,							// Usage
		D3D11_BIND_SHADER_RESOURCE,						// BindFlags
		0,												// CPUAccessFlags
		0												// MiscFlags
	};

	const D3D11_SUBRESOURCE_DATA initData
	{
		pSurface->pixels,								// pSysMem
		UINT(pSurface->pitch),							// SysMemPitch
		UINT(pSurface->h * pSurface->pitch)				// SysMemSlicePitch
	};

	ID3D11Texture2D* pTexture{ nullptr };
	m_pDevice->CreateTexture2D(&textureDesc, &initData, &pTexture);

	SDL_FreeSurface(pSurface);

	return pTexture;
}

ID3D11ShaderResourceView* Texture::CreateShaderResourceView() const
{
	const D3D11_SHADER_RESOURCE_VIEW_DESC Desc
	{
		DXGI_FORMAT_R8G8B8A8_UNORM,				// Format
		D3D11_SRV_DIMENSION_TEXTURE2D,			// ViewDimensio
		0,										// Texture2D.MostDetailedMip
		1										// Texture2D.MipLevels								
	};

	ID3D11ShaderResourceView* pResourceView{ nullptr };
	m_pDevice->CreateShaderResourceView(m_pTexture, &Desc, &pResourceView);

	return pResourceView;
}
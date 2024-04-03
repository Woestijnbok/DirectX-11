#include "pch.h"
#include "Renderer.h"
#include "DataTypes.h"
#include "Phong.h"
#include "Transparent.h"
#include "Mesh.h"
#include "Camera.h"
#include "Texture.h"
#include "Utils.h"

using namespace dae;

Renderer::Renderer(SDL_Window* pWindow) :
	m_pWindow{ pWindow },
	m_Width{ 0 },
	m_Height{ 0 },
	m_pDevice{ nullptr },
	m_pDeviceContext{ nullptr },
	m_pDXGIFactory1{ nullptr },
	m_pSwapChain{ nullptr },
	m_pDepthStencilBuffer{ nullptr },
	m_pDepthStencilVieuw{ nullptr },
	m_pRenderTargetBuffer{ nullptr },
	m_pRenderTargetView{ nullptr },
	m_pPointSamplerState{ nullptr },
	m_pLinearSamplerState{ nullptr },
	m_pAnisotropicSamplerState{ nullptr },
	m_pActiveSamplerState{ nullptr },
	m_BackgroundColor{ 0.39f, 0.59f, 0.93f, 1.0f },
	m_pVehicle{ nullptr },
	m_pFire{ nullptr },
	m_pCamera{ nullptr },
	m_pPhong{ nullptr },
	m_pTransparent{ nullptr },
	m_pDiffuseMapVehicle{ nullptr },
	m_pNormalMapVehicle{ nullptr },
	m_pSpecularMapVehicle{ nullptr },
	m_pGlossinessMapVehicle{ nullptr },
	m_pDiffuseMapFire{ nullptr },
	m_RenderFire{ true }
{
	SDL_GetWindowSize(pWindow, &m_Width, &m_Height);

	if (InitializeDirectX() == S_OK) std::cout << "DirectX is initialized and ready!" << std::endl;
	else std::cout << "DirectX initialization failed!" << std::endl;

	InitializeMeshes();
	m_pCamera = new Camera{ Vector3{ 0.0f, 0.0f, -50.0f }, 45.0f, (float(m_Width) / float(m_Height)) };
	m_pPhong = new Phong{ m_pDevice, m_pDeviceContext, L"Resources/Phong.fx" };
	m_pTransparent = new Transparent{ m_pDevice, m_pDeviceContext, L"Resources/Transparent.fx" };
	m_pDiffuseMapVehicle = new Texture{ m_pDevice, m_pDeviceContext, "Resources/vehicle_diffuse.png" };
	m_pNormalMapVehicle = new Texture{ m_pDevice, m_pDeviceContext, "Resources/vehicle_normal.png" };
	m_pSpecularMapVehicle = new Texture{ m_pDevice, m_pDeviceContext, "Resources/vehicle_specular.png" };
	m_pGlossinessMapVehicle = new Texture{ m_pDevice, m_pDeviceContext, "Resources/vehicle_gloss.png" };
	m_pDiffuseMapFire = new Texture{ m_pDevice, m_pDeviceContext, "Resources/fireFX_diffuse.png" };

	m_pPhong->SetSamplerState(m_pActiveSamplerState);
	m_pPhong->SetWorldMatrix(m_pVehicle->GetWorldMatrix());
	m_pPhong->SetWorldViewProjectionMatrix(m_pVehicle->GetWorldMatrix() * m_pCamera->GetViewMatrix() * m_pCamera->GetProjectionMatrix());
	m_pPhong->SetCameraPosition(m_pCamera->GetPosition());
	m_pPhong->SetDiffuseMap(m_pDiffuseMapVehicle);
	m_pPhong->SetNormalMap(m_pNormalMapVehicle);
	m_pPhong->SetSpecularMap(m_pSpecularMapVehicle);
	m_pPhong->SetGlossinessMap(m_pGlossinessMapVehicle);

	m_pTransparent->SetSamplerState(m_pActiveSamplerState);
	m_pTransparent->SetWorldViewProjectionMatrix(m_pFire->GetWorldMatrix() * m_pCamera->GetViewMatrix() * m_pCamera->GetProjectionMatrix());
	m_pTransparent->SetDiffuseMap(m_pDiffuseMapFire);

	m_pVehicle->SetMaterial(dynamic_cast<Material*>(m_pPhong));
	m_pFire->SetMaterial(dynamic_cast<Material*>(m_pTransparent));
}

Renderer::~Renderer()
{
	m_pAnisotropicSamplerState->Release();
	m_pLinearSamplerState->Release();
	m_pPointSamplerState->Release();
	m_pRenderTargetView->Release();
	m_pRenderTargetBuffer->Release();
	m_pDepthStencilVieuw->Release();
	m_pDepthStencilBuffer->Release();
	m_pSwapChain->Release();
	m_pDXGIFactory1->Release();

	// Some extra stuff is needed for our device context
	if (m_pDeviceContext)
	{
		m_pDeviceContext->ClearState();
		m_pDeviceContext->Flush();
		m_pDeviceContext->Release();
	}

	m_pDevice->Release();

	// Delete our objects on the heap
	delete m_pVehicle;
	delete m_pFire;
	delete m_pCamera;
	delete m_pPhong;
	delete m_pTransparent;
	delete m_pDiffuseMapVehicle;
	delete m_pNormalMapVehicle;
	delete m_pSpecularMapVehicle;
	delete m_pGlossinessMapVehicle;
	delete m_pDiffuseMapFire;
}

void Renderer::Update(float deltaSeconds)
{
	m_pCamera->Update(deltaSeconds);
	m_pVehicle->Update(deltaSeconds);
	m_pFire->Update(deltaSeconds);
}

void Renderer::Render()
{
	// Clear the render target and depth stencil vieuw
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, m_BackgroundColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilVieuw, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Update some effect variables
	m_pPhong->SetWorldMatrix(m_pVehicle->GetWorldMatrix());
	m_pPhong->SetWorldViewProjectionMatrix(m_pVehicle->GetWorldMatrix() * m_pCamera->GetViewMatrix() * m_pCamera->GetProjectionMatrix());
	m_pPhong->SetCameraPosition(m_pCamera->GetPosition());

	m_pTransparent->SetWorldViewProjectionMatrix(m_pFire->GetWorldMatrix() * m_pCamera->GetViewMatrix() * m_pCamera->GetProjectionMatrix());

	// Render mesh
	m_pVehicle->Render();
	if (m_RenderFire) m_pFire->Render();

	// present the backbuffer
	m_pSwapChain->Present(0, 0);
}

HRESULT Renderer::InitializeDirectX()
{
	HRESULT result{};

	result = CreateDeviceAndDeviceContext();
	if (FAILED(result)) return result;

	result = CreateDXGIFactory();
	if (FAILED(result)) return result;

	result = CreateSwapChain();
	if (FAILED(result)) return result;

	result = CreateStencilBuffer();
	if (FAILED(result)) return result;

	result = CreateStencilView();
	if (FAILED(result)) return result;

	result = CreateTargetBuffer();
	if (FAILED(result)) return result;

	result = CreateTargetView();
	if (FAILED(result)) return result;

	result = CreateSamplerStates();
	if (FAILED(result)) return result;

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilVieuw);

	SetViewPort();

	return result;
}

HRESULT Renderer::CreateDeviceAndDeviceContext()
{
	const D3D_FEATURE_LEVEL featureLevel{ D3D_FEATURE_LEVEL_11_1 };

	// Set flags
	uint32_t Flags{ 1 };

	// Set extra flags when in debug
#if defined(DEBUG) || defined(_DEBUG)
	Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	return D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, Flags, &featureLevel, 1, D3D11_SDK_VERSION, &m_pDevice, nullptr, &m_pDeviceContext);
}

HRESULT Renderer::CreateDXGIFactory()
{
	return CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&m_pDXGIFactory1));
}

HRESULT Renderer::CreateSwapChain()
{
	// Get the window hande from SDL
	SDL_SysWMinfo sysWMInfo{};
	SDL_GetWindowWMInfo(m_pWindow, &sysWMInfo);

	DXGI_SWAP_CHAIN_DESC swapChainDesc
	{
		DXGI_MODE_DESC															// BufferDesc		
		{
			UINT(m_Width),								// Width
			UINT(m_Height),								// Height
			DXGI_RATIONAL								// RefreshRate
			{
				1,			// Numerator
				60			// Denominator
			},
			DXGI_FORMAT_R8G8B8A8_UNORM,					// Format
			DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,		// ScanlineOrdering
			DXGI_MODE_SCALING_UNSPECIFIED				// Scaling
		},
		DXGI_SAMPLE_DESC														// SampleDesc
		{
			1,											// Count
			0											// Quality
		},
		DXGI_USAGE_RENDER_TARGET_OUTPUT,										// BufferUsage
		1,																		// BufferCount
		sysWMInfo.info.win.window,												// OutputWindow
		true,																	// Windowed
		DXGI_SWAP_EFFECT_DISCARD,												// SwapEffect
		0																		// Flags
	};

	// Create the swap chain
	return m_pDXGIFactory1->CreateSwapChain(m_pDevice, &swapChainDesc, &m_pSwapChain);
}

HRESULT Renderer::CreateStencilBuffer()
{
	const D3D11_TEXTURE2D_DESC depthStencilDesc
	{
		UINT(m_Width),									// Width
		UINT(m_Height),									// Height
		1,												// MipLevels
		1,												// ArraySize
		DXGI_FORMAT_D24_UNORM_S8_UINT,					// Format
		DXGI_SAMPLE_DESC								// SampleDesc
		{
			1,			// Count
			0			// Quality
		},
		D3D11_USAGE_DEFAULT,							// Usage
		D3D11_BIND_DEPTH_STENCIL,						// BindFlags
		0,												// CPUAccessFlags
		0												// MiscFlags
	};

	return m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencilBuffer);
}

HRESULT Renderer::CreateStencilView()
{
	const D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc
	{
		DXGI_FORMAT_D24_UNORM_S8_UINT,				// Format
		D3D11_DSV_DIMENSION_TEXTURE2D,				// ViewDimension
		0,											// Flags
		0											// Texture2D.MipSlice
	};

	return m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilVieuw);
}

HRESULT Renderer::CreateTargetBuffer()
{
	return m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_pRenderTargetBuffer));
}

HRESULT Renderer::CreateTargetView()
{
	return m_pDevice->CreateRenderTargetView(m_pRenderTargetBuffer, nullptr, &m_pRenderTargetView);
}

void Renderer::SetViewPort()
{
	const D3D11_VIEWPORT viewport
	{
		0.0f,					// TopLeftX
		0.0f,					// TopLeftY
		float(m_Width),			// Height
		float(m_Height),		// Width
		0.0f,					// MinDepth
		1.0f					// MaxDepth
	};

	m_pDeviceContext->RSSetViewports(1, &viewport);
}

void Renderer::InitializeMeshes()
{
	std::vector<Vertex> verticesVehicle{};
	std::vector<int> indicesVehicle{};
	Utils::ParseOBJ("Resources/vehicle.obj", verticesVehicle, indicesVehicle);

	m_pVehicle = new Mesh
	{
		m_pDevice,
		m_pDeviceContext,
		verticesVehicle,
		indicesVehicle,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	};

	std::vector<Vertex> verticesFire{};
	std::vector<int> indicesFire{};
	Utils::ParseOBJ("Resources/fireFX.obj", verticesFire, indicesFire);

	m_pFire = new Mesh
	{
		m_pDevice,
		m_pDeviceContext,
		verticesFire,
		indicesFire,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	};
}

HRESULT Renderer::CreateSamplerStates()
{
	D3D11_SAMPLER_DESC samplerDesc
	{
		D3D11_FILTER_MIN_MAG_MIP_POINT,				// Filter
		D3D11_TEXTURE_ADDRESS_WRAP,					// AddressU
		D3D11_TEXTURE_ADDRESS_WRAP,					// AddressV
		D3D11_TEXTURE_ADDRESS_WRAP,					// AddressW;
	};

	// Point sampler state
	HRESULT result = m_pDevice->CreateSamplerState(&samplerDesc, &m_pPointSamplerState);
	if (FAILED(result)) return result;

	// Linear sampler state
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	result = m_pDevice->CreateSamplerState(&samplerDesc, &m_pLinearSamplerState);
	if (FAILED(result)) return result;

	// Anisotropic sampler state
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	result = m_pDevice->CreateSamplerState(&samplerDesc, &m_pAnisotropicSamplerState);

	m_pActiveSamplerState = m_pPointSamplerState;

	return result;
}

void Renderer::SwitchSampleFilter()
{
	if (m_pActiveSamplerState == m_pPointSamplerState)
	{
		m_pActiveSamplerState = m_pLinearSamplerState;
		std::cout << "Current sample filter: Linear" << std::endl;
	}
	else if (m_pActiveSamplerState == m_pLinearSamplerState)
	{
		m_pActiveSamplerState = m_pAnisotropicSamplerState;
		std::cout << "Current sample filter: Anisotropic" << std::endl;
	}
	else
	{
		m_pActiveSamplerState = m_pPointSamplerState;
		std::cout << "Current sample filter: Point" << std::endl;
	}

	m_pPhong->SetSamplerState(m_pActiveSamplerState);
	m_pTransparent->SetSamplerState(m_pActiveSamplerState);
}

void Renderer::ToggleRotate()
{
	m_pVehicle->ToggleRotate();
	m_pFire->ToggleRotate();
}

void Renderer::ToggleNormalMap()
{
	m_pPhong->ToggleUseNormalMap();
}

void Renderer::ToggleFire()
{
	m_RenderFire = !m_RenderFire;
}
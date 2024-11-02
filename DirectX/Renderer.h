#ifndef RENDERER
#define RENDERER

struct SDL_Window;
class Mesh;
class Camera;
class Phong;
class Texture;
class Camera;
class Transparent;

class Renderer final
{
public:
	Renderer(SDL_Window* pWindow);
	~Renderer();

	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) noexcept = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) noexcept = delete;

	void Update(float deltaSeconds);
	void Render();
	void SwitchSampleFilter();
	void ToggleRotate();
	void ToggleNormalMap();
	void ToggleFire();

private:
	SDL_Window* m_pWindow;
	int m_Width;
	int m_Height;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGIFactory1* m_pDXGIFactory1;
	IDXGISwapChain* m_pSwapChain;
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilView* m_pDepthStencilVieuw;
	ID3D11Resource* m_pRenderTargetBuffer;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11SamplerState* m_pPointSamplerState;
	ID3D11SamplerState* m_pLinearSamplerState;
	ID3D11SamplerState* m_pAnisotropicSamplerState;
	ID3D11SamplerState* m_pActiveSamplerState;
	const float m_BackgroundColor[4];
	Mesh* m_pVehicle;
	Mesh* m_pFire;
	Camera* m_pCamera;
	Phong* m_pPhong;
	Transparent* m_pTransparent;
	Texture* m_pDiffuseMapVehicle;
	Texture* m_pNormalMapVehicle;
	Texture* m_pSpecularMapVehicle;
	Texture* m_pGlossinessMapVehicle;
	Texture* m_pDiffuseMapFire;
	bool m_RenderFire;

	HRESULT InitializeDirectX();
	HRESULT CreateDeviceAndDeviceContext();
	HRESULT CreateDXGIFactory();
	HRESULT CreateSwapChain();
	HRESULT CreateStencilBuffer();
	HRESULT CreateStencilView();
	HRESULT CreateTargetBuffer();
	HRESULT CreateTargetView();
	HRESULT CreateSamplerStates();
	void SetViewPort();
	void InitializeMeshes();
};

#endif
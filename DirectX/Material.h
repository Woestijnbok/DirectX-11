#ifndef MATERIAL
#define MATERIAL

class Texture;

class Material
{
public:
	Material(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::wstring& pathShader);
	virtual ~Material();

	Material(const Material&) = delete;
	Material(Material&&) noexcept = delete;
	Material& operator=(const Material&) = delete;
	Material& operator=(Material&&) noexcept = delete;

	ID3DX11Effect* GetShader() const;
	ID3D11InputLayout* GetInputLayout() const;
	HRESULT SetSamplerState(ID3D11SamplerState* pSamplerState);
	HRESULT SetWorldViewProjectionMatrix(const dae::Matrix& matrix);

protected:
	ID3D11Device* const m_pDevice;
	ID3D11DeviceContext* const m_pDeviceContext;
	ID3DX11Effect* const m_pShader;
	ID3D11InputLayout* const m_pInputLayout;
	ID3DX11EffectSamplerVariable* const m_pSamplerState;
	ID3DX11EffectMatrixVariable* const m_pWorldViewProjectionMatrix;

	ID3DX11Effect* LoadShader(const std::wstring& path) const;
	ID3D11InputLayout* CreateInputLayout() const;
};

#endif
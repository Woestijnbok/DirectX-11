#ifndef TEXTURE
#define TEXTURE

class Texture final
{
public:
	Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,const std::string& path);
	~Texture();

	Texture(const Texture&) = delete;
	Texture(Texture&&) noexcept = delete;
	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&&) noexcept = delete;

	ID3D11ShaderResourceView* GetShaderResourceView() const;

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	ID3D11Texture2D* const m_pTexture;
	ID3D11ShaderResourceView* const m_pShaderResourceView;

	ID3D11Texture2D* CreateTexture(const std::string& path) const;
	ID3D11ShaderResourceView* CreateShaderResourceView() const;
};

#endif
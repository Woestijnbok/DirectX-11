#ifndef MATERIAL_TRANSPARENT
#define MATERIAL_TRANSPARENT

#include "Material.h"

class Texture;

class Transparent final : public Material
{
public:
	Transparent(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::wstring& pathShader);
	virtual ~Transparent();

	Transparent(const Transparent&) = delete;
	Transparent(Transparent&&) noexcept = delete;
	Transparent& operator=(const Transparent&) = delete;
	Transparent& operator=(Transparent&&) noexcept = delete;

	void SetDiffuseMap(Texture* pTexture);

private:
	ID3DX11EffectShaderResourceVariable* const m_pDiffuseMap;
};

#endif
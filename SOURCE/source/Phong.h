#ifndef MATERIAL_PHONG
#define MATERIAL_PHONG

#include "Material.h"

class Texture;

class Phong final : public Material
{
public:
	Phong(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::wstring& pathShader);
	virtual ~Phong();

	Phong(const Phong&) = delete;
	Phong(Phong&&) noexcept = delete;
	Phong& operator=(const Phong&) = delete;
	Phong& operator=(Phong&&) noexcept = delete;

	void SetWorldMatrix(const dae::Matrix& matrix);
	void SetCameraPosition(const dae::Vector3 position);
	void SetDiffuseMap(Texture* pTexture);
	void SetNormalMap(Texture* pTexture);
	void SetSpecularMap(Texture* pTexture);
	void SetGlossinessMap(Texture* pTexture);
	void ToggleUseNormalMap();

private:
	ID3DX11EffectMatrixVariable* const m_pWorldMatrix;
	ID3DX11EffectVectorVariable* const m_pCameraPosition;
	ID3DX11EffectShaderResourceVariable* const m_pDiffuseMap;
	ID3DX11EffectShaderResourceVariable* const m_pNormalMap;
	ID3DX11EffectShaderResourceVariable* const m_pSpecularMap;
	ID3DX11EffectShaderResourceVariable* const m_pGlossinessMap;
	ID3DX11EffectScalarVariable* const m_pUseNormalMap;
};

#endif
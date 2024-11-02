#ifndef MESH
#define MESH

#include "DataTypes.h"

class Material;

class Mesh
{
public:
	Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::vector<Vertex>& vertices, const std::vector<int> indices, D3D_PRIMITIVE_TOPOLOGY topology);
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh(Mesh&&) noexcept = delete;
	Mesh& operator=(const Mesh&) = delete;
	Mesh& operator=(Mesh&&) noexcept = delete;

	const dae::Matrix& GetWorldMatrix() const;
	void Update(float deltaSeconds);
	void Render();
	void SetMaterial(Material* pMaterial);
	void ToggleRotate();

private:
	static const UINT m_StridesVertexBuffers;
	static const UINT m_OffsetsVertexBuffers;

	ID3D11Device* const m_pDevice;
	ID3D11DeviceContext* const m_pDeviceContext;
	Material* m_pMaterial;
	const std::vector<Vertex> m_Vertices;
	const std::vector<int> m_Indices;
	ID3D11Buffer* const m_pVertexBuffer;
	ID3D11Buffer* const m_pIndexBuffer;
	dae::Matrix m_WorldMatrix;
	const D3D_PRIMITIVE_TOPOLOGY m_Topology;
	bool m_CanRotate;
	float m_RotateTimer;

	ID3D11Buffer* const CreateVertexBuffer() const;
	ID3D11Buffer* const CreateIndexBuffer() const;
};

#endif
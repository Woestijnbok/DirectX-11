#include "pch.h"
#include "Mesh.h"
#include "Material.h"

using namespace dae;

const UINT Mesh::m_StridesVertexBuffers{ sizeof(Vertex) };		// Jump size(s) to go to the next element in the vertex buffer(s)
const UINT Mesh::m_OffsetsVertexBuffers{ 0 };					// Offset(s) to skip a set amount of elements in the vertex buffer(s)

Mesh::Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::vector<Vertex>& vertices, const std::vector<int> indices, D3D_PRIMITIVE_TOPOLOGY topology) :
	m_pDevice{ pDevice },
	m_pDeviceContext{ pDeviceContext },
	m_pMaterial{ nullptr },
	m_Vertices{ vertices },
	m_Indices{ indices },
	m_pVertexBuffer{ CreateVertexBuffer() },
	m_pIndexBuffer{ CreateIndexBuffer() },
	m_WorldMatrix{},
	m_Topology{ topology },
	m_CanRotate{ true },
	m_RotateTimer{ 0.0f }
{
	if (!m_pDevice) std::cout << "Mesh Constructor got nullptr to device" << std::endl;
	if (!m_pDeviceContext) std::cout << "Mesh Constructor got nullptr to device context" << std::endl;
	if (m_Vertices.size() == 0) std::cout << "Mesh Constructor got no vertices" << std::endl;
	if (m_Indices.size() == 0) std::cout << "Mesh Constructor got no indices" << std::endl;
	if (!m_pVertexBuffer) std::cout << "Mesh Constructor couldn't create vertex buffer" << std::endl;
	if (!m_pIndexBuffer) std::cout << "Mesh Constructor couldn't create index buffer" << std::endl;
}

Mesh::~Mesh()
{
	m_pIndexBuffer->Release();
	m_pVertexBuffer->Release();
}

const Matrix& Mesh::GetWorldMatrix() const
{
	return m_WorldMatrix;
}

void Mesh::Update(float deltaSeconds)
{
	if (m_CanRotate)
	{
		m_RotateTimer += deltaSeconds;
		m_WorldMatrix = Matrix::CreateRotationY(m_RotateTimer);
	}
}

void Mesh::Render()
{
	// Preparing the input assembler
	m_pDeviceContext->IASetInputLayout(m_pMaterial->GetInputLayout());
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_StridesVertexBuffers, &m_OffsetsVertexBuffers);
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_Topology);
	
	// Draw, we only have one technique and pass in our shader
	m_pMaterial->GetShader()->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, m_pDeviceContext);
	m_pDeviceContext->DrawIndexed(UINT(m_Indices.size()), 0, 0);
}

void Mesh::SetMaterial(Material* pMaterial)
{
	m_pMaterial = pMaterial;
}

void Mesh::ToggleRotate()
{
	m_CanRotate = !m_CanRotate;
}

ID3D11Buffer* const Mesh::CreateVertexBuffer() const
{
	const D3D11_BUFFER_DESC vertexBufferDesc
	{
		UINT(sizeof(Vertex) * m_Vertices.size()),			// ByteWidth
		D3D11_USAGE_IMMUTABLE,								// Usage
		D3D11_BIND_VERTEX_BUFFER,							// BindFlags
		0,													// CPUAccessFlags
		0,													// MiscFlags
		0													// StructureByteStride
	};

	const D3D11_SUBRESOURCE_DATA vertexInitData
	{
		m_Vertices.data(),				// pSysMem
		0,								// SysMemPitch
		0								// SysMemSlicePitch
	};

	ID3D11Buffer* pVertexBuffer{ nullptr };
	m_pDevice->CreateBuffer(&vertexBufferDesc, &vertexInitData, &pVertexBuffer);

	return pVertexBuffer;
}

ID3D11Buffer* const Mesh::CreateIndexBuffer() const
{
	const D3D11_BUFFER_DESC IndexBufferDesc
	{
		UINT(sizeof(int) * m_Indices.size()),				// ByteWidth
		D3D11_USAGE_IMMUTABLE,								// Usage
		D3D11_BIND_INDEX_BUFFER,							// BindFlags
		0,													// CPUAccessFlags
		0,													// MiscFlags
		0													// StructureByteStride
	};

	const D3D11_SUBRESOURCE_DATA IndexInitData
	{
		m_Indices.data(),				// pSysMem
		0,								// SysMemPitch
		0								// SysMemSlicePitch
	};

	ID3D11Buffer* pIndexBuffer{ nullptr };
	m_pDevice->CreateBuffer(&IndexBufferDesc, &IndexInitData, &pIndexBuffer);

	return pIndexBuffer;
}
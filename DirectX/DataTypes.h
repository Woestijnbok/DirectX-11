#ifndef DATATYPES
#define DATATYPES

constexpr UINT g_NumberOfVertexDescriptions{ 5 };

constexpr D3D11_INPUT_ELEMENT_DESC g_VertexDescription[g_NumberOfVertexDescriptions]
{
	D3D11_INPUT_ELEMENT_DESC							// Vertex layout
	{
		"POSITION",										// SemanticName
		0,												// SemanticIndex
		DXGI_FORMAT_R32G32B32_FLOAT,					// Format
		0,												// InputSlot
		0,												// AlignedByteOffset
		D3D11_INPUT_PER_VERTEX_DATA,					// InputSlotClass
		0												// InstanceDataStepRate
	},
	D3D11_INPUT_ELEMENT_DESC
	{
		"COLOR",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		12,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},
	D3D11_INPUT_ELEMENT_DESC
	{
		"TEXCOORD",
		0,
		DXGI_FORMAT_R32G32_FLOAT,
		0,
		24,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},
	D3D11_INPUT_ELEMENT_DESC
	{
		"NORMAL",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		32,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	},
	D3D11_INPUT_ELEMENT_DESC
	{
		"TANGENT",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		44,
		D3D11_INPUT_PER_VERTEX_DATA,
		0
	}
};

struct Vertex
{
	dae::Vector3 position;
	dae::ColorRGB color;
	dae::Vector2 uv;
	dae::Vector3 normal;
	dae::Vector3 tangent;
};

#endif
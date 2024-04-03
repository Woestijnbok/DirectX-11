struct VS_INPUT
{
    float3 position : POSITION;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

const RasterizerState g_RasterizerState
{
    CullMode = none;
};

const BlendState g_BlendState
{
    BlendEnable[0] = true;
    SrcBlend[0] = src_alpha;
    Destblend[0] = inv_src_alpha;
    SrcBlendAlpha[0] = zero;
    RenderTargetWriteMask[0] = 0x0F;
};

const DepthStencilState g_DepthStencilState
{
    DepthWriteMask = zero;
};

SamplerState g_TransparentSampler : Sampler;
float4x4 g_WorldVieuwProjectionMatrix : WorldViewProjectionMatrix;
Texture2D<float4> g_DiffuseMap : DiffuseMap;

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.position = mul(float4(input.position, 1.0f), g_WorldVieuwProjectionMatrix);
    output.uv = input.uv;
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return g_DiffuseMap.Sample(g_TransparentSampler, input.uv);
}

technique11 DefaultTechnique
{
    pass P0
    {
        SetRasterizerState(g_RasterizerState);
        SetDepthStencilState(g_DepthStencilState, 0);
        SetBlendState(g_BlendState, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader( CompileShader(ps_5_0, PS()));
    }
}
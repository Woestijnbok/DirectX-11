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
    float3 viewDirection : VIEWDIRECTION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

const RasterizerState g_RasterizerState{};
const BlendState g_BlendState{};
const DepthStencilState g_DepthStencilState{};

// Variables linked with cpp
SamplerState g_PhongSampler : Sampler;
float4x4 g_WorldMatrix : WorldMatrix;
float4x4 g_WorldVieuwProjectionMatrix : WorldViewProjectionMatrix;
float3 g_CameraPosition : CameraPosition;
Texture2D<float3> g_DiffuseMap : DiffuseMap;
Texture2D<float3> g_NormalMap : NormalMap;
Texture2D<float> g_SpecularMap : SpecularMap;
Texture2D<float> g_GlossinessMap : GlossinessMap;
bool g_UseNormalMap : UseNormalMap = true;

// Constants
const float g_PI = 3.14f;
const float3 g_LightDirection = float3(0.577f, -0.577f, 0.577f);
const float3 g_AmbientColor = float3(0.03f, 0.03f, 0.03f);
const float g_LightIntensity = 7.0f;
const float g_Shininess = 25.0f;

// Vertex Shader
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.position = mul(float4(input.position, 1.0f), g_WorldVieuwProjectionMatrix);
    output.uv = input.uv;
    output.viewDirection = normalize(mul(input.position.xyz, (float3x3) g_WorldMatrix) - g_CameraPosition);
    output.normal = mul(input.normal, (float3x3) g_WorldMatrix);
    output.tangent = mul(input.tangent, (float3x3) g_WorldMatrix);
    
    return output;
}

float3 CalculateNormal(VS_OUTPUT input)
{
    float3 normal;
    
    if(g_UseNormalMap)
    {
        float3 biNormal = cross(input.normal, input.tangent);
    
        float3x3 tangentSpaceMatrix =
        {
            input.tangent.x, input.tangent.y, input.tangent.z,
            biNormal.x, biNormal.y, biNormal.z,
            input.normal.x, input.normal.y, input.normal.z
        };
     
        float3 SampledNormal = g_NormalMap.Sample(g_PhongSampler, input.uv);
        normal = normalize(mul(SampledNormal, tangentSpaceMatrix));
    }
    else
    {
        normal = normalize(input.normal);
    }
      
    return normal;
}

float Phong(float ks, float exp, float3 lightDirection, float3 viewDirection, float3 normal)
{
    const float3 reflection = reflect(lightDirection, normal);
    const float cosine = saturate(dot(reflection, viewDirection));
    const float3 output = 1.0f * ks * pow(cosine, exp);
    
    return output;
}

// Pixel shader
float4 PS(VS_OUTPUT input) : SV_TARGET
{
    const float3 normal = CalculateNormal(input);
    const float specular = g_SpecularMap.Sample(g_PhongSampler, input.uv);
    const float phongExponent = g_GlossinessMap.Sample(g_PhongSampler, input.uv);
    const float phong = Phong(specular, phongExponent * g_Shininess, g_LightDirection, input.viewDirection, normal);
    const float3 diffuseColor = g_DiffuseMap.Sample(g_PhongSampler, input.uv);
    const float3 specularColor = float3(phong, phong, phong);
    
    const float3 color = diffuseColor + specularColor + g_AmbientColor;
    return float4(color, 1.0f);
}

// Technique
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
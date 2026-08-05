cbuffer ConstantData : register(b0)
{
    row_major matrix world;
    row_major matrix view;
    row_major matrix proj;
};

struct VSInput
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD0;
    float4 color : COLOR0;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float4 color : COLOR0;
};

Texture2D g_Texture : register(t0);
SamplerState g_Sampler : register(s0);

PSInput VSMain(VSInput input)
{
    PSInput output;
    float4 pos = float4(input.position, 1.0f);
    pos = mul(pos, world);
    pos = mul(pos, view);
    pos = mul(pos, proj);
    
    output.position = pos;
    output.texcoord = input.texcoord;
    output.color = input.color;
    return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    // Sample texture and blend with vertex/tint color
    float4 texColor = g_Texture.Sample(g_Sampler, input.texcoord);
    return texColor * input.color;
}

struct VSInput
{
    float3 position : POSITION0;
    float4 color : COLOR0;
};

struct VSOutput
{
    float4 position : SV_Position; //The position of the vertex in clip space
    float4 color : COLOR0;
};
VSOutput VSMain(VSInput input)
{
    VSOutput output;
    output.position = float4(input.position, 1.0);
    output.color = input.color;
    return output;
}
float4 PSMain(VSOutput input) : SV_TARGET
{
    //return float4(0.89, 0.44, 0.47, 1.0); //change this to change the color of the triangle. 
    return input.color;
}
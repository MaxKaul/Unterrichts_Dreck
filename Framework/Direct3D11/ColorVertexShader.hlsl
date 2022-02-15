cbuffer MatrixBuffer
{
    float4x4 worldViewProjectionMatrix;
};

struct VertexInput
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VertexOutput main( VertexInput INPUT )
{
    VertexOutput OUTPUT;
    
    OUTPUT.position = mul(float4(INPUT.position, 1.0f), worldViewProjectionMatrix);
    OUTPUT.color = INPUT.color;
    
    return OUTPUT;
}
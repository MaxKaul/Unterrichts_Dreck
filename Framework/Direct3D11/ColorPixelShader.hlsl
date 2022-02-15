struct PixelInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 main(PixelInput INPUT) : SV_TARGET
{
	//return float4(1.0f, 1.0f, 1.0f, 1.0f); // solid color value for all pixels
    return INPUT.color; // solid color value interpolated
    //return 1 - INPUT.color; // negative
    
    //INPUT.color.r = 0.0f;
    //return INPUT.color; // without red channel
}
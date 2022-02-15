Shader "Custom Shader/Directional Lighting - Lambert"
{
    Properties
    {
        _MainTexture ("Main Texture", 2D) = "grey" {}
        _MainColor ("Main Color", Color) = (1.0, 1.0, 1.0, 1.0)
    }

    SubShader
    {
        Pass
        {
            Tags
			{
				"LightMode" = "ForwardBase"
			}
			
			HLSLPROGRAM
            #include "UnityCG.cginc"
            #include "UnityLightingCommon.cginc"

            #pragma vertex VS
            #pragma fragment PS

            UNITY_DECLARE_TEX2D(_MainTexture);

            uniform float4 _MainTexture_ST;
            uniform fixed4 _MainColor;

            struct VertexInput
            {
                float3 position : POSITION;
                float3 normal : NORMAL;
                float2 uv : TEXCOORD;
            };

            struct VertexOutput
            {
                float4 position : SV_POSITION;
                half3 normal : NORMAL;
                float2 uv : TEXCOORD;
            };

            VertexOutput VS(VertexInput INPUT)
            {
                VertexOutput OUTPUT;

                OUTPUT.position = UnityObjectToClipPos(INPUT.position);
                // OUTPUT.normal = normalize(mul((float3x3)UNITY_MATRIX_M, INPUT.normal));
                OUTPUT.normal = UnityObjectToWorldNormal(INPUT.normal);
                OUTPUT.uv = TRANSFORM_TEX(INPUT.uv, _MainTexture);

                return OUTPUT;
            }

            fixed4 PS(VertexOutput INPUT) : SV_TARGET
            {
                fixed4 textureColor = UNITY_SAMPLE_TEX2D(_MainTexture, INPUT.uv) * _MainColor;
                fixed4 ambientColor = UNITY_LIGHTMODEL_AMBIENT;
                fixed4 diffuseColor = 0;

                // light data
                half3 normal = normalize(INPUT.normal);
                half3 light = _WorldSpaceLightPos0.xyz; // is already inverted normalized light vector

                // diffuse color
                float diffuse = max(dot(normal, light), 0);
                diffuseColor = _LightColor0 * diffuse;

                return textureColor * saturate(ambientColor + diffuseColor);
            }
            ENDHLSL
        }
    }
}
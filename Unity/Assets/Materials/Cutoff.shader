Shader "Custom Shader/Cutoff"
{
    Properties
    {
        _MainTexture ("Main Texture", 2D) = "grey" {}
        _MainColor ("Main Color", Color) = (1.0, 1.0, 1.0, 1.0)
        _Cutoff ("Cutoff", Range(-0.51, 0.51)) = 0.51
    }

    SubShader
    {
        Tags
        {
            "Queue" = "AlphaTest"
        }

        Pass
        {
            Cull Off

            HLSLPROGRAM
            #include "UnityCG.cginc"

            #pragma vertex VS
            #pragma fragment PS

            UNITY_DECLARE_TEX2D(_MainTexture);

            uniform float4 _MainTexture_ST;
            uniform fixed4 _MainColor;
            uniform half _Cutoff;

            struct VertexInput
            {
                float3 position : POSITION;
                float2 uv : TEXCOORD;
            };

            struct VertexOutput
            {
                float4 position : SV_POSITION;
                float3 positionLocal : POSITION1;
                float2 uv : TEXCOORD;
            };

            VertexOutput VS(VertexInput INPUT)
            {
                VertexOutput OUTPUT;

                OUTPUT.position = UnityObjectToClipPos(INPUT.position);
                OUTPUT.positionLocal = INPUT.position;
                OUTPUT.uv = TRANSFORM_TEX(INPUT.uv, _MainTexture);

                return OUTPUT;
            }

            fixed4 PS(VertexOutput INPUT) : SV_TARGET
            {
                clip(_Cutoff - INPUT.positionLocal.y);

                return UNITY_SAMPLE_TEX2D(_MainTexture, INPUT.uv) * _MainColor;
            }
            ENDHLSL
        }
    }
}
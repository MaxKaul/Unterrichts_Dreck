Shader "Custom Shader/Alpha Blending - Two Passes"
{
    Properties
    {
        _MainTexture ("Main Texture", 2D) = "grey" {}
        _MainColor ("Main Color", Color) = (1.0, 1.0, 1.0, 1.0)
    }

    SubShader
    {
        Tags
        {
            "Queue" = "Transparent"
        }

        Blend SrcAlpha OneMinusSrcAlpha // SrcColor * SrcColor.a + DstColor * (1 - SrcColor.a)

        HLSLINCLUDE
        #include "UnityCG.cginc"

        UNITY_DECLARE_TEX2D(_MainTexture);

        uniform float4 _MainTexture_ST;
        uniform fixed4 _MainColor;
        uniform half _Cutout;

        struct VertexInput
        {
            float3 position : POSITION;
            float2 uv : TEXCOORD;
        };

        struct VertexOutput
        {
            float4 position : SV_POSITION;
            float2 uv : TEXCOORD;
        };

        VertexOutput VS(VertexInput INPUT)
        {
            VertexOutput OUTPUT;

            OUTPUT.position = UnityObjectToClipPos(INPUT.position);
            OUTPUT.uv = TRANSFORM_TEX(INPUT.uv, _MainTexture);

            return OUTPUT;
        }

        fixed4 PS(VertexOutput INPUT) : SV_TARGET
        {
            return UNITY_SAMPLE_TEX2D(_MainTexture, INPUT.uv) * _MainColor;
        }
        ENDHLSL

        Pass
        {
            Cull Front

            HLSLPROGRAM
            #pragma vertex VS
            #pragma fragment PS
            ENDHLSL
        }

        Pass
        {
            Cull Back

            HLSLPROGRAM
            #pragma vertex VS
            #pragma fragment PS
            ENDHLSL
        }
    }
}
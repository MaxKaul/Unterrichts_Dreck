Shader "Custom Shader/Lighting - light vector in vertex shader"
{
    Properties
    {
        _MainTexture ("Main Texture", 2D) = "grey" {}
        _MainColor ("Main Color", Color) = (1.0, 1.0, 1.0, 1.0)
        _SpecularColor ("Specular Color", Color) = (1.0, 1.0, 1.0, 1.0)
        _SpecularPower ("Specular Power", float) = 64.0
        _Glossy ("Glossiness", Range(0.01, 1.0)) = 1.0
    }

    SubShader
    {
        HLSLINCLUDE
        #include "UnityCG.cginc"
        #include "UnityLightingCommon.cginc"

        UNITY_DECLARE_TEX2D(_MainTexture);

        uniform float4 _MainTexture_ST;
        uniform fixed4 _MainColor;
        uniform fixed4 _SpecularColor;
        uniform fixed _SpecularPower;
        uniform fixed _Glossy;

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
            float2 uv : TEXCOORD0;
            half3 viewDirection : TEXCOORD1;
            half3 lightDirection : TEXCOORD2;
            float attenuation : TEXCOORD3;
        };

        VertexOutput VS(VertexInput INPUT)
        {
            VertexOutput OUTPUT;

            OUTPUT.position = UnityObjectToClipPos(INPUT.position);
            float3 positionWorld = mul(UNITY_MATRIX_M, float4(INPUT.position, 1.0)).xyz;
            OUTPUT.normal = UnityObjectToWorldNormal(INPUT.normal);
            OUTPUT.uv = TRANSFORM_TEX(INPUT.uv, _MainTexture);
            OUTPUT.viewDirection = normalize(_WorldSpaceCameraPos - positionWorld);

            OUTPUT.lightDirection = _WorldSpaceLightPos0.xyz - positionWorld * _WorldSpaceLightPos0.w;
            float lightLength = length(OUTPUT.lightDirection);
            OUTPUT.lightDirection /= lightLength;
            OUTPUT.attenuation = 1 / (1.0 + (0.2 * lightLength + 0.1 * lightLength * lightLength) * _WorldSpaceLightPos0
                .
                w);

            return OUTPUT;
        }

        fixed4 calculateFinalColor(VertexOutput INPUT, fixed4 ambientColor) : SV_TARGET
        {
            fixed4 textureColor = UNITY_SAMPLE_TEX2D(_MainTexture, INPUT.uv) * _MainColor;
            fixed4 diffuseColor = 0;
            fixed4 specularColor = 0;

            // light data
            half3 normal = normalize(INPUT.normal);
            half3 light = normalize(INPUT.lightDirection);
            half3 viewDirection = normalize(INPUT.viewDirection);

            // diffuse color
            float diffuse = max(dot(normal, light), 0);
            diffuseColor = _LightColor0 * (diffuse * INPUT.attenuation);

            // specular color
            half3 reflectVector = 2 * diffuse * normal - light;
            float specular = pow(max(dot(reflectVector, viewDirection), 0), _SpecularPower) * _Glossy;
            specularColor = _SpecularColor * (specular * INPUT.attenuation);

            return saturate(textureColor * saturate(ambientColor + diffuseColor) + specularColor);
        }
        ENDHLSL

        Pass
        {
            Tags
            {
                "LightMode" = "ForwardBase"
            }

            HLSLPROGRAM
            #pragma vertex VS
            #pragma fragment PS

            fixed4 PS(VertexOutput INPUT) : SV_TARGET
            {
                return calculateFinalColor(INPUT, UNITY_LIGHTMODEL_AMBIENT);
            }
            ENDHLSL
        }

        Pass
        {
            Tags
            {
                "LightMode" = "ForwardAdd"
            }

            Blend One One // SrcColor + DstColor
            ZWrite Off

            HLSLPROGRAM
            #pragma vertex VS
            #pragma fragment PS

            fixed4 PS(VertexOutput INPUT) : SV_TARGET
            {
                return calculateFinalColor(INPUT, 0);
            }
            ENDHLSL
        }
    }
}
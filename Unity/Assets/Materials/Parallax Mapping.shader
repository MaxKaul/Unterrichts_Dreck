Shader "Custom Shader/Parallax Mapping"
{
    Properties
    {
        _MainTexture ("Main Texture", 2D) = "grey" {}
        [NoScaleOffset] [Normal] _NormalMap ("Normal Map", 2D) = "bump" {}
        [NoScaleOffset] _HeightMap ("Height Map", 2D) = "black" {}
        _Height ("Height", Range(0.005, 0.08)) = 0.02
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
        UNITY_DECLARE_TEX2D(_NormalMap);
        UNITY_DECLARE_TEX2D(_HeightMap);

        uniform float4 _MainTexture_ST;
        uniform fixed4 _MainColor;
        uniform fixed4 _SpecularColor;
        uniform fixed _SpecularPower;
        uniform fixed _Glossy;
        uniform float _Height;

        struct VertexInput
        {
            float3 position : POSITION;
            float3 normal : NORMAL;
            float4 tangent : TANGENT;
            float2 uv : TEXCOORD;
        };

        struct VertexOutput
        {
            float4 position : SV_POSITION;
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

            // create tangent space
            half3 normal = UnityObjectToWorldNormal(INPUT.normal);
            half3 tangent = UnityObjectToWorldDir(INPUT.tangent);
            half3 binormal = cross(normal, tangent);
            binormal *= INPUT.tangent.w * unity_WorldTransformParams.w;
            binormal = normalize(binormal);
            half3x3 tangentSpace = half3x3(tangent, binormal, normal);

            OUTPUT.uv = TRANSFORM_TEX(INPUT.uv, _MainTexture);

            OUTPUT.viewDirection = normalize(mul(tangentSpace, _WorldSpaceCameraPos - positionWorld));

            OUTPUT.lightDirection = _WorldSpaceLightPos0.xyz - positionWorld * _WorldSpaceLightPos0.w;
            float lightLength = length(OUTPUT.lightDirection);
            OUTPUT.lightDirection = normalize(mul(tangentSpace, OUTPUT.lightDirection));
            OUTPUT.attenuation = 1 / (1.0 + (0.2 * lightLength + 0.1 * lightLength * lightLength) * _WorldSpaceLightPos0
                .
                w);

            return OUTPUT;
        }

        fixed4 calculateFinalColor(VertexOutput INPUT, fixed4 ambientColor) : SV_TARGET
        {
            float2 uv = INPUT.uv + ParallaxOffset(
                UNITY_SAMPLE_TEX2D(_HeightMap, INPUT.uv).r, _Height, INPUT.viewDirection);

            fixed4 textureColor = UNITY_SAMPLE_TEX2D(_MainTexture, uv) * _MainColor;
            fixed4 diffuseColor = 0;
            fixed4 specularColor = 0;

            // light data
            half3 normal = normalize(UnpackNormal(UNITY_SAMPLE_TEX2D(_NormalMap, uv)));
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
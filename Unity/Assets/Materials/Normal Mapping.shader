Shader "Custom Shader/Normal Mapping"
{
    Properties
    {
        _MainTexture ("Main Texture", 2D) = "grey" {}
        [NoScaleOffset] [Normal] _NormalMap ("Normal Map", 2D) = "bump" {}
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

        uniform float4 _MainTexture_ST;
        uniform fixed4 _MainColor;
        uniform fixed4 _SpecularColor;
        uniform fixed _SpecularPower;
        uniform fixed _Glossy;

        struct VertexInput
        {
            float3 position : POSITION;
            float3 normal : NORMAL;
            float4 tangent : TANGENT; // float4 is Unity specific
            float2 uv : TEXCOORD;
        };

        struct VertexOutput
        {
            float4 position : SV_POSITION;
            float3 positionWorld : POSITION1;
            half3 normal : NORMAL;
            half3 tangent : TANGENT;
            half3 binormal : BINORMAL;
            float2 uv : TEXCOORD0;
            half3 viewDirection : TEXCOORD1;
        };

        VertexOutput VS(VertexInput INPUT)
        {
            VertexOutput OUTPUT;

            OUTPUT.position = UnityObjectToClipPos(INPUT.position);
            OUTPUT.positionWorld = mul(UNITY_MATRIX_M, float4(INPUT.position, 1.0)).xyz;
            OUTPUT.normal = UnityObjectToWorldNormal(INPUT.normal);
            OUTPUT.tangent = UnityObjectToWorldDir(INPUT.tangent);
            OUTPUT.binormal = cross(OUTPUT.normal, OUTPUT.tangent);
            OUTPUT.binormal *= INPUT.tangent.w * unity_WorldTransformParams.w; // Unity specific, binormal direction correction
            OUTPUT.binormal = normalize(OUTPUT.binormal);
            OUTPUT.uv = TRANSFORM_TEX(INPUT.uv, _MainTexture);
            OUTPUT.viewDirection = normalize(_WorldSpaceCameraPos - OUTPUT.positionWorld);

            return OUTPUT;
        }

        half3 calculateNormal(half3 tangent, half3 binormal, half3 normal, float2 uv)
        {
            tangent = normalize(tangent);
            binormal = normalize(binormal);
            normal = normalize(normal);

            fixed4 normalMap = UNITY_SAMPLE_TEX2D(_NormalMap, uv);
            // normalMap = normalMap * 2 - 1; // [0;1] -> [-1;+1]
            half3 unpackedNormal = UnpackNormal(normalMap); // Unity specific

            return normalize(
                tangent * unpackedNormal.r +
                binormal * unpackedNormal.g +
                normal * unpackedNormal.b
                );
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
                fixed4 textureColor = UNITY_SAMPLE_TEX2D(_MainTexture, INPUT.uv) * _MainColor;
                fixed4 ambientColor = UNITY_LIGHTMODEL_AMBIENT;
                fixed4 diffuseColor = 0;
                fixed4 specularColor = 0;

                // light data
                half3 normal = calculateNormal(INPUT.tangent, INPUT.binormal, INPUT.normal, INPUT.uv);
                half3 light = _WorldSpaceLightPos0.xyz;
                half3 viewDirection = normalize(INPUT.viewDirection);

                // diffuse color
                float diffuse = max(dot(normal, light), 0);
                diffuseColor = _LightColor0 * diffuse;

                // specular color
                half3 reflectVector = 2 * diffuse * normal - light;
                float specular = pow(max(dot(reflectVector, viewDirection), 0), _SpecularPower) * _Glossy;
                specularColor = _SpecularColor * specular;

                return saturate(textureColor * saturate(ambientColor + diffuseColor) + specularColor);
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
                fixed4 textureColor = UNITY_SAMPLE_TEX2D(_MainTexture, INPUT.uv) * _MainColor;
                fixed4 ambientColor = UNITY_LIGHTMODEL_AMBIENT;
                fixed4 diffuseColor = 0;
                fixed4 specularColor = 0;

                // light data
                half3 normal = calculateNormal(INPUT.tangent, INPUT.binormal, INPUT.normal, INPUT.uv);
                half3 viewDirection = normalize(INPUT.viewDirection);

                // use calculations for different light sources
                half3 light = _WorldSpaceLightPos0.xyz - INPUT.positionWorld * _WorldSpaceLightPos0.w;
                float lightLength = length(light);
                light /= lightLength;
                float attenuation = 1 / (1.0 + (0.2 * lightLength + 0.1 * lightLength * lightLength) * _WorldSpaceLightPos0.w);

                // diffuse color
                float diffuse = max(dot(normal, light), 0);
                diffuseColor = _LightColor0 * (diffuse * attenuation);

                // specular color
                half3 reflectVector = 2 * diffuse * normal - light;
                float specular = pow(max(dot(reflectVector, viewDirection), 0), _SpecularPower) * _Glossy;
                specularColor = _SpecularColor * (specular * attenuation);

                return saturate(textureColor * saturate(diffuseColor) + specularColor);
            }
            ENDHLSL
        }
    }
}
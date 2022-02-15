Shader "Custom Shader/Lighting"
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
            float3 positionWorld : POSITION1;
            half3 normal : NORMAL;
            float2 uv : TEXCOORD0;
            half3 viewDirection : TEXCOORD1;
        };

        VertexOutput VS(VertexInput INPUT)
        {
            VertexOutput OUTPUT;

            OUTPUT.position = UnityObjectToClipPos(INPUT.position);
            OUTPUT.positionWorld = mul(UNITY_MATRIX_M, float4(INPUT.position, 1.0)).xyz;
            OUTPUT.normal = UnityObjectToWorldNormal(INPUT.normal);
            OUTPUT.uv = TRANSFORM_TEX(INPUT.uv, _MainTexture);
            OUTPUT.viewDirection = normalize(_WorldSpaceCameraPos - OUTPUT.positionWorld);

            return OUTPUT;
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
                half3 normal = normalize(INPUT.normal);
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

            //            Blend One Zero // SrcColor * 1 + DstColor * 0 // default
            Blend One One // SrcColor + DstColor
            //            Blend OneMinusDstColor One // soft additive, (1 - DstColor) * SrcColor + DstColor => SrcColor - SrcColor * DstColor + DstColor
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
                half3 normal = normalize(INPUT.normal);
                half3 viewDirection = normalize(INPUT.viewDirection);

                // evaluate for directional or other lights
                // try to avoid using if-else clauses
                // if (_WorldSpaceLightPos0.w != 0)
                // {
                // 	// other lights
                // }
                // else
                // {
                // 	// directional light
                // }

                // use branches with precompiler defines
                // #ifdef USING_DIRECTIONAL_LIGHT
                // // directional light
                // #else
                // // other lights
                // #endif

                // use calculations for different light sources
                half3 light = _WorldSpaceLightPos0.xyz - INPUT.positionWorld * _WorldSpaceLightPos0.w;
                float lightLength = length(light);
                light /= lightLength;
                float attenuation = 1 / (1.0 + (0.2 * lightLength + 0.1 * lightLength * lightLength) *
                    _WorldSpaceLightPos0.w);

                // diffuse color
                float diffuse = max(dot(normal, light), 0);
                diffuseColor = _LightColor0 * (diffuse * attenuation);

                // specular color
                // if (diffuse > 0)
                // {
                //     half3 reflectVector = 2 * diffuse * normal - light;
                //     float specular = pow(max(dot(reflectVector, viewDirection), 0), _SpecularPower) * _Glossy;
                //     specularColor = _SpecularColor * (specular * attenuation);
                // }

                half3 reflectVector = 2 * diffuse * normal - light;
                float specular = pow(max(dot(reflectVector, viewDirection), 0), _SpecularPower) * _Glossy;
                specular = lerp(0, specular, sign(-0.0000001 + diffuse) * 0.5 + 0.5);
                specularColor = _SpecularColor * (specular * attenuation);

                return saturate(textureColor * saturate(diffuseColor) + specularColor);
            }
            ENDHLSL
        }
    }
}
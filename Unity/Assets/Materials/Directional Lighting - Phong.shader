Shader "Custom Shader/Directional Lighting - Phong"
{
    Properties
    {
		_MainTexture ("Main Texture", 2D) = "grey" {}
		_MainColor ("Main Color", Color) = (1.0, 1.0, 1.0, 1.0)
    	_SpecularColor ("Specular Color", Color) = (1.0, 1.0, 1.0, 1.0)
    	_SpecularPower ("Specular Power", float) = 64.0 // Phong Exponent
    	_Glossy ("Glossiness", Range(0.01, 1.0)) = 1.0
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
			};
			
			VertexOutput VS(VertexInput INPUT)
			{
				VertexOutput OUTPUT;
				
				OUTPUT.position = UnityObjectToClipPos(INPUT.position);
				OUTPUT.normal = UnityObjectToWorldNormal(INPUT.normal);
				OUTPUT.uv = TRANSFORM_TEX(INPUT.uv, _MainTexture);
				OUTPUT.viewDirection = normalize(_WorldSpaceCameraPos - mul(UNITY_MATRIX_M, float4(INPUT.position, 1.0)).xyz);
				// OUTPUT.viewDirection = UnityWorldSpaceViewDir(mul(UNITY_MATRIX_M, float4(INPUT.position, 1.0)).xyz);
				
				return OUTPUT;
			}
			
			fixed4 PS(VertexOutput INPUT) : SV_TARGET
			{
				fixed4 textureColor =  UNITY_SAMPLE_TEX2D(_MainTexture, INPUT.uv) * _MainColor;
				fixed4 ambientColor = UNITY_LIGHTMODEL_AMBIENT;
				fixed4 diffuseColor = 0;
				fixed4 specularColor = 0;

				// light data
				half3 normal = normalize(INPUT.normal);
				half3 light = _WorldSpaceLightPos0.xyz; // is already inverted normalized light vector
				half3 viewDirection = normalize(INPUT.viewDirection);
				// half3 reflectVector = reflect(-light, normal);

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
    }
}

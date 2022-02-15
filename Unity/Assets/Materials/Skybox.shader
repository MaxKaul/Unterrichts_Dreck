Shader "Custom Shader/Skybox"
{
    Properties
    {
		[NoScaleOffset] _Skybox ("Skybox Cube Map", Cube) = "grey" {}
    }
    
    SubShader
    {
    	Tags { "Queue" = "Background" "RenderType" = "Backgroud" "PreviewType" = "Skybox" }
    	
		Pass
		{
			Cull Off
			ZWrite Off
			
			HLSLPROGRAM
			
			#include "UnityCG.cginc"
			
			#pragma vertex VS
			#pragma fragment PS

			UNITY_DECLARE_TEXCUBE(_Skybox);
			
			struct VertexInput
			{
				float3 position : POSITION;
			};
			
			struct VertexOutput
			{
				float4 position : SV_POSITION;
				float3 uv : TEXCOORD;
			};
			
			VertexOutput VS(VertexInput INPUT)
			{
				VertexOutput OUTPUT;

				float3 position = mul((float3x3)UNITY_MATRIX_V, INPUT.position);
				OUTPUT.position = mul(UNITY_MATRIX_P, float4(position, 1.0));
				OUTPUT.uv = normalize(INPUT.position);
				
				return OUTPUT;
			}
			
			fixed4 PS(VertexOutput INPUT) : SV_TARGET
			{
				return UNITY_SAMPLE_TEXCUBE(_Skybox, INPUT.uv);
			}
			
			ENDHLSL
		}
    }
}

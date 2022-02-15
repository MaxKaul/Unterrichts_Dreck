Shader "Custom Shader/Cutout (Alpha Testing)"
{
    Properties
    {
		_MainTexture ("Main Texture", 2D) = "grey" {}
		_MainColor ("Main Color", Color) = (1.0, 1.0, 1.0, 1.0)
    	_Cutout ("Cutout", Range(0.0, 1.0)) = 0.5
    }
    
    SubShader
    {
    	Tags { "Queue" = "AlphaTest" }
    	
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
				fixed4 textureColor = UNITY_SAMPLE_TEX2D(_MainTexture, INPUT.uv) * _MainColor;

				// alpha testing
				clip(textureColor.a - _Cutout);
				
				return textureColor;
			}
			
			ENDHLSL
		}
    }
}

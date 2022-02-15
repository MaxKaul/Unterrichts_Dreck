Shader "Custom Shader/Unlit"
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
			HLSLPROGRAM
			
			#include "UnityCG.cginc"
			
			#pragma vertex VS // shader stage main function
			#pragma fragment PS
			//#pragma only_renderers d3d11 opengl // compile for only this renderers
			
			// d3d9
			//uniform sampler2D _MainTexture;
			
			// d3d11
			//uniform Texture2D _MainTexture;
			//uniform SamplerState sampler_MainTexture;
			
			// Unity Way
			UNITY_DECLARE_TEX2D(_MainTexture);
			
			uniform float4 _MainTexture_ST;
			uniform fixed4 _MainColor;
			
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
				
				//OUTPUT.position = mul(UNITY_MATRIX_MVP, float4(INPUT.position, 1.0));
				OUTPUT.position = UnityObjectToClipPos(INPUT.position);
				//OUTPUT.uv = INPUT.uv * _MainTexture_ST.xy + _MainTexture_ST.zw;
				OUTPUT.uv = TRANSFORM_TEX(INPUT.uv, _MainTexture);
				
				return OUTPUT;
			}
			
			fixed4 PS(VertexOutput INPUT) : SV_TARGET
			{
				// d3d9
				//return tex2D(_MainTexture, INPUT.uv);
				
				// d3d11
				//return _MainTexture.Sample(sampler_MainTexture, INPUT.uv);
				
				// Unity way
				return UNITY_SAMPLE_TEX2D(_MainTexture, INPUT.uv) * _MainColor;
			}
			
			ENDHLSL
		}
    }
}

Shader "Custom Shader/Unlit - Fixed Function"
{
    Properties // optional
    {
        _MainTexture ("Main Texture", 2D) = "grey" {}
        _MainColor ("Main Color", Color) = (1.0, 1.0, 1.0, 1.0)
    }

    SubShader // at least one
    {

        // global defines

        Pass // at least one
        {
            Lighting Off
            SetTexture [_MainTexture]
            {
                constantColor [_MainColor]
                combine texture * constant
            }
        }

        //    Pass // additional passes are optional, all passes will be rendered
        //    {
        //    }
    }

    //    SubShader // additional sub shaders are optional
    //    {
    //    // fallback shader
    //    }

    //    Fallback "Unlit/Texture" // optional fallback shader if no sub shader is fitting
    //	CustomEditor "UnlitFixedFunctionEditor" // optional for custom inspector view
}
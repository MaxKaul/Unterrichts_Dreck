using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(MeshRenderer))]
public class Cutoff : MonoBehaviour
{
    private MeshRenderer _meshRenderer;
    private static readonly int CutoffParameter = Shader.PropertyToID("_Cutoff");

    private void Awake()
    {
        _meshRenderer = GetComponent<MeshRenderer>();
    }

    private IEnumerator Start()
    {
        Material material = _meshRenderer.material;

        float cutoff = -0.51f;
        while (cutoff < 0.51f)
        {
            material.SetFloat(CutoffParameter, cutoff);
            yield return null;
            cutoff += Time.deltaTime * 0.5f;
        }
    }
}

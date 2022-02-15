#pragma once
#include <d3d9.h>

#define CheckError(error) if (error != 0) return error;
#define CheckNull(obj, error) if (obj == nullptr) return error;
#define CheckFailed(hr, error) if (FAILED(hr)) return error;

template<typename T>
void safeRelease(T * &obj)
{
    if (obj != nullptr)
    {
        obj->Release();
        obj = nullptr;
    }
}

#define D3DCOLORVALUE_RGB(r, g, b) { r / 255.0f, g / 255.0f, b / 255.0f, 1.0f };
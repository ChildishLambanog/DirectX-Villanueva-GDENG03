#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <d3d11.h>

namespace dx3d
{
    struct Mesh
    {
        RefPtr<VertexBuffer> vb{};
        RefPtr<IndexBuffer> ib{};
        ui32 indexCount{ 0 };
        ID3D11ShaderResourceView* textureSRV{ nullptr };
    };
}

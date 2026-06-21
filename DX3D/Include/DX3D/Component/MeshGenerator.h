#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Component/Mesh.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Math/Vec3.h>
#include <DX3D/Math/Vec4.h>

namespace dx3d
{
    class MeshGenerator
    {
        public:
            static Mesh createCube(GraphicsDevice& device);
            static Mesh createSphere(GraphicsDevice& device, uint32_t sliceCount = 32, uint32_t stackCount = 16, float radius = 0.5f);
        private:
            struct LocalVertex
            {
                Vec3 position;
                Vec4 color;
            };
    };
}

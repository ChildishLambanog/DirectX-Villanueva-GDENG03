#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Game/Component.h>
#include <DX3D/Component/Mesh.h>

namespace dx3d
{
    class SphereComponent final : public Component
    {
        dx3d_typeid(SphereComponent)
        public:
            explicit SphereComponent(const ComponentDesc& data);

            void setMesh(const Mesh* mesh) noexcept { m_mesh = mesh; }
            const Mesh* getMesh() const noexcept { return m_mesh; }

        private:
            const Mesh* m_mesh{ nullptr };
    };
}

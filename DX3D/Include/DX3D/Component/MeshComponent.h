#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Game/Component.h>
#include <DX3D/Component/Mesh.h>

namespace dx3d
{
    class MeshComponent final : public Component
    {
        dx3d_typeid(MeshComponent)
    public:
        explicit MeshComponent(const ComponentDesc& data) : Component(data) {}

        void setMesh(const Mesh* mesh) noexcept { m_mesh = mesh; }
        const Mesh* getMesh() const noexcept { return m_mesh; }

    private:
        const Mesh* m_mesh{ nullptr };
    };
}

#pragma once
#include <DX3D/Game/Component.h>
#include <DX3D/Math/Vec3.h>

namespace dx3d
{
    class BouncingSphere : public Component
    {
        dx3d_typeid(BouncingSphere)

    public:
        explicit BouncingSphere(const ComponentDesc& data);
        Vec3 getVelocity() const noexcept;
        void setVelocity(const Vec3& vel) noexcept;

    private:
        Vec3 m_velocity{ 0.0f, 0.0f, 0.0f };
        float m_speed{ 2.5f }; //Speed in pixels per second
    };
}


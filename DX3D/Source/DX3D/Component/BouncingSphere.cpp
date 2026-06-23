#include <DX3D/Component/BouncingSphere.h>
#include <DX3D/Math/MathUtils.h>

dx3d::BouncingSphere::BouncingSphere(const ComponentDesc& data) : Component(data)
{
    float randomAngle = static_cast<float>(rand() % 360) * (dx3d::MathUtils::PI / 180.0f);
    m_velocity.x = cosf(randomAngle) * m_speed;
    m_velocity.y = sinf(randomAngle) * m_speed;
    m_velocity.z = 0.0f;
}

dx3d::Vec3 dx3d::BouncingSphere::getVelocity() const noexcept
{
    return m_velocity;
}

void dx3d::BouncingSphere::setVelocity(const Vec3& vel) noexcept
{
    m_velocity = vel;
}

#include "Player.h"

Player::Player(const dx3d::GameObjectDesc& desc) : dx3d::GameObject(desc)
{}

Player::~Player()
{}

void Player::onCreate()
{
	dx3d::GameObject::onCreate();

	//Create a camera component for the player and set it up with default values
	auto* camera = createOrGetComponent<dx3d::CameraComponent>();

	if (camera)
	{
		camera->setViewportSize(dx3d::Rect{ 0, 0, 1024, 728 });
		camera->setNearPlane(0.01f);
		camera->setFarPlane(4000.0f);
		camera->setProjectionMode(dx3d::ProjectionMode::Orthographic);
	}

	//Set the player who owns the camera to look at the center but from a distance
	getTransform().setPosition({ 0.0f, 0.0f, -4000.0f });
	getTransform().setRotation({ 0.0f, 0.0f, 0.0f });
}

void Player::onUpdate(dx3d::f32 deltaTime)
{
	auto& input = getInputSystem();
	auto* m_camera = getComponent<dx3d::CameraComponent>();

	if (input.isKeyDown(dx3d::KeyCode::P)) //Toggle perspective mode with P key
	{
		m_camera->setProjectionMode(dx3d::ProjectionMode::Perspective);
	}
	else if (input.isKeyDown(dx3d::KeyCode::O))
	{
		m_camera->setProjectionMode(dx3d::ProjectionMode::Orthographic); //Toggle orthographic mode with O key
	}

	if (m_camera->getProjectionMode() == dx3d::ProjectionMode::Perspective) //Free look and movement controls when in perspective mode
	{
		auto sensitivity = 0.001f;
		auto rot = getTransform().getRotation();
		rot.x += getInputSystem().getMouseDelta().y * sensitivity;
		rot.y += getInputSystem().getMouseDelta().x * sensitivity;
		if (rot.x > 1.57f) rot.x = 1.57f;
		else if (rot.x < -1.57f) rot.x = -1.57f;
		getTransform().setRotation(rot);


		auto pos = getTransform().getPosition();
		auto forward = 0.0f;
		auto right = 0.0f;
		auto speed = 3.0f;
		if (getInputSystem().isKeyDown(dx3d::KeyCode::W)) forward = 1.0f;
		if (getInputSystem().isKeyDown(dx3d::KeyCode::S)) forward = -1.0f;
		if (getInputSystem().isKeyDown(dx3d::KeyCode::D)) right = 1.0f;
		if (getInputSystem().isKeyDown(dx3d::KeyCode::A)) right = -1.0f;
		auto forwardDir = getTransform().forward() * forward;
		auto rightDir = getTransform().right() * right;
		auto direction = dx3d::Vec3::normalize(forwardDir + rightDir);
		pos = pos + direction * speed * deltaTime;
		getTransform().setPosition(pos);
	}

	else if (m_camera->getProjectionMode() == dx3d::ProjectionMode::Orthographic)
	{
		return;
	}
}
#pragma once
#include <DX3D/All.h>

class Player : public dx3d::GameObject
{
	dx3d_typeid(Player)
public:
	explicit Player(const dx3d::GameObjectDesc& desc);
	virtual ~Player() override;
protected:
	virtual void onCreate();
	virtual void onUpdate(dx3d::f32 deltaTime);

private:
	bool m_isRMBHeld = false;
	POINT m_savedCursorPos = { 0, 0 };
	GameObject* m_cam{};
};
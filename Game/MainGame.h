#pragma once
#include <DX3D/All.h>
#include <DX3D/Component/Mesh.h>

class MainGame : public dx3d::Game
{
	public:
		explicit MainGame(const dx3d::GameDesc& desc);

	protected:
		virtual void onCreate();
		virtual void onUpdate(dx3d::f32 deltaTime);

	private:
		dx3d::Mesh m_cubeMesh;
		dx3d::Mesh m_sphereMesh;
		bool m_isInitialized{ false };
};
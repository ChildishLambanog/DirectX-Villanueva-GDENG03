#pragma once
#include <DX3D/All.h>
#include <DX3D/Component/Mesh.h>
#include <DX3D/Input/CommandManager.h>

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

		dx3d::CommandManager m_commandManager{};
		std::vector<dx3d::GameObject*> m_spawnedSpheres{};
		
		
		dx3d::GameObject* m_animatedCube = nullptr; //Test Case 2,3 5
		float m_animationTime = 0.0f; //Test Case 3 and 5
		bool m_isFlat = true; //Test Case 5

		struct CubeInstance //Test Case 4
		{
			dx3d::GameObject* gameObject = nullptr;
			dx3d::Vec3 rotation{};
			dx3d::Vec3 rotationSpeed{};
		};

		std::vector<CubeInstance> m_randomCubes; //Test Case 4
};
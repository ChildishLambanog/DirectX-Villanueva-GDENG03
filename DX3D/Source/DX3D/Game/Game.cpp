#include <DX3D/Game/Game.h>
#include <DX3D/Window/Window.h>

dx3d::Game::Game()
{
	m_display = std::make_unique<Window>(); //Create an instance of the Window class in the constructor of the Game class.
}

dx3d::Game::~Game()
{
}

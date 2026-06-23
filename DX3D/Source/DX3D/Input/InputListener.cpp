//#include <DX3D/Input/InputListener.h>
//#include <DX3D/Input/InputSystem.h>
//#include <DX3D/Input/CommandManager.h>
//#include <DX3D/Input/GameCommands.h> 
//#include <Windows.h>
//
//dx3d::InputListener::InputListener(dx3d::InputSystem& input,
//    dx3d::CommandManager& commandManager,
//    std::function<void()> onSpawnRequested) noexcept
//    : m_input(input),
//    m_commandManager(commandManager),
//    m_onSpawnRequested(onSpawnRequested)
//{}
//
//void dx3d::InputListener::update()
//{
//    if (m_input.isKeyPressed(dx3d::KeyCode::Escape))
//    {
//        PostQuitMessage(0);
//        return;
//    }
//
//    // Spacebar spawns a sphere through our abstract lambda hook
//    if (m_input.isKeyPressed(dx3d::KeyCode::Space))
//    {
//        if (m_onSpawnRequested) m_onSpawnRequested();
//    }
//
//    // Backspace processes a standard deferred single history rollback
//    if (m_input.isKeyPressed(dx3d::KeyCode::Backspace))
//    {
//        m_commandManager.undo();
//    }
//
//    // R key pulls standard deferred items from the redo log track
//    if (m_input.isKeyPressed(dx3d::KeyCode::R))
//    {
//        m_commandManager.redo();
//    }
//
//    // Delete keys execute the isolated mass timeline clear action macro
//    if (m_input.isKeyPressed(dx3d::KeyCode::Delete))
//    {
//        if (!m_commandManager.getUndoStack().empty())
//        {
//            m_commandManager.enqueueCommand(
//                std::make_shared<dx3d::ClearAllSpheresCommand>(m_commandManager.getUndoStack())
//            );
//        }
//    }
//}
#include <DX3D/Input/CommandManager.h>

dx3d::CommandManager::CommandManager(size_t maxHistorySize) : m_maxHistorySize(maxHistorySize)
{
}

void dx3d::CommandManager::enqueueCommand(std::shared_ptr<Command> command)
{
    if (command) m_commandQueue.push(command);
}

void dx3d::CommandManager::processQueue()
{
    //Process the pending undo requests first
    while (!m_pendingUndos.empty())
    {
        m_pendingUndos.pop();
        if (m_undoStack.empty()) continue;

        auto command = m_undoStack.back();
        m_undoStack.pop_back();

        command->undo();
        m_redoStack.push_back(command); // Preserves actual history tracks cleanly
    }

    //Process the pending redo requests second once all undos have been processed 
    while (!m_pendingRedos.empty())
    {
        m_pendingRedos.pop();
        if (m_redoStack.empty()) continue;

        auto command = m_redoStack.back();
        m_redoStack.pop_back();

        command->execute();
        m_undoStack.push_back(command);
    }

    //Proccess the command queue last once all pending undos and redos have been processed
    while (!m_commandQueue.empty())
    {
        auto command = m_commandQueue.front();
        m_commandQueue.pop();

        command->execute();

        m_undoStack.push_back(command);
        m_redoStack.clear(); //Clear the redo stack whenever a new command is executed

        if (m_undoStack.size() > m_maxHistorySize)
        {
            m_undoStack.pop_front();
        }
    }
}

void dx3d::CommandManager::undo()
{
    m_pendingUndos.push(true);
}

void dx3d::CommandManager::redo()
{
    m_pendingRedos.push(true);
}

void dx3d::CommandManager::clearHistory() noexcept
{
    std::queue<std::shared_ptr<Command>> emptyQueue{};
    std::swap(m_commandQueue, emptyQueue);

    std::queue<bool> emptyFlags{};
    std::swap(m_pendingUndos, emptyFlags);
    std::swap(m_pendingRedos, emptyFlags);

    m_undoStack.clear();
    m_redoStack.clear();
}

void dx3d::CommandManager::setMaxHistorySize(size_t size) noexcept
{
    m_maxHistorySize = size;
    while (m_undoStack.size() > m_maxHistorySize)
    {
        m_undoStack.pop_front();
    }
}

std::deque<std::shared_ptr<dx3d::Command>>& dx3d::CommandManager::getUndoStack() noexcept
{
    return m_undoStack;
}

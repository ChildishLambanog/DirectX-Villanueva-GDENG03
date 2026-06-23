#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Input/Command.h>
#include <queue>
#include <deque>
#include <memory>

namespace dx3d
{
	class CommandManager
	{
		public:
			CommandManager(size_t maxHistorySize = 100);
			~CommandManager() = default;

			void enqueueCommand(std::shared_ptr<Command> command);
			void processQueue();

			void undo();
			void redo();

			void clearHistory() noexcept;
			void setMaxHistorySize(size_t size) noexcept;
			std::deque<std::shared_ptr<Command>>& getUndoStack() noexcept;

		private:
			std::queue<std::shared_ptr<Command>> m_commandQueue{};
			std::deque<std::shared_ptr<Command>> m_undoStack{};
			std::deque<std::shared_ptr<Command>> m_redoStack{};
			size_t m_maxHistorySize{ 100 };

			std::queue<bool> m_pendingUndos{};
			std::queue<bool> m_pendingRedos{};
	};
}


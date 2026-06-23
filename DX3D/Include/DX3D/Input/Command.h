#pragma once
#include <DX3D/Core/Core.h>

namespace dx3d
{
    class Command
    {
        public:
            virtual ~Command() = default;
            virtual void execute() = 0;
            virtual void undo() = 0;
    };
}

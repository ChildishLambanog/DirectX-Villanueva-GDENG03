#pragma once
#include <DX3D/Input/Command.h>
#include <DX3D/Component/SphereComponent.h>
#include <DX3D/Component/BouncingSphere.h>
#include <vector>
#include <iostream>

namespace dx3d
{
    //Command for spawning a single sphere with bounce and undoing that spawn
    class SpawnSphereCommand final : public Command
    {
    public:
        SpawnSphereCommand(World& world, const Mesh* sphereMesh, std::vector<GameObject*>& registry)
            : m_world(world), m_sphereMesh(sphereMesh), m_sphereRegistry(registry) {}

        virtual void execute() override
        {
            std::cout << "Sphere has been spawned \n";

            m_spawnedObject = m_world.createGameObject<GameObject>(); //Create a new GameObject to represent the sphere in the world
            m_spawnedObject->createOrGetComponent<BouncingSphere>(); //Add the bouncing sphere component to give it movement behavior

            auto* comp = m_spawnedObject->createOrGetComponent<SphereComponent>(); //Add the sphere component which will handle rendering the sphere mesh
            if (m_sphereMesh) comp->setMesh(m_sphereMesh);

            m_sphereRegistry.push_back(m_spawnedObject); //Add the spawned sphere to the registry for tracking and management
        }

        virtual void undo() override
        {
            if (!m_spawnedObject) return;

            //Logic to remove the sphere by removing it from the registry and marking it for destruction in the World
            auto it = std::find(m_sphereRegistry.begin(), m_sphereRegistry.end(), m_spawnedObject);
            while (it != m_sphereRegistry.end())
            {
                m_sphereRegistry.erase(it);
                it = std::find(m_sphereRegistry.begin(), m_sphereRegistry.end(), m_spawnedObject);
            }

            //Pass the object to the world for destruction, which will handle cleanup and deallocation
            m_world.destroyGameObject(*m_spawnedObject);

            //Set it to nullptr to avoid dangling references in case of multiple undos
            m_spawnedObject = nullptr;
        }

    private:
        World& m_world;
        const Mesh* m_sphereMesh;
        std::vector<GameObject*>& m_sphereRegistry;
        GameObject* m_spawnedObject{ nullptr };
    };

    class ClearAllSpheresCommand final : public Command
    {
    public:
        // Pass references to the registry AND the manager's undo stack history track
        explicit ClearAllSpheresCommand(std::vector<GameObject*>& registry, std::deque<std::shared_ptr<Command>>& undoStack)
            : m_sphereRegistry(registry), m_undoStackRef(undoStack) {}

        virtual void execute() override
        {
            m_capturedSpawnCommands.clear();

            // 1. Move all the SpawnSphereCommands from the manager's undo stack into this command!
            // We loop backwards through history to grab them.
            auto it = m_undoStackRef.begin();
            while (it != m_undoStackRef.end())
            {
                // We verify that the command in history is actually a sphere spawning action
                if (auto spawnCmd = std::dynamic_pointer_cast<SpawnSphereCommand>(*it))
                {
                    m_capturedSpawnCommands.push_back(spawnCmd);
                    it = m_undoStackRef.erase(it); // Remove it from the main history tracking stack
                }
                else
                {
                    ++it;
                }
            }

            // 2. Trigger the undo sequence for each captured spawn command.
            // This safely tells the engine to clean them up and empties your active registry loop vector.
            for (auto& spawnCmd : m_capturedSpawnCommands)
            {
                spawnCmd->undo();
            }
        }

        virtual void undo() override
        {
            for (auto& spawnCmd : m_capturedSpawnCommands)
            {
                spawnCmd->execute();
            }
        }

    private:
        std::vector<GameObject*>& m_sphereRegistry;
        std::deque<std::shared_ptr<Command>>& m_undoStackRef;
        std::vector<std::shared_ptr<SpawnSphereCommand>> m_capturedSpawnCommands{};
    };
}

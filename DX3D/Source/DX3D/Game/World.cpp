#include <DX3D/Game/World.h>
#include <DX3D/Game/GameObject.h>
#include <DX3D/Game/Component.h>
#include <DX3D/Component/TransformComponent.h>

dx3d::World::World(const WorldDesc& desc) : Base(desc.base), m_gameContext(desc.gameContext)
{
}

void dx3d::World::update(f32 deltaTime)
{
	m_garbageObjects.clear();

	if (m_events.size())
	{ 
		std::swap(m_events, m_eventsSwapBuffer);
		std::swap(m_pendingObjects, m_pendingObjectsSwapBuffer);

		for (auto& e : m_eventsSwapBuffer)
		{
			auto objTypeId = e.object->getTypeId();
			auto pendingObjIndex = e.pendingObjectIndex;

			if (e.eventType == EventType::Create)
			{
				auto& obj = m_pendingObjectsSwapBuffer[pendingObjIndex];
				auto ptr = obj.get();

				m_objects[objTypeId].push_back(std::move(obj));
				ptr->onCreate();
			}
			else if (e.eventType == EventType::Destroy)
			{
				auto& activeList = m_objects[objTypeId];
				auto it = std::find_if(activeList.begin(), activeList.end(),
					[&e](const UniquePtr<GameObject>& obj) { return obj.get() == e.object; });

				if (it != activeList.end())
				{
					// 1. Alert the object it's dying
					(*it)->onDestroy();

					// 2. Scrub its transform from the dirty transformation tracking list!
					auto& transformComp = (*it)->getTransform();
					auto dirtyIt = std::find(m_dirtyTransforms.begin(), m_dirtyTransforms.end(), &transformComp);
					if (dirtyIt != m_dirtyTransforms.end())
					{
						m_dirtyTransforms.erase(dirtyIt);
					}

					// 3. Scrub its components from global lookup maps
					for (auto&& [typeId, componentPtr] : (*it)->getComponentsInternalMap())
					{
						auto& worldCompVector = m_components[typeId];
						auto compIt = std::find(worldCompVector.begin(), worldCompVector.end(), componentPtr.get());
						if (compIt != worldCompVector.end())
						{
							worldCompVector.erase(compIt);
						}
					}

					// 4. Safely deallocate on the next frame boundary
					m_garbageObjects.push_back(std::move(*it));
					activeList.erase(it);
				}
			}
		}

		m_pendingObjectsSwapBuffer.clear();
		m_eventsSwapBuffer.clear();
	}

	for (auto&& [typeId, objects] : m_objects)
	{
		for (auto& object : objects)
		{
			object->onUpdate(deltaTime);
		}
	}

	for (auto& comp : m_dirtyTransforms)
	{
		comp->updateWorldMatrix();
	}
	m_dirtyTransforms.clear();
}

dx3d::GameObject* dx3d::World::createGameObjectInternal(UniquePtr<GameObject>& object)
{
	if (!object) return {};

	auto ptr = object.get();

	auto index = m_pendingObjects.size();

	m_pendingObjects.push_back(std::move(object));
	m_events.push_back({ ptr, index, EventType::Create });

	return ptr;
}

void dx3d::World::addComponentInternal(Component& component)
{
	auto typeId = component.getTypeId();
	m_components[typeId].push_back(&component);
}

void dx3d::World::addDirtyTransformInternal(TransformComponent& component)
{
	m_dirtyTransforms.push_back(&component);
}

dx3d::Component* const* dx3d::World::getComponentsInternal(size_t typeId, ui32* numComponents) const noexcept
{
	auto it = m_components.find(typeId);
	if (it != m_components.end())
	{
		*numComponents = static_cast<ui32>(it->second.size());
		return it->second.data();
	}

	*numComponents = 0u;
	return {};
}

void dx3d::World::destroyGameObject(GameObject& object)
{
	m_events.push_back({ &object, 0, EventType::Destroy });
}
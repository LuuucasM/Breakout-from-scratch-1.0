#pragma once

#include "ECSConstants.h"
#include "ComponentArray.h"

#include <unordered_map>
#include <queue>
#include <memory>
#include <cassert>
class ComponentManager {
private:
	std::unordered_map<const char*, ComponentType> ComponentTypes{};
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> ComponentArrays{};
	ComponentType NextComponentType = 0;
	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray() {
		const char* typeName = typeid(T).name();
		assert(ComponentTypes.find(typeName) != ComponentTypes.end() && "Component not registered");

		return std::static_pointer_cast<ComponentArray<T>>(ComponentArrays[typeName]);
	}

	template<typename T>
	void RegisterComponent() {
		const char* typeName = typeid(T).name();

		assert(ComponentTypes.find(typeName) == ComponentTypes.end() && "Cannot Register Component more than once");

		ComponentTypes.insert({ typeName, NextComponentType });
		NextComponentType++;

		ComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });
	}

public:
	void InitComponents();

	template<typename T>
	ComponentType GetComponentType() {
		const char* typeName = typeid(T).name();

		assert(ComponentTypes.find(typeName) != ComponentTypes.end() && "Component not registered");

		return ComponentTypes[typeName];
	}
	template<typename T>
	void AddComponent(EntityID entity, T component) {
		GetComponentArray<T>()->AddComponent(entity, component);
	}
	template<typename T>
	void RemoveComponent(EntityID entity) {
		GetComponentArray<T>()->RemoveComponent(entity);
	}
	template<typename T>
	T *GetComponent(EntityID entity) {
		return GetComponentArray<T>()->GetComponent(entity);
	}
	void EntityDestroyed(EntityID entity){
		for (auto const& pair : ComponentArrays) {
			auto const& componentarray = pair.second;
			componentarray->EntityDestroyed(entity);
		}
	}
};
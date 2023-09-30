#pragma once

#include "ECSConstants.h"
#include "System.h"

#include <unordered_map>
#include <set>
#include <memory>
#include <cassert>

class SystemManager {
private:
	std::unordered_map<const char*, ComponentMask> SystemComponentMasks;
	std::unordered_map<const char*, std::shared_ptr<System>> Systems;

	template<typename T>
	std::shared_ptr<T> RegisterSystem() {
		const char* typeName = typeid(T).name();

		assert(Systems.find(typeName) == Systems.end() && "System already registered");

		auto system = std::make_shared<T>();
		Systems.insert({ typeName, system });

		return system;
	};
	template<typename T>
	void SetMask(ComponentMask mask) {
		const char* typeName = typeid(T).name();
		assert(Systems.find(typeName) != Systems.end() && "System not registered");

		SystemComponentMasks.insert({ typeName, mask });
	};

public:
	void InitSystems();

	template<typename T>
	std::shared_ptr<System> GetSystem() {
		const char* typeName = typeid(T).name();
		assert(Systems.find(typeName) != Systems.end() && "System not registered");

		return Systems[typeName];
	}

	void RemoveEntity(EntityID entity) {
		for (auto const& pair : Systems) {
			auto const& system = pair.second;

			system->Entities.erase(entity);
		}
	};
	void EntityMaskChanged(EntityID entity, ComponentMask entityMask) {
		for (auto const& pair : Systems) {
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemMask = SystemComponentMasks[type];
			if ((entityMask & systemMask) == systemMask) {
				system->Entities.insert(entity);
			}
			else {
				system->Entities.erase(entity);
			}
		}
	};
};
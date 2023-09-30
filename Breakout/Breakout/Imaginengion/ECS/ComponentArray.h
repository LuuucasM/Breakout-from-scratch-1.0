#pragma once

#include "ECSConstants.h"

#include <unordered_map>
#include <array>
#include <cassert>

class IComponentArray {

protected:
	std::unordered_map<EntityID, size_t> EntityToIndexMap;
	std::unordered_map<size_t, EntityID> IndexToEntityMap;
	size_t array_size;
public:
	virtual void RemoveComponent(EntityID entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray{
protected:
	std::array<T, MAX_ENTITIES> ArrayOfComponents;
public:
	void RemoveComponent(EntityID entity) {
		assert(EntityToIndexMap.find(entity) != EntityToIndexMap.end() && "Removing non-existent component!");

		//swap component in last index to the index we are removing from
		size_t indexOfRemovedEntity = EntityToIndexMap[entity];
		size_t indexOfLastElement = array_size - 1;
		ArrayOfComponents[indexOfRemovedEntity] = ArrayOfComponents[indexOfLastElement];

		//update references in the maps so the entity points to the right index and index to the right entity
		EntityID entityOfLastElement = IndexToEntityMap[indexOfLastElement];
		EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		IndexToEntityMap[indexOfLastElement] = entityOfLastElement;

		EntityToIndexMap.erase(entity);
		IndexToEntityMap.erase(indexOfLastElement);

		array_size--;
	};
	void AddComponent(EntityID entity, T component) {
		EntityToIndexMap[entity] = array_size;
		IndexToEntityMap[array_size] = entity;
		ArrayOfComponents[array_size] = component;
		array_size++;
	};
	T *GetComponent(EntityID entity) {
		assert(EntityToIndexMap.find(entity) != EntityToIndexMap.end() && "Removing non-existent component!");

		return &ArrayOfComponents[EntityToIndexMap[entity]];
	};
};

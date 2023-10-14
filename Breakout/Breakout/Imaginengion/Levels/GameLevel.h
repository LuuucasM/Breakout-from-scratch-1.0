#pragma once


#include "GameLevel.h"
#include "../ECS/ECSConstants.h"

#include <unordered_set>

class GameLevel {
public:
	GameLevel() {};
	~GameLevel() {};
	std::unordered_set<EntityID> Entities;
	virtual void RemoveEntity(EntityID ID) {};
	virtual bool CheckGameOver() { return false; };
	const char* filename;
};
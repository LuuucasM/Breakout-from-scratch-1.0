#pragma once

#include "ECSConstants.h"

#include <set>


class System {
public:
	std::set<EntityID> Entities;
	virtual void Init() = 0;
	virtual void Update() = 0;
};
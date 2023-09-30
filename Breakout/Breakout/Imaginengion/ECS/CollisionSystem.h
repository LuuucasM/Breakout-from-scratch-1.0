#pragma once
#include "System.h"
class CollisionSystem : public System {

public:
	virtual void Init() override;
	virtual void Update() override;
};
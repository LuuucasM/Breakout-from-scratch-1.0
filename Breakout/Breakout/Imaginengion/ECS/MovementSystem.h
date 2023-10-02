#pragma once

#include "System.h"

class MovementSystem : public System {
public:
	virtual void Init() override;
	virtual void Update() override;
};
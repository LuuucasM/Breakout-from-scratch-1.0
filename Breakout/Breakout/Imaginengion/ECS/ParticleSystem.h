#pragma once
#include "System.h"

class ParticleSystem : public System {
public:
	void Init() override;
	void Update() override;
};
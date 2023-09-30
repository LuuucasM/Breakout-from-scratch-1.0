#pragma once

#include "System.h"

class RenderSystem2D : public System {
private:
	unsigned int VAO;
public:
	virtual void Init() override;
	virtual void Update() override;
};
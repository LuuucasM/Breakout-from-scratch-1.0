#pragma once

#include "Imaginengion/Levels/GameLevel.h"

#include <string>

class BreakoutLevel : public GameLevel {
public:
	BreakoutLevel(const char* file);
	~BreakoutLevel() {};
	std::vector<std::vector<unsigned int>> tileData;
	void LoadLevel(int window_width, int window_height);
	void RemoveEntity(EntityID ID) override;
	bool CheckGameOver() override;
};
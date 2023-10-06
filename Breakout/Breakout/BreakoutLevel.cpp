

#include "BreakoutLevel.h"
#include "Imaginengion/ECS/ECSCoordinator.h"
#include "Imaginengion/ECS/Components.h"

#include <fstream>
#include <sstream>
#include <iostream>

extern ECSCoordinator ECSCoord;

BreakoutLevel::BreakoutLevel(const char* file) {
	unsigned int tileCode;
	std::string line;
	std::ifstream levelFile(file);

	if (levelFile) {
		while (std::getline(levelFile, line)) {
			std::istringstream sstream(line);
			std::vector<unsigned int> row;
			while (sstream >> tileCode) {
				row.push_back(tileCode);
			}
			tileData.push_back(row);
		}
	}
	//std::cout << "number of rows: " << tileData.size();
	//std::cout << "number of col: " << tileData[0].size();
}

void BreakoutLevel::LoadLevel(int window_width, int window_height) {
	
	if (tileData.size() > 0) {
		size_t height = tileData.size();
		size_t width = tileData[0].size();
		float unit_width = window_width / (float)width;
		float unit_height = window_height / (float)height;
		EntityID block;
		for (size_t y = 0; y < height; y++) {
			for (size_t x = 0; x < width; x++) {
				if (tileData[y][x] == 1) { //a solid block
					block = ECSCoord.CreateEntity();
					ECSCoord.AddComponent<C_RigidBody>(block, { glm::vec3(unit_width * x, unit_height * y, 0.0f), 0.0f, glm::vec3(unit_width, unit_height, 0.0f) });
					ECSCoord.AddComponent<C_Model>(block, { true, "Ball", "block_solid", glm::vec3(0.8f, 0.8f, 0.7f)});
					ECSCoord.AddComponent<C_Collision>(block, { CollisionClasses::WALL, CollisionShapes::RECT });
					Entities.insert(block);
				}
				else if (tileData[y][x] > 1) { //regular block
					block = ECSCoord.CreateEntity();
					ECSCoord.AddComponent<C_RigidBody>(block, { glm::vec3(unit_width * x, unit_height * y, 0.0f), 0.0f, glm::vec3(unit_width, unit_height, 0.0f) });
					if (tileData[y][x] == 2) {
						ECSCoord.AddComponent<C_Model>(block, { true, "Ball", "block", glm::vec3(0.2f, 0.6f, 1.0f)});
					}
					if (tileData[y][x] == 3) {
						ECSCoord.AddComponent<C_Model>(block, { true, "Ball", "block", glm::vec3(0.0f, 0.7f, 0.0f)});
					}
					if (tileData[y][x] == 4) {
						ECSCoord.AddComponent<C_Model>(block, { true, "Ball", "block", glm::vec3(0.8f, 0.8f, 0.4f)});
					}
					if (tileData[y][x] == 5) {
						ECSCoord.AddComponent<C_Model>(block, { true, "Ball", "block", glm::vec3(1.0f, 0.5f, 0.0f)});
					}
					ECSCoord.AddComponent<C_Collision>(block, { CollisionClasses::BRICK, CollisionShapes::RECT });

					Entities.insert(block);
				}
			}
		}
	}
}

void BreakoutLevel::RemoveEntity(EntityID ID) {
	Entities.erase(ID);
}

bool BreakoutLevel::CheckGameOver() {
	return Entities.size() <= 0;
}
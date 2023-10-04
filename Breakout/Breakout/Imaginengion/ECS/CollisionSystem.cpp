#include "CollisionSystem.h"
#include "ECSCoordinator.h"
#include "Components.h"
#include "../Math/Math.h"

extern ECSCoordinator ECSCoord;

void CollisionSystem::Init() {

}

void CollisionSystem::Update() {
	//brute force check all the entities with each other
	for (auto entity1 : Entities) {
		for (auto entity2 : Entities) {
			if (entity1 != entity2) {
				auto ent1Collision = ECSCoord.GetComponent<C_Collision>(entity1);
				auto ent2Collision = ECSCoord.GetComponent<C_Collision>(entity2);
				//check if a collision function exists between entity1 and entity2
				if (ent1Collision->CollisionEntityFunctions.find(entity2) != ent1Collision->CollisionEntityFunctions.end()){
					switch (ent1Collision->CollisionShape & ent2Collision->CollisionShape) {
						case (CollisionShapes::RECT & CollisionShapes::RECT):
						{
							auto ent1rigidBody = ECSCoord.GetComponent<C_RigidBody>(entity1);
							auto ent2rigidBody = ECSCoord.GetComponent<C_RigidBody>(entity2);
							//compute if collided
							if (ent1rigidBody->Position.x + ent1rigidBody->Size.x >= ent2rigidBody->Position.x
								&& ent2rigidBody->Position.x + ent2rigidBody->Size.x >= ent1rigidBody->Position.x) {
								if (ent1rigidBody->Position.y + ent1rigidBody->Size.y >= ent2rigidBody->Position.y
									&& ent2rigidBody->Position.y + ent2rigidBody->Size.y >= ent1rigidBody->Position.y) {
									ent1Collision->CollisionEntityFunctions[entity2](entity1, entity2, glm::vec3(0.0f, 0.0f, 0.0f));
								}
							}
						}
						break;
						case (CollisionShapes::CIRCLE & CollisionShapes::RECT):
						{
							//compute if collided
							C_RigidBody* Circle;
							C_RigidBody* Rect;
							if (ent1Collision->CollisionShape == CollisionShapes::CIRCLE) {
								Circle = ECSCoord.GetComponent<C_RigidBody>(entity1);
								Rect = ECSCoord.GetComponent<C_RigidBody>(entity2);
							}
							else {
								Circle = ECSCoord.GetComponent<C_RigidBody>(entity2);
								Rect = ECSCoord.GetComponent<C_RigidBody>(entity1);
							}
							glm::vec3 circle_center(Circle->Position.x + (Circle->Size.x / 2), Circle->Position.y + (Circle->Size.y / 2), Circle->Position.z + (Circle->Size.z / 2));
							glm::vec3 rectangle_center(Rect->Position.x + (Rect->Size.x / 2), Rect->Position.y + (Rect->Size.y / 2), Rect->Position.z + (Rect->Size.z / 2));
							glm::vec3 rectangle_half_extents(Rect->Size.x / 2, Rect->Size.y / 2, Rect->Size.z / 2);

							glm::vec3 difference = circle_center - rectangle_center;
							glm::vec3 clamped = glm::clamp(difference, -rectangle_half_extents, rectangle_half_extents);

							glm::vec3 closest = rectangle_center + clamped;

							difference = closest - circle_center;

							float circle_radius = circle_center.x - Circle->Position.x;
							if (glm::length(difference) <= circle_radius) {
								ent1Collision->CollisionEntityFunctions[entity2](entity1, entity2, closest);
							}
						}
						break;
						default:
							std::cout << "COLLISION BETWEEN THESE SHAPES NOT DEFINED" << std::endl;
					}
				}
				//check if a collision function exists between entity1 and entity2's collision class
				else if (ent1Collision->CollisionClassFunctions.find(ent2Collision->CollisionClass) != ent1Collision->CollisionClassFunctions.end()) {
					switch (ent1Collision->CollisionShape & ent2Collision->CollisionShape) {
						case (CollisionShapes::RECT & CollisionShapes::RECT):
							{
								auto ent1rigidBody = ECSCoord.GetComponent<C_RigidBody>(entity1);
								auto ent2rigidBody = ECSCoord.GetComponent<C_RigidBody>(entity2);
								//compute if collided
								if (ent1rigidBody->Position.x + ent1rigidBody->Size.x >= ent2rigidBody->Position.x
									&& ent2rigidBody->Position.x + ent2rigidBody->Size.x >= ent1rigidBody->Position.x) {
									if (ent1rigidBody->Position.y + ent1rigidBody->Size.y >= ent2rigidBody->Position.y
										&& ent2rigidBody->Position.y + ent2rigidBody->Size.y >= ent1rigidBody->Position.y) {
										ent1Collision->CollisionClassFunctions[ent2Collision->CollisionClass](entity1, entity2, glm::vec3(0.0f, 0.0f, 0.0f));
									}
								}
							}
							break;
						case (CollisionShapes::CIRCLE & CollisionShapes::RECT):
							{
								//compute if collided
								C_RigidBody* Circle;
								C_RigidBody* Rect;
								if (ent1Collision->CollisionShape == CollisionShapes::CIRCLE) {
									Circle = ECSCoord.GetComponent<C_RigidBody>(entity1);
									Rect = ECSCoord.GetComponent<C_RigidBody>(entity2);
								}
								else {
									Circle = ECSCoord.GetComponent<C_RigidBody>(entity2);
									Rect = ECSCoord.GetComponent<C_RigidBody>(entity1);
								}
								glm::vec3 circle_center(Circle->Position.x + (Circle->Size.x / 2), Circle->Position.y + (Circle->Size.y / 2), Circle->Position.z + (Circle->Size.z / 2));
								glm::vec3 rectangle_center(Rect->Position.x + (Rect->Size.x / 2), Rect->Position.y + (Rect->Size.y / 2), Rect->Position.z + (Rect->Size.z / 2));
								glm::vec3 rectangle_half_extents(Rect->Size.x / 2, Rect->Size.y / 2, Rect->Size.z / 2);

								glm::vec3 difference = circle_center - rectangle_center;
								glm::vec3 clamped = glm::clamp(difference, -rectangle_half_extents, rectangle_half_extents);

								glm::vec3 closest = rectangle_center + clamped;

								difference = closest - circle_center;

								float circle_radius = circle_center.x - Circle->Position.x;
								if (glm::length(difference) <= circle_radius) {
									ent1Collision->CollisionClassFunctions[ent2Collision->CollisionClass](entity1, entity2, closest);
								}
							}
							break;
						default:
							std::cout << "COLLISION BETWEEN THESE SHAPES NOT DEFINED" << std::endl;
					}

				}
			}
		}
	}
}
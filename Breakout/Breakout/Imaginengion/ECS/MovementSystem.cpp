#include "MovementSystem.h"
#include "ECSCoordinator.h"

#include "Components.h"

extern ECSCoordinator ECSCoord;

void MovementSystem::Init() {

}

void MovementSystem::Update() {
	for (auto entity : Entities) {
		auto entRigidBody = ECSCoord.GetComponent<C_RigidBody>(entity);
		auto entMovement = ECSCoord.GetComponent<C_Movement>(entity);
		entRigidBody->Position.x += entMovement->velocity.x;
		entRigidBody->Position.y += entMovement->velocity.y;
		//entRigidBody->Position.z += entMovement->velocity.z;
	}
}
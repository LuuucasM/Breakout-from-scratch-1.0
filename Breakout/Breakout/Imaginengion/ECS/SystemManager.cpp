#include "SystemManager.h"
#include "Components.h"
#include "ECSCoordinator.h"

//put all the systems after this comment
#include "RenderSystem2D.h"
#include "CollisionSystem.h"
#include "MovementSystem.h"

extern ECSCoordinator ECSCoord;

void SystemManager::InitSystems() {
	
	//RenderSystem2D
	RegisterSystem<RenderSystem2D>();
	{
		ComponentMask mask;
		mask.set(ECSCoord.GetComponentType<C_RigidBody>());
		mask.set(ECSCoord.GetComponentType<C_Model>());
		SetMask<RenderSystem2D>(mask);
	}
	GetSystem<RenderSystem2D>()->Init();

	//CollisionSystem
	RegisterSystem<CollisionSystem>();
	{
		ComponentMask mask;
		mask.set(ECSCoord.GetComponentType<C_Collision>());
		mask.set(ECSCoord.GetComponentType<C_RigidBody>());
		SetMask<CollisionSystem>(mask);
	}
	GetSystem<CollisionSystem>()->Init();

	//Movement System
	RegisterSystem<MovementSystem>();
	{
		ComponentMask mask;
		mask.set(ECSCoord.GetComponentType<C_RigidBody>());
		mask.set(ECSCoord.GetComponentType<C_Movement>());
		SetMask<MovementSystem>(mask);
	}
}
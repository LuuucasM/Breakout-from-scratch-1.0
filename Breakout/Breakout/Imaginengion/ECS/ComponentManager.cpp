#include "ComponentManager.h"

//put all the components after this comment
#include "Components.h"

void ComponentManager::InitComponents() {
	//register all the build in engine components
	RegisterComponent<C_RigidBody>();
	RegisterComponent<C_Model>();
	RegisterComponent<C_Movement>();
	RegisterComponent<C_Collision>();
}
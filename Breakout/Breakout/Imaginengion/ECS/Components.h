#pragma once
#include <glm/glm.hpp>

#include "../Resources/Shader.h"
#include "ECSConstants.h"

#include <map>


//a list of all the components
//not sure if i should keep them in a list like this or seperate them into files hmmmm
struct C_RigidBody {
	glm::vec3 Position;
	float Rotation;
	glm::vec3 Size;
};

struct C_Model {
	bool bShouldRender;
	const char* ShaderName;
	const char* TextureName;
	glm::vec3 color;
};

struct C_Movement {
	glm::vec2 velocity;
	float speed;
	float direction;
};

struct C_Collision {
	CollisionClasses CollisionClass;
	CollisionShapes CollisionShape;
	std::map<EntityID, CollisionFunc> CollisionEntityFunctions;
	std::map<CollisionClasses, CollisionFunc> CollisionClassFunctions;
};

struct C_Particles {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	float Lifetime;
};
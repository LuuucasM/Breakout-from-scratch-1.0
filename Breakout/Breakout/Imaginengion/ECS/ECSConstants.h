#pragma once

#include <cstdint>
#include <bitset>

using EntityID = std::uint64_t;
using ComponentType = std::uint64_t;

const uint64_t MAX_ENTITIES = 150;
const uint64_t MAX_COMPONENTS = 10;

using ComponentMask = std::bitset<MAX_COMPONENTS>;

enum CollisionClasses {
	PADDLE,
	BALL,
	BRICK,
	WALL
};

typedef void (*CollisionFunc)(EntityID, EntityID);
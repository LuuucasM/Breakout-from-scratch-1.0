#include "Imaginengion/Rendering/RenderManager.h"
#include "Imaginengion/Resources/ResourceManager.h"
#include "Imaginengion/ECS/ECSCoordinator.h"
#include "BreakoutLevel.h"

#include "Imaginengion/ECS/Components.h"
#include "Imaginengion/ECS/RenderSystem2D.h"
#include "Imaginengion/ECS/CollisionSystem.h"
#include "Imaginengion/ECS/MovementSystem.h"

#include <glm/gtx/string_cast.hpp>

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

RenderManager RenderingManager(RenderInterfaceType::OPENGL, WINDOW_WIDTH, WINDOW_HEIGHT, "Breakout");
ECSCoordinator ECSCoord;
GameLevel* CurrentLevel;

std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Textures;

void BallToWall(EntityID ball, EntityID wall, glm::vec3 location) {
	auto Circle = ECSCoord.GetComponent<C_RigidBody>(ball);
	glm::vec3 circle_center(Circle->Position.x + (Circle->Size.x / 2), Circle->Position.y + (Circle->Size.y / 2), Circle->Position.z + (Circle->Size.z / 2));
	glm::vec3 balltowallvec = glm::normalize(circle_center - location);
	auto ballMovement = ECSCoord.GetComponent<C_Movement>(ball);
	if (balltowallvec.x) {
		ballMovement->velocity.x *= -1.0f;
	}
	else {
		ballMovement->velocity.y *= -1.0f;
	}
}

void BallToBottomWall(EntityID ball, EntityID wall, glm::vec3 location){
	RenderingManager.SetWindowShouldClose(true);
}

void BallToPaddle(EntityID ball, EntityID paddle, glm::vec3 location) {
	auto Rect = ECSCoord.GetComponent<C_RigidBody>(paddle);

	//first split the paddle into sections and then set new_x_val according to which section the ball collided with
	float sections = Rect->Size.x / 11.0f;
	float section_num = (location.x - Rect->Position.x)/sections;
	int new_x_val = (int)(section_num - 6.0f);
	
	auto ballMovement = ECSCoord.GetComponent<C_Movement>(ball);
	ballMovement->velocity.y *= -1;
	ballMovement->velocity.x = (float)new_x_val;
}

void BallToBrick(EntityID ball, EntityID brick, glm::vec3 location) {
	std::cout << "ball entity id: " << ball << std::endl;
	std::cout << "brick entity id: " << brick << std::endl;
	auto Circle = ECSCoord.GetComponent<C_RigidBody>(ball);
	glm::vec3 circle_center(Circle->Position.x + (Circle->Size.x / 2), Circle->Position.y + (Circle->Size.y / 2), Circle->Position.z + (Circle->Size.z / 2));
	glm::vec3 balltowallvec = glm::normalize(circle_center - location);
	auto ballMovement = ECSCoord.GetComponent<C_Movement>(ball);
	if (balltowallvec.x) {
		ballMovement->velocity.x *= -1.0f;
	}
	else {
		ballMovement->velocity.y *= -1.0f;
	}
	ECSCoord.SetEntityToDestroy(brick);
	CurrentLevel->RemoveEntity(brick);
}

int main() {
	RenderingManager.Init();

	ECSCoord.Init();

	ResourceManager::LoadShader("shader.vs", "shader.fs", nullptr, "Ball");
	glm::mat4 projection = glm::ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);
	Shader ball_shader = ResourceManager::GetShader("Ball");
	ball_shader.use();
	ball_shader.setUniform<int>("image", 0);
	ball_shader.setUniform<glm::mat4>("projection", projection);
	ResourceManager::LoadTexture("awesomeface.png", true, "face");
	ResourceManager::LoadTexture("block.png", false, "block");
	ResourceManager::LoadTexture("block_solid.png", false, "block_solid");
	ResourceManager::LoadTexture("paddle.png", true, "paddle");

	EntityID TopWall = ECSCoord.CreateEntity();
	ECSCoord.AddComponent<C_RigidBody>(TopWall, { glm::vec3(0.0f, -3.0f, 0.0f), 0.0f, glm::vec3((float)WINDOW_WIDTH, 3.0f, 0.0f) });
	ECSCoord.AddComponent<C_Collision>(TopWall, { CollisionClasses::WALL, CollisionShapes::RECT });
	EntityID BottomWall = ECSCoord.CreateEntity();
	ECSCoord.AddComponent<C_RigidBody>(BottomWall, { glm::vec3(0.0f, (float)WINDOW_HEIGHT, 0.0f), 0.0f, glm::vec3((float)WINDOW_WIDTH, 3.0f, 0.0f)});
	ECSCoord.AddComponent<C_Collision>(BottomWall, { CollisionClasses::WALL, CollisionShapes::RECT });
	EntityID LeftWall = ECSCoord.CreateEntity();
	ECSCoord.AddComponent<C_RigidBody>(LeftWall, { glm::vec3(-3.0f, 0.0f, 0.0f), 0.0f, glm::vec3(3.0f, (float)WINDOW_HEIGHT, 0.0f)});
	ECSCoord.AddComponent<C_Collision>(LeftWall, { CollisionClasses::WALL, CollisionShapes::RECT });
	EntityID RightWall = ECSCoord.CreateEntity();
	ECSCoord.AddComponent<C_RigidBody>(RightWall, { glm::vec3((float)WINDOW_WIDTH, 0.0f, 0.0f), 0.0f, glm::vec3(3.0f, (float)WINDOW_HEIGHT, 0.0f)});
	ECSCoord.AddComponent<C_Collision>(RightWall, { CollisionClasses::WALL, CollisionShapes::RECT });


	EntityID Ball = ECSCoord.CreateEntity();
	ECSCoord.AddComponent<C_RigidBody>(Ball, { glm::vec3(385.0f, (float)WINDOW_HEIGHT-50.0f, 0.0f), 0.0f, glm::vec3(30.0f, 30.0f, 0.0f)});
	ECSCoord.AddComponent<C_Model>(Ball, { true, "Ball", "face", glm::vec3(1.0f, 1.0f, 1.0f)});
	ECSCoord.AddComponent<C_Movement>(Ball, {glm::vec2(0.0f, 0.0f), 0.0f, 0.0f});
	ECSCoord.AddComponent<C_Collision>(Ball, { CollisionClasses::BALL, CollisionShapes::CIRCLE });
	ECSCoord.GetComponent<C_Collision>(Ball)->CollisionClassFunctions.insert({ CollisionClasses::WALL, BallToWall });
	ECSCoord.GetComponent<C_Collision>(Ball)->CollisionClassFunctions.insert({ CollisionClasses::PADDLE, BallToPaddle });
	ECSCoord.GetComponent<C_Collision>(Ball)->CollisionEntityFunctions.insert({ BottomWall, BallToBottomWall });;
	ECSCoord.GetComponent<C_Collision>(Ball)->CollisionClassFunctions.insert({ CollisionClasses::BRICK, BallToBrick });
	bool IsBallStuck = true;

	EntityID Paddle = ECSCoord.CreateEntity();
	ECSCoord.AddComponent<C_RigidBody>(Paddle, { glm::vec3(350.0f, (float)WINDOW_HEIGHT-20.0f, 0.0f), 0.0f, glm::vec3(100.0f, 20.0f, 0.0f)});
	ECSCoord.AddComponent<C_Model>(Paddle, { true, "Ball", "paddle", glm::vec3(1.0f, 1.0f, 1.0f)});
	ECSCoord.AddComponent<C_Movement>(Paddle, { glm::vec2(0.0f, 0.0f), 0.0f, 0.0f });
	ECSCoord.AddComponent<C_Collision>(Paddle, { CollisionClasses::PADDLE, CollisionShapes::RECT });

	BreakoutLevel level1("one.lvl");
	CurrentLevel = &level1;
	level1.LoadLevel(WINDOW_WIDTH, WINDOW_HEIGHT/2);

	double deltaTime = 0.f;
	double lastFrame = 0.f;
	while (!RenderingManager.GetWindowShouldClose()) {
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		if (RenderingManager.Keys[GLFW_KEY_SPACE] && IsBallStuck) {
			IsBallStuck = false;
			auto ball_movement = ECSCoord.GetComponent<C_Movement>(Ball);
			ball_movement->velocity = glm::vec2(0.0f, -4.0);
		}
		//process input
		if (RenderingManager.Keys[GLFW_KEY_LEFT_ALT] && RenderingManager.Keys[GLFW_KEY_F4]) {
			RenderingManager.SetWindowShouldClose(true);
		}
		if (RenderingManager.Keys[GLFW_KEY_LEFT]) {
			auto movement_component = ECSCoord.GetComponent<C_Movement>(Paddle);
			movement_component->velocity = glm::vec2(-5.0f, 0.0f);
			if (IsBallStuck) {
				auto ball_movement = ECSCoord.GetComponent<C_Movement>(Ball);
				ball_movement->velocity = glm::vec2(-4.0f, 0.0f);
			}
		}
		if (RenderingManager.Keys[GLFW_KEY_RIGHT]) {
			auto movement_component = ECSCoord.GetComponent<C_Movement>(Paddle);
			movement_component->velocity = glm::vec2(5.0f, 0.0f);
			if (IsBallStuck) {
				auto ball_movement = ECSCoord.GetComponent<C_Movement>(Ball);
				ball_movement->velocity = glm::vec2(4.0f, 0.0f);
			}
		}
		if (!RenderingManager.Keys[GLFW_KEY_LEFT] && !RenderingManager.Keys[GLFW_KEY_RIGHT]) {
			auto movement_component = ECSCoord.GetComponent<C_Movement>(Paddle);
			movement_component->velocity = glm::vec2(0.0f, 0.0f);
			if (IsBallStuck) {
				auto ball_movement = ECSCoord.GetComponent<C_Movement>(Ball);
				ball_movement->velocity = glm::vec2(0.0f, 0.0f);
			}
		}
		//update game state
		ECSCoord.GetSystem<MovementSystem>()->Update();
		ECSCoord.GetSystem<CollisionSystem>()->Update();
		ECSCoord.CleanupEntities();

		//render all objects need to be rendered
		RenderingManager.ClearBuffer();
		ECSCoord.GetSystem<RenderSystem2D>()->Update();

		//swap buffers
		RenderingManager.SwapBuffers();

		//poll events
		RenderingManager.PollEvents();

		//check if the game condition has been met
		if (level1.CheckGameOver()) {
			RenderingManager.SetWindowShouldClose(true);
		}
	}

	return 0;
}
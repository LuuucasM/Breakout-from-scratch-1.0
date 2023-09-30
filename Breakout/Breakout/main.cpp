#include "Imaginengion/Rendering/RenderManager.h"
#include "Imaginengion/Resources/ResourceManager.h"
#include "Imaginengion/ECS/ECSCoordinator.h"
#include "BreakoutLevel.h"

#include "Imaginengion/ECS/Components.h"
#include "Imaginengion/ECS/RenderSystem2D.h"

ECSCoordinator ECSCoord;
std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Textures;

int main() {
	RenderManager RenderingManager(RenderInterfaceType::OPENGL, 800, 600, "Breakout");
	RenderingManager.Init();

	ECSCoord.Init();

	ResourceManager::LoadShader("shader.vs", "shader.fs", nullptr, "Ball");
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
	Shader ball_shader = ResourceManager::GetShader("Ball");
	ball_shader.use();
	ball_shader.setUniform<int>("image", 0);
	ball_shader.setUniform<glm::mat4>("projection", projection);
	ResourceManager::LoadTexture("awesomeface.png", true, "face");
	ResourceManager::LoadTexture("block.png", false, "block");
	ResourceManager::LoadTexture("block_solid.png", false, "block_solid");
	ResourceManager::LoadTexture("paddle.png", true, "paddle");

	EntityID Ball = ECSCoord.CreateEntity();
	ECSCoord.AddComponent<C_RigidBody>(Ball, { glm::vec3(200.0f, 500.0f, 0.0f), 0.0f, glm::vec3(30.0f, 30.0f, 0.0f) });
	ECSCoord.AddComponent<C_Model>(Ball, { true, "Ball", "face", glm::vec3(1.0f, 1.0f, 1.0f)});

	EntityID Paddle = ECSCoord.CreateEntity();
	ECSCoord.AddComponent<C_RigidBody>(Paddle, { glm::vec3(350.0f, 580.0f, 0.0f), 0.0f, glm::vec3(100.0f, 20.0f, 0.0f) });
	ECSCoord.AddComponent<C_Model>(Paddle, { true, "Ball", "paddle", glm::vec3(1.0f, 1.0f, 1.0f)});

	BreakoutLevel level1("one.lvl");

	level1.LoadLevel(800, 600/2);

	double deltaTime = 0.f;
	double lastFrame = 0.f;
	while (!RenderingManager.WindowShouldClose()) {
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//process input
		//update game state
		
		//render all objects need to be rendered
		ECSCoord.GetSystem<RenderSystem2D>()->Update();

		//swap buffers
		RenderingManager.SwapBuffers();

		//poll events
		RenderingManager.PollEvents();
	}

	return 0;
}
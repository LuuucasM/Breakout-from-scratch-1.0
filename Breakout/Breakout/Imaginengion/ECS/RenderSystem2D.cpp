#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "RenderSystem2D.h"
#include "ECSCoordinator.h"
#include "Components.h"
#include "../Resources/ResourceManager.h"

#include <iostream>

extern ECSCoordinator ECSCoord;

void RenderSystem2D::Init(){
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    //create vertex array for redendering 2D with a texture
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RenderSystem2D::Update() {
    
    for (auto entity : Entities) {
        //std::cout << "this happens" << std::endl;
        auto rigidBody = ECSCoord.GetComponent<C_RigidBody>(entity);
        auto entityModel = ECSCoord.GetComponent<C_Model>(entity);
        if (entityModel->bShouldRender) {
            Shader shader = ResourceManager::GetShader(entityModel->ShaderName);
            shader.use();

            //translate, rotate, and scale based on the values in the rigidBody component
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, rigidBody->Position);

            model = glm::translate(model, glm::vec3(0.5f * rigidBody->Size.x, 0.5f * rigidBody->Size.y, 0.5f * rigidBody->Size.z));
            model = glm::rotate(model, glm::radians(rigidBody->Rotation), glm::vec3(0.f, 0.f, 1.f));
            model = glm::translate(model, glm::vec3(-0.5f * rigidBody->Size.x, -0.5f * rigidBody->Size.y, -0.5f * rigidBody->Size.z));

            model = glm::scale(model, rigidBody->Size);
            shader.setUniform<glm::mat4>("model", model);
            shader.setUniform<glm::vec3>("spriteColor", entityModel->color);

            glActiveTexture(GL_TEXTURE0);
            ResourceManager::GetTexture(entityModel->TextureName).Bind();

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
}
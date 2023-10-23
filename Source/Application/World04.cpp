#include "World04.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>



namespace nc
{
    bool World04::Initialize() {


        auto material = GET_RESOURCE(Material, "Materials/grid.mtrl");
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
        m_model->Load("Models/buddha.obj", glm::vec3{ 0 }, glm::vec3{-90, 0, 0});

       // m_transform.position.z = -10.0f;

        return true;
    }

    void World04::Shutdown() {
    }

    void World04::Update(float dt) {
        ENGINE.GetSystem<Gui>()->BeginFrame();        
        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0], 0.1f);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0], 0.1f);
        ImGui::DragFloat3("Scale", &m_transform.scale[0], 0.1f);
        ImGui::End();

        ImGui::Begin("Light");
        ImGui::DragFloat3("Position", &lightPosition[0]);
        ImGui::ColorEdit3("Ambient Color", &lightAColor[0]);
        ImGui::ColorEdit3("Diffuse Color", &lightDColor[0]);
        ImGui::End();

        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? +dt * m_speed : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt * m_speed : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? -dt * m_speed : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? +dt * m_speed : 0;

        m_time += dt;

        auto material = m_model->GetMaterial();

        material->processGUI();
        material->Bind();
        //model matrix
        //glm::mat4 position = glm::translate(glm::mat4{ 1 }, m_position);
        //glm::mat4 rotation = glm::rotate(glm::mat4{ 1 }, glm::radians(m_angle), glm::vec3{ 0, 0, 1 });
        //glm::mat4 model = position * rotation;
        //m_program->SetUniform("offset", glm::vec2{ 0,0 });
       // m_program->SetUniform("tiling", glm::vec2{ 2, 2});
        
        material->GetProgram()->SetUniform("model", m_transform.GetMatrix());

        //view matrix
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 3 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 });
        material->GetProgram()->SetUniform("view", view);

        //projection
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), ENGINE.GetSystem<Renderer>()->GetWidth() / (float)ENGINE.GetSystem<Renderer>()->GetHeight(), 0.01f, 100.0f);
        material->GetProgram()->SetUniform("projection", projection);

        material->GetProgram()->SetUniform("light.position", lightPosition);
        material->GetProgram()->SetUniform("light.Acolor", lightAColor);
        material->GetProgram()->SetUniform("light.Dcolor", lightDColor);


        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World04::Draw(Renderer& renderer) {
        // pre-render
        renderer.BeginFrame();
        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_model->Draw(GL_TRIANGLES);    
        ENGINE.GetSystem<Gui>()->Draw();
        // post-render
        renderer.EndFrame();
    }
}
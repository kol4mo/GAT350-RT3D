#include "World04.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>
#include <random>



namespace nc
{
    bool World04::Initialize() {


        m_material = GET_RESOURCE(Material, "materials/squirrel.mtrl");
        m_model = std::make_shared<Model>();
        m_model->Load("models/squirrel.glb", glm::vec3{ 0, -0.7f, 0 }, glm::vec3{ 0 }, glm::vec3{ 0.4f });
        //m_transform.position.y = -1;

       // m_transform.position.z = -10.0f;

        for (size_t i = 0; i < 3; i++)
        {

            m_lights[i].type = light_t::eType::Point;
            //m_lights[i].position = { randomf(-3, 3), 5, 0};
            m_lights[i].direction = glm::vec3{ 0, -1, 0 };
           // m_lights[i].color = glm::rgbColor(glm::vec3{ randomf() * 360, 1, 1 });
            m_lights[i].intensity = 1;
            m_lights[i].range = 10;
            m_lights[i].innerAngle = 10.0f;
            m_lights[i].outerAngle = 30.0f;
        }
        return true;
    }

    void World04::Shutdown() {
    }

    void World04::Update(float dt) {
        ENGINE.GetSystem<Gui>()->BeginFrame();        


        ImGui::Begin("Light");
        const char* types[] = { "Point", "Directional", "Spot" };
        ImGui::Combo("Type", (int*)&m_lights[m_selected].type, types, 3);
        if (m_lights[m_selected].type != light_t::Directional) ImGui::DragFloat3("Position", &m_lights[m_selected].position[0]);
        if (m_lights[m_selected].type != light_t::Point) ImGui::DragFloat3("Direction", &m_lights[m_selected].direction[0], 0.1f);
        if (m_lights[m_selected].type == light_t::Spot) ImGui::DragFloat("innerAngle", &m_lights[m_selected].innerAngle, 0.1f, 0, m_lights[m_selected].outerAngle);
        if (m_lights[m_selected].type == light_t::Spot) ImGui::DragFloat("outerAngle", &m_lights[m_selected].outerAngle, 0.1f, m_lights[m_selected].innerAngle, 90);
        ImGui::DragFloat("intensity", &m_lights[m_selected].intensity, 1.0f);
        if (m_lights[m_selected].type != light_t::Directional) ImGui::DragFloat("Range", &m_lights[m_selected].range, 0.1f, 0.1f, 40);
        ImGui::ColorEdit3("Diffuse Color", &m_lights[m_selected].color[0]);
        ImGui::End();

        ImGui::Begin("Scene");
        ImGui::ColorEdit3("Ambient Color", &lightAColor[0]);
        ImGui::Separator();

        for (int i = 0; i < 3; i++)
        {
            std::string name = "light" + std::to_string(i);
            if (ImGui::Selectable(name.c_str(), m_selected == i)) m_selected = i;
        }
        ImGui::End();

        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? +dt * m_speed : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt * m_speed : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? -dt * m_speed : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? +dt * m_speed : 0;

        m_time += dt;



        m_material->ProcessGui();
        m_material->Bind();
        //model matrix
        //glm::mat4 position = glm::translate(glm::mat4{ 1 }, m_position);
        //glm::mat4 rotation = glm::rotate(glm::mat4{ 1 }, glm::radians(m_angle), glm::vec3{ 0, 0, 1 });
        //glm::mat4 model = position * rotation;
        //m_program->SetUniform("offset", glm::vec2{ 0,0 });
       // m_program->SetUniform("tiling", glm::vec2{ 2, 2});
        
        m_material->GetProgram()->SetUniform("model", m_transform.GetMatrix());

        //view matrix
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 3 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 });
        m_material->GetProgram()->SetUniform("view", view);

        //projection
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), ENGINE.GetSystem<Renderer>()->GetWidth() / (float)ENGINE.GetSystem<Renderer>()->GetHeight(), 0.01f, 100.0f);
        m_material->GetProgram()->SetUniform("projection", projection);

        for (size_t i = 0; i < 3; i++)
        {
            std::string name = "lights[" + std::to_string(i) + "].";
            m_material->GetProgram()->SetUniform(name + "type", m_lights[i].type);
            m_material->GetProgram()->SetUniform(name + "position", m_lights[i].position);
            m_material->GetProgram()->SetUniform(name + "direction", glm::normalize(m_lights[i].direction));
            m_material->GetProgram()->SetUniform(name + "intensity", m_lights[i].intensity);
            m_material->GetProgram()->SetUniform(name + "Dcolor", m_lights[i].color);
            m_material->GetProgram()->SetUniform(name + "range", m_lights[i].range);
            m_material->GetProgram()->SetUniform(name + "innerAngle", glm::radians(m_lights[i].innerAngle));
            m_material->GetProgram()->SetUniform(name + "outerAngle", glm::radians(m_lights[i].outerAngle));
        }
            m_material->GetProgram()->SetUniform("ambientColor", lightAColor);

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World04::Draw(Renderer& renderer) {
        // pre-render
        renderer.BeginFrame();
        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_material->Bind();
        m_model->Draw();    

        ENGINE.GetSystem<Gui>()->Draw();
        // post-render
        renderer.EndFrame();
    }
}
#include "World05.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include "Core/Random.h"
#include "Core/StringUtils.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>



namespace nc
{
    bool World05::Initialize() {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("scenes/scene.json");
        m_scene->Initialize();


        {
            auto actor = CREATE_CLASS(Actor);
            actor->name = "light1";
            actor->transform.position = glm::vec3{ 3, 3, 3 };
            auto lightComponent = CREATE_CLASS(LightComponent);
            lightComponent->type = LightComponent::eType::Point;
            lightComponent->color = glm::vec3{ 1, 1, 1 };//glm::rgbColor(glm::vec3{ randomf() * 360, 1, 1 });
            lightComponent->intensity = 1;
            lightComponent->range = 20;
            lightComponent->innerAngle = 10.0f;
            lightComponent->outerAngle = 30.0f;
            actor->AddComponent(std::move(lightComponent));
            m_scene->Add(std::move(actor));
        }

        auto actor = CREATE_CLASS_BASE(Actor, "tree");
        std::string name = "tree"; CreateUniqueStr(name);
        actor->name = name;
        actor->transform.position = glm::vec3{ randomf(-10,10) , 0, randomf(-10,10) };
        actor->transform.scale = glm::vec3{ random(0.5f, 3.0f), random(0.5f, 3.0f), 0 };
        actor->Initialize();
        m_scene->Add(std::move(actor));

        return true;
    }

    void World05::Shutdown() {
    }

    void World05::Update(float dt) {
        ENGINE.GetSystem<Gui>()->BeginFrame();        

        m_scene->Update(dt);
        m_scene->ProcessGui();

        auto actor = m_scene->GetActorByName<Actor>("actor1");
        /*
        actor->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? +dt * m_speed : 0;
        actor->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt * m_speed : 0;
        actor->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? -dt * m_speed : 0;
        actor->transform.position.z += ENGINE.GetSystem<InsputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? +dt * m_speed : 0;
        */
        m_time += dt;

        auto material = actor->GetComponent<ModelComponent>()->material;

        material->ProcessGui();
        material->Bind();
        
        auto m_material = GET_RESOURCE(Material, "materials/refraction.mtrl");
        if (material) {
            ImGui::Begin("Refraction");

            m_refraction = 1.0f + std::fabs(std::sin(m_time * 0.1));
            ImGui::DragFloat("IOR", &m_refraction, 0.01f, 1.0f, 3.0f);
            auto program = m_material->GetProgram();
            program->Use();
            program->SetUniform("ior", m_refraction);
            
            ImGui::End();
        }
        //model matrix
        
        material->GetProgram()->SetUniform("ambientColor", lightAColor);
        

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World05::Draw(Renderer& renderer) {
        // pre-render
        renderer.BeginFrame();
        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_scene->Draw(renderer);    
        ENGINE.GetSystem<Gui>()->Draw();
        // post-render
        renderer.EndFrame();
    }
}
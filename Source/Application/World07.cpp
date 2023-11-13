#include "World07.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include "Core/Random.h"
#include "Core/StringUtils.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>



namespace nc
{
    bool World07::Initialize() {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("scenes/scene_shadow.json");
        m_scene->Initialize();

        auto texture = std::make_shared<Texture>();
        texture->CreateTexture(2048, 2048);
        ADD_RESOURCE("fb_texture", texture);

        auto frameBuffer = std::make_shared<Framebuffer>();
        frameBuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb", frameBuffer);

        auto material = GET_RESOURCE(Material, "mAterials/postprocess.mtrl");
        if (material) {
            material->albedoTexture = texture;
        }

        return true;
    }

    void World07::Shutdown() {
    }

    void World07::Update(float dt) {
        m_time += dt;
        ENGINE.GetSystem<Gui>()->BeginFrame();        

        m_scene->Update(dt);
        m_scene->ProcessGui();

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World07::Draw(Renderer& renderer) {
        //*** PASS 1 ***
        /*
        m_scene->GetActorByName("postprocess")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.BeginFrame({0,0,0.5});

        framebuffer->Unbind();
        */
        //*** PASS 2 ***
        //m_scene->GetActorByName("postprocess")->active = true;
        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->Draw(renderer);    
        //m_scene->GetActorByName("postprocess")->Draw(renderer);

        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();
    }
}
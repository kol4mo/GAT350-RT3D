#include "World01.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

namespace nc
{
    bool World01::Initialize() {
            m_Ground = {0,0};
            m_Sky = {0,0};
            m_Position = {0,0};
        return true;
    }

    void World01::Shutdown() {
    }

    void World01::Update(float dt) {
        m_angle += 90 * dt; 
        m_Position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? +dt * 2 : 0;
        m_Position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt * 2 : 0;
        m_time += dt;
    }

    void World01::Draw(Renderer& renderer) {
        // pre-render
        renderer.BeginFrame();

        // render
        glPushMatrix();
            glTranslatef(m_Ground.x, m_Ground.y, 0);
            //glRotatef(m_angle, 1, 1, 1);
            //glScalef((sin(m_time * 2) + 1) * 0.5f, 1, 1);

            glBegin(GL_QUADS);

            glColor3f(0, 1, 0);
            glVertex2f(-1, -1);

            glColor3f(0, 1, 0);
            glVertex2f(-1, -0.5f);

            glColor3f(0, 1, 0);
            glVertex2f(1, -0.5f);

            glColor3f(0, 1, 0);
            glVertex2f(1, -1);

            glEnd();

            glTranslatef(m_Sky.x, m_Sky.y, 0);
            //glRotatef(m_angle, 1, 1, 1);
            //glScalef((sin(m_time * 2) + 1) * 0.5f, 1, 1);

            glBegin(GL_QUADS);

            glColor3f(0, 0, 1);
            glVertex2f(-1, -0.5f);

            glColor3f(0, 0, 1);
            glVertex2f(-1, 1);

            glColor3f(0, 0, 1);
            glVertex2f(1, 1);

            glColor3f(0, 0, 1);
            glVertex2f(1, -0.5f);

            glEnd();
            glTranslatef(m_Position.x, m_Position.y, 0);
            //glRotatef(m_angle, 1, 1, 1);
            //glScalef((sin(m_time * 2) + 1) * 0.5f, 1, 1);

            glBegin(GL_QUADS);

            glColor3f(1, 0, 0);
            glVertex2f(-1, -0.75f);

            glColor3f(1, 0, 0);
            glVertex2f(-1, -0.25f);

            glColor3f(1, 0, 0);
            glVertex2f(-0.75f, -0.25f);

            glColor3f(1, 0, 0);
            glVertex2f(-0.75f, -0.75f);

            glEnd();
        glPopMatrix();

        // post-render
        renderer.EndFrame();
    }
}
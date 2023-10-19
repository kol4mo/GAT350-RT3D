#pragma once
#include "Framework/World.h"
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"
#include <vector>

namespace nc
{
	class World04 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time = 0;
		float m_speed = 5;
		
		glm::vec3 lightPosition = {0 ,8, 0};
		glm::vec3 lightAColor = { 0.2, 0.2, 0.2 };
		glm::vec3 lightDColor = { 1, 1, 1 };

		Transform m_transform;
		res_t<Model> m_model;
	};
}
#pragma once
#include "Component.h"

namespace nc
{
	class RotateComponent : public Component
	{
	public:
		CLASS_DECLARATION(RotateComponent)

		bool Initialize() override;
		void ProcessGui();
		void Update(float dt) override;

	public:

		glm::vec3 eulerAngle = { 0,0,0 };
	};
}
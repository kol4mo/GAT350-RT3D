#include "RotateComponent.h"
#include "Core/Math/MathUtils.h"
#include "Framework/Actor.h"

namespace nc {
	CLASS_DEFINITION(RotateComponent);

	bool RotateComponent::Initialize()
	{
		return true;
	}

	void RotateComponent::Update(float dt)
	{
		glm::quat rotation = EulerToQuaternion(eulerAngle * dt);
		m_owner->transform.rotation = m_owner->transform.rotation * rotation;
	}
	void RotateComponent::Read(const nc::json_t& value)
	{
		READ_DATA(value, eulerAngle);
	}
}

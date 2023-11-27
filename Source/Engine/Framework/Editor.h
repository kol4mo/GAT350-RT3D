#pragma once

namespace nc {
	class Editor {
	public:
		void Update();
		void ProcessGui(class Scene* scene);

	private:
		enum eType
		{
			TEXTURE,
			MODEL,
			MATERIAL,
			SHADER
		};

		eType type = eType::TEXTURE;
		//type selectedType = null;
		bool m_active = true;
		class Object* m_selected = nullptr;
	};
}
#ifndef VR_HUB_H
#define VR_HUB_H

#include "Game.h"
#include "Setting.h"
#include "VelociRun.h"
#include "VRunMenu.h"
//#include "VRunSettings.h"

namespace Engine {
	class VRunHub :public Engine::Game {
	public:
		VRunHub(Setting* setting);
		~VRunHub();
		virtual void Init();
		virtual void Update();
		virtual void Render();
	};
}

#endif
#pragma once
/*
#include "Game.h"
#include "PhysicsFactory.h"
#include <gbtBulletDynamicsCommon.h>*/
#include "Game.h"
#include "GameComponent.h"
#include "PhysicsController.h"
#include "PhysicsFactory.h"
#include <btBulletDynamicsCommon.h>
#include <vector>
#include <iostream>
#include "Content.h"
#include "VectorDrawer.h"
#include "XBoxController.h"
#include "Steerable3DController.h"
#include "SteeringControler.h"
#include "Params.h"
#include "FountainEffect.h"
#include "Box.h"
#include "SnowEffect.h"
#include "SpiderController.h"

namespace BGE
{
	class JumpingSpider :
		public Game
	{
	public:
		//standard methods
		JumpingSpider(void);
		~JumpingSpider(void);
		bool Initialise();
		void Update();
		void Cleanup();

		shared_ptr<SpiderController> spider;
		shared_ptr<SpiderController> spider2;
		shared_ptr<SpiderController> spider3;
		shared_ptr<SpiderController> spider4;

		float elapsed;
		float timeChange;
		int count;
	};
}

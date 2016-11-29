#pragma once
/*
#include "Game.h"
#include "PhysicsFactory.h"
#include <btBulletDynamicsCommon.h>*/
#include "Game.h"
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

namespace BGE
{
	class JumpingSpider :
		public Game
	{
	public:
		JumpingSpider(void);
		~JumpingSpider(void);
		bool Initialise();
		void Update(float timeDelta);
		void Cleanup();

		shared_ptr<PhysicsController> CreateSpider();
		shared_ptr<PhysicsController> CreateLeg(bool leftLeg);
		shared_ptr<PhysicsController> CreateBody(vector<shared_ptr<PhysicsController>> limbs);
		shared_ptr<PhysicsController> spider;
		shared_ptr<PhysicsController> forebody;
		shared_ptr<PhysicsController> abdomen;
		shared_ptr<PhysicsController> leg_thigh;
		shared_ptr<PhysicsController> leg_calf;
	};
}

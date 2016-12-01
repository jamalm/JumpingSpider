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

namespace BGE
{
	class JumpingSpider :
		public Game
	{
	private:

		shared_ptr<PhysicsController> CreateLeg(bool leftLeg);
		shared_ptr<PhysicsController> CreateBody(vector<shared_ptr<PhysicsController>> limbs, glm::vec3 scale);
	public:
		//standard methods
		JumpingSpider(void);
		~JumpingSpider(void);
		bool Initialise();
		void Update();
		void Cleanup();

		//custom methods
		void Walk(bool inverse);
		shared_ptr<PhysicsController> CreateSpider(float width, float height, float length);

		//custom variables
		vector<shared_ptr<PhysicsController>> limbs;
		vector<btHingeConstraint*> joints;
		vector<shared_ptr<PhysicsController>> spiders;
		bool alternate;
		float elapsed;
		float timeToSpawn;
		int velocity;
		int strength;
	};
}

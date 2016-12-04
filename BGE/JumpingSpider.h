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
		//for creating the spider
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
		void forward();
		void jump();
		void left(glm::vec3 torque);
		void right(glm::vec3 torque);
		void animate();
		
		shared_ptr<PhysicsController> CreateSpider(float width, float height, float length);

		//custom variables
		vector<shared_ptr<PhysicsController>> limbs;
		vector<btHingeConstraint*> joints;
		shared_ptr<PhysicsController> body;
		shared_ptr<PhysicsController> abdomen;

		//for leg timing and alternations
		bool alternate;
		float elapsed;
		float timeToMove;

		//for movement of legs
		int velocity;
		int strength;
		//for movement of rigidbody
		float force;

		//for rigidbody turning
		glm::vec3 torque;
		glm::vec3 hardTorque;
	};
}

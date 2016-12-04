#include "JumpingSpider.h"
#include "Utils.h"
#include "Content.h"
#include "VectorDrawer.h"

using namespace BGE;

JumpingSpider::JumpingSpider(void)
{

}

JumpingSpider::~JumpingSpider(void)
{

}

bool JumpingSpider::Initialise()
{
	//for autonomous spiders
	elapsed = 0;
	timeChange = 5.0f;
	count = 0;
	//standard calls to create scene
	physicsFactory->CreateGroundPhysics();
	physicsFactory->CreateCameraPhysics();
	setGravity(glm::vec3(0, -9, 0));

	//create the spider 
	spider = make_shared<SpiderController>(3, 3, 16);

	/*
	spider2 = make_shared<SpiderController>(glm::vec3(15,0,0));
	spider3 = make_shared<SpiderController>(glm::vec3(15, 0, 0));
	spider4 = make_shared<SpiderController>(glm::vec3(15, 0, 0));
	*/
	return Game::Initialise();
}

void JumpingSpider::Update()
{
	
	glm::vec3 torque = spider->torque;
	glm::vec3 hardTorque = spider->hardTorque;
	//move forward
	if (keyState[SDL_SCANCODE_UP])
	{
		spider->forward();
	}
	//sharp turns
	else if (keyState[SDL_SCANCODE_LSHIFT] && keyState[SDL_SCANCODE_LEFT])
	{
		spider->left(hardTorque);
	}
	
	else if (keyState[SDL_SCANCODE_LSHIFT] && keyState[SDL_SCANCODE_RIGHT])
	{
		spider->right(-hardTorque);
	}
	//normal turns
	else if(keyState[SDL_SCANCODE_LEFT])
	{
		spider->left(torque);
	}
	else if (keyState[SDL_SCANCODE_RIGHT])
	{
		spider->right(-torque);
	} 
	//jumping/flying
	else if (keyState[SDL_SCANCODE_SPACE])
	{
		spider->jump();
	}
	//else slow spider down to stop
	else {
		spider->body->rigidBody->setFriction(100);
		spider->abdomen->rigidBody->setFriction(100);
	}

	/*
	//this is all extra and for fun, alternate behaviours of autonomous spiders
	///
	///
	///
	///
	if (elapsed > timeChange)
	{
		if (count < 4)
		{
			count++;
		}
		else {
			count = 1;
		}
	}
	else
	{
		elapsed += Time::deltaTime;
	}
	switch (count)
	{
	case 1:
		spider2->forward();
		spider3->left(torque);
		spider4->right(-torque);
		break;
	case 2:
		spider2->left(torque);
		spider3->right(-hardTorque);
		spider4->forward();
		break;
	case 3:
		spider2->forward();
		spider3->jump();
		spider4->forward();
		break;
	case 4:
		spider2->jump();
		spider3->forward();
		spider4->jump();
		break;
	}
	*/
	
	/*
	spider4->Update();
	spider2->Update();
	spider3->Update();

	*/
	spider->Update();
	Game::Update();
}

void JumpingSpider::Cleanup()
{
	Game::Cleanup();
}

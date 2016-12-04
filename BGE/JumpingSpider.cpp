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
	elapsed = 0.0f;
	timeToSpawn = 1.0f;

	force = 1000.0f;

	velocity = 10000000;
	strength = 10000000;
	alternate = true;

	physicsFactory->CreateGroundPhysics();
	physicsFactory->CreateCameraPhysics();
	setGravity(glm::vec3(0, -9, 0));

	
	//box1 = physicsFactory->CreateBox(5,5,5,glm::vec3(0,0,0),glm::quat());
	CreateSpider(3, 3, 16);

	return Game::Initialise();

}

void JumpingSpider::Update()
{
	btVector3 position = GLToBtVector(body->transform->position);
	//body->rigidBody->activate();
	if (keyState[SDL_SCANCODE_UP])
	{
		body->rigidBody->setFriction(0);
		abdomen->rigidBody->setFriction(0);
		body->rigidBody->applyCentralForce(GLToBtVector(body->transform->look * force));
		
		if (elapsed > timeToSpawn) {
			Walk(alternate);
			alternate = !alternate;
			elapsed = 0;

		}
		else {
			elapsed += Time::deltaTime;
		}
	}
	else if (keyState[SDL_SCANCODE_LSHIFT] && keyState[SDL_SCANCODE_LEFT])
	{
		body->rigidBody->setFriction(0);
		abdomen->rigidBody->setFriction(0);
		body->rigidBody->applyTorque(btVector3(force * 10, 0, 0));

		if (elapsed > timeToSpawn) {
			Walk(alternate);
			alternate = !alternate;
			elapsed = 0;

		}
		else {
			elapsed += Time::deltaTime;
		}
	}
	else if (keyState[SDL_SCANCODE_LSHIFT] && keyState[SDL_SCANCODE_RIGHT])
	{
		body->rigidBody->setFriction(0);
		abdomen->rigidBody->setFriction(0);
		body->rigidBody->applyTorque(btVector3(-force * 10, 0, 0));

		if (elapsed > timeToSpawn) {
			Walk(alternate);
			alternate = !alternate;
			elapsed = 0;

		}
		else {
			elapsed += Time::deltaTime;
		}
	}
	else if(keyState[SDL_SCANCODE_LEFT])
	{
		body->rigidBody->setFriction(0);
		abdomen->rigidBody->setFriction(0);
		body->rigidBody->applyTorque(btVector3(force, 0, 0));
		
		if (elapsed > timeToSpawn) {
			Walk(alternate);
			alternate = !alternate;
			elapsed = 0;

		}
		else {
			elapsed += Time::deltaTime;
		}
	}
	else if (keyState[SDL_SCANCODE_RIGHT])
	{
		body->rigidBody->setFriction(0);
		abdomen->rigidBody->setFriction(0);
		body->rigidBody->applyTorque(btVector3(-force, 0, 0));

		if (elapsed > timeToSpawn) {
			Walk(alternate);
			alternate = !alternate;
			elapsed = 0;

		}
		else {
			elapsed += Time::deltaTime;
		}
	} 
	
	else {
		body->rigidBody->setFriction(100);
		abdomen->rigidBody->setFriction(100);
	}
	
	Game::Update();
}

void JumpingSpider::Cleanup()
{
	Game::Cleanup();
}

//creates legs for the spider
shared_ptr<PhysicsController> JumpingSpider::CreateLeg(bool leftLeg)
{
	glm::vec3 position = glm::vec3(0, 10, 0);
	if (leftLeg) {
		glm::quat qupper = glm::angleAxis(90.0f, glm::vec3(0, 0, 1));
		glm::quat qlower = glm::angleAxis(-90.0f, glm::vec3(0, 0, 1));
		position = glm::vec3(5, 10, 0);
		shared_ptr<PhysicsController> segment = physicsFactory->CreateBox(4, .5, .5, position, glm::quat());
		position = glm::vec3(10, 10, 0);
		shared_ptr<PhysicsController> segment2 = physicsFactory->CreateBox(6, .5, .5, position, glm::quat());
		segment2->rigidBody->setFriction(btScalar(0.0f));
		btTransform t1, t2;

		t1.setIdentity();
		t2.setIdentity();
		t1.setOrigin(btVector3(2.5, 0, 0));
		t2.setRotation(GLToBtQuat(glm::angleAxis(90.0f, glm::vec3(0, 0, 1))));
		t2.setOrigin(btVector3(-2.5, 0, 0));

		btFixedConstraint* hinge2 =
			new btFixedConstraint(*segment->rigidBody, *segment2->rigidBody, t1, t2);
		dynamicsWorld->addConstraint(hinge2);

		return segment;
	}
	else {
		glm::quat qupper = glm::angleAxis(-90.0f, glm::vec3(0, 0, 1));
		glm::quat qlower = glm::angleAxis(90.0f, glm::vec3(0, 0, 1));
		position = glm::vec3(-5, 10, 0);
		shared_ptr<PhysicsController> segment = physicsFactory->CreateBox(4, .5, .5, position, glm::quat());
		position = glm::vec3(-10, 10, 0);
		shared_ptr<PhysicsController> segment2 = physicsFactory->CreateBox(6, .5, .5, position, glm::quat());
		segment2->rigidBody->setFriction(btScalar(0.0f));
		btTransform t1, t2;

		t1.setIdentity();
		t2.setIdentity();
		t1.setOrigin(btVector3(-2.5, 0, 0));
		t2.setRotation(GLToBtQuat(glm::angleAxis(-90.0f, glm::vec3(0, 0, 1))));
		t2.setOrigin(btVector3(2.5, 0, 0));

		btFixedConstraint* hinge2 =
			new btFixedConstraint(*segment->rigidBody, *segment2->rigidBody, t1, t2);
		dynamicsWorld->addConstraint(hinge2);

		return segment;
	}



}

//creates the forebody and abdomen, attaches the legs to the body 
shared_ptr<PhysicsController> JumpingSpider::CreateBody(vector<shared_ptr<PhysicsController>> limbs, glm::vec3 scale)
{
	glm::vec3 position = glm::vec3(0, 10, 0);

	glm::vec3 offset;
	float width = scale.x;
	float height = scale.y;
	float length = scale.z;
	float turningAngle = glm::half_pi<float>() / 4;

	offset = glm::vec3(0, 0, length + 1);

	shared_ptr<PhysicsController> forebody = physicsFactory->CreateBox(width, height, length, position, glm::quat());
	abdomen = physicsFactory->CreateBox(width + 1, height + 1, length /2, position + offset, glm::quat());
	forebody->rigidBody->setFriction(0);
	abdomen->rigidBody->setFriction(0);
	btFixedConstraint* forebody_abdomen;

	//adding constraints to forebody

	btTransform transformA;
	btTransform transformB;

	//attach forebody to abdomen
	transformA.setIdentity();
	transformB.setIdentity();
	transformA.setOrigin(btVector3(0, 0, length));
	transformB.setOrigin(btVector3(0, 0, 0));
	forebody_abdomen = new btFixedConstraint(*forebody->rigidBody, *abdomen->rigidBody, transformA, transformB);
	dynamicsWorld->addConstraint(forebody_abdomen);
	
	//connect legs to abdomen
	for (int i = 0; i < 8; i++)
	{
		if (i<4) {
			//right legs first
			btVector3 bodyPivot = btVector3(-width - 1, 1, (length / 2) - ((length / 4)*i));
			btVector3 legPivot = btVector3(0, 0, 0);

			btHingeConstraint* rightHinge
				= new btHingeConstraint(*forebody->rigidBody, *limbs.at(i)->rigidBody, bodyPivot, legPivot, btVector3(0, 1, 0), btVector3(-1, 1, 0), true);
			rightHinge->setLimit(-turningAngle, turningAngle);
			//rightHinge->enableAngularMotor(true, 10, 10);
			joints.push_back(rightHinge);
			dynamicsWorld->addConstraint(rightHinge);
		}
		else {

			//now left legs 
			btVector3 bodyPivot = btVector3(width + 1, 1, (length / 2) - ((length / 4)*(i - 4)));
			btVector3 legPivot = btVector3(0, 0, 0);

			btHingeConstraint* leftHinge
				= new btHingeConstraint(*forebody->rigidBody, *limbs.at(i)->rigidBody, bodyPivot, legPivot, btVector3(0, 1, 0), btVector3(1, 1, 0), true);
			leftHinge->setLimit(-turningAngle, turningAngle);
			//leftHinge->enableAngularMotor(true, 10, 10);
			joints.push_back(leftHinge);
			dynamicsWorld->addConstraint(leftHinge);
		}
	}
	return forebody;
}

shared_ptr<PhysicsController> JumpingSpider::CreateSpider(float _width, float _height, float _length)
{
	float width = _width;
	float height = _height;
	float length = _length;

	glm::vec3 scale = glm::vec3(width, height, length);

	//body parts and legs
	for (int i = 0; i < 4; i++)
	{
		limbs.push_back(CreateLeg(false));
	}
	for (int i = 0; i < 4; i++)
	{
		limbs.push_back(CreateLeg(true));
	}
	body = CreateBody(limbs, scale);


	return body;
}


void JumpingSpider::Walk(bool inverse)
{
	if (inverse)
	{
		for (int i = 0; i < 8; i++)
		{
			//if leg index is an even number, move alternative to odd indexed legs
			if (i % 2 == 0)
			{
				joints.at(i)->enableAngularMotor(true, velocity, strength);
			}
			else {
				joints.at(i)->enableAngularMotor(true, -velocity, strength);
			}
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			//if leg index is an even number, move alternative to odd indexed legs
			if (i % 2 == 0)
			{
				joints.at(i)->enableAngularMotor(true, -velocity, strength);
			}
			else {
				joints.at(i)->enableAngularMotor(true, velocity, strength);
			}
		}
	}

}
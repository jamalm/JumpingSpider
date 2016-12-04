#include "SpiderController.h"
#include "Utils.h"
#include "Content.h"
#include "VectorDrawer.h"

using namespace BGE;

//specific size (for user)
SpiderController::SpiderController(float width, float height, float length)
{
	spawnPosition = glm::vec3(0, 10, 0);
	elapsed = 0.0f;
	timeToMove = 1.0f;

	//strength of movement of rigidbody
	force = 1000.0f;

	//velocity of leg motors
	velocity = 10000000;
	//maximpulse of legs 
	strength = 10000000;
	//alternating boolean for leg motor direction
	alternate = true;
	CreateSpider(width, height, length);
}

//for autonomous spiders
SpiderController::SpiderController(glm::vec3 position)
{
	spawnPosition = position;
	elapsed = 0.0f;
	timeToMove = 1.0f;

	//strength of movement of rigidbody
	force = 1000.0f;

	//velocity of leg motors
	velocity = 10000000;
	//maximpulse of legs 
	strength = 10000000;
	//alternating boolean for leg motor direction
	alternate = true;



	CreateSpider(3, 3, 6);
	body->rigidBody->setFriction(20);
	abdomen->rigidBody->setFriction(20);

}

//default
SpiderController::SpiderController()
{
	spawnPosition = glm::vec3(0,10,0);
	elapsed = 0.0f;
	timeToMove = 1.0f;

	//strength of movement of rigidbody
	force = 1000.0f;

	//velocity of leg motors
	velocity = 10000000;
	//maximpulse of legs 
	strength = 10000000;
	//alternating boolean for leg motor direction
	alternate = true;

	

	CreateSpider(3,3,6);
	body->rigidBody->setFriction(20);
	abdomen->rigidBody->setFriction(20);
	
}

SpiderController::~SpiderController()
{

}

void SpiderController::Update()
{
	torque = glm::cross(body->transform->right*force, body->transform->look*4.0f);
	hardTorque = glm::cross(body->transform->right*(force * 10), body->transform->look*5.0f);

	GameComponent::Update();
}


void SpiderController::Cleanup()
{
	GameComponent::Cleanup();
}

//creates legs for the spider
shared_ptr<PhysicsController> SpiderController::CreateLeg(bool leftLeg)
{

	glm::vec3 position = spawnPosition;

	//creates opposite angles for left and right legs|
	if (leftLeg) {
		//creates lower leg and upper leg segments
		position = glm::vec3(5, 10, 0);
		shared_ptr<PhysicsController> segment = Game::Instance()->physicsFactory->CreateBox(4, .5, .5, position, glm::quat());
		position = glm::vec3(10, 10, 0);
		shared_ptr<PhysicsController> segment2 = Game::Instance()->physicsFactory->CreateBox(6, .5, .5, position, glm::quat());
		segment2->rigidBody->setFriction(btScalar(0.0f));

		//create pivot points for the constraints
		btTransform t1, t2;

		t1.setIdentity();
		t2.setIdentity();
		t1.setOrigin(btVector3(2.5, 0, 0));
		t2.setRotation(GLToBtQuat(glm::angleAxis(90.0f, glm::vec3(0, 0, 1))));
		t2.setOrigin(btVector3(-2.5, 0, 0));

		//create hinge
		btFixedConstraint* hinge2 =
			new btFixedConstraint(*segment->rigidBody, *segment2->rigidBody, t1, t2);
		Game::Instance()->dynamicsWorld->addConstraint(hinge2);

		return segment;
	}
	else {
		position = glm::vec3(-5, 10, 0);
		shared_ptr<PhysicsController> segment = Game::Instance()->physicsFactory->CreateBox(4, .5, .5, position, glm::quat());
		position = glm::vec3(-10, 10, 0);
		shared_ptr<PhysicsController> segment2 = Game::Instance()->physicsFactory->CreateBox(6, .5, .5, position, glm::quat());
		segment2->rigidBody->setFriction(btScalar(0.0f));

		btTransform t1, t2;

		t1.setIdentity();
		t2.setIdentity();
		t1.setOrigin(btVector3(-2.5, 0, 0));
		t2.setRotation(GLToBtQuat(glm::angleAxis(-90.0f, glm::vec3(0, 0, 1))));
		t2.setOrigin(btVector3(2.5, 0, 0));

		btFixedConstraint* hinge2 =
			new btFixedConstraint(*segment->rigidBody, *segment2->rigidBody, t1, t2);
		Game::Instance()->dynamicsWorld->addConstraint(hinge2);

		return segment;
	}



}

//creates the forebody and abdomen, attaches the legs to the body 
shared_ptr<PhysicsController> SpiderController::CreateBody(vector<shared_ptr<PhysicsController>> limbs, glm::vec3 scale)
{
	glm::vec3 position = spawnPosition;

	glm::vec3 offset;
	//this is for the forebody
	float width = scale.x;
	float height = scale.y;
	float length = scale.z;
	//angle limitations for hinge joints on upper legs
	float turningAngle = glm::half_pi<float>() / 4;
	//this is for the abdomen on the back
	offset = glm::vec3(0, 0, length + 1);

	shared_ptr<PhysicsController> forebody = Game::Instance()->physicsFactory->CreateBox(width, height, length, position, glm::quat());
	abdomen = Game::Instance()->physicsFactory->CreateBox(width + 1, height + 1, length / 2, position + offset, glm::quat());
	//forebody->rigidBody->setFriction(0);
	//abdomen->rigidBody->setFriction(0);
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
	Game::Instance()->dynamicsWorld->addConstraint(forebody_abdomen);

	//connect legs to abdomen (hinge names are mixed up , doesn't make a difference)
	for (int i = 0; i < 8; i++)
	{
		if (i<4) {
			//left legs first
			btVector3 bodyPivot = btVector3(-width - 1, 1, (length / 2) - ((length / 4)*i));
			btVector3 legPivot = btVector3(0, 0, 0);

			btHingeConstraint* rightHinge
				= new btHingeConstraint(*forebody->rigidBody, *limbs.at(i)->rigidBody, bodyPivot, legPivot, btVector3(0, 1, 0), btVector3(-1, 1, 0), true);
			rightHinge->setLimit(-turningAngle, turningAngle);
			//rightHinge->enableAngularMotor(true, 10, 10);
			joints.push_back(rightHinge);
			Game::Instance()->dynamicsWorld->addConstraint(rightHinge);
		}
		else {

			//now right legs 
			btVector3 bodyPivot = btVector3(width + 1, 1, (length / 2) - ((length / 4)*(i - 4)));
			btVector3 legPivot = btVector3(0, 0, 0);

			btHingeConstraint* leftHinge
				= new btHingeConstraint(*forebody->rigidBody, *limbs.at(i)->rigidBody, bodyPivot, legPivot, btVector3(0, 1, 0), btVector3(1, 1, 0), true);
			leftHinge->setLimit(-turningAngle, turningAngle);
			//leftHinge->enableAngularMotor(true, 10, 10);
			joints.push_back(leftHinge);
			Game::Instance()->dynamicsWorld->addConstraint(leftHinge);
		}
	}
	return forebody;
}

//time to put it all together
shared_ptr<PhysicsController> SpiderController::CreateSpider(float _width, float _height, float _length)
{
	//for forebody
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

//animation for leg movement, using angular motors to rotate the hinges
void SpiderController::Walk(bool inverse)
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

//move forward relative to spider's look vector
void SpiderController::forward()
{
	body->rigidBody->setFriction(0);
	abdomen->rigidBody->setFriction(0);
	body->rigidBody->applyCentralForce(GLToBtVector(body->transform->look * force));

	animate();
}

//turn left 
void SpiderController::left(glm::vec3 torque)
{
	body->rigidBody->setFriction(0);
	abdomen->rigidBody->setFriction(0);
	body->rigidBody->applyTorque(GLToBtVector(torque));

	animate();
}

//turn right
void SpiderController::right(glm::vec3 torque)
{
	body->rigidBody->setFriction(0);
	abdomen->rigidBody->setFriction(0);
	body->rigidBody->applyTorque(GLToBtVector(torque));

	animate();
}
//lift off the ground
void SpiderController::jump()
{
	body->rigidBody->setFriction(0);
	abdomen->rigidBody->setFriction(0);
	body->rigidBody->applyCentralForce(GLToBtVector(body->transform->up * (force * 10)));
}

//regulate the leg movements
void SpiderController::animate()
{
	if (elapsed > timeToMove) {
		Walk(alternate);
		alternate = !alternate;
		elapsed = 0;

	}
	else {
		elapsed += Time::deltaTime;
	}
}
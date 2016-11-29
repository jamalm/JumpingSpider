#include "JumpingSpider.h"


using namespace BGE;

JumpingSpider::JumpingSpider(void)
{

}


JumpingSpider::~JumpingSpider(void)
{

}

bool JumpingSpider::Initialise()
{
	physicsFactory->CreateGroundPhysics();
	physicsFactory->CreateCameraPhysics();
	setGravity(glm::vec3(0, -9, 0));

	

	return Game::Initialise();
}

void JumpingSpider::Update(float timeDelta)
{
	const Uint8 * KeyState = Game::Instance()->GetKeyState();

	if (KeyState[SDL_SCANCODE_V]) 
	{
		CreateSpider(4, 3, 6);
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
		shared_ptr<PhysicsController> segment = physicsFactory->CreateBox(4, .5, .5, position, qupper);
		position = glm::vec3(10, 10, 0);
		shared_ptr<PhysicsController> segment2 = physicsFactory->CreateBox(8, .5, .5, position, qlower);

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
		shared_ptr<PhysicsController> segment2 = physicsFactory->CreateBox(8, .5, .5, position, qlower);

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
	shared_ptr<PhysicsController> abdomen = physicsFactory->CreateBox(5, 4, 4, position + offset, glm::quat());

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
		if(i<4){
			//right legs first
			btVector3 bodyPivot = btVector3(-width - 1, 1, (length / 2) - ((length / 4)*i));
			btVector3 legPivot = btVector3(0, 0, 0);

			btHingeConstraint* rightHinge
				= new btHingeConstraint(*forebody->rigidBody, *limbs.at(i)->rigidBody, bodyPivot, legPivot, btVector3(0, 1, 0), btVector3(-1, 1, 0), true);
			rightHinge->setLimit(-turningAngle, turningAngle);
			rightHinge->enableAngularMotor(true, 10, 10);
			dynamicsWorld->addConstraint(rightHinge);
		}
		else {
			
			//now left legs 
			btVector3 bodyPivot = btVector3(width + 1, 1, (length / 2) - ((length / 4)*(i-4)));
			btVector3 legPivot = btVector3(0, 0, 0);

			btHingeConstraint* leftHinge
				= new btHingeConstraint(*forebody->rigidBody, *limbs.at(i)->rigidBody, bodyPivot, legPivot, btVector3(0, 1, 0), btVector3(1, 1, 0), true);
			leftHinge->setLimit(-turningAngle, turningAngle);
			leftHinge->enableAngularMotor(true, 10, 10);
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
	shared_ptr<PhysicsController> body = CreateBody(limbs, scale);

	return body;
}
#include "Object.h"
#include "InputManager.h"

Object::Object() : pBridgeObject(nullptr)
{

}

Object::~Object()
{

}

RECT Object::GetCollider()
{	
	return RECT{
		LONG(Collider.Position.x - (Collider.Scale.x / 2)) ,
		LONG(Collider.Position.y - (Collider.Scale.y / 2)) ,
		LONG(Collider.Position.x + (Collider.Scale.x / 2)) ,
		LONG(Collider.Position.y + (Collider.Scale.y / 2)) };
}

bool Object::CheckKeyInputStatus(eInputKey _InputKey, eKeyInputStatus _Status)
{
	return (InputManager::GetInstance()->GetKeyStatus(_InputKey) == _Status);
	
}

void Object::SetStatus(const eObjectStatus& _Status)
{	
	Status = _Status;
	
	// ** ACTIVATED ���°� �ƴ϶�� �浹 üũ�� �ʿ����� ����
	if ( Status != eObjectStatus::ACTIVATED )
	{
		SetGenerateCollisionEvent(false);
	}
}
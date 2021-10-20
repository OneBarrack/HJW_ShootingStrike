#include "Object.h"
#include "Bridge.h"
#include "ObjectManager.h"
#include "InputManager.h"
#include "BitmapManager.h"

Object::Object()
	: pImage(nullptr)
	, tagName(eTagName::NONE)
	, imageOffsetOrder(Point(0,0))
	, pBridge(nullptr)
	, transInfo(Transform())
	, collider(Transform())
	, objectKey(eObjectKey::NONE)
	, objectStatus(eObjectStatus::ACTIVATED)
	, collisionType(eCollisionType::NONE)
	, bGenerateCollisionEvent(false)
	, speed(0.0f)
	, acceleration(0.0f)
{
}

Object::~Object()
{

}

void Object::Initialize()
{
	pImage = nullptr;
	tagName = eTagName::NONE;
	imageOffsetOrder = Point(0, 0);
	pBridge = nullptr;
	transInfo = Transform();
	collider = Transform();
	objectKey = eObjectKey::NONE;
	objectStatus = eObjectStatus::ACTIVATED;
	collisionType = eCollisionType::NONE;
	bGenerateCollisionEvent = false;
	speed = 0.0f;
	acceleration = 0.0f;
}

void Object::Update()
{
	if ( pBridge ) 
		pBridge->Update();
}

void Object::Render(HDC _hdc)
{
	if ( pBridge ) 
		pBridge->Render(_hdc);
}

void Object::Release()
{
	if ( pBridge )
	{
		ObjectManager::GetInstance()->RecallBridge(pBridge);
		pBridge = nullptr;
	}
}

RECT Object::GetColliderL()
{	
	return RECT{
		LONG(collider.Position.x - (collider.Scale.x * 0.5f)),
		LONG(collider.Position.y - (collider.Scale.y * 0.5f)),
		LONG(collider.Position.x + (collider.Scale.x * 0.5f)),
		LONG(collider.Position.y + (collider.Scale.y * 0.5f))};
}

RectF Object::GetColliderF()
{
	return RectF{
		collider.Position.x - (collider.Scale.x * 0.5f),
		collider.Position.y - (collider.Scale.y * 0.5f),
		collider.Position.x + (collider.Scale.x * 0.5f),
		collider.Position.y + (collider.Scale.y * 0.5f)};
}

bool Object::CheckKeyInputStatus(eInputKey _inputKey, eKeyInputState _status)
{
	return (InputManager::GetInstance()->GetKeyState(_inputKey) == _status);
	
}

void Object::SetImage(eImageKey _imageKey)
{
	pImage = BitmapManager::GetInstance()->GetImage(_imageKey);
}

void Object::SetBridge(Bridge* _pBridge)
{
	// ** ���� Bridge�� null�� �ƴϸ�
	if ( _pBridge )
	{
		// ** ���� Bridge�� �����Ѵٸ� Recall
		if ( pBridge ) ObjectManager::GetInstance()->RecallBridge(pBridge);

		// ** Bridge�� Owner ���� �Է�
		_pBridge->SetOwner(this);
		_pBridge->ReceiveInfoFromOwner();
		pBridge = _pBridge;
	}
}

void Object::SetStatus(const eObjectStatus& _status)
{	
	objectStatus = _status;
	
	// ** ACTIVATED ���°� �ƴ϶�� �浹 üũ�� �ʿ����� ����
	if ( objectStatus != eObjectStatus::ACTIVATED )
	{
		SetGenerateCollisionEvent(false);
	}
}
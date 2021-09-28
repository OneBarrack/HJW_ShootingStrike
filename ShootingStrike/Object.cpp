#include "Object.h"
#include "Bridge.h"
#include "ObjectManager.h"
#include "InputManager.h"
#include "BitmapManager.h"

Object::Object()
	: pImage(nullptr)
	, ImageOffsetOrder(Point(0,0))
	, pBridge(nullptr)
	, TransInfo(Transform())
	, Collider(Transform())
	, Offset(Vector3())
	, Key(eObjectKey::NONE)
	, Status(eObjectStatus::ACTIVATED)
	, CollisionType(eCollisionType::NONE)
	, Speed(0.0f)
	, bGenerateCollisionEvent(false)
{
}

Object::~Object()
{

}

void Object::Initialize()
{
	pImage = nullptr;
	ImageOffsetOrder = Point(0, 0);
	pBridge = nullptr;
	TransInfo = Transform();
	Collider = Transform();
	Offset = Vector3();
	Key = eObjectKey::NONE;
	Status = eObjectStatus::ACTIVATED;
	CollisionType = eCollisionType::NONE;
	Speed = 0.0f;
	bGenerateCollisionEvent = false;
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
		ObjectManager::GetInstance()->RecallBridge(pBridge);
}

RECT Object::GetColliderL()
{	
	return RECT{
		LONG(Collider.Position.x - (Collider.Scale.x * 0.5f)),
		LONG(Collider.Position.y - (Collider.Scale.y * 0.5f)),
		LONG(Collider.Position.x + (Collider.Scale.x * 0.5f)),
		LONG(Collider.Position.y + (Collider.Scale.y * 0.5f))};
}

RectF Object::GetColliderF()
{
	return RectF{
		Collider.Position.x - (Collider.Scale.x * 0.5f),
		Collider.Position.y - (Collider.Scale.y * 0.5f),
		Collider.Position.x + (Collider.Scale.x * 0.5f),
		Collider.Position.y + (Collider.Scale.y * 0.5f)};
}

bool Object::CheckKeyInputStatus(eInputKey _InputKey, eKeyInputState _Status)
{
	return (InputManager::GetInstance()->GetKeyState(_InputKey) == _Status);
	
}

void Object::SetImage(eImageKey _ImageKey)
{
	pImage = BitmapManager::GetInstance()->GetImage(_ImageKey);
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
		pBridge = _pBridge;
	}
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
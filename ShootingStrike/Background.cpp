#include "Background.h"
#include "Bridge.h"

Background::Background()
{
}

Background::~Background()
{	
}


void Background::Initialize()
{
	Super::Initialize();

	key = eObjectKey::BACKGROUND;
	status = eObjectStatus::ACTIVATED;
	collisionType = eCollisionType::NONE;
	bGenerateCollisionEvent = false;

	pBridge = nullptr;
}

void Background::Update()
{	
	Super::Update();

	// ** �浹ü ����
	SetCollider(transInfo);
}

void Background::Render(HDC _hdc)
{
	Super::Render(_hdc);
}

void Background::Release()
{
	Super::Release();
}

void Background::OnCollision(Object* _pObject)
{
}

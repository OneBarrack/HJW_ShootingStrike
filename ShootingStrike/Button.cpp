#include "Button.h"
#include "InputManager.h"
#include "CollisionManager.h"

Button::Button()
{

}

Button::~Button()
{

}


void Button::Initialize()
{
	TransInfo.Position = Vector3(0.0f, 0.0f);
	TransInfo.Scale = Vector3(0.0f, 0.0f);
	TransInfo.Direction = Vector3(0.0f, 0.0f);
		
	Collider.Position = TransInfo.Position;
	Collider.Scale = TransInfo.Scale;

	Key = eObjectKey::BUTTON;
	Status = eObjectStatus::ACTIVATED;
	CollisionType = eCollisionType::RECT;
	
	bGenerateCollisionEvent = false;
	
	pButtonImage = nullptr;
	ButtonState = eButtonState::NORMAL;
	ButtonOffset = 0;
	bOnClick = false;
}

void Button::Update()
{
	Vector3 MousePos = InputManager::GetInstance()->GetMousePosition();

	// ** ���콺�� ��ư �̹��� ���� ��ġ �ϴ���
	if ( CollisionManager::IsPointInRect(GetCollider(), MousePos))
	{
		// ** ������ ��
		if ( CheckKeyInputStatus(eInputKey::KEY_LBUTTON, eKeyInputStatus::PRESSED) )
		{
			ButtonState = eButtonState::PRESSED;
		}
		// ** ������ �� ����(Ŭ�� �Ϸ�� ����)
		else if ( CheckKeyInputStatus(eInputKey::KEY_LBUTTON, eKeyInputStatus::UP) )
		{
			ButtonState = eButtonState::NORMAL;
			bOnClick = true;
		}
		// ** ���콺�� ��ư �̹��� ���� ���� (������ �ʴ� ����)
		else
		{
			ButtonState = eButtonState::HOVER;
		}
	}
	else
	{
		ButtonState = eButtonState::NORMAL;
	}

	cout << MousePos.x << " " << MousePos.y << endl;
}

void Button::Render(HDC _hdc)
{
	if ( !pButtonImage )
		return;

	TransparentBlt(_hdc, // ** ���� ��� ��ġ
		int(TransInfo.Position.x - (TransInfo.Scale.x * 0.5f)),
		int(TransInfo.Position.y - (TransInfo.Scale.y * 0.5f)),
		int(TransInfo.Scale.x),
		int(TransInfo.Scale.y),
		pButtonImage->GetMemDC(),
		int(TransInfo.Scale.x * static_cast<int>(ButtonState)),
		0,
		int(TransInfo.Scale.x),
		int(TransInfo.Scale.y),
		RGB(255, 0, 255));
}

void Button::Release()
{

}

void Button::OnCollision(Object* _pObject)
{

}

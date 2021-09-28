#include "ButtonUI.h"
#include "InputManager.h"
#include "CollisionManager.h"

ButtonUI::ButtonUI()
	: bExistHoverImage(false)
	, bExistPressedImage(false)
	, ButtonState(eButtonState::NORMAL)
	, ButtonOffset(0)
	, bOnClick(false)
{

}

ButtonUI::~ButtonUI()
{

}


void ButtonUI::Initialize()
{
	TransInfo.Position = Vector3(0.0f, 0.0f);
	TransInfo.Scale = Vector3(0.0f, 0.0f);
	TransInfo.Direction = Vector3(0.0f, 0.0f);
		
	Collider.Position = TransInfo.Position;
	Collider.Scale = TransInfo.Scale;

	Key = eObjectKey::UI_BUTTON;
	Status = eObjectStatus::ACTIVATED;
	CollisionType = eCollisionType::RECT;
	
	bGenerateCollisionEvent = false;
	
	ButtonState = eButtonState::NORMAL;
	ButtonOffset = 0;
	bOnClick = false;
}

void ButtonUI::Update()
{
	Vector3 MousePos = InputManager::GetInstance()->GetMousePosition();

	// ** ���콺�� ��ư �̹��� ���� ��ġ �ϴ���
	if ( CollisionManager::IsPointInRect(GetColliderL(), MousePos))
	{
		// ** ������ ��
		if ( CheckKeyInputStatus(eInputKey::KEY_LBUTTON, eKeyInputState::PRESSED) )
		{
			ButtonState = eButtonState::PRESSED;
		}
		// ** ������ �� ����(Ŭ�� �Ϸ�� ����)
		else if ( CheckKeyInputStatus(eInputKey::KEY_LBUTTON, eKeyInputState::UP) )
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

	// ** �浹ü ����
	Collider = TransInfo;
}

void ButtonUI::Render(HDC _hdc)
{
	if ( !pImage )
		return;

	TransparentBlt(_hdc, // ** ���� ��� ��ġ
		int(TransInfo.Position.x - (TransInfo.Scale.x * 0.5f)),
		int(TransInfo.Position.y - (TransInfo.Scale.y * 0.5f)),
		int(TransInfo.Scale.x),
		int(TransInfo.Scale.y),
		pImage->GetMemDC(),
		int(TransInfo.Scale.x * static_cast<int>(ButtonState)),
		0,
		int(TransInfo.Scale.x),
		int(TransInfo.Scale.y),
		RGB(255, 0, 255));
}

void ButtonUI::Release()
{

}

void ButtonUI::OnCollision(Object* _pObject)
{

}

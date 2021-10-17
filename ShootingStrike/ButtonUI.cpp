#include "ButtonUI.h"
#include "InputManager.h"
#include "CollisionManager.h"
#include "InputManager.h"

ButtonUI::ButtonUI()
	: bExistHoverImage(false)
	, bExistPressedImage(false)
	, buttonState(eButtonState::NORMAL)
	, bOnClick(false)
	, buttonTypeIndex(0)
{

}

ButtonUI::~ButtonUI()
{

}


void ButtonUI::Initialize()
{
	Super::Initialize();

	key = eBridgeKey::UI_BUTTON;
	
	buttonState = eButtonState::NORMAL;
	bExistPressedImage = false;
	bOnClick = false;
	buttonTypeIndex = 0;
}

void ButtonUI::Update()
{
	Super::Update();

	Vector3 mousePos = InputManager::GetInstance()->GetMousePosition();

	// ** ���콺�� ��ư �̹��� ���� ��ġ �ϴ���
	if ( CollisionManager::IsPointInRect(pOwner->GetColliderL(), mousePos))
	{
		// ** ������ ��
		if ( CHECK_KEYINPUT_STATE(eInputKey::KEY_LBUTTON, eKeyInputState::PRESSED) )
		{
			buttonState = eButtonState::PRESSED;
		}
		// ** ������ �� ����(Ŭ�� �Ϸ�� ����)
		else if ( CHECK_KEYINPUT_STATE(eInputKey::KEY_LBUTTON, eKeyInputState::UP) )
		{
			buttonState = eButtonState::NORMAL;
			bOnClick = true;
		}
		// ** ���콺�� ��ư �̹��� ���� ���� (������ �ʴ� ����)
		else
		{
			buttonState = eButtonState::HOVER;
		}
	}
	else
	{
		buttonState = eButtonState::NORMAL;
	}
}

void ButtonUI::Render(HDC _hdc)
{
	Super::Render(_hdc);

	if ( !pImage )
		return;

	TransparentBlt(_hdc, // ** ���� ��� ��ġ
		int(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		int(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		int(pOwner->GetScale().x),
		int(pOwner->GetScale().y),
		pImage->GetMemDC(),
		int(pImage->GetSegmentationScale().x * static_cast<int>(buttonState)),
		int(pImage->GetSegmentationScale().y * buttonTypeIndex),
		int(pImage->GetSegmentationScale().x),
		int(pImage->GetSegmentationScale().y),
		RGB(255, 0, 255));
}

void ButtonUI::Release()
{
	Super::Release();
}

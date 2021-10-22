#include "GameOverUI.h"
#include "BitmapManager.h"
#include "Bridge.h"
#include "Object.h"
#include "ObjectManager.h"
#include "ButtonUI.h"
#include "SceneManager.h"

GameOverUI::GameOverUI()
	: pGameOverImage(nullptr)
	, pFadeBackImage(nullptr)
	, pYesButton(nullptr)
	, pNoButton(nullptr)
	, alpha(0)
	, fadeSpeed(0)
{

}

GameOverUI::~GameOverUI()
{
}


void GameOverUI::Initialize()
{
	Super::Initialize();
	
	key = eBridgeKey::UI_GAMEOVER;

	alpha = 0;
	fadeSpeed = 3;

	pGameOverImage = BitmapManager::GetInstance()->GetImage(eImageKey::GAMEOVER);
	pFadeBackImage = BitmapManager::GetInstance()->GetImage(eImageKey::FADEBACK_RED);

	InitButton();
}

void GameOverUI::Update()
{
	Super::Update();

	if ( pYesButton && pNoButton )
	{
		// ** Yes : Logo�� �̵�
		if ( static_cast<ButtonUI*>(pYesButton->GetBridgeObject())->OnClick() )
		{			
			SceneManager::GetInstance()->SetScene(eSCENEID::LOGO);
		}

		// ** No : ����
		if ( static_cast<ButtonUI*>(pNoButton->GetBridgeObject())->OnClick() )
		{
			PostQuitMessage(NULL);
		}
	}
	else
	{
		InitButton();
	}
}

void GameOverUI::Render(HDC _hdc)
{
	Super::Render(_hdc);

	if ( !pGameOverImage )
		return;
		
	// ** fadeSpeed�� 40�� ��ġ���� alpha�� ���� ��쿡 alpha ����
	if ( alpha < fadeSpeed * 40 )
		alpha += fadeSpeed;

	BLENDFUNCTION bf;
	bf.AlphaFormat = 0;			// ** �Ϲ� ��Ʈ�� 0, 32��Ʈ ��Ʈ�� AC_SRC_ALPHA
	bf.BlendFlags = 0;			// ** ������ �ʴ� �ɼ�. ������ 0
	bf.BlendOp = AC_SRC_OVER;  // ** AC_SRC_OVER
	bf.SourceConstantAlpha = alpha;		// ** ����(���� 0 - ������ 255)

	// ** Fade Background �̹����� �������� Fade ȿ�� �ο�
	AlphaBlend(_hdc, 0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT,
		pFadeBackImage->GetMemDC(),
		0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT, bf);

	TransparentBlt(_hdc, // ** ���� ��� ��ġ
		int(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.5f)),
		int(pOwner->GetPosition().y - (pOwner->GetScale().y * 0.5f)),
		int(pOwner->GetScale().x),
		int(pOwner->GetScale().y),
		pGameOverImage->GetMemDC(),
		int(0),
		int(0),
		int(pGameOverImage->GetSegmentationScale().x),
		int(pGameOverImage->GetSegmentationScale().y),
		RGB(255, 0, 255));
}

void GameOverUI::Release()
{
	Super::Release();

	pGameOverImage = nullptr;
	pFadeBackImage = nullptr;

	if ( pYesButton )
		ObjectManager::GetInstance()->RecallObject(pYesButton);
	pYesButton = nullptr;

	if ( pNoButton )
		ObjectManager::GetInstance()->RecallObject(pNoButton);
	pNoButton = nullptr;
}

void GameOverUI::InitButton()
{
	if ( pOwner )
	{
		Bridge* pBridge = nullptr;
		pBridge = ObjectManager::GetInstance()->NewBridge(eBridgeKey::UI_BUTTON);
		pYesButton = ObjectManager::GetInstance()->NewObject(eObjectKey::UI);
		pYesButton->SetImage(eImageKey::YES_NO_BUTTON);
		pYesButton->SetPosition(pOwner->GetPosition().x - (pOwner->GetScale().x * 0.11f),
								 pOwner->GetPosition().y + (pOwner->GetScale().y * 0.2f));
		pYesButton->SetScale(100.0f, 50.0f);
		pYesButton->SetCollider(pYesButton->GetTransInfo());
		pYesButton->SetBridge(pBridge);

		// ** Quit Button
		pBridge = ObjectManager::GetInstance()->NewBridge(eBridgeKey::UI_BUTTON);
		pNoButton = ObjectManager::GetInstance()->NewObject(eObjectKey::UI);
		pNoButton->SetImage(eImageKey::YES_NO_BUTTON);
		pNoButton->SetPosition(pOwner->GetPosition().x + (pOwner->GetScale().x * 0.09f),
								 pOwner->GetPosition().y + (pOwner->GetScale().y * 0.2f));
		pNoButton->SetScale(100.0f, 50.0f);
		pNoButton->SetCollider(pNoButton->GetTransInfo());
		pNoButton->SetBridge(pBridge);
		static_cast<ButtonUI*>(pBridge)->SetButtonTypeIndex(1);
	}
	else
	{
		pYesButton = nullptr;
		pNoButton = nullptr;
	}
}
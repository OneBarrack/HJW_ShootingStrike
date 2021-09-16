#include "Background.h"
#include "BitmapManager.h"

Background::Background()
{

}

Background::~Background()
{

}


void Background::Initialize()
{
	TransInfo.Position = Vector3(0.0f, 0.0f);
	TransInfo.Direction = Vector3(0.0f, 0.0f);
	TransInfo.Scale = Vector3(1440.0f, 720.0f);

	Key = eObjectKey::BACKGROUND;
	Status = eObjectStatus::ACTIVATED;
	CollisionType = eCollisionType::NONE;

	Offset = 0.0f;
	bAttached = false;
}

int Background::Update()
{
	// ** Image�� �� ���� �׸� ���
	if ( !bAttached )
	{
		// ** ȭ���� ���� Image�� ���� �������� üũ, ���� ������ �ʾҴٸ� Offset ����
		if ( WindowsWidth + Offset < TransInfo.Scale.x )
		{
			++Offset;
		}
		// ** ȭ���� ���� Image�� ���� �����ٸ�
		else
		{			
			// ** Offset�� 0���� ����� �̾� ���̱� ���� bAttached�� true�� ����
			Offset = 0;
			bAttached = true;
		}
	}
	// ** Image�� ������ �׷����� Image�� �̾� ���� ���
	else
	{
		// ** Offset�� WindowsWidth�� �Ѿ���� üũ, �����̶�� Offset ����
		if ( Offset < WindowsWidth )
		{
			++Offset;
		}
		// ** Offset�� WindowsWidth�� �Ѿ�ٸ�, �̾���� image�� ȭ�� ��ü�� ��������
		else
		{
			// ** Offset�� �ٽ� 0���� ����� �̾���̱⸦ ���� �ϰ� 
			// ** Image�� �� ���� �׸��� ���� bAttached�� false�� ����
			Offset = 0;
			bAttached = false;
		}	
	}

	return 0;
}

void Background::Render(HDC _hdc)
{
	// ** Image�� �� ���� �׸�
	if ( !bAttached )
	{
		TransparentBlt(_hdc, 
			0,
			0,
			WindowsWidth,
			WindowsHeight,
			BitmapManager::GetInstance()->GetMemDC(eImageKey::BACKGROUND),
			Offset, 0,
			WindowsWidth,
			WindowsHeight,
			RGB(255, 0, 255));
	}
	// ** Image�� ������ �׷����� Image�� �̾� ����
	else
	{
		// ** �ռ� �׸��� �̹����� �޺κ��� ������ �ն������� ���
		TransparentBlt(_hdc, 
			0,
			0,
			WindowsWidth - Offset,
			WindowsHeight,
			BitmapManager::GetInstance()->GetMemDC(eImageKey::BACKGROUND),
			(int)TransInfo.Scale.x - WindowsWidth + Offset, 0,
			WindowsWidth - Offset,
			WindowsHeight,
			RGB(255, 0, 255));

		// ** �� �̹����� �޺κп� �̹����� ù ��ŸƮ���� �̾�ٿ� �̹����� ���ӵǵ��� ��
		TransparentBlt(_hdc, 
			WindowsWidth - Offset,
			0,
			Offset,
			WindowsHeight,
			BitmapManager::GetInstance()->GetMemDC(eImageKey::BACKGROUND),
			0, 0,
			Offset,
			WindowsHeight,
			RGB(255, 0, 255));
	}
}

void Background::Release()
{

}

void Background::OnCollision(Object* _pObject)
{
}

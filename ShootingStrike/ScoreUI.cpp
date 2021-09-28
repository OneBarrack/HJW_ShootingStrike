#include "ScoreUI.h"
#include "GameDataManager.h"
#include "CollisionManager.h"
#include "BitmapManager.h"

ScoreUI::ScoreUI()
	: pScoreImage(nullptr)
	, pNumberImage(nullptr)
	, NumberList(queue<int>())
{

}

ScoreUI::~ScoreUI()
{	
}


void ScoreUI::Initialize()
{
	pScoreImage = BitmapManager::GetInstance()->GetImage(eImageKey::SCORE);
	pNumberImage = BitmapManager::GetInstance()->GetImage(eImageKey::NUMBER);

	TransInfo.Position = Vector3(0.0f, 0.0f);
	TransInfo.Scale = Vector3(0.0f, 0.0f);
	TransInfo.Direction = Vector3(0.0f, 0.0f);

	Collider.Position = TransInfo.Position;
	Collider.Scale = TransInfo.Scale;

	Key = eObjectKey::UI_SCORE;
	Status = eObjectStatus::ACTIVATED;
	CollisionType = eCollisionType::NONE;

	bGenerateCollisionEvent = false;
}

void ScoreUI::Update()
{
	MakeScoreNumberList();

	// ** �浹ü ����
	Collider = TransInfo;
}

void ScoreUI::Render(HDC _hdc)
{
	if ( !pScoreImage || !pNumberImage )
		return;

	// ** Score
	TransparentBlt(_hdc, // ** ���� ��� ��ġ
		int(TransInfo.Position.x - TransInfo.Scale.x),
		int(TransInfo.Position.y - TransInfo.Scale.y),
		int(TransInfo.Scale.x),
		int(TransInfo.Scale.y),
		pScoreImage->GetMemDC(),
		0,
		0,
		int(pScoreImage->GetScale().x),
		int(pScoreImage->GetScale().y),
		RGB(255, 0, 255));

	// ** Number
	// ** �ڿ������� �׸��� ���� Offset�� �������� ����
	for ( int Offset = 9; Offset >= 0; --Offset )
	{
		int Num = 0; 

		// ** NumberList�� ���ڸ� �̾� ���ʴ�� �ڿ������� �׸���, 
		// ** ���ڰ� �� �̻� ���ٸ� 0���� ä���
		if ( !NumberList.empty() )
		{
			Num = NumberList.front();
			NumberList.pop();
		}
		
		TransparentBlt(_hdc, // ** ���� ��� ��ġ
			int((TransInfo.Position.x - TransInfo.Scale.x) + ((pNumberImage->GetSegmentationScale().x + 15) * Offset)),
			int(TransInfo.Position.y),
			int(pNumberImage->GetSegmentationScale().x + 13),
			int(TransInfo.Scale.y),
			pNumberImage->GetMemDC(),
			int(pNumberImage->GetSegmentationScale().x * Num),
			0,
			int(pNumberImage->GetSegmentationScale().x),
			int(pNumberImage->GetScale().y),
			RGB(255, 0, 255));
	}
}

void ScoreUI::Release()
{

}

void ScoreUI::OnCollision(Object* _pObject)
{

}

void ScoreUI::MakeScoreNumberList()
{
	int Score = GameDataManager::GetInstance()->GetScore();
	while ( Score > 0 )
	{
		int iTemp = int(Score % 10);
		Score /= 10;

		NumberList.push(iTemp);
	}
}
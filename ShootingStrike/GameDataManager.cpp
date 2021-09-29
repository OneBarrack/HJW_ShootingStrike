#include "GameDataManager.h"

GameDataManager* GameDataManager::Instance = nullptr;

GameDataManager::GameDataManager()
	: Score(0)
	, Frame(0)
	, FPS(0)
{
}

GameDataManager::~GameDataManager()
{
}

void GameDataManager::Initialize()
{
	Score = 0;
	Frame = 0;
	FPS = 0;
}

void GameDataManager::Update()
{
	++Frame;

	CalcFPS();
}

void GameDataManager::AddScore(int _Score)
{
	// ** INT �ִ� ���� �ѱ�ٸ� INT_MAX�� ����
	if ( (ULONGLONG)Score + _Score > INT_MAX )
		Score = INT_MAX;
	else
		Score += _Score;
}

void GameDataManager::CalcFPS()
{
	static ULONGLONG Time = GetTickCount64();

	if ( Time + 1000 < GetTickCount64() )
	{
		FPS = Frame;
		Time = GetTickCount64();
		Frame = 0;
	}
}

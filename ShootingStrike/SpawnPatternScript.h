#pragma once
#include "Headers.h"
#include "Object.h"

class SpawnPatternScript
{
protected:
	// ** ������Ʈ ���� ���� 
	virtual void Spawn() PURE;

	// ** ���� ���鿡 ���� ������Ʈ
	virtual void Update() PURE;

public:
	// ** ���� ����
	virtual void Run() PURE;


public:
	SpawnPatternScript() {}
	~SpawnPatternScript() {}
};


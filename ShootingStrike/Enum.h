#pragma once

enum class eInputKey
{
	KEY_UP		,
	KEY_DOWN	,
	KEY_LEFT	,
	KEY_RIGHT	,
	KEY_ESCAPE	,
	KEY_SPACE	,
	KEY_ENTER	,
	KEY_LBUTTON	, // ** ���콺 ���� Ű �Է� Ȯ��.
	KEY_MAX		, // ** �� Key ����.
};

enum class eKeyInputStatus
{
	NONE	,
	DOWN	,
	PRESSED	,
	UP		,
};

enum class eSCENEID
{
	LOGO ,
	MENU ,
	STAGE,
	EXIT ,
};

enum class eObjectKey
{
	BACKGROUND	,
	PLAYER		,
	ENEMY		,
	ENEMYHOLE	,
	BULLET		,
	HAMMEREFFECT,
};

enum class eImageKey
{	
	BUFFER		  ,
	BACKGROUND	  ,
	LOGOBACK	  ,
	STAGEBACK	  ,
	STAGESIDEBACK ,
	STAGECLOUD	  ,
	PLAYER		  ,
	PROJECTILE	  ,
	HAMMER		  ,
	MOLE		  ,
	HOLE		  ,
	EFFECT		  ,
	KEYMAX		  ,
};

enum class eObjectStatus
{
	ACTIVATED	,
	DEACTIVATED	,
	DESTROYED	,
};

enum class eCollisionType
{
	NONE	,
	ELLIPSE	,
	RECT	,
};
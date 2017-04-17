#pragma once

#include "GameData.h"

#define	BLOOD_TIME	1.5f

class GameRoot
{
public:
	void Init(GameData *data);
	~GameRoot();

	GameData	*Game_Data;

	void Data_Save_Set();
	void Data_Save_Map(int map);
	void Data_Load();
	void Data_Reset();

	bool TouchObj(Object *obj, Windows::UI::Input::PointerPoint ^PointerPoint);
	bool TouchEnemy(Enemy *obj, Windows::UI::Input::PointerPoint ^PointerPoint);

	void CreateShield(Shield *obj, float x, float y, float z);
	void UpdateShield(Shield *obj, float timeTotal, float timeDelta);

	void CreateSpark(Spark *obj, float x, float y, float z, float a_0, float a_1, float s_0, float s_1, float t);
	void UpdateSpark(Spark *obj, float timeTotal, float timeDelta);

	void CreateObj(Object *obj, Enemy* enm, int model_max, float timeTotal, float y_min, float y_max, bool ballistic, int type, int enemy);
	void UpdateObj(Object *obj, Enemy* enm, int model_max, float timeTotal, float timeDelta, float y_min, float y_max, bool ballistic, int type, int enemy);

	void CreateEnm(Enemy *enm, Object* obj, float timeTotal);
	void UpdateEnm(Enemy *enm, float timeTotal, float timeDelta);

	//Положение и направление взгляда
	void Update_Eye(float timeTotal, float timeDelta, float skyline = -10);

	int		i;
	int		j;
	int		tmpi;
	float	tmpf;

private:
	float	x;
	float	y;
	float	z;
};
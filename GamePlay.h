#pragma once

#include "GameRoot.h"
#include "MenuGame.h"
#include "GameHUD.h"
#include "Map_Forest.h"
#include "Map_Space.h"
#include "Scr2D.h"

#define	BLOOD_TIME			1.5f
#define	RUN_TIME_BEFORE		2.0f
#define	RUN_TIME_SHOW		2.0f
#define	RUN_TIME_AFTER		2.0f
#define RUN_TIME_END		2.0f

class GamePlay : public GameRoot
{
public:
	GamePlay(GameData *data);
	~GamePlay();

	void StartClassic();
	void StartTime();
	void StartSurv();

	void Update(float timeTotal, float timeDelta);
	void Draw();

	void OnPointerPressed(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerMoved(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerReleased(Windows::UI::Input::PointerPoint ^PointerPoint);
private:
	//Модели карты
	std::unique_ptr<DirectX::Model>						m_model_map;
	std::unique_ptr<DirectX::Model>						m_model_compass;

	//Номер уровня
	Scr2D		*m_2d_level[10];

	//Подготовка к запуску игры
	float		Game_Timer;
	float		Level_Alpha;
	int			Level_Dig_1;
	int			Level_Dig_2;

	//Кровь на экране
	Scr2D		*m_2d_blood[5];
	int			blood_num;
	float		blood_alpha;
	float		blood_timer;

	//HUD
	GameHUD		*Game_HUD;

	//Внутриигровое меню
	MenuGame	*Menu_Game;

	//Карты
	MapForest	*Map_Forest;
	MapSpace	*Map_Space;

	float		eye_set;
	float		eye_speed;
};
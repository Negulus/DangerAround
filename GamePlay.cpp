#include "pch.h"

#include "GamePlay.h"

#include "DDSTextureLoader.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

GamePlay::GamePlay(GameData *data)
{
	Init(data);

	//Модель карты
	switch (Game_Data->set_angle)
	{
	case deg_45:
		m_model_map = Model::CreateFromCMO( Game_Data->device, L"map_45.cmo", *Game_Data->m_fxFactory, true, false );
		break;
	case deg_90:
		m_model_map = Model::CreateFromCMO( Game_Data->device, L"map_90.cmo", *Game_Data->m_fxFactory, true, false );
		break;
	case deg_120:
		m_model_map = Model::CreateFromCMO( Game_Data->device, L"map_120.cmo", *Game_Data->m_fxFactory, true, false );
		break;
	case deg_180:
		m_model_map = Model::CreateFromCMO( Game_Data->device, L"map_180.cmo", *Game_Data->m_fxFactory, true, false );
		break;
	case deg_270:
		m_model_map = Model::CreateFromCMO( Game_Data->device, L"map_270.cmo", *Game_Data->m_fxFactory, true, false );
		break;
	case deg_360:
		m_model_map = Model::CreateFromCMO( Game_Data->device, L"map_360.cmo", *Game_Data->m_fxFactory, true, false );
		break;
	}
	
	m_model_compass = Model::CreateFromCMO( Game_Data->device, L"compass.cmo", *Game_Data->m_fxFactory, true, false );

	if (Game_Data->Map == MAP_SPACE)
	{
		m_2d_blood[0] = new Scr2D(Game_Data, -112, -16, L"Images\\Other\\glass_0.dds");
		m_2d_blood[1] = new Scr2D(Game_Data, -112, -16, L"Images\\Other\\glass_1.dds");
		m_2d_blood[2] = new Scr2D(Game_Data, -112, -16, L"Images\\Other\\glass_2.dds");
		m_2d_blood[3] = new Scr2D(Game_Data, -112, -16, L"Images\\Other\\glass_3.dds");
		m_2d_blood[4] = new Scr2D(Game_Data, -112, -16, L"Images\\Other\\glass_4.dds");
	}
	else
	{
		m_2d_blood[0] = new Scr2D(Game_Data, -112, -16, L"Images\\Other\\blood_0.dds");
		m_2d_blood[1] = new Scr2D(Game_Data, -112, -16, L"Images\\Other\\blood_1.dds");
		m_2d_blood[2] = new Scr2D(Game_Data, -112, -16, L"Images\\Other\\blood_2.dds");
		m_2d_blood[3] = new Scr2D(Game_Data, -112, -16, L"Images\\Other\\blood_3.dds");
		m_2d_blood[4] = new Scr2D(Game_Data, -112, -16, L"Images\\Other\\blood_4.dds");
	}

	m_2d_level[0] = new Scr2D(Game_Data, 336, 176, L"Images\\Other\\level_dig_0.dds");
	m_2d_level[1] = new Scr2D(Game_Data, 336, 176, L"Images\\Other\\level_dig_1.dds");
	m_2d_level[2] = new Scr2D(Game_Data, 336, 176, L"Images\\Other\\level_dig_2.dds");
	m_2d_level[3] = new Scr2D(Game_Data, 336, 176, L"Images\\Other\\level_dig_3.dds");
	m_2d_level[4] = new Scr2D(Game_Data, 336, 176, L"Images\\Other\\level_dig_4.dds");
	m_2d_level[5] = new Scr2D(Game_Data, 336, 176, L"Images\\Other\\level_dig_5.dds");
	m_2d_level[6] = new Scr2D(Game_Data, 336, 176, L"Images\\Other\\level_dig_6.dds");
	m_2d_level[7] = new Scr2D(Game_Data, 336, 176, L"Images\\Other\\level_dig_7.dds");
	m_2d_level[8] = new Scr2D(Game_Data, 336, 176, L"Images\\Other\\level_dig_8.dds");
	m_2d_level[9] = new Scr2D(Game_Data, 336, 176, L"Images\\Other\\level_dig_9.dds");

	//Данные для создания объектов
	Game_Data->obj_end[0] = 0;
	Game_Data->obj_end[1] = 10;
	Game_Data->obj_end[2] = 20;
	Game_Data->obj_end[3] = 30;
	Game_Data->obj_end[4] = 40;
	Game_Data->obj_end[5] = 50;
	Game_Data->obj_end[6] = 60;
	Game_Data->obj_start_min[0] = 3;
	Game_Data->obj_start_min[1] = 11;
	Game_Data->obj_start_min[2] = 20;
	Game_Data->obj_start_min[3] = 29;
	Game_Data->obj_start_min[4] = 39;
	Game_Data->obj_start_min[5] = 48;
	Game_Data->obj_start_min[6] = 63;
	Game_Data->obj_start_max[0] = -11;
	Game_Data->obj_start_max[1] = -3;
	Game_Data->obj_start_max[2] = 7;
	Game_Data->obj_start_max[3] = 15;
	Game_Data->obj_start_max[4] = 24;
	Game_Data->obj_start_max[5] = 32;
	Game_Data->obj_start_max[6] = 40;

	Game_Data->pause = false;

	Game_Data->Blood = false;
	blood_alpha = 0;
	blood_timer = 0;

	Level_Alpha = 0;
	Level_Dig_1 = 0;
	Level_Dig_2 = 0;

	Game_Data->Score = 0;

	Game_Data->Time_Delta = 0;

	eye_set = 0;
	eye_speed = 0;

	if (Game_Data->Level < 1) Game_Data->Level = 1;
	else if (Game_Data->Level > LEVEL_MAX) Game_Data->Level = LEVEL_MAX;

	if (Game_Data->Map < 0) Game_Data->Map = 0;
	else if (Game_Data->Map >= MAP_MAX) Game_Data->Map = MAP_MAX - 1;

	Game_Data->Level_Start = Game_Data->Level;

	switch (Game_Data->Game_Type)
	{
	case type_time:
		StartTime();
		break;
	case type_surv:
		StartSurv();
		break;
	default:
		StartClassic();
		break;
	}

	//HUD
	Game_HUD = new GameHUD(Game_Data);

	//Внутриигровое меню
	Menu_Game = new MenuGame(Game_Data);

	switch (Game_Data->Map)
	{
	case MAP_SPACE:
		Map_Space = new MapSpace(Game_Data);
		break;
	case MAP_FOREST:
		Map_Forest = new MapForest(Game_Data);
		break;
	case MAP_VOLCANO:
		break;
	default:
		Game_Data->Status_Next = stat_MenuMain;
		break;
	}
};

void GamePlay::StartClassic()
{
	Game_Data->Time_Obj_Next = 0;
	Game_Data->Time_Obj_Min = 0.5f + (7.f / Game_Data->Level) + (1.f / (Game_Data->Level * 0.15f));
	Game_Data->Time_Obj_Max = 0.5f + (11.f / Game_Data->Level) + (1.f / (Game_Data->Level * 0.15f));

	Game_Timer = 0;
	Game_Data->Game_Run = false;
	Game_Data->Game_End = false;
	Game_Data->Game_Stat = game_stat_start;
	Game_Data->Life = 3;
}

void GamePlay::StartTime()
{
	Game_Data->Timer_Set = TIME_GAME_TIME;

	Game_Data->Time_Obj_Next = 0;
	Game_Data->Time_Obj_Min = 0.5f + (7.f / Game_Data->Level) + (1.f / (Game_Data->Level * 0.15f));
	Game_Data->Time_Obj_Max = 0.5f + (11.f / Game_Data->Level) + (1.f / (Game_Data->Level * 0.15f));

	Game_Timer = 0;
	Game_Data->Game_Run = false;
	Game_Data->Game_End = false;
	Game_Data->Game_Stat = game_stat_start;
	Game_Data->Timer = 0;
	Game_Data->Life = 3;
}

void GamePlay::StartSurv()
{
	Game_Data->Count_Set = Game_Data->Level * 10;

	Game_Data->Time_Obj_Next = 0;
	Game_Data->Time_Obj_Min = 0.5f + (7.f / Game_Data->Level) + (1.f / (Game_Data->Level * 0.15f));
	Game_Data->Time_Obj_Max = 0.5f + (11.f / Game_Data->Level) + (1.f / (Game_Data->Level * 0.15f));

	Game_Timer = 0;
	Game_Data->Game_Run = false;
	Game_Data->Game_End = false;
	Game_Data->Game_Stat = game_stat_start_level;
	Game_Data->Count = 0;
	Game_Data->Life = 5;
}

void GamePlay::Update(float timeTotal, float timeDelta)
{
	if (Game_Data->Status_Current != stat_Play)
	{
		if (!Game_Data->pause)
		{
			Game_Data->pause = true;
			switch (Game_Data->Map)
			{
			case MAP_SPACE:
				Map_Space->Pause();
				break;
			case MAP_FOREST:
				Map_Forest->Pause();
				break;
			case MAP_VOLCANO:
				break;
			}
		}
		Menu_Game->Update(timeTotal, timeDelta);
		return;
	}

	if (Game_Data->pause)
	{
		Game_Data->pause = false;
		switch (Game_Data->Map)
		{
		case MAP_SPACE:
			Map_Space->Resume();
			break;
		case MAP_FOREST:
			Map_Forest->Resume();
			break;
		case MAP_VOLCANO:
			break;
		}
	}
	
	Menu_Game->Run = false;

	Game_HUD->Update(timeTotal, timeDelta);

	if (eye_set > 0)
	{
		eye_set -= timeDelta * eye_speed;
		Game_Data->eye.y -= timeDelta * eye_speed;
		if (eye_set < 0)
		{
			Game_Data->eye.y -= eye_set;
			eye_set = 0;
		}
	}
	else if (eye_set < 0)
	{
		eye_set -= timeDelta * eye_speed;
		Game_Data->eye.y -= timeDelta * eye_speed;
		if (eye_set > 0)
		{
			Game_Data->eye.y -= eye_set;
			eye_set = 0;
		}
	}

	Update_Eye(timeTotal, timeDelta);

	//Подготовка запуска
	switch (Game_Data->Game_Stat)
	{
	case game_stat_start_level:
		Game_Data->Game_Stat = game_stat_before_level;
		Game_Data->Game_Run = false;
		Game_Data->Game_End = false;
		Game_Timer = timeTotal + RUN_TIME_BEFORE;
		if (Game_Data->Level > 9)
		{
			Level_Dig_2 = (int)(Game_Data->Level / 10);
			Level_Dig_1 = Game_Data->Level % 10;

			m_2d_level[Level_Dig_1]->x = ((Game_Data->scr_wide) ? 422 : 396) * Game_Data->scr_scale;
			m_2d_level[Level_Dig_2]->x = ((Game_Data->scr_wide) ? 302 : 276) * Game_Data->scr_scale;
		}
		else
		{
			Level_Dig_2 = -1;
			Level_Dig_1 = Game_Data->Level;
			m_2d_level[Level_Dig_1]->x = ((Game_Data->scr_wide) ? 362 : 336) * Game_Data->scr_scale;
		}
		break;
	case game_stat_before_level:
		if (Game_Timer < timeTotal)
		{
			Level_Alpha = 1;
			Game_Data->Game_Stat = game_stat_show_level;
			Game_Timer = timeTotal + RUN_TIME_SHOW;
		}
		break;
	case game_stat_show_level:
		if (Game_Timer >= timeTotal)
		{
			Level_Alpha = ((Game_Timer - timeTotal) / RUN_TIME_SHOW);
		}
		else
		{
			Level_Alpha = 0;
			Game_Data->Game_Stat = game_stat_start;
		}
		break;
	case game_stat_start:
		Game_Data->Game_Stat = game_stat_before_run;
		Game_Timer = timeTotal + RUN_TIME_AFTER;
		break;
	case game_stat_before_run:
		if (Game_Timer < timeTotal)
		{
			Game_Data->Game_Stat = game_stat_run;
			Game_Data->Game_Run = true;
		}
		break;
	case game_stat_end:
		if (Game_Data->Object)
		{
			Game_Data->Game_Stat = game_stat_run;
			break;
		}
		Game_Data->Game_Stat = game_stat_ending;
		Game_Timer = timeTotal + RUN_TIME_END;
		break;
	case game_stat_ending:
		if (Game_Timer < timeTotal)
		{
			switch (Game_Data->Game_Type)
			{
			case type_time:
				Game_Data->Status_Next = stat_Win;
				break;
			case type_surv:
				Game_Data->Level += 1;
				StartSurv();
				break;
			default:
				Game_Data->Status_Next = stat_Win;
				break;
			}
		}
		break;
	}

	//Завершение игры по времени
	if (Game_Data->Game_Type == type_time && Game_Data->Game_Stat == game_stat_run)
	{
		Game_Data->Timer += timeDelta;
		if (Game_Data->Timer >= Game_Data->Timer_Set)
		{
			Game_Data->Game_Run = false;
			Game_Data->Game_End = true;
			Game_Data->Game_Stat = game_stat_end;
		}
	}

	//Отображение крови
	if (Game_Data->Blood)
	{
		blood_num = (int)Game_Data->randf(0, 4);
		blood_timer = timeTotal;
		blood_alpha = 1;
		Game_Data->Blood = false;
	}

	if (blood_alpha > 0)
	{
		if ((blood_timer + BLOOD_TIME) < timeTotal)
		{
			blood_timer = 0;
			blood_alpha = 0;
		}
		else
		{
			blood_alpha = 1 - ((timeTotal - blood_timer) / BLOOD_TIME);
			if (blood_alpha < 0)
				blood_alpha = 0;
		}
	}

	Game_Data->Time_Delta += timeDelta;

	//Фунции обновления разных карт
	switch (Game_Data->Map)
	{
	case MAP_SPACE:
		Map_Space->Update(timeTotal, timeDelta);
		break;
	case MAP_FOREST:
		Map_Forest->Update(timeTotal, timeDelta);
		break;
	case MAP_VOLCANO:
		break;
	}
};

void GamePlay::Draw()
{
	XMMATRIX tmpm;
	XMMATRIX proj3d = XMLoadFloat4x4( &Game_Data->m_proj3d );
	XMMATRIX proj2d = XMLoadFloat4x4( &Game_Data->m_proj2d );
	XMMATRIX world = XMLoadFloat4x4( &Game_Data->m_world );
    XMMATRIX view = XMLoadFloat4x4( &Game_Data->m_view );

	//Вывод 3D
	Game_Data->m_batchEffect->SetProjection( proj3d );

	//Фунции рисования разных карт
	switch (Game_Data->Map)
	{
	case MAP_SPACE:
		Map_Space->Draw();
		break;
	case MAP_FOREST:
		Map_Forest->Draw();
		break;
	case MAP_VOLCANO:
		break;
	}

	//Рисование карты
	Game_Data->context->ClearDepthStencilView(Game_Data->m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	tmpm = XMLoadFloat4x4(&Game_Data->m_map_pos);
	m_model_compass->Draw( Game_Data->context, *Game_Data->m_states, tmpm, view, proj3d, 1.0f, true, true, Colors::White );
	m_model_map->Draw( Game_Data->context, *Game_Data->m_states, tmpm, view, proj3d, 1.0f, true, true, Colors::White );

	switch (Game_Data->Map)
	{
	case MAP_SPACE:
		Map_Space->Draw_MapPoint();
		break;
	case MAP_FOREST:
		Map_Forest->Draw_MapPoint();
		break;
	case MAP_VOLCANO:
		break;
	}

	//Вывод 2D
	Game_Data->m_batchEffect->SetProjection( proj2d );

	Game_Data->m_sprites->Begin();

	if (Level_Alpha > 0)
	{
		if (Level_Dig_2 >= 0) m_2d_level[Level_Dig_2]->Draw(Level_Alpha);
		m_2d_level[Level_Dig_1]->Draw(Level_Alpha);
	}

	if (blood_alpha > 0) m_2d_blood[blood_num]->Draw(blood_alpha);

	Game_HUD->Draw();

	if (Game_Data->Status_Current != stat_Play)
		Menu_Game->Draw();

	Game_Data->m_sprites->End();
};

void GamePlay::OnPointerPressed(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (Game_Data->Status_Current != stat_Play)
	{
		Menu_Game->OnPointerPressed(PointerPoint);
		return;
	}

	if (Game_HUD->OnPointerPressed(PointerPoint))
		return;

	if (PointerPoint->Position.X < 196 && PointerPoint->Position.Y > 665)
	{
		eye_set = Game_Data->eye.y;
		eye_speed = eye_set * 4.f;
	}

	float x1;
	float y1;

	x1 = ((PointerPoint->Position.Y - ((Game_Data->scr_wide) ? 426.f : 400.f)) / ((Game_Data->scr_wide) ? 853.f : 800));
	x1 *= 50.f / 180.f * XM_PI;
	
	y1 = ((PointerPoint->Position.X - 240.f) / ((Game_Data->scr_wide) ? 853.f : 800));
	y1 *= 50.f / 180.f * XM_PI;
		
	Game_Data->Touch_Pos.x = x1 * sinf(Game_Data->eye.z) + y1 * cosf(Game_Data->eye.z);
	Game_Data->Touch_Pos.x += Game_Data->eye.y;

	Game_Data->Touch_Pos.y = y1 * sinf(Game_Data->eye.z) + x1 * cosf(Game_Data->eye.z);
	Game_Data->Touch_Pos.y += Game_Data->eye.x;

	switch (Game_Data->Map)
	{
	case MAP_SPACE:
		Map_Space->OnPointerPressed(PointerPoint);
		break;
	case MAP_FOREST:
		Map_Forest->OnPointerPressed(PointerPoint);
		break;
	case MAP_VOLCANO:
		break;
	}
}

void GamePlay::OnPointerMoved(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (Game_Data->Status_Current != stat_Play)
	{
		Menu_Game->OnPointerMoved(PointerPoint);
		return;
	}

	if (Game_HUD->OnPointerMoved(PointerPoint))
		return;

	switch (Game_Data->Map)
	{
	case MAP_SPACE:
		Map_Space->OnPointerMoved(PointerPoint);
		break;
	case MAP_FOREST:
		Map_Forest->OnPointerMoved(PointerPoint);
		break;
	case MAP_VOLCANO:
		break;
	}
}

void GamePlay::OnPointerReleased(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (Game_Data->Status_Current != stat_Play)
	{
		Menu_Game->OnPointerReleased(PointerPoint);
		return;
	}

	if (Game_HUD->OnPointerReleased(PointerPoint))
		return;

	switch (Game_Data->Map)
	{
	case MAP_SPACE:
		Map_Space->OnPointerReleased(PointerPoint);
		break;
	case MAP_FOREST:
		Map_Forest->OnPointerReleased(PointerPoint);
		break;
	case MAP_VOLCANO:
		break;
	}
}

GamePlay::~GamePlay()
{
	delete Game_HUD;
	delete Menu_Game;
	delete Map_Forest;
	delete Map_Space;
	delete m_2d_level[0];
	delete m_2d_level[1];
	delete m_2d_level[2];
	delete m_2d_level[3];
	delete m_2d_level[4];
	delete m_2d_level[5];
	delete m_2d_level[6];
	delete m_2d_level[7];
	delete m_2d_level[8];
	delete m_2d_level[9];
}
#include "pch.h"

#include "GameHUD.h"

#include "DDSTextureLoader.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

GameHUD::GameHUD(GameData *data)
{
	Game_Data = data;

	//Инициализация моделей и текстур
	
	m_2d_top = new Scr2D(Game_Data, -112, 0, (Game_Data->Game_Type == type_time) ? L"Images\\Other\\hud_top_time.dds" : L"Images\\Other\\hud_top.dds");
	m_2d_bottom = new Scr2D(Game_Data, -112, 224, L"Images\\Other\\hud_bottom.dds");
	m_2d_life = new Scr2D(Game_Data, (Game_Data->scr_wide) ? 780 : 754, (Game_Data->Game_Type == type_time) ? 74 : 25, L"Images\\Other\\hud_life.dds");

	m_2d_but_pause = new But2D(Game_Data, (Game_Data->scr_wide) ? -28 : -15, 365, L"Images\\Other\\hud_but_pause.dds", 19, 19, 90, 90);

	m_2d_dig = new Scr2DDigit(Game_Data, 0, 0, 22, 0);

	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_0.dds", nullptr, m_2d_dig->m_tex[0].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_1.dds", nullptr, m_2d_dig->m_tex[1].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_2.dds", nullptr, m_2d_dig->m_tex[2].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_3.dds", nullptr, m_2d_dig->m_tex[3].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_4.dds", nullptr, m_2d_dig->m_tex[4].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_5.dds", nullptr, m_2d_dig->m_tex[5].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_6.dds", nullptr, m_2d_dig->m_tex[6].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_7.dds", nullptr, m_2d_dig->m_tex[7].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_8.dds", nullptr, m_2d_dig->m_tex[8].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_9.dds", nullptr, m_2d_dig->m_tex[9].ReleaseAndGetAddressOf());

	m_2d_dig_big = new Scr2DDigit(Game_Data, 408, 12, 29, 0);
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_big_0.dds", nullptr, m_2d_dig_big->m_tex[0].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_big_1.dds", nullptr, m_2d_dig_big->m_tex[1].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_big_2.dds", nullptr, m_2d_dig_big->m_tex[2].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_big_3.dds", nullptr, m_2d_dig_big->m_tex[3].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_big_4.dds", nullptr, m_2d_dig_big->m_tex[4].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_big_5.dds", nullptr, m_2d_dig_big->m_tex[5].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_big_6.dds", nullptr, m_2d_dig_big->m_tex[6].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_big_7.dds", nullptr, m_2d_dig_big->m_tex[7].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_big_8.dds", nullptr, m_2d_dig_big->m_tex[8].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_game_dig_big_9.dds", nullptr, m_2d_dig_big->m_tex[9].ReleaseAndGetAddressOf());

	mm = 0;
	ss = 0;
	moved = false;
	score = Game_Data->Score;

	life = Game_Data->Life;
	life_last = Game_Data->Life;
	life_stat = 0;
	life_time_set = 0;;
};

void GameHUD::Update(float timeTotal, float timeDelta)
{
	tmpi = (int)(TIME_GAME_TIME - Game_Data->Timer);
	if (tmpi < 0) tmpi = 0;
	mm = tmpi / 60;
	ss = tmpi % 60;

	if (score < Game_Data->Score)
	{
		score += timeDelta * 100.f;
		if (score > Game_Data->Score)
			score = Game_Data->Score;
	}
	else if (score > Game_Data->Score)
	{
		score -= (int)(timeDelta * 100.f);
		if (score < Game_Data->Score)
			score = Game_Data->Score;
	}

	switch (life_stat)
	{
	case 0:
		if (life != Game_Data->Life)
		{
			life = Game_Data->Life;
			life_time_set = timeTotal + 0.2f;
			life_stat = 1;
		}
		break;
	case 1:
		if (timeTotal >= life_time_set)
		{
			life = life_last;
			life_time_set = timeTotal + 0.2f;
			life_stat = 2;
		}
		break;
	case 2:
		if (timeTotal >= life_time_set)
		{
			life = Game_Data->Life;
			life_time_set = timeTotal + 0.2f;
			life_stat = 3;
		}
		break;
	case 3:
		if (timeTotal >= life_time_set)
		{
			life = life_last;
			life_time_set = timeTotal + 0.2f;
			life_stat = 4;
		}
		break;
	case 4:
		if (timeTotal >= life_time_set)
		{
			life = Game_Data->Life;
			life_last = life;
			life_stat = 0;
		}
		break;
	default:
		life_stat = 0;
		break;
	}

	if (life_stat == 0)
	{
	}

	m_2d_but_pause->Update(timeDelta);
};

void GameHUD::Draw()
{
	m_2d_top->Draw();
	m_2d_bottom->Draw();

	if (life > 0)
		m_2d_life->Draw();
	if (life > 1)
		m_2d_life->Draw(1.0f, m_2d_life->x, m_2d_life->y - (int)(34 * Game_Data->scr_scale));
	if (life > 2)
		m_2d_life->Draw(1.0f, m_2d_life->x, m_2d_life->y - (int)(68 * Game_Data->scr_scale));
	if (life > 3)
		m_2d_life->Draw(1.0f, m_2d_life->x, m_2d_life->y - (int)(102 * Game_Data->scr_scale));
	if (life > 4)
		m_2d_life->Draw(1.0f, m_2d_life->x, m_2d_life->y - (int)(136 * Game_Data->scr_scale));


	m_2d_dig->Draw(5, score, 1.0f, ((Game_Data->scr_wide) ? 65 : 39) * Game_Data->scr_scale, 464 * Game_Data->scr_scale);

	if (Game_Data->Game_Type == type_time)
	{
		m_2d_dig->Draw(2, mm, 1.0f, ((Game_Data->scr_wide) ? 677 : 651) * Game_Data->scr_scale, 464 * Game_Data->scr_scale);
		m_2d_dig->Draw(2, ss, 1.0f, ((Game_Data->scr_wide) ? 733 : 707) * Game_Data->scr_scale, 464 * Game_Data->scr_scale);
	}

	m_2d_dig_big->Draw(2, Game_Data->Level);

	m_2d_but_pause->Draw();
};

bool GameHUD::OnPointerPressed(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	moved = false;
	if (m_2d_but_pause->Click(PointerPoint, false))
		return true;
	else
		return false;
}

bool GameHUD::OnPointerMoved(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	moved = true;
	if (m_2d_but_pause->Move(PointerPoint))
		return true;
	else
		return false;
}

bool GameHUD::OnPointerReleased(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (m_2d_but_pause->Release(PointerPoint, moved))
	{
		Game_Data->Status_Next = stat_Pause;
		moved = false;
		return true;
	}
	else
	{
		moved = false;
		return false;
	}
}

GameHUD::~GameHUD()
{
	delete m_2d_top;
	delete m_2d_bottom;
	delete m_2d_life;
	delete m_2d_dig;
	delete m_2d_dig_big;
	delete m_2d_but_pause;
}
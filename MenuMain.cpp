#include "pch.h"

#include "MenuMain.h"

#include "DDSTextureLoader.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

MenuMain::MenuMain(GameData *data)
{
	Init(data);

	//Инициализация моделей и текстур
	m_model_bg = Model::CreateFromCMO( Game_Data->device, L"menu_bg.cmo", *Game_Data->m_fxFactory, true, true );

	//Главное меню
	m_2d_main_but_classic = new But2D(Game_Data, 16, 10, L"Images\\Other\\menu_main_but_classic.dds", 28, 68, 200, 120);
//	m_2d_main_but_time = new But2D(Game_Data, 272, 10, (Game_Data->isTrial) ? L"Images\\Other\\menu_main_but_time_trial.dds" : L"Images\\Other\\menu_main_but_time.dds", 28, 68, 200, 120);
//	m_2d_main_but_surv = new But2D(Game_Data, 528, 10, (Game_Data->isTrial) ? L"Images\\Other\\menu_main_but_surv_trial.dds" : L"Images\\Other\\menu_main_but_surv.dds", 28, 68, 200, 120);
	m_2d_main_but_time = new But2D(Game_Data, 272, 10, L"Images\\Other\\menu_main_but_time.dds", 28, 68, 200, 120);
	m_2d_main_but_surv = new But2D(Game_Data, 528, 10, L"Images\\Other\\menu_main_but_surv.dds", 28, 68, 200, 120);
	m_2d_main_but_set = new But2D(Game_Data, 208, 254, L"Images\\Other\\menu_main_but_set.dds", 4, 4, 120, 120);
	m_2d_main_but_help = new But2D(Game_Data, 464, 254, L"Images\\Other\\menu_main_but_help.dds", 4, 4, 120, 120);

	//Меню выбора уровня
	m_2d_sel_map_bg = new Scr2D(Game_Data, -20, 13, L"Images\\Other\\menu_sel_map_bg.dds");
	m_2d_sel_level_bg = new Scr2D(Game_Data, -35, 296, L"Images\\Other\\menu_sel_level_bg.dds");
	m_2d_sel_rec_bg_classic = new Scr2D(Game_Data, 492, 21, L"Images\\Other\\menu_sel_rec_bg_classic.dds");
	m_2d_sel_rec_bg_time = new Scr2D(Game_Data, 492, 21, L"Images\\Other\\menu_sel_rec_bg_time.dds");
	m_2d_sel_rec_bg_surv = new Scr2D(Game_Data, 492, 21, L"Images\\Other\\menu_sel_rec_bg_surv.dds");
	m_2d_sel_but_start = new But2D(Game_Data, 492, 307, L"Images\\Other\\menu_sel_but_start.dds", 28, 24, 200, 80);

	m_2d_sel_map[MAP_SPACE] = new Scr2D(Game_Data, 93, 12, L"Images\\Other\\menu_sel_map_space.dds");
	m_2d_sel_map[MAP_FOREST] = new Scr2D(Game_Data, 93, 12, L"Images\\Other\\menu_sel_map_forest.dds");
//	m_2d_sel_map[MAP_VOLCANO] = new Scr2D(Game_Data, 93, 12, L"Images\\Other\\menu_sel_map_volcano.dds");

	m_2d_sel_level_dig = new Scr2DDigit(Game_Data, 78, 330, 43, 0);
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_level_0.dds", nullptr, m_2d_sel_level_dig->m_tex[0].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_level_1.dds", nullptr, m_2d_sel_level_dig->m_tex[1].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_level_2.dds", nullptr, m_2d_sel_level_dig->m_tex[2].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_level_3.dds", nullptr, m_2d_sel_level_dig->m_tex[3].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_level_4.dds", nullptr, m_2d_sel_level_dig->m_tex[4].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_level_5.dds", nullptr, m_2d_sel_level_dig->m_tex[5].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_level_6.dds", nullptr, m_2d_sel_level_dig->m_tex[6].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_level_7.dds", nullptr, m_2d_sel_level_dig->m_tex[7].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_level_8.dds", nullptr, m_2d_sel_level_dig->m_tex[8].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_level_9.dds", nullptr, m_2d_sel_level_dig->m_tex[9].ReleaseAndGetAddressOf());

	m_2d_sel_rec_dig = new Scr2DDigit(Game_Data, 529, 180, 32, 0);
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_rec_0.dds", nullptr, m_2d_sel_rec_dig->m_tex[0].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_rec_1.dds", nullptr, m_2d_sel_rec_dig->m_tex[1].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_rec_2.dds", nullptr, m_2d_sel_rec_dig->m_tex[2].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_rec_3.dds", nullptr, m_2d_sel_rec_dig->m_tex[3].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_rec_4.dds", nullptr, m_2d_sel_rec_dig->m_tex[4].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_rec_5.dds", nullptr, m_2d_sel_rec_dig->m_tex[5].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_rec_6.dds", nullptr, m_2d_sel_rec_dig->m_tex[6].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_rec_7.dds", nullptr, m_2d_sel_rec_dig->m_tex[7].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_rec_8.dds", nullptr, m_2d_sel_rec_dig->m_tex[8].ReleaseAndGetAddressOf());
	CreateDDSTextureFromFile( Game_Data->device, L"Images\\Other\\menu_sel_rec_9.dds", nullptr, m_2d_sel_rec_dig->m_tex[9].ReleaseAndGetAddressOf());

	//Меню помощи
	m_2d_help = new Scr2D(Game_Data, -112, -16, L"Images\\Other\\menu_help.dds");

	//Полоса выбора карты
	m_2d_sel_map_sel_bg = new Scr2D(Game_Data, -112, 15, L"Images\\Other\\menu_sel_map_sel_bg.dds");
	sel_map_sel_alpha = 0;
	sel_map_sel_alpha_new = 0;
	sel_map_sel_x = 0;
	sel_map_sel_x_old = 0;
	sel_map_sel_x_moved = false;

	//Полоса выбора уровня
	m_2d_sel_level_sel_bg = new Scr2D(Game_Data, -112, 296, L"Images\\Other\\menu_sel_level_sel_bg.dds");
	sel_level_sel_alpha = 0;
	sel_level_sel_alpha_new = 0;
	sel_level_sel_x = 0;
	sel_level_sel_x_old = 0;
	sel_level_sel_x_moved = false;

	//Меню настроек
	m_2d_set_sound_bg = new Scr2D(Game_Data, 144, 77, L"Images\\Other\\menu_set_sound_bg.dds");
	m_2d_set_sound_check = new Scr2D(Game_Data, 122, 144, L"Images\\Other\\menu_set_sound_check.dds");
	m_2d_set_but_cal = new But2D(Game_Data, 144, 258, L"Images\\Other\\menu_set_but_cal.dds", 0, 24, 512, 80);

	moved = false;
	move_x = 0;
	move_x_new = 0;
	move_x_old = 0;
	move_y = 0;
	move_y_old = 0;
	move_y_new = 0;
	status_up = stat_Null;
	status_down = stat_Null;
	status_left = stat_Null;
	status_right = stat_Null;

	move_x_dlt = 800 * Game_Data->scr_scale;
	move_y_dlt = 500 * Game_Data->scr_scale;

	Menu_Update();
};

void MenuMain::Menu_Update()
{
	move_x = 0;
	move_x_new = 0;
	move_y = 0;
	move_y_new = 0;
	switch (Game_Data->Status_Next)
	{
	case stat_MenuMain:
		status_up = stat_MenuSet;
		status_down = stat_MenuHelp;
		status_left = stat_Null;
		status_right = stat_MenuSelect;
		break;
	case stat_MenuSelect:
		status_up = stat_Null;
		status_down = stat_Null;
		status_left = stat_MenuMain;
		status_right = stat_Null;
		break;
	case stat_MenuSet:
		status_up = stat_Null;
		status_down = stat_MenuMain;
		status_left = stat_Null;
		status_right = stat_Null;
		break;
	case stat_MenuHelp:
		status_up = stat_MenuMain;
		status_down = stat_Null;
		status_left = stat_Null;
		status_right = stat_Null;
		break;
	}
}

void MenuMain::Update(float timeTotal, float timeDelta)
{
	if (!moved)
	{
		if (move_x_new != move_x)
		{
			if (move_x_new > move_x)
			{
				move_x += timeDelta * 1600.f * Game_Data->scr_scale;
				if (move_x_new <= move_x)
					move_x = move_x_new;
			}
			else
			{
				move_x -= (int)(timeDelta * 1600.f * Game_Data->scr_scale);
				if (move_x_new >= move_x)
				{
					move_x = move_x_new;
				}
			}
			if (move_x_new == move_x && move_x != 0)
				Game_Data->Status_Next = status_new;
		}

		if (move_y_new != move_y)
		{
			if (move_y_new > move_y)
			{
				move_y += (int)(timeDelta * 1600.0f * Game_Data->scr_scale);
				if (move_y_new <= move_y)
					move_y = move_y_new;
			}
			else
			{
				move_y -= (int)(timeDelta * 1600.0f * Game_Data->scr_scale);
				if (move_y_new >= move_y)
					move_y = move_y_new;
			}
			if (move_y_new == move_y && move_y != 0)
				Game_Data->Status_Next = status_new;
		}
	}

	Update_Eye(timeTotal, timeDelta);

	if (sel_map_sel_alpha != sel_map_sel_alpha_new)
	{
		if (sel_map_sel_alpha_new > sel_map_sel_alpha)
		{
			sel_map_sel_alpha += timeDelta * 4;
			if (sel_map_sel_alpha_new <= sel_map_sel_alpha)
				sel_map_sel_alpha = sel_map_sel_alpha_new;
		}
		else
		{
			sel_map_sel_alpha -= timeDelta * 4;
			if (sel_map_sel_alpha_new >= sel_map_sel_alpha)
				sel_map_sel_alpha = sel_map_sel_alpha_new;
		}
	}

	if (sel_level_sel_alpha != sel_level_sel_alpha_new)
	{
		if (sel_level_sel_alpha_new > sel_level_sel_alpha)
		{
			sel_level_sel_alpha += timeDelta * 4;
			if (sel_level_sel_alpha_new <= sel_level_sel_alpha)
				sel_level_sel_alpha = sel_level_sel_alpha_new;
		}
		else
		{
			sel_level_sel_alpha -= timeDelta * 4;
			if (sel_level_sel_alpha_new >= sel_level_sel_alpha)
				sel_level_sel_alpha = sel_level_sel_alpha_new;
		}
	}

	m_2d_main_but_classic->Update(timeDelta);
	m_2d_main_but_time->Update(timeDelta);
	m_2d_main_but_surv->Update(timeDelta);
	m_2d_main_but_set->Update(timeDelta);
	m_2d_main_but_help->Update(timeDelta);

	m_2d_sel_but_start->Update(timeDelta);

	m_2d_set_but_cal->Update(timeDelta);
};

void MenuMain::Draw()
{
	XMMATRIX tmpm;
	XMMATRIX proj3d = XMLoadFloat4x4( &Game_Data->m_proj3d );
	XMMATRIX proj2d = XMLoadFloat4x4( &Game_Data->m_proj2d );
	XMMATRIX world = XMLoadFloat4x4( &Game_Data->m_world );
    XMMATRIX view = XMLoadFloat4x4( &Game_Data->m_view );

	//Вывод 3D
	Game_Data->m_batchEffect->SetProjection( proj3d );

	//Рисование модели фона
	m_model_bg->Draw( Game_Data->context, *Game_Data->m_states, world, view, proj3d, 1.f, true, true, Colors::Gray);

	//Вывод 2D
	Game_Data->m_batchEffect->SetProjection( proj2d );

	Game_Data->m_sprites->Begin();

	switch (Game_Data->Status_Current)
	{
	case stat_MenuMain:
		DrawMain(move_x, move_y);
		break;
	case stat_MenuSelect:
		DrawSelect(move_x, move_y);
		break;
	case stat_MenuSet:
		DrawSet(move_x, move_y);
		break;
	case stat_MenuHelp:
		DrawHelp(move_x, move_y);
		break;
	}

	if (move_x > 0)
	{
		switch (status_right)
		{
		case stat_MenuMain:
			DrawMain(move_x - move_x_dlt, move_y);
			break;
		case stat_MenuSelect:
			DrawSelect(move_x - move_x_dlt, move_y);
			break;
		case stat_MenuSet:
			DrawSet(move_x - move_x_dlt, move_y);
			break;
		case stat_MenuHelp:
			DrawHelp(move_x - move_x_dlt, move_y);
			break;
		}
	}
	else if (move_x < 0)
	{
		switch (status_left)
		{
		case stat_MenuMain:
			DrawMain(move_x + move_x_dlt, move_y);
			break;
		case stat_MenuSelect:
			DrawSelect(move_x + move_x_dlt, move_y);
			break;
		case stat_MenuSet:
			DrawSet(move_x + move_x_dlt, move_y);
			break;
		case stat_MenuHelp:
			DrawHelp(move_x + move_x_dlt, move_y);
			break;
		}
	}

	if (move_y < 0)
	{
		switch (status_down)
		{
		case stat_MenuMain:
			DrawMain(move_x, move_y + move_y_dlt);
			break;
		case stat_MenuSelect:
			DrawSelect(move_x, move_y + move_y_dlt);
			break;
		case stat_MenuSet:
			DrawSet(move_x, move_y + move_y_dlt);
			break;
		case stat_MenuHelp:
			DrawHelp(move_x, move_y + move_y_dlt);
			break;
		}
	}
	else if (move_y > 0)
	{
		switch (status_up)
		{
		case stat_MenuMain:
			DrawMain(move_x, move_y - move_y_dlt);
			break;
		case stat_MenuSelect:
			DrawSelect(move_x, move_y - move_y_dlt);
			break;
		case stat_MenuSet:
			DrawSet(move_x, move_y - move_y_dlt);
			break;
		case stat_MenuHelp:
			DrawHelp(move_x, move_y - move_y_dlt);
			break;
		}
	}
	Game_Data->m_sprites->End();
};

void MenuMain::DrawMain(int x, int y)
{
	m_2d_main_but_classic->Draw(1.f, m_2d_main_but_classic->x - x, m_2d_main_but_classic->y - y);
	m_2d_main_but_time->Draw(1.f, m_2d_main_but_time->x - x, m_2d_main_but_time->y - y);
	m_2d_main_but_surv->Draw(1.f, m_2d_main_but_surv->x - x, m_2d_main_but_surv->y- y);
	m_2d_main_but_set->Draw(1.f, m_2d_main_but_set->x - x, m_2d_main_but_set->y - y);
	m_2d_main_but_help->Draw(1.f, m_2d_main_but_help->x - x, m_2d_main_but_help->y - y);
}

void MenuMain::DrawSelect(int x, int y)
{
	m_2d_sel_map_bg->Draw(1.f, m_2d_sel_map_bg->x - x, m_2d_sel_map_bg->y - y);
	m_2d_sel_map[Game_Data->Map]->Draw(1.f, m_2d_sel_map[Game_Data->Map]->x - x, m_2d_sel_map[Game_Data->Map]->y - y);

	m_2d_sel_level_bg->Draw(1.f, m_2d_sel_level_bg->x - x, m_2d_sel_level_bg->y - y);
	m_2d_sel_level_dig->Draw(2, Game_Data->Level, 1.f, m_2d_sel_level_dig->x - x, m_2d_sel_level_dig->y - y);

	switch (Game_Data->Game_Type)
	{
		case type_time:
			m_2d_sel_rec_bg_time->Draw(1.f, m_2d_sel_rec_bg_time->x - x, m_2d_sel_rec_bg_time->y - y);
			break;
		case type_surv:
			m_2d_sel_rec_bg_surv->Draw(1.f, m_2d_sel_rec_bg_surv->x - x, m_2d_sel_rec_bg_surv->y - y);
			break;
		default:
			m_2d_sel_rec_bg_classic->Draw(1.f, m_2d_sel_rec_bg_classic->x - x, m_2d_sel_rec_bg_classic->y - y);
			break;
	}

	m_2d_sel_rec_dig->Draw(5, Game_Data->scores[Game_Data->Map][Game_Data->Level-1][Game_Data->Game_Type], 1.f, m_2d_sel_rec_dig->x - x, m_2d_sel_rec_dig->y - y);

	m_2d_sel_but_start->Draw(1.f, m_2d_sel_but_start->x - x, m_2d_sel_but_start->y - y);

	if (sel_map_sel_alpha > 0)
	{
		m_2d_sel_map_sel_bg->Draw(sel_map_sel_alpha);
		for (i = 0; i < MAP_MAX; i++)
		{
			m_2d_sel_map[i]->Draw(sel_map_sel_alpha, m_2d_sel_map[i]->x + sel_map_sel_x + (i * 300 * Game_Data->scr_scale));
		}
	}

	if (sel_level_sel_alpha > 0)
	{
		m_2d_sel_level_sel_bg->Draw(sel_level_sel_alpha);
		for (i = 0; i < LEVEL_MAX ; i++)
		{
			m_2d_sel_level_dig->Draw(2, (i + 1), sel_level_sel_alpha, m_2d_sel_level_dig->x + sel_level_sel_x + (i * 110 * Game_Data->scr_scale));
		}
	}
}

void MenuMain::DrawSet(int x, int y)
{
	m_2d_set_sound_bg->Draw(1.f, m_2d_set_sound_bg->x - x, m_2d_set_sound_bg->y - y);
	if (Game_Data->set_sound)
		m_2d_set_sound_check->Draw(1.f, m_2d_set_sound_bg->x + (468 * Game_Data->scr_scale) - x, m_2d_set_sound_check->y - y);
	else
		m_2d_set_sound_check->Draw(1.f, m_2d_set_sound_bg->x + (210 * Game_Data->scr_scale) - x, m_2d_set_sound_check->y - y);

	m_2d_set_but_cal->Draw(1.f, m_2d_set_but_cal->x - x, m_2d_set_but_cal->y - y);
}

void MenuMain::DrawHelp(int x, int y)
{
	m_2d_help->Draw(1.f, m_2d_help->x - x, m_2d_help->y - y);
}

void MenuMain::OnPointerPressed(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	moved = false;
	move_x_old = (int)PointerPoint->Position.Y;
	move_x_start = (int)PointerPoint->Position.Y;
	move_y_old = (int)PointerPoint->Position.X;
	move_y_start = (int)PointerPoint->Position.X;

	switch (Game_Data->Status_Current)
	{
	case stat_MenuMain:
		OnPointerPressed_Main(PointerPoint);
		break;
	case stat_MenuSelect:
		OnPointerPressed_Select(PointerPoint);
		break;
	case stat_MenuSet:
		OnPointerPressed_Set(PointerPoint);
		break;
	case stat_MenuHelp:
		OnPointerPressed_Help(PointerPoint);
		break;
	}
}

void MenuMain::OnPointerPressed_Main(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	m_2d_main_but_classic->Click(PointerPoint);
//	if (!Game_Data->isTrial)
//	{
	m_2d_main_but_time->Click(PointerPoint);
	m_2d_main_but_surv->Click(PointerPoint);
//	}
	m_2d_main_but_set->Click(PointerPoint);
	m_2d_main_but_help->Click(PointerPoint);
}

void MenuMain::OnPointerPressed_Select(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (sel_map_sel_alpha == 1)
	{
		sel_map_sel_x_moved = false;
		sel_map_sel_x_old = PointerPoint->Position.Y;
	}
	else if (sel_level_sel_alpha == 1)
	{
		sel_level_sel_x_old = PointerPoint->Position.Y;
		sel_level_sel_x_moved = false;
	}
	else
	{
		m_2d_sel_but_start->Click(PointerPoint);
		moved = false;
	}
}

void MenuMain::OnPointerPressed_Set(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	m_2d_set_but_cal->Click(PointerPoint);
}

void MenuMain::OnPointerPressed_Help(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (PointerPoint->Position.Y > 0 && PointerPoint->Position.Y < 128)
	{
		if (PointerPoint->Position.X < 128 && PointerPoint->Position.X > 0)
		{
			Game_Data->Status_Next = stat_MenuMain;
		}
	}
}

void MenuMain::OnPointerMoved(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	switch (Game_Data->Status_Current)
	{
	case stat_MenuMain:
		OnPointerMoved_Main(PointerPoint);
		break;
	case stat_MenuSelect:
		OnPointerMoved_Select(PointerPoint);
		break;
	case stat_MenuSet:
		OnPointerMoved_Set(PointerPoint);
		break;
	case stat_MenuHelp:
		OnPointerMoved_Help(PointerPoint);
		break;
	}
}

void MenuMain::OnPointerMoved_Main(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (abs(PointerPoint->Position.Y - move_x_start) > abs(PointerPoint->Position.X - move_y_start))
	{
		move_x -= (int)PointerPoint->Position.Y - move_x_old;
		move_x_old = (int)PointerPoint->Position.Y;
		move_y = 0;
	}
	else
	{
		move_y -= (int)PointerPoint->Position.X - move_y_old;
		move_y_old = (int)PointerPoint->Position.X;
		move_x = 0;
	}
	moved = true;

	m_2d_main_but_classic->Move(PointerPoint);
//	if (!Game_Data->isTrial)
//	{
	m_2d_main_but_time->Move(PointerPoint);
	m_2d_main_but_surv->Move(PointerPoint);
//	}
	m_2d_main_but_set->Move(PointerPoint);
	m_2d_main_but_help->Move(PointerPoint);
}

void MenuMain::OnPointerMoved_Select(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (sel_map_sel_alpha == 1)
	{
		if (m_2d_sel_map_sel_bg->Click(PointerPoint, 0, 0, 1024 * Game_Data->scr_scale, 256 * Game_Data->scr_scale))
		{
			sel_map_sel_x += PointerPoint->Position.Y - sel_map_sel_x_old;
			sel_map_sel_x_old = PointerPoint->Position.Y;
			if (sel_map_sel_x > 0)
				sel_map_sel_x = 0;
			else if (sel_map_sel_x < -((MAP_MAX - 1) * 300 * Game_Data->scr_scale))
				sel_map_sel_x = -(MAP_MAX - 1) * 300 * Game_Data->scr_scale;
			sel_map_sel_x_moved = true;
		}
	}
	else if (sel_level_sel_alpha == 1)
	{
		if (m_2d_sel_level_sel_bg->Click(PointerPoint, 0, 0, 1024 * Game_Data->scr_scale, 128 * Game_Data->scr_scale))
		{
			sel_level_sel_x += PointerPoint->Position.Y - sel_level_sel_x_old;
			sel_level_sel_x_old = PointerPoint->Position.Y;
			if (sel_level_sel_x > 0)
				sel_level_sel_x = 0;
			else if (sel_level_sel_x < -((LEVEL_MAX - 1) * 110 * Game_Data->scr_scale))
				sel_level_sel_x = -(LEVEL_MAX - 1) * 110 * Game_Data->scr_scale;
			sel_level_sel_x_moved = true;
		}
	}
	else
	{
		if (abs(PointerPoint->Position.Y - move_x_start) > abs(PointerPoint->Position.X - move_y_start))
		{
			move_x -= (int)PointerPoint->Position.Y - move_x_old;
			move_x_old = (int)PointerPoint->Position.Y;
			move_y = 0;
		}
		else
		{
			move_y -= (int)PointerPoint->Position.X - move_y_old;
			move_y_old = (int)PointerPoint->Position.X;
			move_x = 0;
		}
		moved = true;

		m_2d_sel_but_start->Move(PointerPoint);
	}
}

void MenuMain::OnPointerMoved_Set(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (abs(PointerPoint->Position.Y - move_x_start) > abs(PointerPoint->Position.X - move_y_start))
	{
		move_x -= (int)PointerPoint->Position.Y - move_x_old;
		move_x_old = (int)PointerPoint->Position.Y;
		move_y = 0;
	}
	else
	{
		move_y -= (int)PointerPoint->Position.X - move_y_old;
		move_y_old = (int)PointerPoint->Position.X;
		move_x = 0;
	}
	moved = true;

	m_2d_set_but_cal->Move(PointerPoint);
}

void MenuMain::OnPointerMoved_Help(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (abs(PointerPoint->Position.Y - move_x_start) > abs(PointerPoint->Position.X - move_y_start))
	{
		move_x -= (int)PointerPoint->Position.Y - move_x_old;
		move_x_old = (int)PointerPoint->Position.Y;
		move_y = 0;
	}
	else
	{
		move_y -= (int)PointerPoint->Position.X - move_y_old;
		move_y_old = (int)PointerPoint->Position.X;
		move_x = 0;
	}
	moved = true;
}

void MenuMain::OnPointerReleased(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	switch (Game_Data->Status_Current)
	{
	case stat_MenuMain:
		OnPointerReleased_Main(PointerPoint);
		break;
	case stat_MenuSelect:
		OnPointerReleased_Select(PointerPoint);
		break;
	case stat_MenuSet:
		OnPointerReleased_Set(PointerPoint);
		break;
	case stat_MenuHelp:
		OnPointerReleased_Help(PointerPoint);
		break;
	}

	if (moved)
	{
		switch (Game_Data->Status_Current)
		{
		case stat_MenuMain:
			if (move_x > 150)
			{
				move_x_new = move_x_dlt;
				status_new = stat_MenuSelect;
			}
			else if (move_y > 150)
			{
				move_y_new = move_y_dlt;
				status_new = stat_MenuSet;
			}
			else if (move_y < -150)
			{
				move_y_new = -move_y_dlt;
				status_new = stat_MenuHelp;
			}
			else
			{
				move_x_new = 0;
				move_y_new = 0;
			}
			break;
		case stat_MenuSelect:
			if (move_x < -150)
			{
				move_x_new = -move_x_dlt;
				status_new = stat_MenuMain;
			}
			else
			{
				move_x_new = 0;
				move_y_new = 0;
			}
			break;
		case stat_MenuSet:
			if (move_y < -150)
			{
				move_y_new = -move_y_dlt;
				status_new = stat_MenuMain;
			}
			else
			{
				move_x_new = 0;
				move_y_new = 0;
			}
			break;
		case stat_MenuHelp:
			if (move_y > 150)
			{
				move_y_new = move_y_dlt;
				status_new = stat_MenuMain;
			}
			else
			{
				move_x_new = 0;
				move_y_new = 0;
			}
			break;
		}

		moved = false;
	}
}

void MenuMain::OnPointerReleased_Main(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (m_2d_main_but_classic->Release(PointerPoint, moved))
	{
		move_x_new = move_x_dlt;
		status_new = stat_MenuSelect;
		Game_Data->Game_Type = type_classic;
	}

//	if (!Game_Data->isTrial)
//	{
	if (m_2d_main_but_time->Release(PointerPoint, moved))
	{
		move_x_new = move_x_dlt;
		status_new = stat_MenuSelect;
		Game_Data->Game_Type = type_time;
	}

	if (m_2d_main_but_surv->Release(PointerPoint, moved))
	{
		move_x_new = move_x_dlt;
		status_new = stat_MenuSelect;
		Game_Data->Game_Type = type_surv;
	}
//	}

	if (m_2d_main_but_set->Release(PointerPoint, moved))
	{
		move_y_new = move_y_dlt;
		status_new = stat_MenuSet;
	}

	if (m_2d_main_but_help->Release(PointerPoint, moved))
	{
		move_y_new = -move_y_dlt;
		status_new = stat_MenuHelp;
	}
}

void MenuMain::OnPointerReleased_Select(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (sel_map_sel_alpha == 1)
	{
		if (!sel_map_sel_x_moved)
		{
			if (m_2d_sel_map_sel_bg->Click(PointerPoint, 0, 0, 1024 * Game_Data->scr_scale, 256 * Game_Data->scr_scale))
			{
				for (i = 0; i < MAP_MAX; i++)
				{
					if (m_2d_sel_map[0]->Click(PointerPoint, (5 * Game_Data->scr_scale + sel_map_sel_x + (i * 300 * Game_Data->scr_scale)), 51 * Game_Data->scr_scale, 245 * Game_Data->scr_scale, 154 * Game_Data->scr_scale))
					{
						Game_Data->Map = i;
						sel_map_sel_alpha_new = 0;
						break;
					}
				}
			}
			else
			{
				sel_map_sel_alpha_new = 0;
			}
		}
	}
	else if (sel_level_sel_alpha == 1)
	{
		if (!sel_level_sel_x_moved)
		{
			if (m_2d_sel_level_sel_bg->Click(PointerPoint, 0, 0, 1024 * Game_Data->scr_scale, 128 * Game_Data->scr_scale))
			{
				for (i = 0; i < LEVEL_MAX; i++)
				{
					if (m_2d_sel_level_bg->Click(PointerPoint, (115 * Game_Data->scr_scale + sel_level_sel_x + (i * 110 * Game_Data->scr_scale)), 13 * Game_Data->scr_scale, 100 * Game_Data->scr_scale, 100 * Game_Data->scr_scale))
					{
						Game_Data->Level = i + 1;
						sel_level_sel_alpha_new = 0;
						break;
					}
				}
			}
			else
			{
				sel_level_sel_alpha_new = 0;
			}
		}
	}
	else
	{
		if (m_2d_sel_map_bg->Click(PointerPoint, 100 * Game_Data->scr_scale, 38 * Game_Data->scr_scale, 280 * Game_Data->scr_scale, 180 * Game_Data->scr_scale))
		{
			sel_map_sel_alpha_new = 1;
			sel_map_sel_x = -Game_Data->Map * 300 * Game_Data->scr_scale;
		}

		if (m_2d_sel_level_bg->Click(PointerPoint, 115 * Game_Data->scr_scale, 13 * Game_Data->scr_scale, 100 * Game_Data->scr_scale, 102 * Game_Data->scr_scale))
		{
			sel_level_sel_alpha_new = 1;
			sel_level_sel_x = -(Game_Data->Level - 1) * 110 * Game_Data->scr_scale;
		}

		if (m_2d_sel_but_start->Release(PointerPoint, moved))
		{
			Game_Data->Status_Next = stat_Start;
			Game_Data->loading = true;
		}
	}
}

void MenuMain::OnPointerReleased_Set(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (m_2d_set_but_cal->Release(PointerPoint, moved))
	{
		Game_Data->Status_Next = stat_MenuCal;
		Game_Data->loading = true;
	}
	else if (m_2d_set_sound_bg->Click(PointerPoint, 0, 41 * Game_Data->scr_scale, 256 * Game_Data->scr_scale, 80 * Game_Data->scr_scale))
	{
		Game_Data->set_sound = false;
	}
	else if (m_2d_set_sound_bg->Click(PointerPoint, 256 * Game_Data->scr_scale, 41 * Game_Data->scr_scale, 256 * Game_Data->scr_scale, 80 * Game_Data->scr_scale))
	{
		Game_Data->set_sound = true;
	}
}

void MenuMain::OnPointerReleased_Help(Windows::UI::Input::PointerPoint ^PointerPoint)
{
}

void MenuMain::OnBackButtonPressed(Windows::Phone::UI::Input::BackPressedEventArgs^ args)
{
	switch (Game_Data->Status_Current)
	{
	case stat_MenuMain:
		args->Handled = false;
		break;
	case stat_MenuSelect:
		move_x_new = -move_x_dlt;
		status_new = stat_MenuMain;
		break;
	case stat_MenuSet:
		move_y_new = -move_y_dlt;
		status_new = stat_MenuMain;
		break;
	case stat_MenuHelp:
		move_y_new = move_y_dlt;
		status_new = stat_MenuMain;
		break;
	}
}

MenuMain::~MenuMain()
{
	//Главное меню
	delete m_2d_main_but_classic;
	delete m_2d_main_but_time;
	delete m_2d_main_but_surv;
	delete m_2d_main_but_set;
	delete m_2d_main_but_help;

	//Меню выбора уровня
	delete m_2d_sel_map_bg;
	delete m_2d_sel_level_bg;
	delete m_2d_sel_rec_bg_classic;
	delete m_2d_sel_rec_bg_time;
	delete m_2d_sel_rec_bg_surv;

	for (i = 0; i < MAP_MAX; i++)
		delete m_2d_sel_map[i];

	delete m_2d_sel_level_dig;
	delete m_2d_sel_rec_dig;
	delete m_2d_sel_but_start;

	//Полоса выбора карты
	delete m_2d_sel_map_sel_bg;

	//Полоса выбора уровня
	delete m_2d_sel_level_sel_bg;

	//Меню настроек
	delete m_2d_set_sound_bg;
	delete m_2d_set_sound_check;
	delete m_2d_set_but_cal;

	//меню помощи
	delete m_2d_help;
}
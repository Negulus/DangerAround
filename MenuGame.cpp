#include "pch.h"

#include "MenuGame.h"

#include "DDSTextureLoader.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

MenuGame::MenuGame(GameData *data)
{
	Game_Data = data;

	//Инициализация моделей и текстур
	m_2d_bg_win = new Scr2D(Game_Data, -112, -16, L"Images\\Other\\menu_game_bg_win.dds");
	m_2d_bg_lose = new Scr2D(Game_Data, -112, -16, L"Images\\Other\\menu_game_bg_lose.dds");
	m_2d_bg_pause = new Scr2D(Game_Data, -112, -16, L"Images\\Other\\menu_game_bg_pause.dds");
	m_2d_bg_surv = new Scr2D(Game_Data, -112, -16, L"Images\\Other\\menu_game_bg_surv.dds");
	m_2d_bg_classic = new Scr2D(Game_Data, -112, -16, L"Images\\Other\\menu_game_bg_classic.dds");

	m_2d_but_restart = new But2D(Game_Data, 133, 208, L"Images\\Other\\menu_game_but_restart.dds", 9, 2, 238, 60);
	m_2d_but_resume = new But2D(Game_Data, 413, 298, L"Images\\Other\\menu_game_but_resume.dds", 9, 2, 238, 60);
	m_2d_but_next = new But2D(Game_Data, 133, 118, L"Images\\Other\\menu_game_but_next.dds", 9, 2, 238, 60);
	m_2d_but_exit = new But2D(Game_Data, 133, 298, L"Images\\Other\\menu_game_but_exit.dds", 9, 2, 238, 60);

	m_2d_dig = new Scr2DDigit(Game_Data, 536, 301, 22, 0);
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

	m_2d_dig_big = new Scr2DDigit(Game_Data, 509, 249, 29, 0);
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

	//Звуки
	m_sound_win.reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\win.wav"));
	m_inst_win = m_sound_win->CreateInstance();

	m_sound_lose.reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\lose.wav"));
	m_inst_lose = m_sound_lose->CreateInstance();

	m_sound_pause.reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\pause.wav"));
	m_inst_pause = m_sound_pause->CreateInstance();

	m_sound_surv.reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\surv.wav"));
	m_inst_surv = m_sound_surv->CreateInstance();
};

void MenuGame::Update(float timeTotal, float timeDelta)
{
	if (!Run)
	{
		Run = true;
		alpha = 0;
		score_alpha = 0;
		score_time_set = 0;
		score_stat = 0;
		if (Game_Data->set_sound)
		{
			switch (Game_Data->Status_Current)
			{
			case stat_Win:
				m_inst_win->Stop();
				m_inst_win->Play();
				break;
			case stat_Lose:
				m_inst_lose->Stop();
				m_inst_lose->Play();
				break;
			case stat_Pause:
				m_inst_pause->Stop();
				m_inst_pause->Play();
				break;
			case stat_GameOver:
				m_inst_surv->Stop();
				m_inst_surv->Play();
				break;
			}
		}
	}

	if (alpha < 1)
	{
		alpha += timeDelta * 4;
		if (alpha > 1) alpha = 1;
	}
	else
	{
		switch (score_stat)
		{
		case 0:
			score_alpha = 1;
			score_time_set = timeTotal + 0.2f;
			score_stat = 1;
			break;
		case 1:
			if (timeTotal >= score_time_set)
			{
				score_alpha = 0;
				score_time_set = timeTotal + 0.2f;
				score_stat = 2;
			}
			break;
		case 2:
			if (timeTotal >= score_time_set)
			{
				score_alpha = 1;
				score_time_set = timeTotal + 0.2f;
				score_stat = 3;
			}
			break;
		case 3:
			if (timeTotal >= score_time_set)
			{
				score_alpha = 0;
				score_time_set = timeTotal + 0.2f;
				score_stat = 4;
			}
			break;
		case 4:
			if (timeTotal >= score_time_set)
			{
				score_alpha = 1;
				score_stat = 5;
			}
			break;
		}
	}

	m_2d_but_restart->Update(timeDelta);
	m_2d_but_resume->Update(timeDelta);
	m_2d_but_next->Update(timeDelta);
	m_2d_but_exit->Update(timeDelta);
};

void MenuGame::Draw()
{
	switch (Game_Data->Status_Current)
	{
	case stat_Win:
		DrawWin();
		break;
	case stat_Lose:
		DrawLose();
		break;
	case stat_Pause:
		DrawPause();
		break;
	case stat_GameOver:
		DrawSurv();
		break;
	}
};

void MenuGame::DrawWin()
{
	m_2d_bg_win->Draw(alpha);

	m_2d_but_next->Draw(alpha);
	m_2d_but_restart->Draw(alpha);
	m_2d_but_exit->Draw(alpha);

	m_2d_dig->Draw(5, Game_Data->Score_Last);

	if (Game_Data->Score > Game_Data->Score_Last)
		m_2d_dig_big->Draw(5, Game_Data->Score, score_alpha);
	else
		m_2d_dig->Draw(5, Game_Data->Score, score_alpha, m_2d_dig->x, m_2d_dig->y + 52);
}

void MenuGame::DrawLose()
{
	m_2d_bg_lose->Draw(alpha);

	m_2d_but_restart->Draw(alpha);
	m_2d_but_exit->Draw(alpha);
}

void MenuGame::DrawPause()
{
	m_2d_bg_pause->Draw(alpha);

	m_2d_but_resume->Draw(alpha);
	m_2d_but_restart->Draw(alpha);
	m_2d_but_exit->Draw(alpha);
}

void MenuGame::DrawSurv()
{
	if (Game_Data->Game_Type == type_classic)
		m_2d_bg_classic->Draw(alpha);
	else
		m_2d_bg_surv->Draw(alpha);

	m_2d_but_restart->Draw(alpha);
	m_2d_but_exit->Draw(alpha);

	m_2d_dig->Draw(5, Game_Data->Score_Last);

	if (Game_Data->Score > Game_Data->Score_Last)
		m_2d_dig_big->Draw(5, Game_Data->Score, score_alpha);
	else
		m_2d_dig->Draw(5, Game_Data->Score, score_alpha, m_2d_dig->x, m_2d_dig->y + 52);
}

void MenuGame::OnPointerPressed(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	moved = false;

	m_2d_but_restart->Click(PointerPoint);
	m_2d_but_resume->Click(PointerPoint);
	m_2d_but_next->Click(PointerPoint);
	m_2d_but_exit->Click(PointerPoint);
}

void MenuGame::OnPointerMoved(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	moved = true;

	m_2d_but_restart->Move(PointerPoint);
	m_2d_but_resume->Move(PointerPoint);
	m_2d_but_next->Move(PointerPoint);
	m_2d_but_exit->Move(PointerPoint);
}

void MenuGame::OnPointerReleased(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	switch (Game_Data->Status_Current)
	{
	case stat_Win:
		if (m_2d_but_next->Release(PointerPoint, moved))
		{
			Game_Data->Status_Next = stat_Next;
			Game_Data->loading = true;
		}
		else if (m_2d_but_restart->Release(PointerPoint, moved))
		{
			Game_Data->Status_Next = stat_Restart;
			Game_Data->loading = true;
		}
		else if (m_2d_but_exit->Release(PointerPoint, moved))
		{
			Game_Data->Status_Next = stat_MenuMain;
			Game_Data->loading = true;
		}
		break;
	case stat_Lose:
		if (m_2d_but_restart->Release(PointerPoint, moved))
		{
			Game_Data->Status_Next = stat_Restart;
			Game_Data->loading = true;
		}
		else if (m_2d_but_exit->Release(PointerPoint, moved))
		{
			Game_Data->Status_Next = stat_MenuMain;
			Game_Data->loading = true;
		}
		break;
	case stat_Pause:
		if (m_2d_but_resume->Release(PointerPoint, moved))
		{
			Game_Data->Status_Next = stat_Resume;
		}
		else if (m_2d_but_restart->Release(PointerPoint, moved))
		{
			Game_Data->Status_Next = stat_Restart;
			Game_Data->loading = true;
		}
		else if (m_2d_but_exit->Release(PointerPoint, moved))
		{
			Game_Data->Status_Next = stat_MenuMain;
			Game_Data->loading = true;
		}
		break;
	case stat_GameOver:
		if (m_2d_but_restart->Release(PointerPoint, moved))
		{
			Game_Data->Status_Next = stat_Restart;
			Game_Data->loading = true;
		}
		else if (m_2d_but_exit->Release(PointerPoint, moved))
		{
			Game_Data->Status_Next = stat_MenuMain;
			Game_Data->loading = true;
		}
		break;
	}
}

MenuGame::~MenuGame()
{
	//Фоны
	delete m_2d_bg_win;
	delete m_2d_bg_lose;
	delete m_2d_bg_surv;
	delete m_2d_bg_pause;

	//Кнопки
	delete m_2d_but_restart;
	delete m_2d_but_resume;
	delete m_2d_but_next;
	delete m_2d_but_exit;

	delete m_2d_dig;
	delete m_2d_dig_big;
}
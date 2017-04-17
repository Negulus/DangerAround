#pragma once

#include "GameRoot.h"
#include "Scr2D.h"

class MenuMain : public GameRoot
{
public:
	MenuMain(GameData *data);
	~MenuMain();

	void Update(float timeTotal, float timeDelta);

	void Draw();
	void DrawMain(int x, int y);
	void DrawSet(int x, int y);
	void DrawSelect(int x, int y);
	void DrawHelp(int x, int y);

	void OnPointerPressed(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerPressed_Main(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerPressed_Select(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerPressed_Set(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerPressed_Help(Windows::UI::Input::PointerPoint ^PointerPoint);

	void OnPointerMoved(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerMoved_Main(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerMoved_Select(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerMoved_Set(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerMoved_Help(Windows::UI::Input::PointerPoint ^PointerPoint);

	void OnPointerReleased(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerReleased_Main(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerReleased_Select(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerReleased_Set(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerReleased_Help(Windows::UI::Input::PointerPoint ^PointerPoint);

	void OnBackButtonPressed(Windows::Phone::UI::Input::BackPressedEventArgs^ args);

	void Menu_Update();

	

private:
	//Модель фона
	std::unique_ptr<DirectX::Model>		m_model_bg;

	//Главное меню
	But2D		*m_2d_main_but_classic;
	But2D		*m_2d_main_but_time;
	But2D		*m_2d_main_but_surv;
	But2D		*m_2d_main_but_set;
	But2D		*m_2d_main_but_help;

	//Меню выбора уровня
	Scr2D		*m_2d_sel_map_bg;
	Scr2D		*m_2d_sel_level_bg;
	Scr2D		*m_2d_sel_rec_bg_classic;
	Scr2D		*m_2d_sel_rec_bg_time;
	Scr2D		*m_2d_sel_rec_bg_surv;
	Scr2D		*m_2d_sel_map[MAP_MAX];
	Scr2DDigit	*m_2d_sel_level_dig;
	Scr2DDigit	*m_2d_sel_rec_dig;
	But2D		*m_2d_sel_but_start;

	//Полоса выбора карты
	Scr2D		*m_2d_sel_map_sel_bg;
	float		sel_map_sel_alpha;
	float		sel_map_sel_alpha_new;
	bool		sel_map_sel_x_moved;
	float		sel_map_sel_x;
	float		sel_map_sel_x_old;

	//Полоса выбора уровня
	Scr2D		*m_2d_sel_level_sel_bg;
	float		sel_level_sel_alpha;
	float		sel_level_sel_alpha_new;
	int			sel_level_sel_x;
	int			sel_level_sel_x_old;
	bool		sel_level_sel_x_moved;

	//Меню настроек
	Scr2D		*m_2d_set_sound_bg;
	Scr2D		*m_2d_set_sound_check;
	But2D		*m_2d_set_but_cal;

	//Меню помощи
	Scr2D		*m_2d_help;

	//Перемещение между меню	
	bool		moved;
	int			move_x;
	int			move_x_old;
	int			move_x_start;
	int			move_x_new;
	int			move_y;
	int			move_y_old;
	int			move_y_start;
	int			move_y_new;
	Stats		status_up;
	Stats		status_down;
	Stats		status_left;
	Stats		status_right;
	Stats		status_new;

	float	move_x_dlt;
	float	move_y_dlt;
};
#pragma once

#include "GameRoot.h"
#include "Scr2D.h"

class GameHUD : public GameRoot
{
public:
	GameHUD(GameData *data);
	~GameHUD();

	void Update(float timeTotal, float timeDelta);
	void Draw();

	bool OnPointerPressed(Windows::UI::Input::PointerPoint ^PointerPoint);
	bool OnPointerMoved(Windows::UI::Input::PointerPoint ^PointerPoint);
	bool OnPointerReleased(Windows::UI::Input::PointerPoint ^PointerPoint);
private:

	//HUD
	Scr2D			*m_2d_top;
	Scr2D			*m_2d_bottom;
	Scr2D			*m_2d_life;
	Scr2DDigit		*m_2d_dig;
	Scr2DDigit		*m_2d_dig_big;
	But2D			*m_2d_but_pause;

	int				mm;
	int				ss;

	int				score;

	int				life;
	int				life_last;
	int				life_stat;
	float			life_time_set;

	bool			moved;
};
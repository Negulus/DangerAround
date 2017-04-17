#pragma once

#include "GameRoot.h"
#include "Scr2D.h"

class MenuGame : public GameRoot
{
public:
	MenuGame(GameData *data);
	~MenuGame();

	void Update(float timeTotal, float timeDelta);
	void Draw();
	void DrawWin();
	void DrawLose();
	void DrawPause();
	void DrawSurv();

	void OnPointerPressed(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerMoved(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerReleased(Windows::UI::Input::PointerPoint ^PointerPoint);

	bool Run;
private:
	//‘оны
	Scr2D		*m_2d_bg_win;
	Scr2D		*m_2d_bg_lose;
	Scr2D		*m_2d_bg_surv;
	Scr2D		*m_2d_bg_pause;
	Scr2D		*m_2d_bg_classic;

	// нопки
	But2D		*m_2d_but_restart;
	But2D		*m_2d_but_resume;
	But2D		*m_2d_but_next;
	But2D		*m_2d_but_exit;

	Scr2DDigit	*m_2d_dig;
	Scr2DDigit	*m_2d_dig_big;

	float		alpha;
	bool		moved;

	float		score_alpha;
	float		score_time_set;
	int			score_stat;

	//«вуки
	std::unique_ptr<DirectX::SoundEffect>				m_sound_win;
	std::unique_ptr<DirectX::SoundEffectInstance>		m_inst_win;

	std::unique_ptr<DirectX::SoundEffect>				m_sound_lose;
	std::unique_ptr<DirectX::SoundEffectInstance>		m_inst_lose;

	std::unique_ptr<DirectX::SoundEffect>				m_sound_pause;
	std::unique_ptr<DirectX::SoundEffectInstance>		m_inst_pause;

	std::unique_ptr<DirectX::SoundEffect>				m_sound_surv;
	std::unique_ptr<DirectX::SoundEffectInstance>		m_inst_surv;
};
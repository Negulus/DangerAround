#pragma once

#include "GameRoot.h"
#include "Scr2D.h"

class MenuCal : public GameRoot
{
public:
	MenuCal(GameData *data);
	~MenuCal();

	void Update(float timeTotal, float timeDelta);
	void Draw();

	void OnPointerPressed(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerMoved(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerReleased(Windows::UI::Input::PointerPoint ^PointerPoint);

private:
	//Модель фона
	std::unique_ptr<DirectX::Model>		m_model_bg;
	std::unique_ptr<DirectX::Model>		m_model_cal;

	std::unique_ptr<DirectX::Model>		m_model_deg_45;
	std::unique_ptr<DirectX::Model>		m_model_deg_90;
	std::unique_ptr<DirectX::Model>		m_model_deg_120;
	std::unique_ptr<DirectX::Model>		m_model_deg_180;
	std::unique_ptr<DirectX::Model>		m_model_deg_270;
	std::unique_ptr<DirectX::Model>		m_model_deg_360;

	//Модели карты
	std::unique_ptr<DirectX::Model>		m_model_map_45;
	std::unique_ptr<DirectX::Model>		m_model_map_90;
	std::unique_ptr<DirectX::Model>		m_model_map_120;
	std::unique_ptr<DirectX::Model>		m_model_map_180;
	std::unique_ptr<DirectX::Model>		m_model_map_270;
	std::unique_ptr<DirectX::Model>		m_model_map_360;

	std::unique_ptr<DirectX::Model>		m_model_compass;

	//Кнопки
	But2D		*m_2d_but_ok;
	But2D		*m_2d_but_cancel;
	But2D		*m_2d_but_reset;

	Scr2D		*m_2d_scale;
	Scr2D		*m_2d_point;

	float		dist;

	bool		moved;
	bool		scale;
	int			move_y;
	int			move_y_old;
	float		tmp_skyline;
	float		tmp_skyline_set;
	SetAngle	tmp_angle;
};
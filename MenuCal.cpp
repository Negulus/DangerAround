#include "pch.h"

#include "MenuCal.h"

#include "DDSTextureLoader.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

float t;

MenuCal::MenuCal(GameData *data)
{
	Init(data);
	Game_Data = data;

	//Инициализация моделей и текстур
	m_model_bg = Model::CreateFromCMO( Game_Data->device, L"menu_bg.cmo", *Game_Data->m_fxFactory, true, true );
	m_model_cal = Model::CreateFromCMO( Game_Data->device, L"menu_cal.cmo", *Game_Data->m_fxFactory, true, true );

	m_model_map_45 = Model::CreateFromCMO( Game_Data->device, L"map_45.cmo", *Game_Data->m_fxFactory, true, false );
	m_model_map_90 = Model::CreateFromCMO( Game_Data->device, L"map_90.cmo", *Game_Data->m_fxFactory, true, false );
	m_model_map_120 = Model::CreateFromCMO( Game_Data->device, L"map_120.cmo", *Game_Data->m_fxFactory, true, false );
	m_model_map_180 = Model::CreateFromCMO( Game_Data->device, L"map_180.cmo", *Game_Data->m_fxFactory, true, false );
	m_model_map_270 = Model::CreateFromCMO( Game_Data->device, L"map_270.cmo", *Game_Data->m_fxFactory, true, false );
	m_model_map_360 = Model::CreateFromCMO( Game_Data->device, L"map_360.cmo", *Game_Data->m_fxFactory, true, false );

	m_model_compass = Model::CreateFromCMO( Game_Data->device, L"compass.cmo", *Game_Data->m_fxFactory, true, false );

	m_model_deg_45 = Model::CreateFromCMO( Game_Data->device, L"menu_cal_deg_45.cmo", *Game_Data->m_fxFactory, true, true );
	m_model_deg_90 = Model::CreateFromCMO( Game_Data->device, L"menu_cal_deg_90.cmo", *Game_Data->m_fxFactory, true, true );
	m_model_deg_120 = Model::CreateFromCMO( Game_Data->device, L"menu_cal_deg_120.cmo", *Game_Data->m_fxFactory, true, true );
	m_model_deg_180 = Model::CreateFromCMO( Game_Data->device, L"menu_cal_deg_180.cmo", *Game_Data->m_fxFactory, true, true );
	m_model_deg_270 = Model::CreateFromCMO( Game_Data->device, L"menu_cal_deg_270.cmo", *Game_Data->m_fxFactory, true, true );
	m_model_deg_360 = Model::CreateFromCMO( Game_Data->device, L"menu_cal_deg_360.cmo", *Game_Data->m_fxFactory, true, true );

	m_2d_but_ok = new But2D(Game_Data, -25, 284, L"Images\\Other\\menu_cal_but_ok.dds", 48, 5, 160, 54);
	m_2d_but_cancel = new But2D(Game_Data, -25, 348, L"Images\\Other\\menu_cal_but_cancel.dds", 48, 5, 160, 54);
	m_2d_but_reset = new But2D(Game_Data, -25, 412, L"Images\\Other\\menu_cal_but_reset.dds", 48, 5, 160, 54);
	m_2d_scale = new Scr2D(Game_Data, 174, 350, L"Images\\Other\\menu_cal_scale.dds");
	m_2d_point = new Scr2D(Game_Data, m_2d_scale->x + 90, 391, L"Images\\Other\\menu_cal_point.dds");

	dist = 1;

	moved = false;
	tmp_skyline = Game_Data->set_skyline;
	tmp_skyline_set = tmp_skyline;
	tmp_angle = Game_Data->set_angle;
	move_y = 0;
	move_y_old = 0;
};


void MenuCal::Update(float timeTotal, float timeDelta)
{
	Update_Eye(timeTotal, timeDelta, tmp_skyline);

	m_2d_but_ok->Update(timeDelta);
	m_2d_but_cancel->Update(timeDelta);
	m_2d_but_reset->Update(timeDelta);

	if (dist < 50)
		dist += timeDelta * 50.0f;

	if (tmp_skyline_set != tmp_skyline)
	{
		if (tmp_skyline_set > tmp_skyline)
		{
			tmp_skyline += timeDelta;
			if (tmp_skyline_set < tmp_skyline)
				tmp_skyline = tmp_skyline_set;
		}
		else if (tmp_skyline_set < tmp_skyline)
		{
			tmp_skyline -= timeDelta;
			if (tmp_skyline_set > tmp_skyline)
				tmp_skyline = tmp_skyline_set;
		}
	}
};

void MenuCal::Draw()
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

	switch (tmp_angle)
	{
	case deg_45:
		m_model_deg_45->Draw( Game_Data->context, *Game_Data->m_states, world, view, proj3d);
		break;
	case deg_90:
		m_model_deg_90->Draw( Game_Data->context, *Game_Data->m_states, world, view, proj3d);
		break;
	case deg_120:
		m_model_deg_120->Draw( Game_Data->context, *Game_Data->m_states, world, view, proj3d);
		break;
	case deg_180:
		m_model_deg_180->Draw( Game_Data->context, *Game_Data->m_states, world, view, proj3d);
		break;
	case deg_270:
		m_model_deg_270->Draw( Game_Data->context, *Game_Data->m_states, world, view, proj3d);
		break;
	case deg_360:
		m_model_deg_360->Draw( Game_Data->context, *Game_Data->m_states, world, view, proj3d);
		break;
	}

	if (dist < 50)
	{
		float aspectRatio = 480.0f / 800.0f;
		float fovAngleY = 50.0f * XM_PI / 180.0f;
		proj3d = XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, 0.01f, dist);
	}

	m_model_cal->Draw( Game_Data->context, *Game_Data->m_states, world, view, proj3d);

	//Рисование карты
	Game_Data->context->ClearDepthStencilView(Game_Data->m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	tmpm = XMLoadFloat4x4(&Game_Data->m_map_pos);
	m_model_compass->Draw( Game_Data->context, *Game_Data->m_states, tmpm, view, proj3d, 1.0f, true, true, Colors::White );

	switch (tmp_angle)
	{
	case deg_45:
		m_model_map_45->Draw( Game_Data->context, *Game_Data->m_states, tmpm, view, proj3d, 1.0f, true, true, Colors::White );
		break;
	case deg_90:
		m_model_map_90->Draw( Game_Data->context, *Game_Data->m_states, tmpm, view, proj3d, 1.0f, true, true, Colors::White );
		break;
	case deg_120:
		m_model_map_120->Draw( Game_Data->context, *Game_Data->m_states, tmpm, view, proj3d, 1.0f, true, true, Colors::White );
		break;
	case deg_180:
		m_model_map_180->Draw( Game_Data->context, *Game_Data->m_states, tmpm, view, proj3d, 1.0f, true, true, Colors::White );
		break;
	case deg_270:
		m_model_map_270->Draw( Game_Data->context, *Game_Data->m_states, tmpm, view, proj3d, 1.0f, true, true, Colors::White );
		break;
	case deg_360:
		m_model_map_360->Draw( Game_Data->context, *Game_Data->m_states, tmpm, view, proj3d, 1.0f, true, true, Colors::White );
		break;
	}

	//Вывод 2D
	Game_Data->m_batchEffect->SetProjection( proj2d );

	Game_Data->m_sprites->Begin();;
	m_2d_but_ok->Draw();
	m_2d_but_cancel->Draw();
	m_2d_but_reset->Draw();
	m_2d_scale->Draw();
	m_2d_point->Draw(1.0f, (m_2d_scale->x + 30 * Game_Data->scr_scale + 60 * Game_Data->scr_scale * tmp_angle));
	Game_Data->m_sprites->End();
};

void MenuCal::OnPointerPressed(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	move_y_old = (int)PointerPoint->Position.X;
	moved = false;

	if (m_2d_scale->Click(PointerPoint, 77, 6, 358, 115))
	{
		scale = true;
		if (m_2d_scale->Click(PointerPoint, 77 * Game_Data->scr_scale, 0, 60 * Game_Data->scr_scale, 128 * Game_Data->scr_scale))
		{
			tmp_angle = deg_45;
		}
		else if (m_2d_scale->Click(PointerPoint, 137 * Game_Data->scr_scale, 0, 60 * Game_Data->scr_scale, 128 * Game_Data->scr_scale))
		{
			tmp_angle = deg_90;
		}
		else if (m_2d_scale->Click(PointerPoint, 197 * Game_Data->scr_scale, 0, 60 * Game_Data->scr_scale, 128 * Game_Data->scr_scale))
		{
			tmp_angle = deg_120;
		}
		else if (m_2d_scale->Click(PointerPoint, 257 * Game_Data->scr_scale, 0, 60 * Game_Data->scr_scale, 128 * Game_Data->scr_scale))
		{
			tmp_angle = deg_180;
		}
		else if (m_2d_scale->Click(PointerPoint, 317 * Game_Data->scr_scale, 0, 60 * Game_Data->scr_scale, 128 * Game_Data->scr_scale))
		{
			tmp_angle = deg_270;
		}
		else if (m_2d_scale->Click(PointerPoint, 367 * Game_Data->scr_scale, 0, 60 * Game_Data->scr_scale, 128 * Game_Data->scr_scale))
		{
			tmp_angle = deg_360;
		}
	}

	m_2d_but_ok->Click(PointerPoint);
	m_2d_but_cancel->Click(PointerPoint);
	m_2d_but_reset->Click(PointerPoint);
}

void MenuCal::OnPointerMoved(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	m_2d_but_ok->Move(PointerPoint);
	m_2d_but_cancel->Move(PointerPoint);
	m_2d_but_reset->Move(PointerPoint);

	if (scale)
	{
		if (m_2d_scale->Click(PointerPoint, 77 * Game_Data->scr_scale, 0, 60 * Game_Data->scr_scale, 128 * Game_Data->scr_scale))
		{
			tmp_angle = deg_45;
		}
		else if (m_2d_scale->Click(PointerPoint, 137 * Game_Data->scr_scale, 0, 60 * Game_Data->scr_scale, 128 * Game_Data->scr_scale))
		{
			tmp_angle = deg_90;
		}
		else if (m_2d_scale->Click(PointerPoint, 197 * Game_Data->scr_scale, 0, 60 * Game_Data->scr_scale, 128 * Game_Data->scr_scale))
		{
			tmp_angle = deg_120;
		}
		else if (m_2d_scale->Click(PointerPoint, 257 * Game_Data->scr_scale, 0, 60 * Game_Data->scr_scale, 128 * Game_Data->scr_scale))
		{
			tmp_angle = deg_180;
		}
		else if (m_2d_scale->Click(PointerPoint, 317 * Game_Data->scr_scale, 0, 60 * Game_Data->scr_scale, 128 * Game_Data->scr_scale))
		{
			tmp_angle = deg_270;
		}
		else if (m_2d_scale->Click(PointerPoint, 367 * Game_Data->scr_scale, 0, 60 * Game_Data->scr_scale, 128 * Game_Data->scr_scale))
		{
			tmp_angle = deg_360;
		}
	}
	else
	{
		move_y += (int)PointerPoint->Position.X - move_y_old;

		tmp_skyline -= (PointerPoint->Position.X - move_y_old) * 0.001f;
		if (tmp_skyline > 0.54f)
			tmp_skyline = 0.54f;
		else if (tmp_skyline < -0.74f)
			tmp_skyline = -0.74f;

		tmp_skyline_set = tmp_skyline;
		move_y_old = (int)PointerPoint->Position.X;
	}

	moved = true;
}

void MenuCal::OnPointerReleased(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (m_2d_but_ok->Release(PointerPoint, moved))
	{
		Game_Data->set_angle = tmp_angle;
		Game_Data->set_skyline = tmp_skyline;
		Data_Save_Set();
		Game_Data->Status_Next = stat_MenuSet;
		Game_Data->loading = true;
	}
	else if (m_2d_but_cancel->Release(PointerPoint, moved))
	{
		Game_Data->Status_Next = stat_MenuSet;
		Game_Data->loading = true;
	}
	else if (m_2d_but_reset->Release(PointerPoint, moved))
	{
		tmp_angle = deg_90;
		tmp_skyline_set = 0;
	}
	moved = false;
	scale = false;
}

MenuCal::~MenuCal()
{
	delete m_2d_but_ok;
	delete m_2d_but_cancel;
	delete m_2d_but_reset;
	delete m_2d_scale;
	delete m_2d_point;
}
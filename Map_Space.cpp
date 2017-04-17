#include "pch.h"

#include "Map_Space.h"
#include "DDSTextureLoader.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

MapSpace::MapSpace(GameData *data)
{
	Init(data);

	//Инициализация моделей и текстур
	m_model_level_space = Model::CreateFromCMO( Game_Data->device, L"level_space_b.cmo", *Game_Data->m_fxFactory, true, false );;
	m_model_level_planet = Model::CreateFromCMO( Game_Data->device, L"level_space_p.cmo", *Game_Data->m_fxFactory, true, false );;
	m_model_level_sattelite = Model::CreateFromCMO( Game_Data->device, L"level_space_s.cmo", *Game_Data->m_fxFactory, true, false );;
	m_model_level_star = Model::CreateFromCMO( Game_Data->device, L"level_space_st.cmo", *Game_Data->m_fxFactory, true, false );;
	m_model_level_star_1 = Model::CreateFromCMO( Game_Data->device, L"level_space_st1.cmo", *Game_Data->m_fxFactory, true, false );;

	m_model_obj[0] = Model::CreateFromCMO( Game_Data->device, L"asteroid_0.cmo", *Game_Data->m_fxFactory, true, false );
	m_model_obj[1] = Model::CreateFromCMO( Game_Data->device, L"asteroid_2.cmo", *Game_Data->m_fxFactory, true, false );

	m_model_spark = Model::CreateFromCMO( Game_Data->device, L"sparks_1.cmo", *Game_Data->m_fxFactory, true, false );

	m_shape_mappoint = GeometricPrimitive::CreateSphere( Game_Data->context, 0.03f);

	for (i = 0; i < LEVEL_OBJ_MAX; i++)
		Objects[i].en = false;

	for (i = 0; i < LEVEL_SPARK_MAX; i++)
		Sparks[i].en = false;

	m_planet_selfrot = 0;
	m_planet_angle = 20 / 180.f * XM_PI;
	m_planet_pos.x = 36;
	m_planet_pos.y = -10;
	m_planet_pos.z = 53;

	m_sattelite_pos = 28;
	m_sattelite_selfrot = 0;
	m_sattelite_angle = 10 / 180.f * XM_PI;
	m_sattelite_rot = 0;

	m_star_selfrot = 0;
	m_star_selfrot_1 = 0;
	m_star_pos.x = -57;
	m_star_pos.y = 0;
	m_star_pos.z = -5;

	m_sound_ambient.reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_space_ambient.wav"));
	m_inst_ambient = m_sound_ambient->CreateInstance();

	m_sound_stone.reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_space_obj_stone.wav"));
	m_inst_stone = m_sound_stone->CreateInstance();

	m_sound_shield.reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_space_shield.wav"));
	m_inst_shield = m_sound_shield->CreateInstance();

	m_sound_hit.reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_space_hit.wav"));
	m_inst_hit = m_sound_hit->CreateInstance();

	m_sound_other[0].reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_space_other_0.wav"));
	m_sound_other[1].reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_space_other_1.wav"));
	m_sound_other[2].reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_space_other_2.wav"));
	m_sound_other[3].reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_space_other_3.wav"));
	m_sound_other[4].reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_space_other_4.wav"));
	m_sound_other[5].reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_space_other_5.wav"));
	m_inst_other[0] = m_sound_other[0]->CreateInstance();
	m_inst_other[1] = m_sound_other[1]->CreateInstance();
	m_inst_other[2] = m_sound_other[2]->CreateInstance();
	m_inst_other[3] = m_sound_other[3]->CreateInstance();
	m_inst_other[4] = m_sound_other[4]->CreateInstance();
	m_inst_other[5] = m_sound_other[5]->CreateInstance();

	Game_Data->sound_object = false;
	Game_Data->sound_enemy = false;
	Game_Data->sound_hit = false;

	sound_id = (int)Game_Data->randf(0, (MAP_SPACE_SOUND_MAX-1));
	sound_time = Game_Data->randf(10, 40);

	if (Game_Data->set_sound) m_inst_ambient->Play(true);
};

void MapSpace::Pause()
{
	if (m_inst_ambient->GetState() == SoundState::PLAYING)
		m_inst_ambient->Pause();

	if (m_inst_stone->GetState() == SoundState::PLAYING)
		m_inst_stone->Pause();

	if (m_inst_shield->GetState() == SoundState::PLAYING)
		m_inst_shield->Pause();

	if (m_inst_hit->GetState() == SoundState::PLAYING)
		m_inst_hit->Pause();

	for (i = 0; i < MAP_SPACE_SOUND_MAX; i++)
	{
		if (m_inst_other[i]->GetState() == SoundState::PLAYING)
			m_inst_other[i]->Pause();
	}
}

void MapSpace::Resume()
{
	if (m_inst_ambient->GetState() == SoundState::PAUSED)
		m_inst_ambient->Resume();

	if (m_inst_stone->GetState() == SoundState::PAUSED)
		m_inst_stone->Resume();

	if (m_inst_shield->GetState() == SoundState::PAUSED)
		m_inst_shield->Resume();

	if (m_inst_hit->GetState() == SoundState::PAUSED)
		m_inst_hit->Resume();

	for (i = 0; i < MAP_SPACE_SOUND_MAX; i++)
	{
		if (m_inst_other[i]->GetState() == SoundState::PAUSED)
			m_inst_other[i]->Resume();
	}
}

void MapSpace::Update(float timeTotal, float timeDelta)
{
	XMMATRIX tmpm;
	XMMATRIX world = XMLoadFloat4x4( &Game_Data->m_world );

	//Вращение планеты
	m_planet_selfrot += 0.05f * timeDelta;
	if (m_planet_selfrot > 6.283185f) m_planet_selfrot -= 6.283185f;
	else if (m_planet_selfrot < -6.283185f) m_planet_selfrot += 6.283185f;

	tmpm = XMMatrixRotationY(m_planet_selfrot);
	tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationZ(m_planet_angle));
	tmpm = XMMatrixMultiply(tmpm, XMMatrixTranslation(m_planet_pos.x, m_planet_pos.y, m_planet_pos.z));
	tmpm = XMMatrixMultiply(tmpm, world);
	XMStoreFloat4x4(&m_matrix_planet, tmpm);

	//Вращение спутника вокруг планеты
	m_sattelite_selfrot += 0.1f * timeDelta;
	if (m_sattelite_selfrot > 6.283185f) m_sattelite_selfrot -= 6.283185f;
	else if (m_sattelite_selfrot < -6.283185f) m_sattelite_selfrot += 6.283185f;

	m_sattelite_rot += 0.3f * timeDelta;
	if (m_sattelite_rot > 6.283185f) m_sattelite_rot -= 6.283185f;
	else if (m_sattelite_rot < -6.283185f) m_sattelite_rot += 6.283185f;

	tmpm = XMMatrixRotationY(m_sattelite_selfrot);
	tmpm = XMMatrixMultiply(tmpm, XMMatrixTranslation(m_sattelite_pos, 0, 0));
	tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationY(m_sattelite_rot));
	tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationZ(m_sattelite_angle));
	tmpm = XMMatrixMultiply(tmpm, XMMatrixTranslation(m_planet_pos.x, m_planet_pos.y, m_planet_pos.z));
	tmpm = XMMatrixMultiply(tmpm, world);
	XMStoreFloat4x4(&m_matrix_sattelite, tmpm);

	//Вращение звезды
	m_star_selfrot += 2.0f * timeDelta;
	if (m_star_selfrot > 6.283185f) m_star_selfrot -= 6.283185f;
	else if (m_star_selfrot < -6.283185f) m_star_selfrot += 6.283185f;

	tmpm = XMMatrixRotationY(m_star_selfrot);
	tmpm = XMMatrixMultiply(tmpm, XMMatrixTranslation(m_star_pos.x, m_star_pos.y, m_star_pos.z));
	tmpm = XMMatrixMultiply(tmpm, world);
	XMStoreFloat4x4(&m_matrix_star, tmpm);

	m_star_selfrot_1 -= 2.0f * timeDelta;
	if (m_star_selfrot_1 > 6.283185f) m_star_selfrot_1 -= 6.283185f;
	else if (m_star_selfrot_1 < -6.283185f) m_star_selfrot_1 += 6.283185f;

	tmpm = XMMatrixRotationY(m_star_selfrot_1);
	tmpm = XMMatrixMultiply(tmpm, XMMatrixTranslation(m_star_pos.x, m_star_pos.y, m_star_pos.z));
	tmpm = XMMatrixMultiply(tmpm, world);
	XMStoreFloat4x4(&m_matrix_star_1, tmpm);

	UpdateObj(Objects, nullptr, 2, timeTotal, timeDelta, -40, 40, false, OBJ_STONE, 0);
	UpdateSpark(Sparks, timeTotal, timeDelta);

	if (Game_Data->set_sound)
	{
		sound_time -= timeDelta;
		if (sound_time <= 0)
		{
			m_inst_other[sound_id]->Stop();
			m_inst_other[sound_id]->Play();
			sound_id = (int)Game_Data->randf(0, (MAP_SPACE_SOUND_MAX-1));
			if (sound_id < 0) sound_id = 0;
			else if (sound_id >= MAP_SPACE_SOUND_MAX) sound_id = MAP_SPACE_SOUND_MAX - 1;
			sound_time = Game_Data->randf(10, 40);
		}
		if (Game_Data->sound_object)
		{
			if (m_inst_stone->GetState() == SoundState::STOPPED)
			{
				m_inst_stone->Stop();
				m_inst_stone->Play();
			}
			Game_Data->sound_object = false;
		}

		if (Game_Data->sound_hit)
		{
			if (m_inst_hit->GetState() == SoundState::STOPPED)
			{
				m_inst_hit->Stop();
				m_inst_hit->Play();
			}
			Game_Data->sound_hit = false;
		}
	}
};

void MapSpace::Draw()
{
	XMMATRIX tmpm;
	XMMATRIX proj3d = XMLoadFloat4x4( &Game_Data->m_proj3d );
	XMMATRIX world = XMLoadFloat4x4( &Game_Data->m_world );
    XMMATRIX view = XMLoadFloat4x4( &Game_Data->m_view );

	//Рисование модели космоса
	m_model_level_space->Draw( Game_Data->context, *Game_Data->m_states, world, view, proj3d, 1.0f );

	//Рисование моделей планет
	m_model_level_planet->Draw( Game_Data->context, *Game_Data->m_states, XMLoadFloat4x4( &m_matrix_planet ), view, proj3d, 1.0f );
	m_model_level_sattelite->Draw( Game_Data->context, *Game_Data->m_states, XMLoadFloat4x4( &m_matrix_sattelite ), view, proj3d, 1.0f );

	//Рисование моделей звезды
	m_model_level_star->Draw( Game_Data->context, *Game_Data->m_states, XMLoadFloat4x4( &m_matrix_star ), view, proj3d, 2.0f, true, true, Colors::LightGoldenrodYellow );
	m_model_level_star_1->Draw( Game_Data->context, *Game_Data->m_states, XMLoadFloat4x4( &m_matrix_star_1 ), view, proj3d, 1.0f, true, true, Colors::LightGoldenrodYellow );

	Game_Data->context->ClearDepthStencilView(Game_Data->m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	//Рисование объектов
	for (i = 0; i < 8; i++)
	{
		if (Objects[i].en)
		{
			//Рисование объекта
			if (Objects[i].run)
			{
				tmpm = XMLoadFloat4x4(&Objects[i].m_matrix_pos);
				m_model_obj[Objects[i].model]->Draw( Game_Data->context, *Game_Data->m_states, tmpm, view, proj3d, 1.0f, true, true, Colors::Moccasin );
			}

			//Рисование точки на карте
			tmpm = XMLoadFloat4x4(&Objects[i].m_matrix_map);
			m_shape_mappoint->Draw( tmpm, view, proj3d, Colors::Red);
		}
	}

	//Рисование искр
	for (i = 0; i < 8; i++)
	{
		if (Sparks[i].en)
		{
			m_model_spark->Draw(Game_Data->context, *Game_Data->m_states, XMLoadFloat4x4(&Sparks[i].m_matrix_pos), view, proj3d, Sparks[i].alpha);
		}
	}
};

void MapSpace::Draw_MapPoint()
{
	XMMATRIX tmpm;
	XMMATRIX proj3d = XMLoadFloat4x4( &Game_Data->m_proj3d );
	XMMATRIX world = XMLoadFloat4x4( &Game_Data->m_world );
    XMMATRIX view = XMLoadFloat4x4( &Game_Data->m_view );

	for (i = 0; i < LEVEL_OBJ_MAX; i++)
	{
		if (Objects[i].en)
		{
			tmpm = XMLoadFloat4x4(&Objects[i].m_matrix_map);
			m_shape_mappoint->Draw(tmpm, view, proj3d, Colors::Red);
		}
	}
}

void MapSpace::OnPointerPressed(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	for (int k = 0; k < 8; k++)
	{
		if (TouchObj(&Objects[k], PointerPoint))
		{
			CreateSpark(Sparks, Objects[k].pos.x, Objects[k].pos.y, Objects[k].pos.z, 2.f, 0.f, 0.5f, 3.f, 0.7f);
			if (Game_Data->set_sound)
			{
				m_inst_shield->Stop();
				m_inst_shield->Play();
			}
			return;
		}
	}
}

void MapSpace::OnPointerMoved(Windows::UI::Input::PointerPoint ^PointerPoint)
{
}

void MapSpace::OnPointerReleased(Windows::UI::Input::PointerPoint ^PointerPoint)
{
}

MapSpace::~MapSpace()
{
}
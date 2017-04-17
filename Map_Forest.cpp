#include "pch.h"

#include "Map_Forest.h"
#include "DDSTextureLoader.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

MapForest::MapForest(GameData *data)
{
	Init(data);

	//Данные для создания объектов
	end[0] = 0;
	end[1] = 10;
	end[2] = 20;
	end[3] = 30;
	end[4] = 40;
	end[5] = 50;
	end[6] = 60;
	start_min[0] = 3;
	start_min[1] = 11;
	start_min[2] = 20;
	start_min[3] = 29;
	start_min[4] = 39;
	start_min[5] = 48;
	start_min[6] = 63;
	start_max[0] = -11;
	start_max[1] = -3;
	start_max[2] = 7;
	start_max[3] = 15;
	start_max[4] = 24;
	start_max[5] = 32;
	start_max[6] = 40;

	//Инициализация моделей и текстур
	m_model_level_f = Model::CreateFromCMO( Game_Data->device, L"level_forest_f.cmo", *Game_Data->m_fxFactory, true, false );
	m_model_level_n1 = Model::CreateFromCMO( Game_Data->device, L"level_forest_n1.cmo", *Game_Data->m_fxFactory, true, false );
	m_model_level_n2 = Model::CreateFromCMO( Game_Data->device, L"level_forest_n2.cmo", *Game_Data->m_fxFactory, true, false );
	m_model_level_n3 = Model::CreateFromCMO( Game_Data->device, L"level_forest_n3.cmo", *Game_Data->m_fxFactory, true, false );
	m_model_level_b = Model::CreateFromCMO( Game_Data->device, L"level_forest_b.cmo", *Game_Data->m_fxFactory, true, true );

	m_model_obj[obj_type_arrow] = Model::CreateFromCMO( Game_Data->device, L"arrow.cmo", *Game_Data->m_fxFactory, true, false );
	m_model_obj[obj_type_stone] = Model::CreateFromCMO( Game_Data->device, L"stone.cmo", *Game_Data->m_fxFactory, true, false );

	m_model_enemy[0] = Model::CreateFromCMO( Game_Data->device, L"enemy_1.cmo", *Game_Data->m_fxFactory, true, false );
	m_model_enemy[1] = Model::CreateFromCMO( Game_Data->device, L"enemy_1.cmo", *Game_Data->m_fxFactory, true, false );
	m_model_enemy[2] = Model::CreateFromCMO( Game_Data->device, L"enemy_2.cmo", *Game_Data->m_fxFactory, true, false );
	m_model_enemy[3] = Model::CreateFromCMO( Game_Data->device, L"enemy_3.cmo", *Game_Data->m_fxFactory, true, false );

	m_model_shield = Model::CreateFromCMO( Game_Data->device, L"shield.cmo", *Game_Data->m_fxFactory, true, true );

	m_model_spark = Model::CreateFromCMO( Game_Data->device, L"sparks_0.cmo", *Game_Data->m_fxFactory, true, false );

	m_shape_mappoint = GeometricPrimitive::CreateSphere( Game_Data->context, 0.03f);

	for (i = 0; i < LEVEL_OBJ_MAX; i++)
		Objects[i].en = false;

	for (i = 0; i < LEVEL_ENEMY_MAX; i++)
		Enemys[i].en = false;

	for (i = 0; i < LEVEL_PART_MAX; i++)
		Parts[i].en = false;

	for (i = 0; i < LEVEL_SHIELD_MAX; i++)
		Shields[i].en = false;

	for (i = 0; i < LEVEL_SPARK_MAX; i++)
		Sparks[i].en = false;

	m_sound_ambient.reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_forest_ambient.wav"));
	m_inst_ambient = m_sound_ambient->CreateInstance();

	m_sound_stone.reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_forest_obj_stone.wav"));
	m_inst_stone = m_sound_stone->CreateInstance();

	m_sound_arrow.reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_forest_obj_arrow.wav"));
	m_inst_arrow = m_sound_arrow->CreateInstance();

	m_sound_shield.reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_forest_shield.wav"));
	m_inst_shield = m_sound_shield->CreateInstance();

	m_sound_hit.reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_forest_hit.wav"));
	m_inst_hit = m_sound_hit->CreateInstance();

	m_sound_enemy_die.reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_forest_enemy_die.wav"));
	m_inst_enemy_die = m_sound_enemy_die->CreateInstance();

	m_sound_other[0].reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_forest_other_0.wav"));
	m_sound_other[1].reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_forest_other_1.wav"));
	m_sound_other[2].reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_forest_other_2.wav"));
	m_sound_other[3].reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_forest_other_3.wav"));
	m_sound_other[4].reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_forest_other_4.wav"));
	m_sound_other[5].reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_forest_other_5.wav"));
	m_sound_other[6].reset(new SoundEffect(Game_Data->m_audEngine.get(), L"Audio\\level_forest_other_6.wav"));
	m_inst_other[0] = m_sound_other[0]->CreateInstance();
	m_inst_other[1] = m_sound_other[1]->CreateInstance();
	m_inst_other[2] = m_sound_other[2]->CreateInstance();
	m_inst_other[3] = m_sound_other[3]->CreateInstance();
	m_inst_other[4] = m_sound_other[4]->CreateInstance();
	m_inst_other[5] = m_sound_other[5]->CreateInstance();
	m_inst_other[6] = m_sound_other[6]->CreateInstance();

	Game_Data->sound_object = false;
	Game_Data->sound_enemy = false;
	Game_Data->sound_hit = false;

	sound_id = (int)Game_Data->randf(0, (MAP_FOREST_SOUND_MAX-1));
	sound_time = Game_Data->randf(10, 40);

	if (Game_Data->set_sound) m_inst_ambient->Play(true);
};

void MapForest::Pause()
{
	if (m_inst_ambient->GetState() == SoundState::PLAYING)
		m_inst_ambient->Pause();

	if (m_inst_stone->GetState() == SoundState::PLAYING)
		m_inst_stone->Pause();

	if (m_inst_arrow->GetState() == SoundState::PLAYING)
		m_inst_arrow->Pause();

	if (m_inst_shield->GetState() == SoundState::PLAYING)
		m_inst_shield->Pause();

	if (m_inst_hit->GetState() == SoundState::PLAYING)
		m_inst_hit->Pause();

	for (i = 0; i < MAP_FOREST_SOUND_MAX; i++)
	{
		if (m_inst_other[i]->GetState() == SoundState::PLAYING)
			m_inst_other[i]->Pause();
	}
}

void MapForest::Resume()
{
	if (m_inst_ambient->GetState() == SoundState::PAUSED)
		m_inst_ambient->Resume();

	if (m_inst_stone->GetState() == SoundState::PAUSED)
		m_inst_stone->Resume();

	if (m_inst_arrow->GetState() == SoundState::PAUSED)
		m_inst_arrow->Resume();

	if (m_inst_shield->GetState() == SoundState::PAUSED)
		m_inst_shield->Resume();

	if (m_inst_hit->GetState() == SoundState::PAUSED)
		m_inst_hit->Resume();

	for (i = 0; i < MAP_FOREST_SOUND_MAX; i++)
	{
		if (m_inst_other[i]->GetState() == SoundState::PAUSED)
			m_inst_other[i]->Resume();
	}
}

void MapForest::Update(float timeTotal, float timeDelta)
{
	UpdateObj(Objects, Enemys, 1, timeTotal, timeDelta, 0, 40, true, (OBJ_STONE|OBJ_ARROW), OBJ_ARROW);
	UpdateEnm(Enemys, timeTotal, timeDelta);
	UpdateShield(Shields, timeTotal, timeDelta);
	UpdateSpark(Sparks, timeTotal, timeDelta);

	if (Game_Data->set_sound)
	{
		sound_time -= timeDelta;
		if (sound_time <= 0)
		{
			m_inst_other[sound_id]->Stop();
			m_inst_other[sound_id]->Play();
			sound_id = (int)Game_Data->randf(0, (MAP_FOREST_SOUND_MAX-1));
			if (sound_id < 0) sound_id = 0;
			else if (sound_id >= MAP_FOREST_SOUND_MAX) sound_id = MAP_FOREST_SOUND_MAX - 1;
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

		if (Game_Data->sound_enemy)
		{
			if (m_inst_arrow->GetState() == SoundState::STOPPED)
			{
				m_inst_arrow->Stop();
				m_inst_arrow->Play();
			}
			Game_Data->sound_enemy = false;
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

void MapForest::Draw()
{
	XMMATRIX tmpm;
	XMMATRIX proj3d = XMLoadFloat4x4( &Game_Data->m_proj3d );
	XMMATRIX world = XMLoadFloat4x4( &Game_Data->m_world );
    XMMATRIX view = XMLoadFloat4x4( &Game_Data->m_view );

	//Рисование модели уровня заднего плана
	m_model_level_b->Draw( Game_Data->context, *Game_Data->m_states, world, view, proj3d, 1.0f );

	//Рисование моделей уровня средних планов
	m_model_level_n3->Draw( Game_Data->context, *Game_Data->m_states, world, view, proj3d, 1.0f );
	m_model_level_n2->Draw( Game_Data->context, *Game_Data->m_states, world, view, proj3d, 1.0f );
	m_model_level_n1->Draw( Game_Data->context, *Game_Data->m_states, world, view, proj3d, 1.0f );

	//Рисование объектов
	for (i = 0; i < 8; i++)
	{
		if (Objects[i].en)
		{
			//Рисование объекта
			if (Objects[i].run)
			{
				tmpm = XMLoadFloat4x4(&Objects[i].m_matrix_pos);
				m_model_obj[Objects[i].type]->Draw( Game_Data->context, *Game_Data->m_states, tmpm, view, proj3d, 1.0f, true, true, Colors::Moccasin );
			}

			//Рисование точки на карте
			tmpm = XMLoadFloat4x4(&Objects[i].m_matrix_map);
			m_shape_mappoint->Draw( tmpm, view, proj3d, Colors::Red);
		}
	}

	//Рисование атакующих
	for (i = 0; i < 8; i++)
	{
		if (Enemys[i].en)
		{
			m_model_enemy[Enemys[i].model]->Draw(Game_Data->context, *Game_Data->m_states, XMLoadFloat4x4(&Enemys[i].m_matrix_pos), view, proj3d, Enemys[i].alpha);
		}
	}

	//Рисование модели уровня переднего плана
	m_model_level_f->Draw( Game_Data->context, *Game_Data->m_states, world, view, proj3d, 1.0f );

	//Рисование систем частиц
	for (i = 0; i < 32; i++)
	{
		if (Parts[i].en)
		{
			tmpm = XMMatrixMultiply(XMMatrixTranslation(Parts[i].pos.x, Parts[i].pos.y, -Parts[i].pos.z), world);
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

	//Рисование щитов
	for (i = 0; i < 4; i++)
	{
		if (Shields[i].en)
		{
			m_model_shield->Draw(Game_Data->context, *Game_Data->m_states, XMLoadFloat4x4(&Shields[i].m_matrix_pos), view, proj3d, Shields[i].alpha);
		}
	}
};

void MapForest::Draw_MapPoint()
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

void MapForest::OnPointerPressed(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	for (int k = 0; k < 8; k++)
	{
		if (TouchObj(&Objects[k], PointerPoint))
		{
			CreateShield(Shields, Objects[k].pos.x, Objects[k].pos.y, Objects[k].pos.z);
			CreateSpark(Sparks, Objects[k].pos.x, Objects[k].pos.y, Objects[k].pos.z, 2.f, 0.f, 0.5f, 2.f, 0.1f);
			if (Game_Data->set_sound)
			{
				m_inst_shield->Stop();
				m_inst_shield->Play();
			}
			return;
		}
	}

	for (int k = 0; k < 8; k++)
	{
		if (TouchEnemy(&Enemys[k], PointerPoint))
		{
			if (Game_Data->set_sound)
			{
				m_inst_arrow->Stop();
				m_inst_enemy_die->Stop();
				m_inst_enemy_die->Play();
			}
			return;
		}
	}
}

void MapForest::OnPointerMoved(Windows::UI::Input::PointerPoint ^PointerPoint)
{
}

void MapForest::OnPointerReleased(Windows::UI::Input::PointerPoint ^PointerPoint)
{
}

MapForest::~MapForest()
{
}
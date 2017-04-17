#pragma once

#include "GameRoot.h"
#include "MenuGame.h"
#include "GameHUD.h"

#define	MAP_FOREST_SOUND_MAX	7

class MapForest : public GameRoot
{
public:
	MapForest(GameData *data);
	~MapForest();

	void Pause();
	void Resume();

	void Update(float timeTotal, float timeDelta);
	void Draw();
	void Draw_MapPoint();

	void OnPointerPressed(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerMoved(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerReleased(Windows::UI::Input::PointerPoint ^PointerPoint);

private:
	//Модель уровня
	std::unique_ptr<DirectX::Model>						m_model_level_b;
	std::unique_ptr<DirectX::Model>						m_model_level_n3;
	std::unique_ptr<DirectX::Model>						m_model_level_n2;
	std::unique_ptr<DirectX::Model>						m_model_level_n1;
	std::unique_ptr<DirectX::Model>						m_model_level_f;

	//Модели объектов
	std::unique_ptr<DirectX::Model>						m_model_obj[2];
	std::unique_ptr<DirectX::GeometricPrimitive>		m_shape_mappoint;

	//Модели атакующих
	std::unique_ptr<DirectX::Model>						m_model_enemy[4];

	//Модель щита
	std::unique_ptr<DirectX::Model>						m_model_shield;

	//Модели искр
	std::unique_ptr<DirectX::Model>						m_model_spark;

	//Объекты в игре
	ParticleItem	Parts[LEVEL_PART_MAX];
	Shield			Shields[LEVEL_SHIELD_MAX];
	Object			Objects[LEVEL_OBJ_MAX];
	Enemy			Enemys[LEVEL_ENEMY_MAX];
	Spark			Sparks[LEVEL_SPARK_MAX];

	//Переменные для создания объектов
	float	end[7];
	float	start_min[7];
	float	start_max[7];

	//Звуки
	std::unique_ptr<DirectX::SoundEffect>				m_sound_ambient;
	std::unique_ptr<DirectX::SoundEffectInstance>		m_inst_ambient;

	std::unique_ptr<DirectX::SoundEffect>				m_sound_stone;
	std::unique_ptr<DirectX::SoundEffectInstance>		m_inst_stone;

	std::unique_ptr<DirectX::SoundEffect>				m_sound_arrow;
	std::unique_ptr<DirectX::SoundEffectInstance>		m_inst_arrow;

	std::unique_ptr<DirectX::SoundEffect>				m_sound_shield;
	std::unique_ptr<DirectX::SoundEffectInstance>		m_inst_shield;

	std::unique_ptr<DirectX::SoundEffect>				m_sound_hit;
	std::unique_ptr<DirectX::SoundEffectInstance>		m_inst_hit;

	std::unique_ptr<DirectX::SoundEffect>				m_sound_enemy_die;
	std::unique_ptr<DirectX::SoundEffectInstance>		m_inst_enemy_die;

	std::unique_ptr<DirectX::SoundEffect>				m_sound_other[MAP_FOREST_SOUND_MAX];
	std::unique_ptr<DirectX::SoundEffectInstance>		m_inst_other[MAP_FOREST_SOUND_MAX];

	int		sound_id;
	float	sound_time;
};
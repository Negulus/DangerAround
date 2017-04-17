#pragma once

#include "GameRoot.h"
#include "MenuGame.h"
#include "GameHUD.h"

#define	MAP_SPACE_SOUND_MAX	6

class MapSpace : public GameRoot
{
public:
	MapSpace(GameData *data);
	~MapSpace();

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
	std::unique_ptr<DirectX::Model>						m_model_level_space;
	std::unique_ptr<DirectX::Model>						m_model_level_planet;
	std::unique_ptr<DirectX::Model>						m_model_level_sattelite;
	std::unique_ptr<DirectX::Model>						m_model_level_star;
	std::unique_ptr<DirectX::Model>						m_model_level_star_1;

	float												m_planet_selfrot;
	float												m_planet_angle;
	vector3												m_planet_pos;
	DirectX::XMFLOAT4X4									m_matrix_planet;

	float												m_sattelite_selfrot;
	float												m_sattelite_angle;
	float												m_sattelite_pos;
	float												m_sattelite_rot;
	DirectX::XMFLOAT4X4									m_matrix_sattelite;

	float												m_star_selfrot;
	float												m_star_selfrot_1;
	vector3												m_star_pos;
	DirectX::XMFLOAT4X4									m_matrix_star;
	DirectX::XMFLOAT4X4									m_matrix_star_1;

	//Модели объектов
	std::unique_ptr<DirectX::Model>						m_model_obj[2];
	std::unique_ptr<DirectX::GeometricPrimitive>		m_shape_mappoint;

	//Модели искр
	std::unique_ptr<DirectX::Model>						m_model_spark;

	//Объекты в игре
	Object			Objects[LEVEL_OBJ_MAX];
	Spark			Sparks[LEVEL_SPARK_MAX];

	//Звуки
	std::unique_ptr<DirectX::SoundEffect>				m_sound_ambient;
	std::unique_ptr<DirectX::SoundEffectInstance>		m_inst_ambient;

	std::unique_ptr<DirectX::SoundEffect>				m_sound_stone;
	std::unique_ptr<DirectX::SoundEffectInstance>		m_inst_stone;

	std::unique_ptr<DirectX::SoundEffect>				m_sound_shield;
	std::unique_ptr<DirectX::SoundEffectInstance>		m_inst_shield;

	std::unique_ptr<DirectX::SoundEffect>				m_sound_hit;
	std::unique_ptr<DirectX::SoundEffectInstance>		m_inst_hit;

	std::unique_ptr<DirectX::SoundEffect>				m_sound_other[MAP_SPACE_SOUND_MAX];
	std::unique_ptr<DirectX::SoundEffectInstance>		m_inst_other[MAP_SPACE_SOUND_MAX];

	int		sound_id;
	float	sound_time;
};
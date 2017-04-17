#pragma once

#include "Direct3DBase.h"

#include "Audio.h"
#include "CommonStates.h"
#include "Effects.h"
#include "GeometricPrimitive.h"
#include "Model.h"
#include "PrimitiveBatch.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"

using namespace DirectX;

#define TIME_OBJ_START	1.0f
#define	TIME_OBJ_END	1.0f

#define TIME_GAME_TIME	60

#define	MAP_SPACE		0
#define	MAP_FOREST		1
#define	MAP_VOLCANO		2

#define	MAP_MAX		2
#define	LEVEL_MAX	20

#define	FILE_SET	"settings"
#define FILE_MAP	"map_"

#define OBJ_STONE	(1<<0)
#define	OBJ_ARROW	(1<<1)

#define	LEVEL_OBJ_MAX		8
#define	LEVEL_ENEMY_MAX		8
#define	LEVEL_SHIELD_MAX	4
#define	LEVEL_SPARK_MAX		8
#define	LEVEL_PART_MAX		32

enum Stats
{
	stat_Null,
	stat_MenuMain,
	stat_MenuSelect,
	stat_MenuSet,
	stat_MenuHelp,
	stat_MenuCal,
	stat_Start,
	stat_Play,
	stat_Win,
	stat_Lose,
	stat_Pause,
	stat_GameOver,
	stat_Next,
	stat_Resume,
	stat_Restart,
	stat_Exit
};

enum GameType
{
	type_classic = 0,
	type_time,
	type_surv
};

enum ObjectMove
{
	obj_move_linear = 0,
	obj_move_ballistic
};

enum ObjectType
{
	obj_type_stone = 0,
	obj_type_arrow
};

enum EnemyState
{
	enemy_start = 0,
	enemy_wait_1,
	enemy_wait_2,
	enemy_end,
	enemy_kill,
	enemy_kill_end
};

enum SetAngle
{
	deg_45 = 1,
	deg_90,
	deg_120,
	deg_180,
	deg_270,
	deg_360
};

enum GameStat
{
	game_stat_start_level,
	game_stat_before_level,
	game_stat_show_level,
	game_stat_start,
	game_stat_before_run,
	game_stat_run,
	game_stat_end,
	game_stat_ending
};

struct vector2
{
	float x;
	float y;
};

struct vector3
{
	float x;
	float y;
	float z;
};

struct Object
{
	bool					en;
	bool					run;
	int						model;
	ObjectType				type;
	ObjectMove				move;

	//Перемещение
	float					speed;
	vector3					speed3;
	vector3					pos;
	vector3					pos_start;

	//Баллистика
	float					angle_x;
	float					angle_y;
	float					angle_y_start;
	float					angle_y_end;

	//Вращение
	vector3					rot;
	vector3					rot_speed;

	DirectX::XMFLOAT4X4		m_matrix_pos;
	DirectX::XMFLOAT4X4		m_matrix_map;
};

struct Enemy
{
	bool					en;
	EnemyState				state;
	vector3					pos;
	float					angle;
	float					angle_x;
	int						model;
	float					alpha;
	float					time;
	float					time_set;

	Object					*obj;
	DirectX::XMFLOAT4X4		m_matrix_pos;
};

struct Shield
{
	bool					en;
	vector3					pos;
	float					alpha;

	DirectX::XMFLOAT4X4		m_matrix_pos;
};

struct Spark
{
	bool					en;
	vector3					pos;
	float					alpha;
	float					alpha_speed;
	float					scale;
	float					scale_speed;
	float					time;
	float					life;

	DirectX::XMFLOAT4X4		m_matrix_pos;
};

struct GameData
{
	//Статус игры
	Stats	Status_Current;
	Stats	Status_Next;
//	bool	isTrial;

	//Переменные для вывода изображения
	ID3D11Device1										*device;
	ID3D11DeviceContext1								*context;
	std::unique_ptr<DirectX::CommonStates>				m_states;
	std::unique_ptr<DirectX::BasicEffect>				m_batchEffect;
	std::unique_ptr<DirectX::SpriteBatch>				m_sprites;
	std::unique_ptr<DirectX::IEffectFactory>			m_fxFactory;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_depthStencilView;
	bool												scr_wide;
	float												scr_scale;
	bool												loading;

	//Матрицы видов
	DirectX::XMFLOAT4X4									m_proj2d;
	DirectX::XMFLOAT4X4									m_proj3d;
	DirectX::XMFLOAT4X4									m_view;
	DirectX::XMFLOAT4X4									m_world;
	DirectX::XMFLOAT4X4									m_map_pos;

	//Данные сенсоров
	vector3		acc;
	vector3		gyro;

	//Положение и направление взгляда
	vector3		eye;
	vector3		eye_pos;
	vector3		map_pos;

	//Звуковой движок
	std::unique_ptr<DirectX::AudioEngine>				m_audEngine;

	//Игровые параметры
	int			Life;		//Жизни
	int			Level;		//Уровень
	int			Level_Start;
	int			Map;		//Карта
	GameType	Game_Type;	//Тип игры
	int			Score;		//Очки
	int			Score_Last;
	float		Timer;		//Таймер игры
	float		Timer_Set;
	int			Count;		//Счётчик объектов	
	int			Count_Set;
	bool		Object;		//Наличие на карте объектов
	float		Time_Obj_Min;
	float		Time_Obj_Max;
	float		Time_Obj_Next;
	float		Time_Delta;
	vector2		Touch_Pos;
	GameStat	Game_Stat;
	bool		Game_Run;
	bool		Game_End;
	bool		Blood;

	//Переменные для создания объектов
	float	obj_end[7];
	float	obj_start_min[7];
	float	obj_start_max[7];

	//Настройки
	bool		set_sound;
	float		set_skyline;
	SetAngle	set_angle;
	
	//Звуки
	bool		sound_object;
	bool		sound_enemy;
	bool		sound_hit;
	std::unique_ptr<DirectX::SoundEffect>				m_sound_click;
	std::unique_ptr<DirectX::SoundEffectInstance>		m_inst_click;

	//Режимы
	bool		pause;

	//Очки карт
	int			scores[MAP_MAX][LEVEL_MAX][3];

	float Vector3Length(vector3 f, vector3 t)
	{
		f.x -= t.x;
		f.y -= t.y;
		f.z -= t.z;
		return abs(sqrtf((f.x * f.x) + (f.y * f.y) + (f.z * f.z)));
	}

	float randf(float min, float max)
	{
		return min + (((float)rand() / (float)RAND_MAX) * (max - min));
	}
};

struct ParticleItem
{
	bool	en;
	vector3	pos;
	vector3	speed;
	RECT	rect;
	float	width;
	float	height;
	float	mas;
	float	alpha;
	float	alpha_speed;
	float	start;
	float	life;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;
};


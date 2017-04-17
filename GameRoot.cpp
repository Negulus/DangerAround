#include "pch.h"

#include "GameRoot.h"

#include "DDSTextureLoader.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

void GameRoot::Init(GameData *data)
{
	Game_Data = data;

	//Данные гироскопа и акселерометра
	Game_Data->acc.x = 0;
	Game_Data->acc.y = 0;
	Game_Data->acc.z = 0;
	Game_Data->gyro.x = 0;
	Game_Data->gyro.y = 0;
	Game_Data->gyro.z = 0;

	//Направление и положение взгляда
	Game_Data->eye.x = 0;
	Game_Data->eye.y = 0;
	Game_Data->eye.z = 0;
	Game_Data->eye_pos.x = 0;
	Game_Data->eye_pos.y = 1.5f;
	Game_Data->eye_pos.z = 0;

	//Положение карты
	Game_Data->map_pos.x = (Game_Data->scr_wide) ? 0.57f : 0.6f;
	Game_Data->map_pos.y = (Game_Data->scr_wide) ? -1.14f : -1.16f;
	Game_Data->map_pos.z = 3;

	x = 0;
	y = 0;
	z = 0;
}

void GameRoot::Data_Save_Set()
{
	auto Folder = Windows::Storage::ApplicationData::Current->LocalFolder;
	auto FileName = Folder->Path + "\\" + FILE_SET + ".sav";

	FILE* pFile;
	_wfopen_s(&pFile, FileName->Data(), L"w");
	fprintf(pFile, (Game_Data->set_sound) ? "1" : "0");
	fprintf(pFile, " ");
	fprintf(pFile, "%d", Game_Data->set_angle);
	fprintf(pFile, " ");
	fprintf(pFile, "%f", Game_Data->set_skyline); 
	fprintf(pFile, " ");
	fprintf(pFile, "%d", Game_Data->Level); 
	fprintf(pFile, " ");
	fprintf(pFile, "%d", Game_Data->Map); 
	fclose(pFile);
}

void GameRoot::Data_Save_Map(int map)
{
	auto Folder = Windows::Storage::ApplicationData::Current->LocalFolder;
	auto FileName = Folder->Path + "\\" + FILE_MAP + map.ToString() + ".sav";

	FILE* pFile;
	_wfopen_s(&pFile, FileName->Data(), L"w");
	for (int i = 0; i < LEVEL_MAX; i++)
	{
		fprintf(pFile, "%d ", Game_Data->scores[map][i][0]);
		fprintf(pFile, "%d ", Game_Data->scores[map][i][1]);
		fprintf(pFile, "%d ", Game_Data->scores[map][i][2]);
	}
	fclose(pFile);
}

void GameRoot::Data_Load()
{
	Game_Data->set_angle = deg_90;
	Game_Data->set_skyline = 0;
	Game_Data->Level = 1;
	Game_Data->Map = 0;

	auto local = Windows::Storage::ApplicationData::Current->LocalFolder;
	auto localFileNamePlatformString = local->Path + "\\" + FILE_SET + ".sav";

	FILE* pFile;
	if (_wfopen_s(&pFile, localFileNamePlatformString->Data(), L"r") == 0)
	{
		fscanf_s(pFile, "%d %d %f %d %d", &tmpi, &Game_Data->set_angle, &Game_Data->set_skyline, &Game_Data->Level, &Game_Data->Map);
		Game_Data->set_sound = (tmpi == 1) ? true : false;
		fclose(pFile);
	}
	else
	{
		Game_Data->set_sound = true;
		Game_Data->set_angle = deg_90;
		Game_Data->set_skyline = 0;
	}

	for (int j = 0; j < MAP_MAX; j++)
	{
		local = Windows::Storage::ApplicationData::Current->LocalFolder;
		localFileNamePlatformString = local->Path + "\\" + FILE_MAP + j.ToString() + ".sav";
		if (_wfopen_s(&pFile, localFileNamePlatformString->Data(), L"r") == 0)
		{
			for (int i = 0; i < LEVEL_MAX; i++)
			{
				if (fscanf_s(pFile, "%d %d %d ", &Game_Data->scores[j][i][0], &Game_Data->scores[j][i][1], &Game_Data->scores[j][i][2]) == 0)
				{
					Game_Data->scores[j][i][0] = 0;
					Game_Data->scores[j][i][1] = 0;
					Game_Data->scores[j][i][2] = 0;
				}
			}
			fclose(pFile);
		}
		else
		{
			for (int i = 0; i < LEVEL_MAX; i++)
			{
				Game_Data->scores[j][i][0] = 0;
				Game_Data->scores[j][i][1] = 0;
				Game_Data->scores[j][i][2] = 0;
			}
		}
	}
}

void GameRoot::Data_Reset()
{
	for (int j = 0; j < MAP_MAX; j++)
	{
		for (int i = 0; i < LEVEL_MAX; i++)
		{
			Game_Data->scores[j][i][0] = 0;
			Game_Data->scores[j][i][1] = 0;
			Game_Data->scores[j][i][2] = 0;
		}
		Data_Save_Map(j);
	}
}

void GameRoot::Update_Eye(float timeTotal, float timeDelta, float skyline)
{
	XMMATRIX tmpm;

	if (skyline < -9)
		skyline = Game_Data->set_skyline;

	x = (float)(Game_Data->acc.x * XM_PI / 2);
	y = (float)(Game_Data->acc.y * XM_PI / 2);
	z = (float)(Game_Data->acc.z * XM_PI / 2);

	Game_Data->eye.x = acosf((-Game_Data->acc.z - skyline) / sqrt(Game_Data->acc.x*Game_Data->acc.x + Game_Data->acc.y*Game_Data->acc.y + (Game_Data->acc.z + skyline)*(Game_Data->acc.z + skyline))) - XM_PI / 2;
	Game_Data->eye.z = acosf(-Game_Data->acc.y / sqrt(Game_Data->acc.x*Game_Data->acc.x + Game_Data->acc.y*Game_Data->acc.y + (Game_Data->acc.z + skyline)*(Game_Data->acc.z + skyline)));

    x = (float)(Game_Data->gyro.x * 2 * sinf(x));
    y = (float)(Game_Data->gyro.y * 2 * sinf(y));
    z = (float)(Game_Data->gyro.z * 2 * sinf(z));

    if ((x + y + z) > 0)
		Game_Data->eye.y += timeDelta * sqrtf((x * x) + (y * y) + (z * z)) / 180 * XM_PI;
    else
        Game_Data->eye.y -= timeDelta * sqrtf((x * x) + (y * y) + (z * z)) / 180 * XM_PI;

	if (Game_Data->eye.y > XM_PI)
		Game_Data->eye.y = -(XM_PI * 2) + Game_Data->eye.y;
	else if (Game_Data->eye.y < -XM_PI)
		Game_Data->eye.y = (XM_PI * 2) + Game_Data->eye.y;

	tmpm = XMMatrixTranslation(-Game_Data->eye_pos.x, -Game_Data->eye_pos.y, -Game_Data->eye_pos.z);
	tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationY(Game_Data->eye.y));
	tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationX(Game_Data->eye.x));
	tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationZ(Game_Data->eye.z));
	XMStoreFloat4x4(&Game_Data->m_world, tmpm);

	tmpm = XMMatrixRotationY(Game_Data->eye.y - 0.32f);
	tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationX(Game_Data->eye.x));
	tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationZ(Game_Data->eye.z));
	tmpm = XMMatrixMultiply(tmpm, XMMatrixTranslation(Game_Data->map_pos.x, Game_Data->map_pos.y, Game_Data->map_pos.z));
	XMStoreFloat4x4(&Game_Data->m_map_pos, tmpm);
}

bool GameRoot::TouchObj(Object *obj, Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (obj->en && obj->run)
	{
		vector3 tmpv;
		vector3 tmpv2;

		tmpv.x = obj->pos.x - Game_Data->eye_pos.x;
		tmpv.y = obj->pos.y - Game_Data->eye_pos.y;
		tmpv.z = obj->pos.z - Game_Data->eye_pos.z;

		tmpf = sqrt(tmpv.x*tmpv.x + tmpv.y*tmpv.y + tmpv.z*tmpv.z);
		tmpv2.y = obj->angle_x / 180 * XM_PI;// acosf(tmpv.x / tmpf);// - XM_PI / 2;
		tmpv2.x = acosf(-tmpv.y / tmpf) - XM_PI / 2;

		if (tmpf < 1) tmpf = 1;
		if (abs(Game_Data->Touch_Pos.y - tmpv2.x) < (0.6f / ((tmpf / 30) * 10)))
		{
			if (abs(Game_Data->Touch_Pos.x - tmpv2.y) < (0.6f / ((tmpf / 30) * 10)))
			{
				obj->en = false;
				switch (Game_Data->Game_Type)
				{
				case type_classic:
					Game_Data->Score += (int)(Game_Data->Level * Game_Data->set_angle * Game_Data->Time_Delta * 0.2f);
					break;
				case type_time:
					Game_Data->Score += (int)(Game_Data->Level * Game_Data->set_angle * 10);
					break;
				case type_surv:
					Game_Data->Score += (int)(Game_Data->Level * Game_Data->set_angle * 10);
					break;
				}
				if (Game_Data->Game_End)
					Game_Data->Game_Stat = game_stat_end;
				return true;
			}
		}
	}
	return false;
}

bool GameRoot::TouchEnemy(Enemy *obj, Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (obj->en && obj->state != enemy_kill_end && obj->en && obj->state <= enemy_wait_1)
	{
		vector3 tmpv;
		vector3 tmpv2;

		tmpv.x = obj->pos.x - Game_Data->eye_pos.x;
		tmpv.y = 2.5f - Game_Data->eye_pos.y;
		tmpv.z = obj->pos.z - Game_Data->eye_pos.z;

		tmpf = sqrt(tmpv.x*tmpv.x + tmpv.y*tmpv.y + tmpv.z*tmpv.z);
		tmpv2.y = -obj->angle / 180 * XM_PI;//acosf(tmpv.x / tmpf) - XM_PI / 2;
		tmpv2.x = acosf(-tmpv.y / tmpf) - XM_PI / 2;

		if (tmpf < 1) tmpf = 1;
		if (abs(Game_Data->Touch_Pos.y - tmpv2.x) < (0.6f / ((tmpf / 30) * 10)))
		{
			if (abs(Game_Data->Touch_Pos.x - tmpv2.y) < (0.6f / ((tmpf / 30) * 10)))
			{
				obj->state = enemy_kill;
				obj->obj->en = false;
				switch (Game_Data->Game_Type)
				{
				case type_classic:
					Game_Data->Score += (int)(Game_Data->Level * Game_Data->set_angle * Game_Data->Time_Delta * 0.4f);
					break;
				case type_time:
					Game_Data->Score += (int)(Game_Data->Level * Game_Data->set_angle * 20);
					break;
				case type_surv:
					Game_Data->Score += (int)(Game_Data->Level * Game_Data->set_angle * 20);
					break;
				}
				if (Game_Data->Game_End)
					Game_Data->Game_Stat = game_stat_end;
				return true;
			}
		}
	}
	return false;
}

void GameRoot::CreateShield(Shield *obj, float x, float y, float z)
{
	for (i = 0; i < LEVEL_SHIELD_MAX; i++)
	{
		if (!obj[i].en)
		{
			obj[i].en = true;
			obj[i].alpha = 1.1f;
			obj[i].pos.x = x;
			obj[i].pos.y = y;
			obj[i].pos.z = -z;
			break;
		}
	}
}

void GameRoot::UpdateShield(Shield *obj, float timeTotal, float timeDelta)
{
	XMMATRIX tmpm;
	for (i = 0; i < LEVEL_SHIELD_MAX; i++)
	{
		if (obj[i].en)
		{
			obj[i].alpha -= timeDelta;
			if (obj[i].alpha <= 0)
			{
				obj[i].en = false;
				continue;
			}
			tmpm = XMMatrixRotationX(-Game_Data->eye.x);
			tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationY(-Game_Data->eye.y));
			tmpm = XMMatrixMultiply(tmpm, XMMatrixTranslation(obj[i].pos.x, obj[i].pos.y, obj[i].pos.z));
			tmpm = XMMatrixMultiply(tmpm, XMLoadFloat4x4(&Game_Data->m_world));
			XMStoreFloat4x4(&obj[i].m_matrix_pos, tmpm);
		}
	}
}

void GameRoot::CreateSpark(Spark *obj, float x, float y, float z, float a_0, float a_1, float s_0, float s_1, float t)
{
	for (i = 0; i < LEVEL_SPARK_MAX; i++)
	{
		if (!obj[i].en)
		{
			obj[i].en = true;
			obj[i].alpha = a_0;
			obj[i].alpha_speed = (a_1 - a_0) / t;
			obj[i].scale = s_0;
			obj[i].scale_speed = (s_1 - s_0) / t;
			obj[i].pos.x = x;
			obj[i].pos.y = y;
			obj[i].pos.z = -z;
			obj[i].time = 0;
			obj[i].life = t;
			break;
		}
	}
}

void GameRoot::UpdateSpark(Spark *obj, float timeTotal, float timeDelta)
{
	XMMATRIX tmpm;
	for (i = 0; i < LEVEL_SPARK_MAX; i++)
	{
		if (obj[i].en)
		{
			obj[i].alpha += timeDelta * obj[i].alpha_speed;
			obj[i].scale += timeDelta * obj[i].scale_speed;
			obj[i].time += timeDelta;
			if (obj[i].time >= obj[i].life)
			{
				obj[i].en = false;
				continue;
			}
			tmpm = XMMatrixScaling(obj[i].scale, obj[i].scale, obj[i].scale);
			tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationX(-Game_Data->eye.x));
			tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationY(-Game_Data->eye.y));
			tmpm = XMMatrixMultiply(tmpm, XMMatrixTranslation(obj[i].pos.x, obj[i].pos.y, obj[i].pos.z));
			tmpm = XMMatrixMultiply(tmpm, XMLoadFloat4x4(&Game_Data->m_world));
			XMStoreFloat4x4(&obj[i].m_matrix_pos, tmpm);
		}
	}
}

void GameRoot::CreateObj(Object *obj, Enemy* enm, int model_max, float timeTotal, float y_min, float y_max, bool ballistic, int type, int enemy)
{
	XMMATRIX tmpm;

	//Основные параметры
	obj->en = true;
	obj->run = false;
	obj->model = (int)Game_Data->randf(0, (model_max - 1.f));
	if (obj->model < 0) obj->model = 0;
	else if (obj->model > (model_max - 1)) obj->model = model_max - 1;
	obj->speed = Game_Data->randf(30 + Game_Data->Level * 5.f, 50.f + Game_Data->Level * 5.f) * 0.005f;
	obj->angle_x = 0;

	//Рассчёт углов
	switch (Game_Data->set_angle)
	{
	case deg_45:
		obj->angle_x = Game_Data->randf(-20, 20);
		break;
	case deg_90:
		obj->angle_x = Game_Data->randf(-45, 45);
		break;
	case deg_120:
		obj->angle_x = Game_Data->randf(-60, 60);
		break;
	case deg_180:
		obj->angle_x = Game_Data->randf(-90, 90);
		break;
	case deg_270:
		obj->angle_x = Game_Data->randf(-135, 135);
		break;
	case deg_360:
		obj->angle_x = Game_Data->randf(-180, 180);
		break;
	default:
		Game_Data->set_angle = deg_45;
		break;
	}

	obj->angle_y_end = Game_Data->randf(y_min, y_max);

	//Рассчёт положения
	obj->angle_y = obj->angle_y_end;
	obj->pos = Game_Data->eye_pos;

	if (ballistic)
	{
		obj->move = obj_move_ballistic;
		obj->angle_y_start = 0;
		for (j = 1; j < 7; j++)
		{
			if (obj->angle_y_end <= Game_Data->obj_end[j])
			{
				obj->angle_y_start = -Game_Data->randf((Game_Data->obj_start_min[j-1] + (Game_Data->obj_end[j] - obj->angle_y_end) / (Game_Data->obj_start_min[j] - Game_Data->obj_start_min[j-1])), (Game_Data->obj_start_max[j-1] + (Game_Data->obj_end[j] - obj->angle_y_end) / (Game_Data->obj_start_max[j] - Game_Data->obj_start_max[j-1])));
				break;
			}
		}

		for (j = 0; j < 20; j++)
		{
			obj->angle_y = obj->angle_y - ((obj->angle_y_end - obj->angle_y_start) / 20.f);
			obj->pos.x = obj->pos.x - cosf(obj->angle_y / 180.f * XM_PI) * sinf(obj->angle_x / 180.f * XM_PI) * 35.f / 20.f;
			obj->pos.y = obj->pos.y + sinf(obj->angle_y / 180.f * XM_PI) * 35.f / 20.f;
			obj->pos.z = obj->pos.z - cosf(obj->angle_y / 180.f * XM_PI) * cosf(obj->angle_x / 180.f * XM_PI) * 35.f / 20.f;

			if (obj->pos.y < Game_Data->eye_pos.y)
			{
				obj->move = obj_move_linear;
				break;
			}
		}
	}
	else
	{
		obj->move = obj_move_linear;
		obj->pos.x = cosf(obj->angle_y / 180.f * XM_PI) * sinf(-obj->angle_x / 180.f * XM_PI) * 35.f;
		obj->pos.y = sinf(obj->angle_y / 180.f * XM_PI) * 35.f;
		obj->pos.z = -cosf(obj->angle_y / 180.f * XM_PI) * cosf(-obj->angle_x / 180.f * XM_PI) * 35.f;
	}

	if (obj->move == obj_move_linear)
	{
		obj->angle_y = obj->angle_y_end;
		tmpf = Game_Data->Vector3Length(obj->pos, Game_Data->eye_pos);
		obj->speed3.x = obj->speed * (Game_Data->eye_pos.x - obj->pos.x) / tmpf;
		obj->speed3.y = obj->speed * (Game_Data->eye_pos.y - obj->pos.y) / tmpf;
		obj->speed3.z = obj->speed * (Game_Data->eye_pos.z - obj->pos.z) / tmpf;
	}

	//Вращение
	obj->rot.x = 0;
	obj->rot.y = 0;
	obj->rot.z = 0;

	if (type == (OBJ_STONE|OBJ_ARROW) || type == 0)
	{
		if (obj->pos.y > 5)
			type = OBJ_STONE;
		else
			type = OBJ_ARROW;
	}

	if (type == OBJ_STONE)
	{
		obj->type = obj_type_stone;
		obj->rot_speed.x = Game_Data->randf(-0.1f, 0.1f);
		obj->rot_speed.y = Game_Data->randf(-0.1f, 0.1f);
		obj->rot_speed.z = Game_Data->randf(-0.1f, 0.1f);
		if (enemy & OBJ_STONE)
		{
			obj->run = false;
			CreateEnm(enm, obj, timeTotal);
		}
		else
		{
			Game_Data->sound_object = true;
			obj->run = true;
		}
	}
	else if (type == OBJ_ARROW)
	{
		obj->type = obj_type_arrow;
		obj->rot_speed.x = 0;
		obj->rot_speed.y = 0;
		obj->rot_speed.z = Game_Data->randf(-0.25, 0.25);
		if (enemy & OBJ_ARROW)
		{
			obj->run = false;
			CreateEnm(enm, obj, timeTotal);
		}
		else
		{
			Game_Data->sound_object = true;
			obj->run = true;
		}
	}

	tmpm = XMMatrixRotationX(-obj->angle_y / 180 * XM_PI);
	tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationY(-obj->angle_x / 180 * XM_PI));
	tmpm = XMMatrixMultiply(tmpm, XMMatrixTranslation(obj->pos.x, obj->pos.y, -obj->pos.z));
	tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationX(obj->rot.x));
	tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationY(obj->rot.y));
	tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationZ(obj->rot.z));
	tmpm = XMMatrixMultiply(tmpm, XMLoadFloat4x4(&Game_Data->m_world));
	XMStoreFloat4x4(&obj->m_matrix_pos, tmpm);
}

void GameRoot::UpdateObj(Object *obj, Enemy* enm, int model_max, float timeTotal, float timeDelta, float y_min, float y_max, bool ballistic, int type, int enemy)
{
	//Создание нового объекта
	if (Game_Data->Game_Run)
	{
		if (Game_Data->Time_Obj_Next <= timeTotal)
		{
			for (i = 0; i < LEVEL_OBJ_MAX; i++)
			{
				if (!obj[i].en)
				{
					CreateObj(&obj[i], enm, model_max, timeTotal, y_min, y_max, ballistic, type, enemy);
					Game_Data->Time_Obj_Next = timeTotal + Game_Data->randf(Game_Data->Time_Obj_Min, Game_Data->Time_Obj_Max);
					Game_Data->Count++;
					if (Game_Data->Game_Type == type_surv)
					{
						if (Game_Data->Count >= Game_Data->Count_Set)
						{
							Game_Data->Game_Run = false;
							Game_Data->Game_End = true;
						}
					}
					break;
				}
			}
		}
	}

	//Проверка всех объектов
	XMMATRIX tmpm;
	Game_Data->Object = false;
	for (i = 0; i < LEVEL_OBJ_MAX; i++)
	{
		if (obj[i].en)
		{
			Game_Data->Object = true;
			if (obj[i].run)
			{
				tmpf = Game_Data->Vector3Length(Game_Data->eye_pos, obj[i].pos);
				switch (obj[i].move)
				{
				case obj_move_linear:
					obj[i].pos.x += obj[i].speed3.x * timeDelta * 25.f;
					obj[i].pos.y += obj[i].speed3.y * timeDelta * 25.f;
					obj[i].pos.z += obj[i].speed3.z * timeDelta * 25.f;
					break;
				case obj_move_ballistic:
					obj[i].angle_y = obj[i].angle_y + ((obj[i].angle_y_end - obj[i].angle_y_start) * obj[i].speed * timeDelta);
					obj[i].pos.x = obj[i].pos.x + cosf(obj[i].angle_y / 180.f * XM_PI) * sinf(obj[i].angle_x / 180.f * XM_PI) * obj[i].speed * timeDelta * 36.5f;
					obj[i].pos.y = obj[i].pos.y - sinf(obj[i].angle_y / 180.f * XM_PI) * obj[i].speed * timeDelta * 36.5f;
					obj[i].pos.z = obj[i].pos.z + cosf(obj[i].angle_y / 180.f * XM_PI) * cosf(obj[i].angle_x / 180.f * XM_PI) * obj[i].speed * timeDelta * 36.5f;
					break;
				}

				obj[i].rot.x += obj[i].rot_speed.x;
				if (obj[i].rot.x > 6.283185f) obj[i].rot.x -= 6.283185f;
				else if (obj[i].rot.x < -6.283185f) obj[i].rot.x += 6.283185f;

				obj[i].rot.y += obj[i].rot_speed.y;
				if (obj[i].rot.y > 6.283185f) obj[i].rot.y -= 6.283185f;
				else if (obj[i].rot.y < -6.283185f) obj[i].rot.y += 6.283185f;

				obj[i].rot.z += obj[i].rot_speed.z;
				if (obj[i].rot.z > 6.283185f) obj[i].rot.z -= 6.283185f;
				else if (obj[i].rot.z < -6.283185f) obj[i].rot.z += 6.283185f;

				tmpm = XMMatrixRotationX(obj[i].rot.x);
				tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationY(obj[i].rot.y));
				tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationZ(obj[i].rot.z));
				tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationX(-obj[i].angle_y / 180 * XM_PI));
				tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationY(-obj[i].angle_x / 180 * XM_PI));
				tmpm = XMMatrixMultiply(tmpm, XMMatrixTranslation(obj[i].pos.x, obj[i].pos.y, -obj[i].pos.z));
				tmpm = XMMatrixMultiply(tmpm, XMLoadFloat4x4(&Game_Data->m_world));
				XMStoreFloat4x4(&obj[i].m_matrix_pos, tmpm);

				//Попадание
				if (tmpf < Game_Data->Vector3Length(Game_Data->eye_pos, obj[i].pos))
				{
					obj[i].en = false;
					Game_Data->Blood = true;
					Game_Data->sound_hit = true;
					if (Game_Data->Life > 0)
					{
						Game_Data->Life -= 1;
						if (Game_Data->Game_End)
						{
							Game_Data->Game_Stat = game_stat_end;
						}
					}
					else
					{
						switch (Game_Data->Game_Type)
						{
						case type_time:
							Game_Data->Status_Next = stat_Lose;
							break;
						case type_surv:
							Game_Data->Status_Next = stat_GameOver;
							break;
						default:
							Game_Data->Status_Next = stat_GameOver;
							break;
						}
					}
				}
			}

			//Точки на карте
			tmpm = XMMatrixTranslation(sin(-obj[i].angle_x / 180 * XM_PI) * 0.2f * cosf(obj[i].angle_y_end / 180.f * XM_PI), 0.2f * sinf(obj[i].angle_y_end / 180.f * XM_PI), cos(-obj[i].angle_x / 180 * XM_PI) * 0.2f * cosf(obj[i].angle_y_end / 180.f * XM_PI));
			tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationY(Game_Data->eye.y - 0.32f));
			tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationX(Game_Data->eye.x));
			tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationZ(Game_Data->eye.z));
			tmpm = XMMatrixMultiply(tmpm, XMMatrixTranslation(Game_Data->map_pos.x, Game_Data->map_pos.y, Game_Data->map_pos.z));
			XMStoreFloat4x4(&obj[i].m_matrix_map, tmpm);
		}
	}
}

void GameRoot::CreateEnm(Enemy *enm, Object* obj, float timeTotal)
{
	XMMATRIX tmpm;

	for (i = 0; i < LEVEL_ENEMY_MAX; i++)
	{
		if (!enm[i].en)
		{
			//Атакующий
			enm[i].en = true;
			enm[i].obj = obj;
			enm[i].state = enemy_start;
			enm[i].alpha = 0;
			enm[i].time = timeTotal;
			enm[i].time_set = TIME_OBJ_START;
			enm[i].angle_x = 0;

			enm[i].model = (int)(Game_Data->randf(0,7) / 2);
			enm[i].angle = -obj->angle_x;
			enm[i].pos.x = obj->pos.x;
			enm[i].pos.y = 0;
			enm[i].pos.z = -obj->pos.z;

			tmpm = XMMatrixRotationY(enm[i].angle / 180 * XM_PI);
			tmpm = XMMatrixMultiply(tmpm, XMMatrixTranslation(enm[i].pos.x, enm[i].pos.y, enm[i].pos.z));
			tmpm = XMMatrixMultiply(tmpm, XMLoadFloat4x4(&Game_Data->m_world));
			XMStoreFloat4x4(&enm[i].m_matrix_pos, tmpm);
			break;
		}
	}
}

void GameRoot::UpdateEnm(Enemy *enm, float timeTotal, float timeDelta)
{
	//Проверка всех атакующих
	XMMATRIX tmpm;
	for (i = 0; i < LEVEL_ENEMY_MAX; i++)
	{
		if (enm[i].en)
		{
			switch (enm[i].state)
			{
			case enemy_start:
				enm[i].alpha = (timeTotal - enm[i].time) / enm[i].time_set;
				if (enm[i].alpha >= 1)
				{
					Game_Data->sound_enemy = true;
					enm[i].alpha = 1;
					enm[i].state = enemy_wait_1;
					enm[i].time = timeTotal;
					enm[i].time_set = 2;
				}
				break;
			case enemy_wait_1:
				if (timeTotal > (enm[i].time + enm[i].time_set))
				{
					enm[i].obj->run = true;
					enm[i].state = enemy_wait_2;
					enm[i].time = timeTotal;
					enm[i].time_set = 1;
				}
				break;
			case enemy_wait_2:
				if (timeTotal > (enm[i].time + enm[i].time_set))
				{
					enm[i].state = enemy_end;
					enm[i].time = timeTotal;
					enm[i].time_set = 1;
				}
				break;
			case enemy_end:
				enm[i].alpha = 1 - ((timeTotal - enm[i].time) / enm[i].time_set);
				if (enm[i].alpha <= 0)
				{
					enm[i].alpha = 0;
					enm[i].en = false;
				}
				break;
			case enemy_kill:
				enm[i].state = enemy_kill_end;
				enm[i].time = timeTotal;
				enm[i].time_set = 0.5f;
				enm[i].angle_x = 0;
				break;
			case enemy_kill_end:
				enm[i].alpha -= timeDelta / enm[i].time_set;
				enm[i].angle_x -= (timeDelta / enm[i].time_set) * 1.57f;
				if (enm[i].alpha <= 0)
				{
					enm[i].alpha = 0;
					enm[i].en = false;
				}
				break;
			}

			if (enm[i].angle_x != 0)
			{
				tmpm = XMMatrixRotationX(enm[i].angle_x);
				tmpm = XMMatrixMultiply(tmpm, XMMatrixRotationY(enm[i].angle / 180 * XM_PI));
			}
			else
			{
				tmpm = XMMatrixRotationY(enm[i].angle / 180 * XM_PI);
			}			
			tmpm = XMMatrixMultiply(tmpm, XMMatrixTranslation(enm[i].pos.x, enm[i].pos.y, enm[i].pos.z));
			tmpm = XMMatrixMultiply(tmpm, XMLoadFloat4x4(&Game_Data->m_world));
			XMStoreFloat4x4(&enm[i].m_matrix_pos, tmpm);
		}
	}
}

GameRoot::~GameRoot()
{
}
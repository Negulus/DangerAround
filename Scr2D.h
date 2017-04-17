#pragma once

#include "GameRoot.h"

class Scr2D
{
public:
	Scr2D(GameData *data, int x, int y, const wchar_t *file);
	~Scr2D();

	void Draw(float alphaf = 1.0, int x = -10000, int y = -10000, int x_o = 0, int y_o = 0, float scale = 1.0);
	bool Click(Windows::UI::Input::PointerPoint ^PointerPoint, int x, int y, int w, int h);

	GameData *Game_Data;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_tex;
	int	x;
	int	y;
};

class Scr2DDigit
{
public:
	Scr2DDigit(GameData *data, int x, int y, int x_r, int y_r);
	~Scr2DDigit();

	void Draw(int count, int num, float alphaf = 1.0, int x = -10000, int y = -10000);

	GameData *Game_Data;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_tex[10];
	int	x;
	int	y;
	int	x_r;
	int	y_r;

private:
	int		dig_1;
	int		dig_2;
	int		dig_3;
	int		dig_4;
	int		dig_5;
};

class But2D : public Scr2D
{
public:
	But2D(GameData *data, int x, int y, const wchar_t *file, int x_s, int y_s, int w, int h);
	~But2D();

	void Draw(float alphaf = 1.0, int x = -10000, int y = -10000, int x_o = 0, int y_o = 0, float scale = 1.0);
	void Update(float timeDelta);
	bool Click(Windows::UI::Input::PointerPoint ^PointerPoint, bool sound = true);
	bool Move(Windows::UI::Input::PointerPoint ^PointerPoint);
	bool Release(Windows::UI::Input::PointerPoint ^PointerPoint, bool moved = false);

	int		x_s;
	int		y_s;
	int		w;
	int		h;
	float	blink;
	bool	touch;
};
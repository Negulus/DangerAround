#include "pch.h"

#include "Scr2D.h"

#include "DDSTextureLoader.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

Scr2D::Scr2D(GameData *data, int x, int y, const wchar_t *file)
{
	Game_Data = data;
	this->x = ((Game_Data->scr_wide) ? (x + 26) : x) * Game_Data->scr_scale;
	this->y = (480 - y) * Game_Data->scr_scale;
	CreateDDSTextureFromFile( Game_Data->device, file, nullptr, m_tex.ReleaseAndGetAddressOf());
}

Scr2D::~Scr2D()
{
}

void Scr2D::Draw(float alphaf, int x, int y, int x_o, int y_o, float scale)
{
	DirectX::XMVECTORF32 color = Colors::White;
	if (alphaf < 1.0f)
	{
		color.f[0] = alphaf;
		color.f[1] = alphaf;
		color.f[2] = alphaf;
		color.f[3] = alphaf;
	}
	Game_Data->m_sprites->Draw(m_tex.Get(), XMFLOAT2((y < -9999) ? (float)this->y : (float)y, (x < -9999) ? (float)this->x : (float)x), nullptr, color, XM_PI/2, XMFLOAT2((y_o * Game_Data->scr_scale), (x_o * Game_Data->scr_scale)), scale * Game_Data->scr_scale);
}

bool Scr2D::Click(Windows::UI::Input::PointerPoint ^PointerPoint, int x, int y, int w, int h)
{
	if ((PointerPoint->Position.Y * Game_Data->scr_scale) > (this->x + x) && (PointerPoint->Position.Y * Game_Data->scr_scale) < (this->x + x + w))
	{
		if ((PointerPoint->Position.X * Game_Data->scr_scale) < (this->y - y) && (PointerPoint->Position.X * Game_Data->scr_scale) > (this->y - y - h))
		{
			return true;
		}
	}
	return false;
}

Scr2DDigit::Scr2DDigit(GameData *data, int x, int y, int x_r, int y_r)
{
	Game_Data = data;

	this->x = ((Game_Data->scr_wide) ? (x + 26) : x) * Game_Data->scr_scale;
	this->y = (480 - y) * Game_Data->scr_scale;
	this->x_r = x_r * Game_Data->scr_scale;
	this->y_r = y_r * Game_Data->scr_scale;
}

Scr2DDigit::~Scr2DDigit()
{
}

void Scr2DDigit::Draw(int count, int num, float alphaf, int x, int y)
{
	DirectX::XMVECTORF32 color = Colors::White;
	if (alphaf < 1.0f)
	{
		color.f[0] = alphaf;
		color.f[1] = alphaf;
		color.f[2] = alphaf;
		color.f[3] = alphaf;
	}

	dig_1 = num / 10000;
    dig_2 = (num - (dig_1 * 10000)) / 1000;
	dig_3 = (num - (dig_1 * 10000) - (dig_2 * 1000)) / 100;
	dig_4 = (num - (dig_1 * 10000) - (dig_2 * 1000) - (dig_3 * 100)) / 10;
	dig_5 = num - (dig_1 * 10000) - (dig_2 * 1000) - (dig_3 * 100) - (dig_4 * 10);

	float tmpx = (x < -9999) ? (float)this->x : (float)x;
	float tmpy = (y < -9999) ? (float)this->y : (float)y;

	if (count >= 5)
	{
		Game_Data->m_sprites->Draw(m_tex[dig_1].Get(), XMFLOAT2(tmpy, tmpx), nullptr, color, XM_PI/2, XMFLOAT2(0,0), Game_Data->scr_scale);
		tmpx += x_r;
		tmpy += y_r;
	}

	if (count >= 4)
	{
		Game_Data->m_sprites->Draw(m_tex[dig_2].Get(), XMFLOAT2(tmpy, tmpx), nullptr, color, XM_PI/2, XMFLOAT2(0,0), Game_Data->scr_scale);
		tmpx += x_r;
		tmpy += y_r;
	}

	if (count >= 3)
	{
		Game_Data->m_sprites->Draw(m_tex[dig_3].Get(), XMFLOAT2(tmpy, tmpx), nullptr, color, XM_PI/2, XMFLOAT2(0,0), Game_Data->scr_scale);
		tmpx += x_r;
		tmpy += y_r;
	}
    
	if (count >= 2)
	{
		Game_Data->m_sprites->Draw(m_tex[dig_4].Get(), XMFLOAT2(tmpy, tmpx), nullptr, color, XM_PI/2, XMFLOAT2(0,0), Game_Data->scr_scale);
		tmpx += x_r;
		tmpy += y_r;
	}
    
	if (count >= 1)
	{
		Game_Data->m_sprites->Draw(m_tex[dig_5].Get(), XMFLOAT2(tmpy, tmpx), nullptr, color, XM_PI/2, XMFLOAT2(0,0), Game_Data->scr_scale);
	}
}

But2D::But2D(GameData *data, int x, int y, const wchar_t *file, int x_s, int y_s, int w, int h) : Scr2D(data, x, y, file)
{
	this->x_s = x_s * Game_Data->scr_scale;
	this->y_s = y_s * Game_Data->scr_scale;
	this->w = w * Game_Data->scr_scale;
	this->h = h * Game_Data->scr_scale;
	this->blink = 1.0f;
	this->touch = false;
}

But2D::~But2D()
{
}

void But2D::Draw(float alphaf, int x, int y, int x_o, int y_o, float scale)
{
	DirectX::XMVECTORF32 color = Colors::White;
	if (blink > 1.0f)
	{
		color.f[0] = blink;
		color.f[1] = blink;
		color.f[2] = blink;
		color.f[3] = blink;
	}
	if (alphaf < 1.0f)
	{
		color.f[0] *= alphaf;
		color.f[1] *= alphaf;
		color.f[2] *= alphaf;
		color.f[3] *= alphaf;
	}
	Game_Data->m_sprites->Draw(m_tex.Get(), XMFLOAT2((y < -9999) ? (float)this->y : (float)y, (x < -9999) ? (float)this->x : (float)x), nullptr, color, XM_PI/2, XMFLOAT2((y_o * Game_Data->scr_scale), (x_o * Game_Data->scr_scale)), scale * Game_Data->scr_scale);
}

void But2D::Update(float timeDelta)
{
	if (touch)
	{
		if (blink < 2.0f)
		{
			blink += timeDelta * 0.5f;
			if (blink > 2.0f) blink = 2.0f;
		}
	}
	else
	{
		blink = 1.0f;
	}
}

bool But2D::Click(Windows::UI::Input::PointerPoint ^PointerPoint, bool sound)
{
	if (Move(PointerPoint))
	{
		if (Game_Data->set_sound && sound)
		{
			if (Game_Data->m_inst_click->GetState() == SoundState::STOPPED)
			{
				Game_Data->m_inst_click->Stop();
				Game_Data->m_inst_click->Play();
			}
		}
		return true;
	}
	return false;
}

bool But2D::Move(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if ((PointerPoint->Position.Y * Game_Data->scr_scale) > (x + x_s) && (PointerPoint->Position.Y * Game_Data->scr_scale) < (x + x_s + w))
	{
		if ((PointerPoint->Position.X * Game_Data->scr_scale) < (y - y_s) && (PointerPoint->Position.X * Game_Data->scr_scale) > (y - y_s - h))
		{
			touch = true;
			return true;
		}
	}
	touch = false;
	return false;
}

bool But2D::Release(Windows::UI::Input::PointerPoint ^PointerPoint, bool moved)
{
	touch = false;

	if (moved) return false;

	if ((PointerPoint->Position.Y * Game_Data->scr_scale) > (x + x_s) && (PointerPoint->Position.Y * Game_Data->scr_scale) < (x + x_s + w))
	{
		if ((PointerPoint->Position.X * Game_Data->scr_scale) < (y - y_s) && (PointerPoint->Position.X * Game_Data->scr_scale) > (y - y_s - h))
		{
			return true;
		}
	}
	return false;
}
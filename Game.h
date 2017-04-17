#pragma once

#include "GameRoot.h"
#include "GameData.h"
#include "MenuMain.h"
#include "MenuCal.h"
#include "GamePlay.h"
#include "Scr2D.h"

// This class renders a simple spinning cube.
ref class Game sealed : public Direct3DBase
{
public:
	Game();

	// Direct3DBase methods.
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	
	// Method for updating time-dependent objects.
	void Update(float timeTotal, float timeDelta);
	virtual void Draw() override;

	void OnPointerPressed(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerMoved(Windows::UI::Input::PointerPoint ^PointerPoint);
	void OnPointerReleased(Windows::UI::Input::PointerPoint ^PointerPoint);

	void OnBackButtonPressed(Windows::Phone::UI::Input::BackPressedEventArgs^ args);
private:
	void	ChangeStat();
	void	StatMenuMain();
	void	StatMenuSelect();
	void	StatMenuSet();
	void	StatMenuHelp();
	void	StatMenuCal();
	void	StatStart();
	void	StatWin();
	void	StatLose();
	void	StatPause();
	void	StatNext();
	void	StatResume();
	void	StatSurv();
	void	StatRestart();

    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>  m_batch;
    std::unique_ptr<DirectX::SpriteFont>                                    m_font;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>                               m_batchInputLayout;

	Windows::Devices::Sensors::Accelerometer								^m_acc;
	Windows::Devices::Sensors::Gyrometer									^m_gyro;
	long long																gyro_last;

	void Acc_Changed(Windows::Devices::Sensors::Accelerometer^ acc, Windows::Devices::Sensors::AccelerometerReadingChangedEventArgs^ arg);
	void Gyro_Changed(Windows::Devices::Sensors::Gyrometer^ acc, Windows::Devices::Sensors::GyrometerReadingChangedEventArgs^ arg);

	GameRoot *Game_Root;
	GamePlay *Game_Play;
	MenuMain *Menu_Main;
	MenuCal *Menu_Cal;

	GameData Game_Data;

	Scr2D	*loading;
	float	t_loading;

	void Sleep(DWORD ms);
};
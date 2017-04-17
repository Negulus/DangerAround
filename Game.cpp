#include "pch.h"
#include "Game.h"
#include "DDSTextureLoader.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::Storage;
using namespace Windows::Graphics::Display;
using namespace Windows::Phone::Management;
//using namespace Windows::ApplicationModel;
//using namespace Windows::ApplicationModel::Store;

Game::Game()
{
}

void Game::CreateDeviceResources()
{
//	Game_Data.isTrial = false;
//#ifdef _DEBUG
//	Game_Data.isTrial = true;
//#else
//	Game_Data.isTrial = CurrentApp::LicenseInformation->IsTrial;
//#endif

	m_acc = Windows::Devices::Sensors::Accelerometer::GetDefault();
	m_acc->ReportInterval = m_acc->MinimumReportInterval;
	m_acc->ReadingChanged += ref new TypedEventHandler<Windows::Devices::Sensors::Accelerometer^, Windows::Devices::Sensors::AccelerometerReadingChangedEventArgs^>(this, &Game::Acc_Changed);

	//Инициализация гироскопа
	Game_Data.gyro.x = 0;
	Game_Data.gyro.y = 0;
	Game_Data.gyro.z = 0;
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	gyro_last = (LONGLONG)ft.dwLowDateTime + ((LONGLONG)(ft.dwHighDateTime) << 32LL);
	m_gyro = Windows::Devices::Sensors::Gyrometer::GetDefault();
	m_gyro->ReadingChanged += ref new TypedEventHandler<Windows::Devices::Sensors::Gyrometer^, Windows::Devices::Sensors::GyrometerReadingChangedEventArgs^>(this, &Game::Gyro_Changed);

	Direct3DBase::CreateDeviceResources();

	// Create DirectXTK objects
	Game_Data.device = m_d3dDevice.Get();
	Game_Data.context = m_d3dContext.Get();

	Game_Data.m_states.reset( new CommonStates( Game_Data.device ) );

	DirectX::EffectFactory::EffectInfo inf;
    auto fx = new EffectFactory( Game_Data.device );
    fx->SetDirectory( L"Images\\Textures" );
    Game_Data.m_fxFactory.reset( fx );

	Game_Data.m_sprites.reset( new SpriteBatch( Game_Data.context ) );
    m_batch.reset( new PrimitiveBatch<VertexPositionColor>( Game_Data.context ) );

    Game_Data.m_batchEffect.reset( new BasicEffect( Game_Data.device ) );
    Game_Data.m_batchEffect->SetVertexColorEnabled(true);

    {
        void const* shaderByteCode;
        size_t byteCodeLength;

        Game_Data.m_batchEffect->GetVertexShaderBytecode( &shaderByteCode, &byteCodeLength );

        DX::ThrowIfFailed(
			Game_Data.device->CreateInputLayout( VertexPositionColor::InputElements,
                                        VertexPositionColor::InputElementCount,
                                        shaderByteCode, byteCodeLength,
                                        m_batchInputLayout.ReleaseAndGetAddressOf() ) 
                         );
    }
	
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
//	new AudioEngine(eflags);
	Game_Data.m_audEngine.reset( new AudioEngine(eflags) );
	Game_Data.m_sound_click.reset(new SoundEffect(Game_Data.m_audEngine.get(), L"Audio\\click.wav"));
	Game_Data.m_inst_click = Game_Data.m_sound_click->CreateInstance();

	//Инициализация игрового статуса
	Game_Data.Status_Current = stat_Null;
	Game_Data.Status_Next = stat_MenuMain;

	Game_Data.Map = 0;

	srand((int)ft.dwLowDateTime);

	Game_Root = new GameRoot();
	Game_Root->Game_Data = &Game_Data;

//	Sleep(2000);
	Game_Root->Data_Load();
//	Sleep(1000);

	Game_Data.loading = true;
}

void Game::Sleep(DWORD ms)
{
	static HANDLE singletonEvent = nullptr;
	HANDLE sleepEvent = singletonEvent;

	if (!sleepEvent)
	{
		sleepEvent = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);

		if (!sleepEvent) return;

		HANDLE previousEvent = InterlockedCompareExchangePointerRelease(&singletonEvent, sleepEvent, nullptr);
            
		if (previousEvent)
		{
			CloseHandle(sleepEvent);
			sleepEvent = previousEvent;
		}
	}

	WaitForSingleObjectEx(sleepEvent, ms, false);
}

void Game::CreateWindowSizeDependentResources()
{
	Direct3DBase::CreateWindowSizeDependentResources();

	float aspectRatio = m_windowBounds.Width / m_windowBounds.Height;
	float fovAngleY = 50.0f * XM_PI / 180.0f;

	XMMATRIX projection = XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, 0.01f, 220.0f);

	XMVECTOR eye = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMMATRIX view = XMMatrixLookAtRH(eye, at, up);
	XMMATRIX world = XMMatrixTranslation(0.f, 0.f, 0.f);

	Game_Data.m_batchEffect->SetProjection( projection );
    Game_Data.m_batchEffect->SetView( view );
    Game_Data.m_batchEffect->SetWorld( XMMatrixIdentity() );

	Game_Data.m_batchEffect->EnableDefaultLighting();

	XMStoreFloat4x4(&Game_Data.m_proj3d, projection);
	XMStoreFloat4x4(&Game_Data.m_view, view);
	XMStoreFloat4x4(&Game_Data.m_world, world);

	Game_Data.scr_scale = (float)Windows::Graphics::Display::DisplayProperties::ResolutionScale * 0.01f;

	if (m_windowBounds.Height < 810)
	{
		projection = XMMatrixOrthographicRH(480, 800, -1, 1);
		Game_Data.scr_wide = false;
	}
	else
	{
		projection = XMMatrixOrthographicRH(480, 853, -1, 1);
		Game_Data.scr_wide = true;
	}
	XMStoreFloat4x4(&Game_Data.m_proj2d, projection);

	loading = new Scr2D(&Game_Data, -112, -16, L"Images\\Other\\loading.dds");
	t_loading = 0;
}

void Game::Update(float timeTotal, float timeDelta)
{
    UNREFERENCED_PARAMETER( timeDelta );

	t_loading -= timeDelta;
	if (t_loading < 0) t_loading = 0;

	ChangeStat();

	switch (Game_Data.Status_Current)
	{
	case stat_MenuMain:
	case stat_MenuSelect:
	case stat_MenuSet:
	case stat_MenuHelp:
		Menu_Main->Update(timeTotal, timeDelta);
		break;
	case stat_MenuCal:
		Menu_Cal->Update(timeTotal, timeDelta);
		break;
	case stat_Start:
	case stat_Play:
	case stat_Win:
	case stat_Lose:
	case stat_Pause:
	case stat_Next:
	case stat_Resume:
	case stat_GameOver:
	case stat_Restart:
		Game_Play->Update(timeTotal, timeDelta);
		break;
	default:
		break;
	}
}

void Game::Draw()
{
	Game_Data.m_depthStencilView = m_depthStencilView;
	m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::Black);
	m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	if (Game_Data.Status_Next != stat_Null && (Game_Data.loading || t_loading > 0))
	{
		Game_Data.m_batchEffect->SetProjection( XMLoadFloat4x4( &Game_Data.m_proj2d ) );
		Game_Data.m_sprites->Begin();
		loading->Draw();
		Game_Data.m_sprites->End();
		if (Game_Data.loading) t_loading = 0.5f;
		Game_Data.loading = false;	
		return;
	}

	switch (Game_Data.Status_Current)
	{
	case stat_MenuMain:
	case stat_MenuSelect:
	case stat_MenuSet:
	case stat_MenuHelp:
		Menu_Main->Draw();
		break;
	case stat_MenuCal:
		Menu_Cal->Draw();
		break;
	case stat_Start:
	case stat_Play:
	case stat_Win:
	case stat_Lose:
	case stat_Pause:
	case stat_Next:
	case stat_Resume:
	case stat_GameOver:
	case stat_Restart:
		Game_Play->Draw();
		break;
	default:
		break;
	}
}

PCWSTR SaveStateFile = L"savedstate.txt";

void Game::ChangeStat()
{
    if (Game_Data.Status_Next == stat_Null)
        return;

	if (Game_Data.loading || t_loading > 0)
		return;
    
	switch (Game_Data.Status_Next)
	{
	case stat_MenuMain:
		StatMenuMain();
		break;
	case stat_MenuSelect:
		StatMenuSelect();
		break;
	case stat_MenuSet:
		StatMenuSet();
		break;
	case stat_MenuHelp:
		StatMenuHelp();
		break;
	case stat_MenuCal:
		StatMenuCal();
		break;
	case stat_Start:
		StatStart();
		break;
	case stat_Win:
		StatWin();
		break;
	case stat_Lose:
		StatLose();
		break;
	case stat_Pause:
		StatPause();
		break;
	case stat_Next:
		StatNext();
		break;
	case stat_Resume:
		StatResume();
		break;
	case stat_GameOver:
		StatSurv();
		break;
	case stat_Restart:
		StatRestart();
		break;
	case stat_Exit:
		exit(0);
		break;
	default:
		Game_Data.Status_Next = stat_MenuMain;
		break;
	}
}

void Game::StatMenuMain()
{
	switch (Game_Data.Status_Current)
	{
	case stat_Null:
		Menu_Main = new MenuMain(&Game_Data);
		break;
	case stat_MenuSet:
	case stat_MenuSelect:
		Game_Root->Data_Save_Set();
	case stat_MenuHelp:
		Menu_Main->Menu_Update();
		break;
	case stat_Win:
	case stat_Lose:
	case stat_Pause:
	case stat_GameOver:
		delete Game_Play;
		Menu_Main = new MenuMain(&Game_Data);
		break;
	}

	Game_Data.Status_Current = stat_MenuMain;
	Game_Data.Status_Next = stat_Null;
}

void Game::StatMenuSelect()
{
	switch (Game_Data.Status_Current)
	{
	case stat_MenuMain:
		Menu_Main->Menu_Update();
		break;
	case stat_Win:
	case stat_Lose:
	case stat_Pause:
	case stat_GameOver:
		delete Game_Play;
		Menu_Main = new MenuMain(&Game_Data);
		break;
	}

	Game_Data.Status_Current = stat_MenuSelect;
	Game_Data.Status_Next = stat_Null;
}

void Game::StatMenuSet()
{
	switch (Game_Data.Status_Current)
	{
	case stat_MenuMain:
		Menu_Main->Menu_Update();
		break;
	case stat_MenuCal:
		delete Menu_Cal;
		Menu_Main = new MenuMain(&Game_Data);
		break;
	}

	Game_Data.Status_Current = stat_MenuSet;
	Game_Data.Status_Next = stat_Null;
}

void Game::StatMenuHelp()
{
	Menu_Main->Menu_Update();

	Game_Data.Status_Current = stat_MenuHelp;
	Game_Data.Status_Next = stat_Null;
}

void Game::StatMenuCal()
{
	delete Menu_Main;
	Menu_Cal = new MenuCal(&Game_Data);

	Game_Data.Status_Current = stat_MenuCal;
	Game_Data.Status_Next = stat_Null;
}

void Game::StatStart()
{
	switch (Game_Data.Status_Current)
	{
	case stat_MenuMain:
	case stat_MenuSelect:
	case stat_MenuSet:
	case stat_MenuHelp:
		Game_Root->Data_Save_Set();
		delete Menu_Main;
		Game_Play = new GamePlay(&Game_Data);
		break;
	case stat_Win:
	case stat_Lose:
	case stat_Pause:
	case stat_GameOver:
		delete Game_Play;
		Game_Play = new GamePlay(&Game_Data);
		break;
	}

	Game_Data.Status_Current = stat_Play;
	Game_Data.Status_Next = stat_Null;
}

void Game::StatWin()
{
	Game_Data.Score_Last = Game_Data.scores[Game_Data.Map][Game_Data.Level-1][Game_Data.Game_Type];
	if (Game_Data.Score > Game_Data.scores[Game_Data.Map][Game_Data.Level-1][Game_Data.Game_Type])
	{
		Game_Data.scores[Game_Data.Map][Game_Data.Level-1][Game_Data.Game_Type] = Game_Data.Score;
		Game_Root->Data_Save_Map(Game_Data.Map);
	}
	Game_Data.Status_Current = stat_Win;
	Game_Data.Status_Next = stat_Null;
}

void Game::StatLose()
{
	Game_Data.Status_Current = stat_Lose;
	Game_Data.Status_Next = stat_Null;
}

void Game::StatPause()
{
	Game_Data.Status_Current = stat_Pause;
	Game_Data.Status_Next = stat_Null;
}

void Game::StatSurv()
{
	Game_Data.Score_Last = Game_Data.scores[Game_Data.Map][Game_Data.Level_Start-1][Game_Data.Game_Type];
	if (Game_Data.Score > Game_Data.scores[Game_Data.Map][Game_Data.Level_Start-1][Game_Data.Game_Type])
	{
		Game_Data.scores[Game_Data.Map][Game_Data.Level_Start-1][Game_Data.Game_Type] = Game_Data.Score;
		Game_Root->Data_Save_Map(Game_Data.Map);
	}
	Game_Data.Status_Current = stat_GameOver;
	Game_Data.Status_Next = stat_Null;
}

void Game::StatNext()
{
	Game_Data.Level += 1;
	Game_Data.Status_Next = stat_Start;
}

void Game::StatResume()
{
	Game_Data.Status_Current = stat_Play;
	Game_Data.Status_Next = stat_Null;
}

void Game::StatRestart()
{
	Game_Data.Status_Next = stat_Start;
}


void Game::OnPointerPressed(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (Game_Data.Status_Next != stat_Null && (Game_Data.loading || t_loading > 0)) return;

	switch (Game_Data.Status_Current)
	{
	case stat_MenuMain:
	case stat_MenuSelect:
	case stat_MenuSet:
	case stat_MenuHelp:
		Menu_Main->OnPointerPressed(PointerPoint);
		break;
	case stat_MenuCal:
		Menu_Cal->OnPointerPressed(PointerPoint);
		break;
	case stat_Start:
	case stat_Play:
	case stat_Win:
	case stat_Lose:
	case stat_Pause:
	case stat_GameOver:
	case stat_Next:
	case stat_Resume:
	case stat_Restart:
		Game_Play->OnPointerPressed(PointerPoint);
		break;
	default:
		break;
	}
}

void Game::OnPointerMoved(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (Game_Data.Status_Next != stat_Null && (Game_Data.loading || t_loading > 0)) return;

	switch (Game_Data.Status_Current)
	{
	case stat_MenuMain:
	case stat_MenuSelect:
	case stat_MenuSet:
	case stat_MenuHelp:
		Menu_Main->OnPointerMoved(PointerPoint);
		break;
	case stat_MenuCal:
		Menu_Cal->OnPointerMoved(PointerPoint);
		break;
	case stat_Start:
	case stat_Play:
	case stat_Win:
	case stat_Lose:
	case stat_Pause:
	case stat_GameOver:
	case stat_Next:
	case stat_Resume:
	case stat_Restart:
		Game_Play->OnPointerMoved(PointerPoint);
		break;
	default:
		break;
	}
}

void Game::OnPointerReleased(Windows::UI::Input::PointerPoint ^PointerPoint)
{
	if (Game_Data.Status_Next != stat_Null && (Game_Data.loading || t_loading > 0)) return;

	switch (Game_Data.Status_Current)
	{
	case stat_MenuMain:
	case stat_MenuSelect:
	case stat_MenuSet:
	case stat_MenuHelp:
		Menu_Main->OnPointerReleased(PointerPoint);
		break;
	case stat_MenuCal:
		Menu_Cal->OnPointerReleased(PointerPoint);
		break;
	case stat_Start:
	case stat_Play:
	case stat_Win:
	case stat_Lose:
	case stat_Pause:
	case stat_GameOver:
	case stat_Next:
	case stat_Resume:
	case stat_Restart:
		Game_Play->OnPointerReleased(PointerPoint);
		break;
	default:
		break;
	}
}

void Game::OnBackButtonPressed(Windows::Phone::UI::Input::BackPressedEventArgs^ args)
{
	if (Game_Data.Status_Next != stat_Null && (Game_Data.loading || t_loading > 0)) return;

	switch (Game_Data.Status_Current)
	{
	case stat_MenuMain:
	case stat_MenuSelect:
	case stat_MenuSet:
	case stat_MenuHelp:
		Menu_Main->OnBackButtonPressed(args);
		break;
	case stat_MenuCal:
		Game_Data.Status_Next = stat_MenuSet;
		break;
	case stat_Start:
	case stat_Play:
		Game_Data.Status_Next = stat_Pause;
		break;
	case stat_Pause:
		Game_Data.Status_Next = stat_Resume;
		break;
	default:
		break;
	}
}

void Game::Acc_Changed(Windows::Devices::Sensors::Accelerometer^ acc, Windows::Devices::Sensors::AccelerometerReadingChangedEventArgs^ arg)
{
	Game_Data.acc.x += (float)(arg->Reading->AccelerationX - Game_Data.acc.x) * 0.05f;
	Game_Data.acc.y += (float)(arg->Reading->AccelerationY - Game_Data.acc.y) * 0.05f;
	Game_Data.acc.z += (float)(arg->Reading->AccelerationZ - Game_Data.acc.z) * 0.05f;
}

void Game::Gyro_Changed(Windows::Devices::Sensors::Gyrometer^ acc, Windows::Devices::Sensors::GyrometerReadingChangedEventArgs^ arg)
{
	float ttt = (arg->Reading->Timestamp.UniversalTime - gyro_last) * 0.00000052f;
	Game_Data.gyro.x += (float)(arg->Reading->AngularVelocityX * ttt - Game_Data.gyro.x) * 0.5f;
	Game_Data.gyro.y += (float)(arg->Reading->AngularVelocityY * ttt - Game_Data.gyro.y) * 0.5f;
	Game_Data.gyro.z += (float)(arg->Reading->AngularVelocityZ * ttt - Game_Data.gyro.z) * 0.5f;
	gyro_last = arg->Reading->Timestamp.UniversalTime;
}
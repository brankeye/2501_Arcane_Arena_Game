#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

using namespace std;
#include <windows.h>

#include <d3d11.h>
#include <directxmath.h>

#include "Rect.h"

#include "Sprite.h"
#include "Hud.h"
#include "Menu.h"
#include "./Inc/CommonStates.h"
#include "UnitControl.h"
#include "CollisionBox.h"
#include "Unit.h"
#include "Player.h"

#include "./Inc/WICTextureLoader.h"
#include "./Inc/Effects.h"
#include "./Inc/GeometricPrimitive.h"
#include "./Inc/PrimitiveBatch.h"
#include "./Inc/ScreenGrab.h"
#include "./Inc/SpriteBatch.h"
#include "./Inc/SpriteFont.h"
#include "./Inc/VertexTypes.h"
#include "Enemy.h"

#include "resource.h"
#include <sstream>
#include "KeyInput.h"
#include <vector>
#include <chrono>
#include "Environment.h"
#include "LevelSystem.h"

// soundtrack stuff
#include "SDL.h"
#include "SDL_mixer.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE                           g_hInst = nullptr;
HWND                                g_hWnd = nullptr;
D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*                       g_pd3dDevice = nullptr;
ID3D11DeviceContext*                g_pImmediateContext = nullptr;
IDXGISwapChain*                     g_pSwapChain = nullptr;
ID3D11RenderTargetView*             g_pRenderTargetView = nullptr;
ID3D11Texture2D*                    g_pDepthStencil = nullptr;
ID3D11DepthStencilView*             g_pDepthStencilView = nullptr;

ID3D11Resource*						g_pTextureBirdSheetRes = nullptr;
ID3D11ShaderResourceView*           g_pTextureBirdSheet = nullptr;
vector<ID3D11Resource *>            TextureRes;
vector<ID3D11ShaderResourceView *>  Texture;

ID3D11InputLayout*                  g_pBatchInputLayout = nullptr;

std::unique_ptr<BasicEffect>                            g_BatchEffect;
std::unique_ptr<GeometricPrimitive>                     g_Shape;
std::unique_ptr<PrimitiveBatch<VertexPositionColor>>    g_Batch;
PrimitiveBatch<VertexPositionColor>* tempBatch; // for resetting g_Batch
std::unique_ptr<SpriteFont>                             g_Font;

SpriteBatch*				                            g_Sprites;

KeyInput*										keyInput;

XMMATRIX                            g_World;
XMMATRIX                            g_View;
XMMATRIX                            g_Projection;

// Window
int screenWidth = 0, screenHeight = 0; // values set in InitWindow function

// Sprites
Sprite* g_Sprite;
Sprite* hud_Top;
Sprite* hud_Health;
Sprite* hud_Bar;
Sprite* hud_HP;
Sprite* hud_Mana;
Sprite* hud_MP;
Sprite* hud_GreyBar;
Sprite* hud_Center;
Sprite* hud_PauseMenu;
Sprite* hud_Start;
Sprite* unit_Body;
Sprite* unit_Foot;
Sprite* unit_Hand;
Sprite* unit_Hat;
Sprite* unit_Head;
Sprite* unit_Staff;

Sprite* enemy_Body;
Sprite* enemy_Foot;
Sprite* enemy_Head;
Sprite* enemy_Staff;

Sprite* map;
Sprite* water;
Sprite* clouds;


Sprite* heatSphere;
Sprite* iceSphere;
Sprite* energySphere;
Sprite* poisonSphere;
Sprite* earthSphere;

Sprite* sphereParticle;

vector<Sprite*> proj_Sprites; // the projectile sprites

Sprite* sprite_Zombie;

Hud* hud;
Menu menu;
UnitControl* unitStorage;
Player* thePlayer;
//Enemy*  enem_Zombie;
//Enemy*  enem_Zombie2;
Environment* environment;
LevelSystem levelSystem;

vector<Enemy*> enemies; // a vector containing all of the enemies

// Soundtrack
Mix_Music* music = 0;

vector<vector<Mix_Chunk*>> proj_Sounds; // list containing lists to sounds for specific projectiles
int projectileChannel = -1;
vector<Mix_Chunk*> chunk_Basic;
vector<Mix_Chunk*> chunk_Fire;
vector<Mix_Chunk*> chunk_Ice;
vector<Mix_Chunk*> chunk_Earth;
vector<Mix_Chunk*> chunk_Poison;
vector<Mix_Chunk*> chunk_Bolt;

vector<Mix_Chunk*> unit_Walking_Sounds;
int walkingChannel = -1;

vector<Mix_Chunk*> hit_Sounds;

bool musicStopped = false;

long int timePlaying = 0;
long int timeNotPlaying = 0;
long int timeTracker = 0;
long int timeNPTracker = 0;
bool timePlayingHandled = false;


//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
void Render();
void UpdateKeys();
void HandleAllMovement();
void HandleAllCollisions();
void HandleMusic();
void HandleTime();
void HandleEnemySpawning();
void handleHitSounds();
void resetEverything();

// Handle mouse events
HHOOK g_hMouseHook;
bool leftMousePressed = false;
bool leftButton = false;
bool rightMousePressed = false;
bool rightButton = false;

// Get the horizontal and vertical screen sizes in pixel
void GetDesktopResolution(int& horizontal, int& vertical)
{
   RECT desktop;
   // Get a handle to the desktop window
   const HWND hDesktop = GetDesktopWindow();
   // Get the size of screen to the variable desktop
   GetWindowRect(hDesktop, &desktop);
   // The top left corner will have coordinates (0,0)
   // and the bottom right corner will have coordinates
   // (horizontal, vertical)
   horizontal = desktop.right;
   vertical = desktop.bottom;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        PMSLLHOOKSTRUCT pmll = (PMSLLHOOKSTRUCT) lParam;

		if(rightButton == false && wParam == WM_LBUTTONDOWN) {
			leftButton = true;
		}
		if(leftButton == true && wParam == WM_LBUTTONUP) {
			leftMousePressed = true;
			leftButton = false;
		}

		if(leftButton == false && wParam == WM_RBUTTONDOWN) {
			rightButton = true;
		}
		if(rightButton == true && wParam == WM_RBUTTONUP) {
			rightMousePressed = true;
			rightButton = false;
		}
    }
 
    return CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);
}

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
	// soundtrack stuff
	/* We're going to be requesting certain things from our audio
     device, so we set them up beforehand */
	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
	int audio_channels = 2;
	int audio_buffers = 4096;
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	/* This is where we open up our audio device.  Mix_OpenAudio takes
    as its parameters the audio format we'd /like/ to have. */
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
		exit(1);
	}

    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }

	g_hMouseHook = SetWindowsHookEx( WH_MOUSE_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0 );
	srand(unsigned(time(0))); // seed random numbers
    // Main message loop
    MSG msg = {0};
	long int timer = clock();
	long int timerMod = 8;
	long int timer2 = timer;
	long int timer2Mod = 2;
    while( WM_QUIT != msg.message )
    {
		/* Actually loads up the music */
		if(music == 0) {
			music = Mix_LoadMUS("./Sound/Tracks/Start.wav");
			Mix_VolumeMusic(16);
			Mix_FadeInMusic(music, -1, 2000);
		}

		/* This begins playing the music - the first argument is a
	   pointer to Mix_Music structure, and the second is how many
	   times you want it to loop (use -1 for infinite, and 0 to
	   have it just play once) */

		if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ))
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else {
			bool timeToUpdate = clock() > timer2 + timer2Mod;

			HandleTime();
			HandleEnemySpawning();

			if(timeToUpdate) {
				HandleAllMovement();
			}

			if(clock() > timer + timerMod) {
				timer += timerMod;
				HandleAllCollisions();
			}

			HandleMusic();
			Render();

			if(timeToUpdate) {
				leftMousePressed = false;
				rightMousePressed = false;
				
				timer2 += timer2Mod;
			}
        }
    }

    CleanupDevice();
	UnhookWindowsHookEx(g_hMouseHook);
	/* This is the cleaning up part */
	Mix_HaltMusic();
	Mix_FreeMusic(music);
	music = NULL;
	Mix_CloseAudio();
	SDL_Quit();
    return ( int )msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_SAMPLE1 );
    wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"SampleWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_SAMPLE1 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

	GetDesktopResolution(screenWidth, screenHeight);
	
    // Create window
    g_hInst = hInstance;

	DEVMODE screen;
    DWORD dwStyle, dwExStyle;
    memset(&screen,0,sizeof(screen));
    screen.dmSize = sizeof(screen);
    screen.dmPelsWidth = screenWidth;
    screen.dmPelsHeight = screenHeight;
    screen.dmBitsPerPel = 32;
    screen.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
    if(ChangeDisplaySettings(&screen, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL){
        dwExStyle = 0;      
        dwStyle = WS_POPUP;                 
        ShowCursor(FALSE);
    }
    else{
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle = WS_OVERLAPPEDWINDOW;
    }

	if(screenWidth != 1366 && screenHeight != 768) {
		if(screenWidth > 1366) {
			screenWidth = 1366;
		}
		if(screenHeight > 768) {
			screenHeight = 768;
		}
		RECT rc = { 0, 0, 1366, 768};
		
		AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
		g_hWnd = CreateWindow( L"SampleWindowClass", L"Tutorial 8", WS_OVERLAPPEDWINDOW,
						       CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
							   nullptr );
	} else {
		//AdjustWindowRect( &g_Window, WS_MAXIMIZE, FALSE );
		g_hWnd = CreateWindow( L"SampleWindowClass", L"DirectXTK Simple Sample", WS_POPUP,
				               CW_USEDEFAULT, CW_USEDEFAULT, screenWidth, screenHeight, nullptr, nullptr, hInstance,
				               nullptr );
	}

	
    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    //GetClientRect( g_hWnd, &rc );
    UINT width = screenWidth;
    UINT height = screenHeight;

	menu = Menu(screenWidth, screenHeight);


    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
        return hr;

    hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &g_pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = g_pd3dDevice->CreateTexture2D( &descDepth, nullptr, &g_pDepthStencil );
    if( FAILED( hr ) )
        return hr;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );
    if( FAILED( hr ) )
        return hr;

    g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView );

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports( 1, &vp );

    // Create DirectXTK objects
    g_Sprites = new SpriteBatch( g_pImmediateContext );
    g_Batch.reset( new PrimitiveBatch<VertexPositionColor>( g_pImmediateContext ) );

    g_BatchEffect.reset( new BasicEffect( g_pd3dDevice ) );
    g_BatchEffect->SetVertexColorEnabled(true);

    {
        void const* shaderByteCode;
        size_t byteCodeLength;

        g_BatchEffect->GetVertexShaderBytecode( &shaderByteCode, &byteCodeLength );

        hr = g_pd3dDevice->CreateInputLayout( VertexPositionColor::InputElements,
                                              VertexPositionColor::InputElementCount,
                                              shaderByteCode, byteCodeLength,
                                              &g_pBatchInputLayout );
        if( FAILED( hr ) )
            return hr;
    }

    g_Font.reset( new SpriteFont( g_pd3dDevice, L"arial.spritefont" ) );

	
	//Load Images
	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Hud/basehud.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	hud_Center = new Sprite(TextureRes.back(),Texture.back(),1,1,0);


	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Units/BodyBlue1.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	unit_Body = new Sprite(TextureRes.back(),Texture.back(),1,1,0);
	
	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Units/Foot1.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	unit_Foot = new Sprite(TextureRes.back(),Texture.back(),1,1,0);
	
	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Units/Hand1.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	unit_Hand = new Sprite(TextureRes.back(),Texture.back(),1,1,0);
	
	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Units/HatBlue1.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	unit_Hat = new Sprite(TextureRes.back(),Texture.back(),1,1,0);
	
	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Units/HeadWhiteBeard1.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	unit_Head = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Units/Staff1.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	unit_Staff = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Projectiles/basic.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	proj_Sprites.push_back(new Sprite(TextureRes.back(),Texture.back(),1,1,0));

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Projectiles/fire.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	proj_Sprites.push_back(new Sprite(TextureRes.back(),Texture.back(),1,1,0));

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Projectiles/ice.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	proj_Sprites.push_back(new Sprite(TextureRes.back(),Texture.back(),1,1,0));

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Projectiles/earth.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	proj_Sprites.push_back(new Sprite(TextureRes.back(),Texture.back(),1,1,0));

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Projectiles/poison.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	proj_Sprites.push_back(new Sprite(TextureRes.back(),Texture.back(),1,1,0));

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Projectiles/bolt.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	proj_Sprites.push_back(new Sprite(TextureRes.back(),Texture.back(),1,1,0));

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Enemies/zombie.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	sprite_Zombie = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	//ElementSpheres
	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Other/SphereHeat.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	heatSphere = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Other/SphereCold.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	iceSphere = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Other/SpherePoison.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	poisonSphere = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Other/SphereEnergy.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	energySphere = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Other/SphereEarth.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	earthSphere = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Hud/SphereParticle.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	sphereParticle = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Hud/HudCenterTop.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	hud_Top = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Hud/HealthBar.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	hud_Health = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Hud/HudBar.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	hud_Bar = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Hud/Health.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	hud_HP = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Hud/ManaBar.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	hud_Mana = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Hud/Mana.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	hud_MP = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Hud/GreyBar.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	hud_GreyBar = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Hud/PauseMenu.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	hud_PauseMenu = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Hud/Start.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	hud_Start = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	//Load Enemy
	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Enemies/HeadHoodedBrown1L.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	enemy_Head = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Enemies/BodyBrown1L.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	enemy_Body = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Enemies/Foot1L.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	enemy_Foot = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Enemies/Staff1L.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	enemy_Staff = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Environment/Map1.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	map = new Sprite(TextureRes.back(),Texture.back(),1,1,0);
		
	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Environment/Water.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	water = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	TextureRes.push_back(nullptr); Texture.push_back(nullptr);
	hr = CreateWICTextureFromFile( g_pd3dDevice, g_pImmediateContext, L"Images/Environment/Clouds.png", &TextureRes.back(), &Texture.back() );
    if( FAILED( hr ) ) return hr;
	clouds = new Sprite(TextureRes.back(),Texture.back(),1,1,0);

	// Sound Effects
	unit_Walking_Sounds.push_back(Mix_LoadWAV("Sound/SoundEffects/Walking/Walking_01.wav"));
	unit_Walking_Sounds.push_back(Mix_LoadWAV("Sound/SoundEffects/Walking/Walking_02.wav"));
	unit_Walking_Sounds.push_back(Mix_LoadWAV("Sound/SoundEffects/Walking/Walking_03.wav"));
	unit_Walking_Sounds.push_back(Mix_LoadWAV("Sound/SoundEffects/Walking/Walking_04.wav"));
	unit_Walking_Sounds.push_back(Mix_LoadWAV("Sound/SoundEffects/Walking/Walking_05.wav"));

	for(int i = 0; i < 5; i++) {
		Mix_VolumeChunk(unit_Walking_Sounds.at(i), 4);
	}

	chunk_Basic.push_back(Mix_LoadWAV("Sound/SoundEffects/Basic/Basic_01.wav"));
	chunk_Basic.push_back(Mix_LoadWAV("Sound/SoundEffects/Basic/Basic_02.wav"));
	chunk_Basic.push_back(Mix_LoadWAV("Sound/SoundEffects/Basic/Basic_03.wav"));

	chunk_Fire.push_back(Mix_LoadWAV("Sound/SoundEffects/Fire/Fire_01.wav"));
	chunk_Fire.push_back(Mix_LoadWAV("Sound/SoundEffects/Fire/Fire_02.wav"));
	chunk_Fire.push_back(Mix_LoadWAV("Sound/SoundEffects/Fire/Fire_03.wav"));

	chunk_Ice.push_back(Mix_LoadWAV("Sound/SoundEffects/Ice/Ice_01.wav"));
	chunk_Ice.push_back(Mix_LoadWAV("Sound/SoundEffects/Ice/Ice_02.wav"));
	chunk_Ice.push_back(Mix_LoadWAV("Sound/SoundEffects/Ice/Ice_03.wav"));

	chunk_Earth.push_back(Mix_LoadWAV("Sound/SoundEffects/Earth/Earth_01.wav"));
	chunk_Earth.push_back(Mix_LoadWAV("Sound/SoundEffects/Earth/Earth_02.wav"));
	chunk_Earth.push_back(Mix_LoadWAV("Sound/SoundEffects/Earth/Earth_03.wav"));

	chunk_Poison.push_back(Mix_LoadWAV("Sound/SoundEffects/Poison/Poison_01.wav"));
	chunk_Poison.push_back(Mix_LoadWAV("Sound/SoundEffects/Poison/Poison_02.wav"));
	chunk_Poison.push_back(Mix_LoadWAV("Sound/SoundEffects/Poison/Poison_03.wav"));

	chunk_Bolt.push_back(Mix_LoadWAV("Sound/SoundEffects/Bolt/Bolt_01.wav"));
	chunk_Bolt.push_back(Mix_LoadWAV("Sound/SoundEffects/Bolt/Bolt_02.wav"));
	chunk_Bolt.push_back(Mix_LoadWAV("Sound/SoundEffects/Bolt/Bolt_03.wav"));

	proj_Sounds.push_back(chunk_Basic);
	proj_Sounds.push_back(chunk_Fire);
	proj_Sounds.push_back(chunk_Ice);
	proj_Sounds.push_back(chunk_Earth);
	proj_Sounds.push_back(chunk_Poison);
	proj_Sounds.push_back(chunk_Bolt);
	// 0 - Fire, 1 - Ice, 2 - Earth, 3 - Poison, 4 - Bolt

	for(int i = 0; i < proj_Sounds.size(); i++) {
		for(int k = 0; k < proj_Sounds[i].size(); k++) {
			Mix_VolumeChunk(proj_Sounds[i].at(k), 8);
		}
	}

	hit_Sounds.push_back(Mix_LoadWAV("Sound/SoundEffects/Hit/Hit_01.wav"));
	hit_Sounds.push_back(Mix_LoadWAV("Sound/SoundEffects/Hit/Hit_02.wav"));

	for(int i = 0; i < hit_Sounds.size(); i++) {
		Mix_VolumeChunk(hit_Sounds[i], 8);
	}

	// Create Input Class
	keyInput = new KeyInput;

	keyInput->Initialize();


    // Initialize the world matrices
    g_World = XMMatrixIdentity();

    // Initialize the view matrix
    XMVECTOR Eye = XMVectorSet( 0.0f, 0.0f, -6.0f, 0.0f );
    XMVECTOR At = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
    XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
    g_View = XMMatrixLookAtLH( Eye, At, Up );

    g_BatchEffect->SetView( g_View );

    // Initialize the projection matrix
    g_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f );

    g_BatchEffect->SetProjection( g_Projection );

	tempBatch = 0;

	Sprite** spheres = new Sprite*[5];
	spheres[0] = heatSphere;
	spheres[1] = iceSphere;
	spheres[2] = earthSphere;
	spheres[3] = poisonSphere;
	spheres[4] = energySphere;
	hud = new Hud(XMFLOAT2(width, height), hud_Center, spheres, sphereParticle, hud_Top, hud_Health, hud_Bar, hud_HP, hud_Mana, hud_MP, hud_GreyBar);
	hud->loadGame();

	environment = new Environment(XMFLOAT2(width, height), map, water, clouds);


	unitStorage = new UnitControl;
	thePlayer = new Player(unit_Hat, unit_Head, unit_Body, unit_Foot, unit_Hand, unit_Staff, &proj_Sprites);
	thePlayer->setPos(50, screenHeight / 2 - thePlayer->getCollisionBox()->getHeight() / 2);
	unitStorage->addUnit(thePlayer);

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
    if( g_pImmediateContext ) g_pImmediateContext->ClearState();

    if ( g_pBatchInputLayout ) g_pBatchInputLayout->Release();

    if( g_pTextureBirdSheet ) g_pTextureBirdSheet->Release();

    if( g_pDepthStencilView ) g_pDepthStencilView->Release();
    if( g_pDepthStencil ) g_pDepthStencil->Release();
    if( g_pRenderTargetView ) g_pRenderTargetView->Release();
    if( g_pSwapChain ) g_pSwapChain->Release();
    if( g_pImmediateContext ) g_pImmediateContext->Release();
    if( g_pd3dDevice ) g_pd3dDevice->Release();
	if( keyInput ) delete(keyInput);
}

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

		case WM_KEYDOWN:
		{

			keyInput->KeyDown((unsigned int)wParam);
			return 0;
		}

		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			keyInput->KeyUp((unsigned int)wParam);
			return 0;
		}

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}


void updateKeys(){
	const int ascii_A = 65;//
	const int ascii_B = 66;
	const int ascii_C = 67;
	const int ascii_D = 68;//
	const int ascii_E = 69;
	const int ascii_P = 80;
	const int ascii_R = 82;
	const int ascii_S = 83;//
	const int ascii_W = 87;//
	const int ascii_X = 88;
	const int ascii_Y = 89;
	const int ascii_Z = 90;
	const int ascii_U = 85;
	const int ascii_H = 72;
	const int ascii_J = 74;
	const int ascii_K = 75;
}


//--------------------------------------------------------------------------------------
// Render a grid using PrimitiveBatch
//--------------------------------------------------------------------------------------
void DrawGrid()
{
    g_BatchEffect->Apply( g_pImmediateContext );
    g_pImmediateContext->IASetInputLayout( g_pBatchInputLayout );

    g_Batch->Begin();

	int numRows = 100;
	int numColumns = 10;
	int minX = -(numColumns/2);
	int maxX = numColumns/2;
	int minY = -(numRows/2);
	int maxY = numRows/2;

	// TODO: Add code to draw a grid here
	for( int i = minX; i <= maxX; ++i )
    {
		XMFLOAT3 pos1 = XMFLOAT3(i, minY, 0);
		XMFLOAT3 pos2 = XMFLOAT3(i, maxY, 0);
		VertexPositionColor v1(XMLoadFloat3(&pos1), Colors::Gray);
        VertexPositionColor v2(XMLoadFloat3(&pos2), Colors::Gray);
        g_Batch->DrawLine( v1, v2 );
    }
	for( int i = minY; i <= maxY; ++i )
    {
		XMFLOAT3 pos1 = XMFLOAT3(minX, i, 0);
		XMFLOAT3 pos2 = XMFLOAT3(maxX, i, 0);
		VertexPositionColor v1(XMLoadFloat3(&pos1), Colors::Gray);
        VertexPositionColor v2(XMLoadFloat3(&pos2), Colors::Gray);
        g_Batch->DrawLine( v1, v2 );
    }

	XMFLOAT3 yAxis1 = XMFLOAT3(0, maxY, 0);
	XMFLOAT3 yAxis2 = XMFLOAT3(0, minY, 0);
	VertexPositionColor yA1(XMLoadFloat3(&yAxis1), Colors::Red);
	VertexPositionColor yA2(XMLoadFloat3(&yAxis2), Colors::Red);
	g_Batch->DrawLine( yA1, yA2);

	XMFLOAT3 xAxis1 = XMFLOAT3(maxX, 0, 0);
	XMFLOAT3 xAxis2 = XMFLOAT3(minX, 0, 0);
	VertexPositionColor xA1(XMLoadFloat3(&xAxis1), Colors::Red);
	VertexPositionColor xA2(XMLoadFloat3(&xAxis2), Colors::Red);
	g_Batch->DrawLine( xA1, xA2);

    g_Batch->End();
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{   
	long int time = clock();
	// Update our time
    static float t = 0.0f;
    if( g_driverType == D3D_DRIVER_TYPE_REFERENCE )
    {
        t += ( float )XM_PI * 0.0125f;
    }
    else
    {
        static uint64_t dwTimeStart = 0;
        uint64_t dwTimeCur = GetTickCount64();
        if( dwTimeStart == 0 )
            dwTimeStart = dwTimeCur;
        t = ( dwTimeCur - dwTimeStart ) / 1000.0f;
    }

	g_World = XMMatrixIdentity();

    //
    // Clear the back buffer
    //
	g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, Colors::DarkSlateGray );

    //
    // Clear the depth buffer to 1.0 (max depth)
    //
    g_pImmediateContext->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	// Draw the grid
	//DrawGrid();

    // Draw sprite
	//CommonStates states(deviceContext);
	CommonStates states(g_pd3dDevice);

	// Draw the Sprite
	if(menu.playPressed) {
		g_Sprites->Begin( SpriteSortMode_Deferred , states.NonPremultiplied() );

		environment->Draw(g_Sprites, time);

		thePlayer->drawCasting(g_Sprites, t);
		
		for(int i = 0; i < enemies.size(); i++) {
			enemies[i]->drawCasting(g_Sprites, t);
		}
		
		hud->Draw(g_Sprites, time, thePlayer);

		std::wostringstream wossScore;
		wossScore << hud->getScore();
		const std::wstring& scoreString = wossScore.str();
		if(hud->getScore() <= hud->highestScore) {
			g_Font->DrawString( g_Sprites, L"S:", XMFLOAT2( 0, 0 ), Colors::Yellow, 0.0f, XMFLOAT2( 0, 0 ), 0.5f, SpriteEffects(), 0.0f);
			g_Font->DrawString( g_Sprites, scoreString.c_str(), XMFLOAT2( 50, 0 ), Colors::Yellow, 0.0f, XMFLOAT2( 0, 0 ), 0.5f, SpriteEffects(), 0.0f);
		} else {
			g_Font->DrawString( g_Sprites, L"S:", XMFLOAT2( 0, 0 ), Colors::DarkOrange, 0.0f, XMFLOAT2( 0, 0 ), 0.5f, SpriteEffects(), 0.0f);
			g_Font->DrawString( g_Sprites, scoreString.c_str(), XMFLOAT2( 50, 0 ), Colors::DarkOrange, 0.0f, XMFLOAT2( 0, 0 ), 0.5f, SpriteEffects(), 0.0f);
		}

		std::wostringstream wossLevel;
		wossLevel << levelSystem.getLevel();
		const std::wstring& levelString = wossLevel.str();
		if(levelSystem.getLevel() <= hud->highestLevel) {
			g_Font->DrawString( g_Sprites, L"L:", XMFLOAT2( 0, 50 ), Colors::Yellow, 0.0f, XMFLOAT2( 0, 0 ), 0.5f, SpriteEffects(), 0.0f);
			g_Font->DrawString( g_Sprites, levelString.c_str(), XMFLOAT2( 50, 50 ), Colors::Yellow, 0.0f, XMFLOAT2( 0, 0 ), 0.5f, SpriteEffects(), 0.0f);
		} else {
			g_Font->DrawString( g_Sprites, L"L:", XMFLOAT2( 0, 50 ), Colors::DarkOrange, 0.0f, XMFLOAT2( 0, 0 ), 0.5f, SpriteEffects(), 0.0f);
			g_Font->DrawString( g_Sprites, levelString.c_str(), XMFLOAT2( 50, 50 ), Colors::DarkOrange, 0.0f, XMFLOAT2( 0, 0 ), 0.5f, SpriteEffects(), 0.0f);
		}

		std::wostringstream wossHScore;
		wossHScore << hud->highestScore;
		const std::wstring& hsString = wossHScore.str();
		g_Font->DrawString( g_Sprites, L"HS:", XMFLOAT2( 0, screenHeight - 100 ), Colors::Yellow, 0.0f, XMFLOAT2( 0, 0 ), 0.5f, SpriteEffects(), 0.0f);
		g_Font->DrawString( g_Sprites, hsString.c_str(), XMFLOAT2( 85, screenHeight - 100 ), Colors::Yellow, 0.0f, XMFLOAT2( 0, 0 ), 0.5f, SpriteEffects(), 0.0f);

		std::wostringstream wossHLevel;
		wossHLevel << hud->highestLevel;
		const std::wstring& hlString = wossHLevel.str();
		g_Font->DrawString( g_Sprites, L"HL:", XMFLOAT2( 0, screenHeight - 50 ), Colors::Yellow, 0.0f, XMFLOAT2( 0, 0 ), 0.5f, SpriteEffects(), 0.0f);
		g_Font->DrawString( g_Sprites, hlString.c_str(), XMFLOAT2( 85, screenHeight - 50 ), Colors::Yellow, 0.0f, XMFLOAT2( 0, 0 ), 0.5f, SpriteEffects(), 0.0f);

		unitStorage->drawUnits(g_Sprites, time);
		g_Sprites->End();
	}

	

	const int ascii_P = 80;
	if(menu.playPressed && (keyInput->IsKeyDown(ascii_P) || (hud->paused == true && !menu.isGameOver))) {
		hud->paused = true;

		float width = 2.5f;
		float height = 2;

		float x = (screenWidth - width / 2) - screenWidth;
		float y = (screenHeight - height / 2) / screenHeight;
		XMVECTOR color = Colors::DarkSlateGray;

		XMFLOAT2 pos1 = XMFLOAT2(x, y);                  // top left
		XMFLOAT2 pos2 = XMFLOAT2(x + width, y);          // top right
		XMFLOAT2 pos3 = XMFLOAT2(x + width, y - height); // bottom right
		XMFLOAT2 pos4 = XMFLOAT2(x, y - height);         // bottom left

		VertexPositionColor v1 = VertexPositionColor(XMLoadFloat2(&pos1), color);
		VertexPositionColor v2 = VertexPositionColor(XMLoadFloat2(&pos2), color);
		VertexPositionColor v3 = VertexPositionColor(XMLoadFloat2(&pos3), color);
		VertexPositionColor v4 = VertexPositionColor(XMLoadFloat2(&pos4), color);

		g_BatchEffect->Apply( g_pImmediateContext );
		g_pImmediateContext->IASetInputLayout( g_pBatchInputLayout );
		g_Batch->Begin();

		g_Batch->DrawQuad(v1, v2, v3, v4);

		g_Batch->End();

		const int ascii_R = 82;
		const int ascii_Q = 81;
		if(menu.pmResumePressed) {
			hud->paused = false;
			menu.pmResumePressed = false;
		}
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		ScreenToClient(g_hWnd, &cursorPos);
		menu.DrawMiniMenu(XMFLOAT2(cursorPos.x, cursorPos.y), leftMousePressed, g_pImmediateContext, g_pd3dDevice);
		if(menu.pmQuitPressed) {
			hud->saveGame(levelSystem.getLevel());
			exit(1);
		}
	}

	if(thePlayer->getVitals()->getHealth() <= 0 || menu.isGameOver) {
		menu.isGameOver = true;
		hud->paused = true;

		float width = 2.5f;
		float height = 1.6f;

		float x = (screenWidth - width / 2) - screenWidth;
		float y = (screenHeight - height / 2) / screenHeight;
		XMVECTOR color = Colors::DarkSlateGray;

		XMFLOAT2 pos1 = XMFLOAT2(x, y);                  // top left
		XMFLOAT2 pos2 = XMFLOAT2(x + width, y);          // top right
		XMFLOAT2 pos3 = XMFLOAT2(x + width, y - height); // bottom right
		XMFLOAT2 pos4 = XMFLOAT2(x, y - height);         // bottom left

		VertexPositionColor v1 = VertexPositionColor(XMLoadFloat2(&pos1), color);
		VertexPositionColor v2 = VertexPositionColor(XMLoadFloat2(&pos2), color);
		VertexPositionColor v3 = VertexPositionColor(XMLoadFloat2(&pos3), color);
		VertexPositionColor v4 = VertexPositionColor(XMLoadFloat2(&pos4), color);

		g_BatchEffect->Apply( g_pImmediateContext );
		g_pImmediateContext->IASetInputLayout( g_pBatchInputLayout );
		g_Batch->Begin();

		g_Batch->DrawQuad(v1, v2, v3, v4);

		g_Batch->End();

		POINT cursorPos;
		GetCursorPos(&cursorPos);
		ScreenToClient(g_hWnd, &cursorPos);

		menu.DrawGameOver(XMFLOAT2(cursorPos.x, cursorPos.y), leftMousePressed, g_pImmediateContext, g_pd3dDevice);
		if(menu.pmQuitPressed) {
			hud->saveGame(levelSystem.getLevel());
			exit(1);
		}
		if(!menu.playPressed) {
			hud->saveGame(levelSystem.getLevel());
			hud->paused = false;
			resetEverything();
		}
	}

	if(!menu.playPressed) {
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		ScreenToClient(g_hWnd, &cursorPos);

		menu.DrawMenu(XMFLOAT2(cursorPos.x, cursorPos.y), leftMousePressed, g_pImmediateContext, g_pd3dDevice);
	}
	
    // Present our back buffer to our front buffer
    //
    g_pSwapChain->Present( 0, 0 );
}

void HandleAllMovement()
{
	if(hud->paused == false && menu.playPressed) {
		long int time = clock();
		unitStorage->updateUnits(time);

		POINT cursorPos;
		GetCursorPos(&cursorPos);
		ScreenToClient(g_hWnd, &cursorPos);

		long xLong = cursorPos.x;
		long yLong = cursorPos.y;

		// Update our Sprite
		thePlayer->move(keyInput, (screenWidth / 2) - water->getWidth() / 2, screenHeight);
		thePlayer->handleCasting(leftMousePressed, rightMousePressed, XMFLOAT2(xLong, yLong));
		thePlayer->updateCasting(XMFLOAT2(screenWidth, screenHeight));

		for(int i = 0; i < enemies.size(); i++) {
			enemies[i]->handleCasting(thePlayer->getPos());
			enemies[i]->updateCasting(XMFLOAT2(screenWidth, screenHeight));
			if(enemies[i]->getVitals()->getHealth() <= 0 && enemies[i]->getProjectileList()->size() <= 0) {
				enemies.erase(enemies.begin() + i);
			}
		}
	}
}

void HandleAllCollisions()
{
	// check player projectile collisions against other enemy collision boxes
	std::vector<Projectile*>* projList = thePlayer->getProjectileList();
	for(int i = 0; i < enemies.size(); i++) {
		XMFLOAT2 posBox1 = enemies[i]->getPos();
		int colWidth = enemies[i]->getCollisionBox()->getWidth();
		int colHeight = enemies[i]->getCollisionBox()->getHeight();

		for(int k = 0; k < projList->size(); k++) {
			if(projList->at(k)->getXPosition() > screenWidth / 2) {
				XMFLOAT2 posBox2 = projList->at(k)->getCollisionBox()->getPos();
				int projWidth = projList->at(k)->getCollisionBox()->getWidth();
				int projHeight = projList->at(k)->getCollisionBox()->getHeight();
				if( !(posBox1.x + colWidth <= posBox2.x ||
					posBox1.x + colWidth >= posBox2.x + projWidth ||
					posBox1.y + colHeight <= posBox2.y ||
					posBox1.y + colHeight >= posBox2.y + projHeight)) {
						handleHitSounds();
						if((*projList)[k]->getID() < 0) {
							thePlayer->manaBoost();
						}
						enemies[i]->handleDamage((*projList)[k]->getDamage());
						hud->handleScore((*projList)[k]->getID());
						if(enemies[i]->getVitals()->getHealth() <= 0) {
							hud->changeScore(hud->pointsForKill);
						}
						projList->erase(projList->begin() + k);
						k--;
				}
			}
		}
	}

	// check enemy projectile collisions against the player
	
	for(int i = 0; i < enemies.size(); i++) {
		projList = enemies[i]->getProjectileList();
		for(int k = 0; k < projList->size(); k++) {
			XMFLOAT2 posBox1 = (*projList)[k]->getPos();
			int projWidth = (*projList)[k]->getWidth();
			int projHeight = (*projList)[k]->getHeight();

			if(projList->at(k)->getXPosition() < screenWidth / 2) {
				XMFLOAT2 posBox2 = thePlayer->getCollisionBox()->getPos();
				int playerWidth = thePlayer->getCollisionBox()->getWidth();
				int playerHeight = thePlayer->getCollisionBox()->getHeight();
				if( !(posBox1.x + projWidth <= posBox2.x ||
					posBox1.x + projWidth >= posBox2.x + playerWidth ||
					posBox1.y + projHeight <= posBox2.y ||
					posBox1.y + projHeight >= posBox2.y + playerHeight)) {
						handleHitSounds();
						if((*projList)[k]->getID() < 0) {
							enemies[i]->manaBoost();
						}
						thePlayer->handleDamage((*projList)[k]->getDamage());
						projList->erase(projList->begin() + k);
						k--;
				}
			}
		}
	}
}

void HandleMusic() {
	if((keyInput->IsKeyDown(VK_RETURN) && !menu.playPressed) || (menu.playPressed && menu.stopIntroMusic) || (music != 0 && menu.stopIntroMusic)) { // player has chosen to start the game, stop the current music so the gameplay music can be played
		menu.stopIntroMusic = false;
		Mix_FadeOutMusic(500);
		Mix_FreeMusic(music);
		music = 0;
		//SDL_Delay(100);
	}

	if(music == 0) { // assign music pointer a gameplay track
		if(!menu.playPressed) {
			menu.stopIntroMusic = false;
			music = Mix_LoadMUS("./Sound/Tracks/Start.wav");
			Mix_VolumeMusic(16);
			Mix_FadeInMusic(music, -1, 3000);
		} else {
			music = Mix_LoadMUS("./Sound/Tracks/Song05.wav");
			Mix_VolumeMusic(16);
			Mix_FadeInMusic(music, -1, 3000);
		}
	}

	if( Mix_PlayingMusic() && menu.musicMuted ) {
		menu.musicMuted = true;
		musicStopped = true;
		Mix_FadeOutMusic(2000);
		//SDL_Delay(1);
	}
	if( !Mix_PlayingMusic() && !menu.musicMuted ) {
		menu.musicMuted = false;
		musicStopped = false;
		Mix_FadeInMusic(music, -1, 2000);
		//SDL_Delay(1);
	}

	if(!menu.soundMuted) {
		if(thePlayer->isMoving && walkingChannel < 0) {
			int floor = 0;
			int ceiling = unit_Walking_Sounds.size();
			int range = ceiling - floor;
			int rnd = floor + int((range * rand()) / (RAND_MAX + 1.0f));
			walkingChannel = Mix_PlayChannel(-1, unit_Walking_Sounds.at(rnd), 0);
		} else if(walkingChannel >= 0 && !Mix_Playing(walkingChannel)) {
			Mix_HaltChannel(walkingChannel);
			walkingChannel = -1;
		}

		if(thePlayer->fired) {
			if(rightMousePressed) {
				int floor = 0;
				int ceiling = proj_Sounds[thePlayer->currentElement + 1].size();
				int range = ceiling - floor;
				int rnd = floor + int((range * rand()) / (RAND_MAX + 1.0f));
				Mix_PlayChannelTimed(-1, proj_Sounds[thePlayer->currentElement + 1].at(rnd), 0, 500);
			} else {
				int floor = 0;
				int ceiling = proj_Sounds[0].size();
				int range = ceiling - floor;
				int rnd = floor + int((range * rand()) / (RAND_MAX + 1.0f));
				Mix_PlayChannelTimed(-1, proj_Sounds[0].at(rnd), 0, 500);
			}
		}
	} else {
		Mix_HaltChannel(walkingChannel);
		walkingChannel = -1;
	}
}

void resetEverything()
{
	hud->setScore(0);
	levelSystem.resetLevel();
	unitStorage->unitList.clear();
	enemies.clear();
	timeTracker += timePlaying + timeNotPlaying;
	timeNPTracker += timeNotPlaying;

	thePlayer = new Player(unit_Hat, unit_Head, unit_Body, unit_Foot, unit_Hand, unit_Staff, &proj_Sprites);
	thePlayer->setPos(50, screenHeight / 2 - thePlayer->getCollisionBox()->getHeight() / 2);
	unitStorage->addUnit(thePlayer);
}

void handleHitSounds()
{
	if(!menu.soundMuted) {
		int floor = 0;
		int ceiling = hit_Sounds.size();
		int range = ceiling - floor;
		int rnd = floor + int((range * rand()) / (RAND_MAX + 1.0f));
		Mix_PlayChannel(-1, hit_Sounds[rnd], 0);
	}
}

void HandleTime()
{
	long int time = clock();
	if(!timePlayingHandled) {
		timeNotPlaying = time - timePlaying - timeNPTracker;
	} else {
		timePlaying = time - timeNotPlaying - timeTracker;
	}
	timePlayingHandled = false;

	if(menu.playPressed) {
		timePlayingHandled = true;
	} else {
		timePlayingHandled = false;
	}
}

void HandleEnemySpawning()
{
	if(menu.playPressed && !hud->paused) {
		levelSystem.handleLevels(timePlaying);
		if(enemies.size() <= 0 || levelSystem.spawnReady) {
			float yFloor = 0;
			float yCeiling = screenHeight;
			float yRange = yCeiling - yFloor;
			int yRND = yFloor + ((yRange * rand()) / (RAND_MAX + 1.0f));

			Enemy* enem = new Enemy(enemy_Head, enemy_Body, enemy_Foot, unit_Hand, enemy_Staff, &proj_Sprites);
			enem->initLocation(screenWidth + 50, yRND);
			unitStorage->addUnit(enem);
			enemies.push_back(enem);
		}
	}
}
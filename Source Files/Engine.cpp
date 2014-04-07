////////////////////////////////////////////////////////////////////////
// File: Engine.cpp			Class: Engine
// 
// Description:
// Handling of the main DirectX rendering members, and provides methods
// for initiation and rendering to the screen.
//
////////////////////////////////////////////////////////////////////////

#include "Engine.h"

// -- Object Management --

Engine* Engine::pInstance = NULL;

Engine * Engine::Instance()
{
  if(pInstance == NULL){
    pInstance = new Engine();
  }
  return pInstance;
}

Engine::Engine()
{
	lpD3D = NULL;
	lpDevice = NULL;
	m_Camera = Camera();
	m_Inputs = Inputs();
	m_state = 1;
	fFOV = 4;
}

Engine::~Engine()
{
}

// -- Get / Set Methods --

LPDIRECT3DDEVICE9* Engine::GetDevice()
{
	return &lpDevice;
}

void Engine::SetDevice(LPDIRECT3DDEVICE9 device)
{
	lpDevice = device;
}

LPD3DXEFFECT Engine::GetEffect()
{
	return lpEffect;
}

void Engine::SetEffect(LPD3DXEFFECT effect)
{
	lpEffect = effect;
}

Inputs* Engine::GetInput()
{
	return &m_Inputs;
}

void Engine::SetInput(Inputs input)
{
	m_Inputs = input;
}

Camera Engine::GetCamera()
{
	return m_Camera;
}

void Engine::SetCamera(Camera camera)
{
	m_Camera = camera;
}

int Engine::GetState()
{
	return m_state;
}

void Engine::SetState(int state)
{
	m_state = state;
}

// -- Methods --

HRESULT Engine::InitD3D( HWND hWnd )
{
    // Create the D3D object.
    if( NULL == ( lpD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice. Since we are now
    // using more complex geometry, we will create a device with a zbuffer.
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    // Create the D3DDevice
    if( FAILED( lpD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &lpDevice ) ) )
    {
        return E_FAIL;
    }

	if( FAILED( D3DXCreateEffectFromFile( lpDevice,"..\\PlanetWars\\Shaders.fx", NULL, NULL, D3DXSHADER_DEBUG | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, NULL, &lpEffect, NULL ) ) )
	{
		MessageBox(NULL, "Failed to create the effect from file", "Effect Creation Error", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	D3DXHANDLE hTechnique = lpEffect->GetTechniqueByName( "ShadingTexture" );
	lpEffect->SetTechnique( hTechnique );

    // Turn on the zbuffer
    lpDevice->SetRenderState( D3DRS_ZENABLE,	TRUE );
	lpDevice->SetRenderState( D3DRS_CULLMODE,	D3DCULL_NONE );

    // Turn on ambient lighting 
    lpDevice->SetRenderState( D3DRS_AMBIENT, 0xffffffff );

	lpDevice->SetRenderState( D3DRS_LIGHTING,	true);
	
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 2.0f;
	light.Diffuse.g = 2.0f;
	light.Diffuse.b = 2.0f;
	light.Diffuse.a = 2.0f;
	light.Ambient.r = 2.0f;
	light.Ambient.g = 2.0f;
	light.Ambient.b = 2.0f;
	light.Ambient.a = 2.0f;
	light.Specular.r = 2.0f;
	light.Specular.g = 2.0f;
	light.Specular.b = 2.0f;
	light.Specular.a = 2.0f;
	light.Direction.x = 0.0f;
	light.Direction.y = -1.0f;
	light.Direction.z = -1.0f;

	lpDevice->SetLight(0, &light);
	lpDevice->LightEnable(0, TRUE);

	//maximum ambient light (NEW)
	lpDevice->SetRenderState(D3DRS_AMBIENT,RGB(255,255,255));
	//lighting disabled (NEW)
	lpDevice->SetRenderState(D3DRS_LIGHTING,false);
	//don't cull backside (NEW)
	lpDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	//enable depth buffering (NEW)
	lpDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);

	lpDevice->SetRenderState(D3DRS_POINTSPRITEENABLE,true);
	lpDevice->SetRenderState(D3DRS_POINTSCALEENABLE,true);
	lpDevice->SetRenderState(D3DRS_POINTSIZE, ((DWORD)1.0f));
	lpDevice->SetRenderState(D3DRS_POINTSCALE_B, ((DWORD)1.0f));
	lpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	lpDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	lpDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	Menu::Instance()->Init();

    return S_OK;
}

VOID Engine::Cleanup()
{
    if( lpDevice != NULL )
        lpDevice->Release();

    if( lpD3D != NULL )
        lpD3D->Release();
}

VOID Engine::SetupMatrices(Model * model) { SetupMatrices(&(model->GetPosition()), &(model->GetRotation()), &(model->GetScaling())); }
VOID Engine::SetupMatrices(Quad * Quad) { SetupMatrices(&(Quad->GetPosition()), &(Quad->GetRotation()), &(Quad->GetScaling())); }
VOID Engine::SetupMatrices(Planet * planet) { SetupMatrices(&(planet->GetPosition()), &(planet->GetRotation()), &(planet->GetScaling())); }
VOID Engine::SetupMatrices(D3DXVECTOR3* g_Position, D3DXVECTOR3* g_Rotation, D3DXVECTOR3* g_Scaling)
{
    // Set up world matrix
    D3DXMATRIXA16 matPos, matRot, matSca, matWorld;
	D3DXMATRIXA16 mWorldViewProjection;
	D3DXMatrixTranslation( &matPos, g_Position->x, g_Position->y, g_Position->z );
	D3DXMatrixRotationYawPitchRoll( &matRot, g_Rotation->y, g_Rotation->x, g_Rotation->z );
	D3DXMatrixScaling( &matSca, g_Scaling->x, g_Scaling->y, g_Scaling->z );
	D3DXMatrixIdentity( &matWorld );
	matWorld = matSca * matRot * matPos;
    lpEffect->SetMatrix( "wMat", &matWorld );
    lpDevice->SetTransform( D3DTS_WORLD, &matWorld );

    D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &m_Camera.GetEyePt(), &m_Camera.GetLookAt(), &m_Camera.GetUpVec() );
    lpEffect->SetMatrix( "vMat", &matView );
    lpDevice->SetTransform( D3DTS_VIEW, &matView );

    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / fFOV, 1.0f, 1.0f, 100.0f );
    lpEffect->SetMatrix( "pMat", &matProj );
    lpDevice->SetTransform( D3DTS_PROJECTION, &matProj );

    mWorldViewProjection = matWorld * matView * matProj;
    lpEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection );
    lpEffect->SetMatrix( "g_mWorld", &matWorld );


	//D3DXCreateTextureFromFile(g_pd3dDevice, "..\\PlanetWars\\Earth_Atmos.dds", &m_pAtmosTexture);
	//m_pEffect->SetTexture( "atmosMapTexture", m_pAtmosTexture);
	//g_pd3dDevice->SetTexture( 2, m_pAtmosTexture );

	//matWorldInverseTransponse
	/*D3DXMATRIXA16 matWorldInverseTransponse;
	D3DXMatrixInverse(&matWorldInverseTransponse, NULL, &matWorld);
	D3DXMatrixTranspose(&matWorldInverseTransponse, &matWorldInverseTransponse);
    m_pEffect->SetMatrix( "WorldInverseTranspose", &matWorldInverseTransponse );*/
}

VOID Engine::Update()
{
	switch(m_state)
	{
		case 1:
			{
				Menu::Instance()->Update();
			}
			break;
		case 2:
			{
				LevelSelect::Instance()->Update();
			}
			break;
		case 3:
			{
				MainGame::Instance()->Update();
			}
			break;
	}
}

VOID Engine::Render()
{
    // Clear the backbuffer and the zbuffer
    lpDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

    // Begin the scene
    lpDevice->BeginScene();
	
	lpEffect->SetValue( "g_LightDir", D3DXVECTOR3( 0.0f,0.0,0.2f ), sizeof( D3DXVECTOR3 ) );
    lpEffect->SetValue( "g_LightDiffuse", &D3DXCOLOR( 0.9f, 0.9f, 0.9f, 1 ), sizeof( D3DXVECTOR4 ) );
    lpEffect->SetValue( "g_MaterialDiffuseColor", &D3DXCOLOR( 1, 1, 1, 1 ), sizeof( D3DXCOLOR ) );
    lpEffect->SetValue( "g_LightAmbient", &D3DXCOLOR( 0.5f, 0.5f, 0.5f, 1 ), sizeof( D3DXVECTOR4 ) );

	lpEffect->SetFloat( "transparency", 1.0f );
	lpEffect->SetVector( "vLightPos", &D3DXVECTOR4(0,0,-10,1) );
    lpEffect->SetVector( "vEyePos", &D3DXVECTOR4(m_Camera.GetEyePt().x,m_Camera.GetEyePt().y,m_Camera.GetEyePt().z,1) );
	
	switch(m_state)
	{
		case STATE_MAINMENU:
			{
				Menu::Instance()->Render();
			}
			break;
		case STATE_LEVELSELECT:
			{
				LevelSelect::Instance()->Render();
			}
			break;
		case STATE_MAINGAME:
			{
				MainGame::Instance()->Render(true);
			}
			break;
	}

    // Present the backbuffer contents to the display
    lpDevice->EndScene(); // End the scene
    lpDevice->Present( NULL, NULL, NULL, NULL );
}
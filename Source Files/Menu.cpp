////////////////////////////////////////////////////////////////////////
// File: Menu.cpp			Class: Menu
// 
// Description:
// A simple introduction screen which is first presented to the player.
//
////////////////////////////////////////////////////////////////////////

#include "Menu.h"

// -- Object Management --

Menu * Menu::pInstance = NULL;

Menu * Menu::Instance()
{
  if(pInstance == NULL){
    pInstance = new Menu();
  }
  return pInstance;
}

Menu::Menu()
{
	m_planet = Planet();
	m_BG = Quad();

	bFirstTimeRound = true;
}

Menu::~Menu()
{
}

// -- Methods --

VOID Menu::Init()
{
	m_planet = Planet();
	m_BG = Quad();

	m_planet.Initialise(*(Engine::Instance()->GetDevice()), 12);
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\planetTextures\\planet1.bmp", m_planet.GetTexture());
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\planetTextures\\planet1BM.bmp", m_planet.GetBumpMap());
	m_planet.SetPosition( D3DXVECTOR3(12.0f, -10.0f, 0.0f) );

	m_BG.CreateQuad(*(Engine::Instance()->GetDevice()),
				D3DXVECTOR3(-21,21,10),
				D3DXVECTOR3(21,21,10),
				D3DXVECTOR3(-21,-21,10),
				D3DXVECTOR3(21,-21,10));
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\plasma2.jpg", m_BG.GetTexture());

	m_titleBanner.CreateQuad(*(Engine::Instance()->GetDevice()),
				D3DXVECTOR3(-10.4f,-3,-15),
				D3DXVECTOR3(10.4f,-3,-15),
				D3DXVECTOR3(-10.4f,-6,-15),
				D3DXVECTOR3(10.4f,-6,-15));
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\titleBanner.png", m_titleBanner.GetTexture());

	m_insBanner.CreateQuad(*(Engine::Instance()->GetDevice()),
				D3DXVECTOR3(5.2f,-9.55,-15),
				D3DXVECTOR3(10.4f,-9.55,-15),
				D3DXVECTOR3(5.2f,-10.4,-15),
				D3DXVECTOR3(10.4f,-10.4,-15));
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\ins_Main.png", m_insBanner.GetTexture());
	
}

VOID Menu::Update()
{
	if (bFirstTimeRound)
	{
		LevelSelect::Instance()->Init();
		Init();
		bFirstTimeRound = false;
	}

	m_planet.SetRotation( m_planet.GetRotation() + D3DXVECTOR3(0,-0.001f,-0.001f) );
	
	if (Engine::Instance()->GetInput()->returnDown)
	{
		Engine::Instance()->SetState(Engine::STATE_LEVELSELECT);
	}
}

VOID Menu::Render()
{
	Engine::Instance()->GetEffect()->SetTechnique( Engine::Instance()->GetEffect()->GetTechniqueByName( "BasicTexture" ) );

	Engine::Instance()->SetupMatrices(&m_BG);
	m_BG.Render(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());

	Engine::Instance()->GetEffect()->SetTechnique( Engine::Instance()->GetEffect()->GetTechniqueByName( "ShadingTexture" ) );

	Engine::Instance()->SetupMatrices(&m_planet);
	m_planet.RenderWithEffect(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());

	
	Engine::Instance()->GetEffect()->SetTechnique( Engine::Instance()->GetEffect()->GetTechniqueByName( "BasicTexture" ) );

	Engine::Instance()->SetupMatrices(&m_titleBanner);
	m_titleBanner.Render(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());

	Engine::Instance()->SetupMatrices(&m_insBanner);
	m_insBanner.Render(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());
	
	Engine::Instance()->GetEffect()->SetTechnique( Engine::Instance()->GetEffect()->GetTechniqueByName( "ShadingTexture" ) );
}
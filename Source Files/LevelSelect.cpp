////////////////////////////////////////////////////////////////////////
// File: LevelSelect.cpp			Class: LevelSelect
// 
// Description:
// Provides an interface for the player to be able to select a level to
// play.
//
////////////////////////////////////////////////////////////////////////

#include "LevelSelect.h"

// -- Object Management --

LevelSelect * LevelSelect::pInstance = NULL;

LevelSelect * LevelSelect::Instance()
{
  if(pInstance == NULL){
    pInstance = new LevelSelect();
  }
  return pInstance;
}

LevelSelect::LevelSelect()
{
	m_BG = Quad();
	lPanels = std::list<Quad>();
	iSelection = 1;

	bLeftDown = true;
	bRightDown = true;
	bEnterDown = true;
	fTransition = 0;
	bMovingLeft = false;
	bMovingRight = false;
	MainGame::Instance()->Init();
}

LevelSelect::~LevelSelect()
{
}

// -- Methods --

VOID LevelSelect::Init()
{
	lPanels = std::list<Quad>();
	lPanelPics = std::list<Quad>();

	for (int i = 0; i < 15; i++)
	{
		Quad* panel = new Quad();

		char imagePath[255];
		if (MainGame::Instance()->GetLevelPassed(i) == false)
			sprintf(imagePath, "..\\PlanetWars\\levelPanels\\Level%i.png", i+1);
		else
			sprintf(imagePath, "..\\PlanetWars\\levelPanels\\Level%iW.png", i+1);

		D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), imagePath, panel->GetTexture());
		
		lPanels.push_back(*panel);

		Quad* panelPic = new Quad();

		MainGame::Instance()->GoToLevel(i+1);
		MainGame::Instance()->GetLevelTexture();

		panelPic->SetTexture(*(MainGame::Instance()->GetRenderTexture()));
		
		lPanelPics.push_back(*panelPic);
		
		fTransition = 0;
		bMovingLeft = false;
		bMovingRight = false;

		MovePanels();
	}


	m_BG.CreateQuad(*(Engine::Instance()->GetDevice()),
				D3DXVECTOR3(-21,21,10),
				D3DXVECTOR3(21,21,10),
				D3DXVECTOR3(-21,-21,10),
				D3DXVECTOR3(21,-21,10));
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\plasma2.jpg", m_BG.GetTexture());

	m_titleBanner.CreateQuad(*(Engine::Instance()->GetDevice()),
				D3DXVECTOR3(-10.4f,9.5,-15),
				D3DXVECTOR3(10.4f,9.5,-15),
				D3DXVECTOR3(-10.4f,6.5,-15),
				D3DXVECTOR3(10.4f,6.5,-15));
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\levelBanner.png", m_titleBanner.GetTexture());

	m_insBanner.CreateQuad(*(Engine::Instance()->GetDevice()),
				D3DXVECTOR3(0.0f,-9.55,-15),
				D3DXVECTOR3(10.4f,-9.55,-15),
				D3DXVECTOR3(0.0f,-10.4,-15),
				D3DXVECTOR3(10.4f,-10.4,-15));
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\ins_Level.png", m_insBanner.GetTexture());
}

VOID LevelSelect::Update()
{
	if (Engine::Instance()->GetInput()->returnDown)
	{
		if (bEnterDown == false)
		{
			MainGame::Instance()->GoToLevel(iSelection);
			Engine::Instance()->SetState(Engine::STATE_MAINGAME);
		}
	}
	else
		bEnterDown = false;

	if (Engine::Instance()->GetInput()->leftArrowDown)
	{
		if (!bMovingLeft)
		{
			if (iSelection > 1)
			{
				iSelection -= 1;
				fTransition = 0.0f;
				bMovingLeft = true;
			}
		}
	}
	
	if (Engine::Instance()->GetInput()->rightArrowDown)
	{
		if (!bMovingRight)
		{
			if (iSelection < 15)
			{
				iSelection += 1;
				fTransition = 1.0f;
				bMovingRight = true;
			}
		}
	}

	if (bMovingRight)
	{
		fTransition-=0.05f;
		MovePanels();
		
		if (fTransition <= 0)
			bMovingRight = false;
	}

	if (bMovingLeft)
	{
		fTransition+=0.05f;
		MovePanels();
		
		if (fTransition >= 1)
			bMovingLeft = false;
	}
}

VOID LevelSelect::Render()
{
	Engine::Instance()->GetEffect()->SetTechnique( Engine::Instance()->GetEffect()->GetTechniqueByName( "BasicTexture" ) );

	Engine::Instance()->SetupMatrices(&m_BG);
	m_BG.Render(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());

	Engine::Instance()->SetupMatrices(&m_titleBanner);
	m_titleBanner.Render(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());

	Engine::Instance()->SetupMatrices(&m_insBanner);
	m_insBanner.Render(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());

	std::list<Quad>::iterator i;
	for(i=lPanels.begin(); i != lPanels.end(); ++i)
	{
		Quad* panel = &(*i);
		
		Engine::Instance()->SetupMatrices(panel);
		panel->Render(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());
	}
	for(i=lPanelPics.begin(); i != lPanelPics.end(); ++i)
	{
		Quad* panelPic = &(*i);
		
		Engine::Instance()->SetupMatrices(panelPic);
		panelPic->Render(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());
	}

	Engine::Instance()->GetEffect()->SetTechnique( Engine::Instance()->GetEffect()->GetTechniqueByName( "ShadingTexture" ) );
}

VOID LevelSelect::MovePanels()
{

	int curPanel = 1;
	std::list<Quad>::iterator i;
	for(i=lPanels.begin(); i != lPanels.end(); ++i)
	{
		Quad* panel = &(*i);

		float left = 0;
		float right = 0;
		float top = 0;
		float bottom = 0;

		int sP = iSelection;
		if (bMovingLeft)
			sP = iSelection + 1;

		if (curPanel == sP-2)
		{
			left = (-27.4f + (fTransition*11.0f));
			right = (-17.4f + (fTransition*11.0f));
			top = (5.0f + (fTransition*(0.0f)));
			bottom = (-5.0f + (fTransition*(0.0f)));
		}
		else if (curPanel == sP-1)
		{
			left = (-16.4f + (fTransition*11.0f));
			right = (-6.4f + (fTransition*11.0f));
			top = (5.0f + (fTransition*1.0f));
			bottom = (-5.0f + (fTransition*-1.0f));
		}
		else if (curPanel == sP)
		{
			left = (-5.4f + (fTransition*11.0f));
			right = (5.4f + (fTransition*11.0f));
			top = (6.0f + (fTransition*-1.0f));
			bottom = (-6.0f + (fTransition*1.0f));
		}
		else if (curPanel == sP+1)
		{
			left = (6.4f + (fTransition*11.0f));
			right = (16.4f + (fTransition*11.0f));
			top = (5.0f + (fTransition*(1.0f)));
			bottom = (-5.0f + (fTransition*(-1.0f)));
		}
		
		panel->CreateQuad(*(Engine::Instance()->GetDevice()),
			D3DXVECTOR3(left,top,-15),
			D3DXVECTOR3(right,top,-15),
			D3DXVECTOR3(left,bottom,-15),
			D3DXVECTOR3(right,bottom,-15));

		curPanel++;
	}

	curPanel = 1;

	for(i=lPanelPics.begin(); i != lPanelPics.end(); ++i)
	{
		Quad* panelPic = &(*i);

		float left = 0;
		float right = 0;
		float top = 0;
		float bottom = 0;

		int sP = iSelection;
		if (bMovingLeft)
			sP = iSelection + 1;

		if (curPanel == sP-2)
		{
			left = (-27.2f + (fTransition*11.0f));
			right = (-17.6f + (fTransition*11.0f));
			top = (4.9f + (fTransition*(0.0f)));
			bottom = (-3.5f + (fTransition*(0.0f)));
		}
		else if (curPanel == sP-1)
		{
			left = (-16.2f + (fTransition*11.0f));
			right = (-6.6f + (fTransition*11.0f));
			top = (4.9f + (fTransition*0.9f));
			bottom = (-3.5f + (fTransition*-0.6f));
		}
		else if (curPanel == sP)
		{
			left = (-5.2f + (fTransition*11.0f));
			right = (5.2f + (fTransition*11.0f));
			top = (5.8f + (fTransition*-0.9f));
			bottom = (-4.1f + (fTransition*0.6f));
		}
		else if (curPanel == sP+1)
		{
			left = (6.6f + (fTransition*11.0f));
			right = (16.2f + (fTransition*11.0f));
			top = (4.9f + (fTransition*(0.9f)));
			bottom = (-3.5f + (fTransition*(-0.6f)));
		}
		
		panelPic->CreateQuad(*(Engine::Instance()->GetDevice()),
			D3DXVECTOR3(left,top,-15),
			D3DXVECTOR3(right,top,-15),
			D3DXVECTOR3(left,bottom,-15),
			D3DXVECTOR3(right,bottom,-15));

		curPanel++;
	}
}
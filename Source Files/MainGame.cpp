////////////////////////////////////////////////////////////////////////
// File: MainGame.cpp			Class: MainGame
// 
// Description:
// The core logic of the game - this class contains the main logic for a
// level.
//
////////////////////////////////////////////////////////////////////////

#include "MainGame.h"

// -- Object Management --

MainGame* MainGame::pInstance = NULL;

MainGame * MainGame::Instance()
{
  if(pInstance == NULL){
    pInstance = new MainGame();
  }
  return pInstance;
}

MainGame::MainGame()
{
	m_ship = Model();
	m_enemy = Model();
	m_rocket = Model();
	m_background = Quad();
	bHasFired = false;
	fShotPower = 0.01f;
	m_curLevel = Level();
	m_powerBar = Quad();
	m_particles = ParticleSystem();
	iCurLevelID = 0;
	m_screenBounds = Rect();
	m_offScreen = Quad();
	for (int i = 0; i < 15; i++)
		bLevelPassed[i] = false;
	iAttempts = 0;
}

MainGame::~MainGame()
{
}

// -- Get / Set Methods --

bool MainGame::GetLevelPassed(int level)
{
	if (level >= 20)
		return false;

	return bLevelPassed[level];
}

void MainGame::SetLevelPassed(int level, bool passed)
{
	if (level < 20)
		bLevelPassed[level] = passed;
}

LPDIRECT3DTEXTURE9* MainGame::GetRenderTexture()
{
	return &lpRenderTexture;
}

void MainGame::SetRenderTexture(LPDIRECT3DTEXTURE9 renderTexture)
{
	lpRenderTexture = renderTexture;
}

// -- Methods --

VOID MainGame::Init()
{
	m_ship.Initialise(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\player.x");
	m_ship.SetScaling(D3DXVECTOR3( 0.3f, 0.3f, 0.3f ));
	m_ship.SetRadius(0.6f);
	
	m_enemy.Initialise(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\enemy.x");
	m_enemy.SetScaling(D3DXVECTOR3( 0.3f, 0.3f, 0.3f ));
	m_enemy.SetRadius(0.7f);

	m_curLevel.Initialise(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\levels\\level1.txt");
	m_ship.SetPosition(m_curLevel.GetShipPosition());
	m_ship.SetRotation(m_curLevel.GetShipRotation());
	m_enemy.SetPosition(m_curLevel.GetEnemyPosition());
	m_enemy.SetRotation(m_curLevel.GetEnemyRotation());

	m_background.CreateQuad(*(Engine::Instance()->GetDevice()),
				D3DXVECTOR3(-21,21,10),
				D3DXVECTOR3(21,21,10),
				D3DXVECTOR3(-21,-21,10),
				D3DXVECTOR3(21,-21,10));
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\plasma.jpg", m_background.GetTexture());

	m_powerBack.CreateQuad(*(Engine::Instance()->GetDevice()),
				D3DXVECTOR3(-10.45,-9.3,-15),
				D3DXVECTOR3(-3,-9.3,-15),
				D3DXVECTOR3(-10.45,-10.4,-15),
				D3DXVECTOR3(-3,-10.4,-15));
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\pow.png", m_powerBack.GetTexture());

	m_attemptBack.CreateQuad(*(Engine::Instance()->GetDevice()),
				D3DXVECTOR3(3,-9.3,-15),
				D3DXVECTOR3(10.4,-9.3,-15),
				D3DXVECTOR3(3,-10.4,-15),
				D3DXVECTOR3(10.4,-10.4,-15));
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\att.png", m_attemptBack.GetTexture());

	m_powerBar.CreateQuad(*(Engine::Instance()->GetDevice()),
				D3DXVECTOR3(-8,-9.7,-15),
				D3DXVECTOR3(-6,-9.7,-15),
				D3DXVECTOR3(-8,-10.0,-15),
				D3DXVECTOR3(-6,-10.0,-15));
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\power.png", m_powerBar.GetTexture());

	m_powerBarFULL.CreateQuad(*(Engine::Instance()->GetDevice()),
				D3DXVECTOR3(-8,-9.7,-15),
				D3DXVECTOR3(-5,-9.7,-15),
				D3DXVECTOR3(-8,-10.0,-15),
				D3DXVECTOR3(-5,-10.0,-15));
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\powerFULL.png", m_powerBarFULL.GetTexture());

	fPowerBarMaxWidth = 3.0f;

	m_rocket.Initialise(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\bullet.x");
	m_rocket.SetPosition(D3DXVECTOR3( 0, 0, 0 ));
	m_rocket.SetScaling(D3DXVECTOR3( 0.3f, 0.3f, 0.3f ));
	m_rocket.SetRadius(0.2f);
	m_rocket.SetMass(0.5f);

	
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\smoke.png", m_particles.GetTexture());

	m_screenBounds = Rect(D3DXVECTOR2( -16.6f, -16.6f ),
						D3DXVECTOR2( 16.6f, 16.6f ));
	
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\smoke.png", m_offScreen.GetTexture());

	float div = 1.0f / 12.0f;
	m_attQ1.CreateQuad(*(Engine::Instance()->GetDevice()),
				Quad::CUSTOMVERTEX(5,-12,0,div*11.0f,0),
				Quad::CUSTOMVERTEX(7,-12,0,div*12.0f,0),
				Quad::CUSTOMVERTEX(5,-15,0,div*11.0f,1),
				Quad::CUSTOMVERTEX(7,-15,0,div*12.0f,1));
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\numbers.png", m_attQ1.GetTexture());
	m_attQ2.CreateQuad(*(Engine::Instance()->GetDevice()),
				Quad::CUSTOMVERTEX(8,-12,0,div*11.0f,0),
				Quad::CUSTOMVERTEX(10,-12,0,div*12.0f,0),
				Quad::CUSTOMVERTEX(8,-15,0,div*11.0f,1),
				Quad::CUSTOMVERTEX(10,-15,0,div*12.0f,1));
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\numbers.png", m_attQ2.GetTexture());
	m_attQ3.CreateQuad(*(Engine::Instance()->GetDevice()),
				Quad::CUSTOMVERTEX(11,-12,0,div*11.0f,0),
				Quad::CUSTOMVERTEX(13,-12,0,div*12.0f,0),
				Quad::CUSTOMVERTEX(11,-15,0,div*11.0f,1),
				Quad::CUSTOMVERTEX(13,-15,0,div*12.0f,1));
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\numbers.png", m_attQ3.GetTexture());

	
	bShowInstructions = true;
	m_instructions.CreateQuad(*(Engine::Instance()->GetDevice()),
				D3DXVECTOR3(-10.4,10.4,-15),
				D3DXVECTOR3(10.4,10.4,-15),
				D3DXVECTOR3(-10.4,-10.4,-15),
				D3DXVECTOR3(10.4,-10.4,-15));
	D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\instructions.png", m_instructions.GetTexture());

}

VOID MainGame::Update()
{
	if (!bShowInstructions)
	{		
		if (Engine::Instance()->GetInput()->escapeDown)
		{
			if (bEscapeDown == false)
			{
				// quit back to level select
				Engine::Instance()->SetState(Engine::STATE_LEVELSELECT);
				return;
			}
			bEscapeDown = true;
		}
		else
			bEscapeDown = false;
	
		if (Engine::Instance()->GetInput()->leftArrowDown)
			m_ship.SetRotation( m_ship.GetRotation() + D3DXVECTOR3(0,0,0.05f) );
		if (Engine::Instance()->GetInput()->rightArrowDown)
			m_ship.SetRotation( m_ship.GetRotation() + D3DXVECTOR3(0,0,-0.05f) );
		if (Engine::Instance()->GetInput()->upArrowDown)
		{
			fShotPower += 0.005f;
			if (fShotPower > 0.3f)
				fShotPower = 0.3f;
		}
		if (Engine::Instance()->GetInput()->downArrowDown)
		{
			fShotPower -= 0.005f;
			if (fShotPower < 0.01f)
				fShotPower = 0.01f;
		}

		float percentPower = fShotPower / 0.3f;
		float curEnd = -8.0f+(fPowerBarMaxWidth * percentPower);
		m_powerBar.CreateQuad(*(Engine::Instance()->GetDevice()),
					D3DXVECTOR3(-8,-9.7,-15),
					D3DXVECTOR3(curEnd,-9.7,-15),
					D3DXVECTOR3(-8,-10.0,-15),
					D3DXVECTOR3(curEnd,-10.0,-15));
		D3DXCreateTextureFromFile(*(Engine::Instance()->GetDevice()), "..\\PlanetWars\\power.png", m_powerBar.GetTexture());


		char outputString[20];
		sprintf(outputString, "\n%f", fShotPower);
		OutputDebugString(outputString);

		if (Engine::Instance()->GetInput()->returnDown)
		{
			if (bEnterDown == false)
			{
				D3DXVECTOR3 direction = D3DXVECTOR3(1, 1, 0);
				D3DXVECTOR4 dir4 = D3DXVECTOR4();
				D3DXMATRIX matRot = D3DXMATRIX();
				D3DXMatrixRotationYawPitchRoll( &matRot, 0, 0, m_ship.GetRotation().z - 0.6f );
				D3DXVec3Transform( &dir4, &direction, &matRot );
				dir4 *= fShotPower;

				m_rocket.SetPosition(m_ship.GetPosition());
				m_rocket.SetVelocity( D3DXVECTOR3(dir4.x, dir4.y, dir4.z) );
				bHasFired = true;

				iAttempts++;
				RecalculateAttemptImages();
				
			}
			bEnterDown = true;
		}
		else
			bEnterDown = false;

		m_particles.Update();

		std::list<Planet>::iterator i;
		for(i=m_curLevel.GetPlanets()->begin(); i != m_curLevel.GetPlanets()->end(); ++i)
		{
			Planet* planet = &(*i);
			planet->SetRotation( planet->GetRotation() + planet->GetRotVel() );
		}

		if (bHasFired)
		{
			float G = -9.8f;
			D3DXVECTOR2 F = D3DXVECTOR2(0,0);

			D3DXVECTOR2 rocketPos = D3DXVECTOR2(m_rocket.GetPosition().x, m_rocket.GetPosition().y);

			for(i=m_curLevel.GetPlanets()->begin(); i != m_curLevel.GetPlanets()->end(); ++i)
			{
				Planet* planet = &(*i);
				D3DXVECTOR2 planetPos = D3DXVECTOR2(planet->GetPosition().x, planet->GetPosition().y);
				D3DXVECTOR2 R = (rocketPos - planetPos);
				D3DXVec2Normalize(&R, &R);
				D3DXVECTOR2 distance = planetPos - rocketPos;
				float rSquared = sqrt((distance.x * distance.x) + (distance.y * distance.y));
				F += (G * planet->GetMass() * m_rocket.GetMass() / rSquared) * R; // sum of all gravitational forces
			}
			D3DXVECTOR3 addition = D3DXVECTOR3( F.x/m_rocket.GetMass(), F.y/m_rocket.GetMass(), 0 );
			addition *= 0.001f;
			m_rocket.SetVelocity( m_rocket.GetVelocity() + addition );
			m_rocket.SetPosition( m_rocket.GetPosition() + m_rocket.GetVelocity() );
			m_particles.Add(m_rocket.GetPosition());

			if (m_screenBounds.Contains(D3DXVECTOR2(m_rocket.GetPosition().x, m_rocket.GetPosition().y)))
			{
				/* COLLISION DETECTION */
				D3DXVECTOR3 v;
				float centDist;

				//enemy
				v = m_enemy.GetPosition() - m_rocket.GetPosition();
				centDist = m_enemy.GetRadius() + m_rocket.GetRadius();
				if (D3DXVec3LengthSq(&v)<=centDist*centDist)
				{
					// collided
					bHasFired = false;
					//ProgressLevel();
					bLevelPassed[iCurLevelID-1] = true;
					iLevelAttempts[iCurLevelID-1] = iAttempts;
					LevelSelect::Instance()->Init();
					Engine::Instance()->SetState(Engine::STATE_LEVELSELECT);
				}
			
				// planets
				for(i=m_curLevel.GetPlanets()->begin(); i != m_curLevel.GetPlanets()->end(); ++i)
				{
					Planet* planet = &(*i);
				
					v = planet->GetPosition() - m_rocket.GetPosition();
					centDist = planet->GetRadius() + m_rocket.GetRadius();
					if (D3DXVec3LengthSq(&v)<=centDist*centDist)
					{
						// collided
						bHasFired = false;
					}
				}
			}
			else // outside screen bounds
			{
				D3DXVECTOR2 dispPoint = D3DXVECTOR2(m_rocket.GetPosition().x, m_rocket.GetPosition().y);
			
				if (dispPoint.x < m_screenBounds.GetTopLeft().x)
					dispPoint.x = m_screenBounds.GetTopLeft().x - 4;
				else if (dispPoint.x > m_screenBounds.GetBottomRight().x)
					dispPoint.x = m_screenBounds.GetBottomRight().x + 4;
				if (dispPoint.y < m_screenBounds.GetTopLeft().y)
					dispPoint.y = m_screenBounds.GetTopLeft().y - 4;
				else if (dispPoint.y > m_screenBounds.GetBottomRight().y)
					dispPoint.y = m_screenBounds.GetBottomRight().y + 4;

				m_offScreen.CreateQuad(*(Engine::Instance()->GetDevice()),
					D3DXVECTOR3(dispPoint.x-0.5f,dispPoint.y+0.5f,9.9f),
					D3DXVECTOR3(dispPoint.x+0.5f,dispPoint.y+0.5f,9.9f),
					D3DXVECTOR3(dispPoint.x-0.5f,dispPoint.y-0.5f,9.9f),
					D3DXVECTOR3(dispPoint.x+0.5f,dispPoint.y-0.5f,9.9f));
			}
		}

	}
	else
	{
		if (Engine::Instance()->GetInput()->returnDown)
		{
			if (bEnterDown == false)
			{
				bShowInstructions = false;
			}
			bEnterDown = true;
		}
		else
			bEnterDown = false;

		if (Engine::Instance()->GetInput()->escapeDown)
		{
			bShowInstructions = false;
			bEscapeDown = true;
		}
		else
			bEscapeDown = false;
	}

}

void MainGame::RecalculateAttemptImages()
{
	// recalculate count images
	char attStr[5];
	itoa(iAttempts, attStr, 10);
	std::string sAttempts(attStr);

	float start = 0;
	float end = 0;
	float div = 1.0f / 12.0f;
		
	// 1
	if (strlen(attStr) > 0)
	{
		int num = atoi((sAttempts.substr(0,1)).c_str());
		start = ((float)num) * div;
		end = ((float)num+1) * div;
	}
	else
	{
		start = 11.0f * div;
		end = 12.0f * div;
	}
	m_attQ1.CreateQuad(*(Engine::Instance()->GetDevice()),
			Quad::CUSTOMVERTEX(5.5,-9.55,-15,start,0),
			Quad::CUSTOMVERTEX(6,-9.55,-15,end,0),
			Quad::CUSTOMVERTEX(5.5,-10.05,-15,start,1),
			Quad::CUSTOMVERTEX(6,-10.05,-15,end,1));

	// 3
	if (strlen(attStr) > 1)
	{
		int num = atoi((sAttempts.substr(1,1)).c_str());
		start = ((float)num) * div;
		end = ((float)num+1) * div;
	}
	else
	{
		start = 11.0f * div;
		end = 12.0f * div;
	}
	m_attQ2.CreateQuad(*(Engine::Instance()->GetDevice()),
			Quad::CUSTOMVERTEX(6.1,-9.55,-15,start,0),
			Quad::CUSTOMVERTEX(6.6,-9.55,-15,end,0),
			Quad::CUSTOMVERTEX(6.1,-10.05,-15,start,1),
			Quad::CUSTOMVERTEX(6.6,-10.05,-15,end,1));

	// 3
	if (strlen(attStr) > 2)
	{
		int num = atoi((sAttempts.substr(2,1)).c_str());
		start = ((float)num) * div;
		end = ((float)num+1) * div;
	}
	else
	{
		start = 11.0f * div;
		end = 12.0f * div;
	}
	m_attQ3.CreateQuad(*(Engine::Instance()->GetDevice()),
			Quad::CUSTOMVERTEX(6.7,-9.55,-15,start,0),
			Quad::CUSTOMVERTEX(7.2,-9.55,-15,end,0),
			Quad::CUSTOMVERTEX(6.7,-10.05,-15,start,1),
			Quad::CUSTOMVERTEX(7.2,-10.05,-15,end,1));
}

VOID MainGame::Render(bool drawHUD)
{
	if (bShowInstructions == false)
	{
		Engine::Instance()->SetupMatrices(&m_background);
		m_background.Render(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());

		std::list<Planet>::iterator i;
		for(i=m_curLevel.GetPlanets()->begin(); i != m_curLevel.GetPlanets()->end(); ++i)
		{
			Planet* planet = &(*i);
			Engine::Instance()->SetupMatrices(planet);
			planet->RenderWithEffect(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());
		}
	
		if (bHasFired)
		{
			Engine::Instance()->GetEffect()->SetTechnique( Engine::Instance()->GetEffect()->GetTechniqueByName( "ConstCol" ) );
			Engine::Instance()->SetupMatrices(&m_rocket);
			m_rocket.RenderWithEffect(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());
			Engine::Instance()->GetEffect()->SetTechnique( Engine::Instance()->GetEffect()->GetTechniqueByName( "ShadingTexture" ) );
		}

		Engine::Instance()->SetupMatrices(&m_ship);
		m_ship.RenderWithEffect(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());
	
		Engine::Instance()->SetupMatrices(&m_enemy);
		m_enemy.RenderWithEffect(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());
	
		if (drawHUD)
		{
			Engine::Instance()->GetEffect()->SetTechnique( Engine::Instance()->GetEffect()->GetTechniqueByName( "BasicTexture" ) );

			Engine::Instance()->SetupMatrices(&m_powerBack);
			m_powerBack.Render(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());

			Engine::Instance()->SetupMatrices(&m_attemptBack);
			m_attemptBack.Render(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());

			Engine::Instance()->SetupMatrices(&m_powerBarFULL);
			m_powerBarFULL.Render(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());

			Engine::Instance()->SetupMatrices(&m_powerBar);
			m_powerBar.Render(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());
		}

		Engine::Instance()->GetEffect()->SetTechnique( Engine::Instance()->GetEffect()->GetTechniqueByName( "ShadingTexture" ) );

		m_particles.Render((*Engine::Instance()->GetDevice()));

		if (!m_screenBounds.Contains(D3DXVECTOR2(m_rocket.GetPosition().x, m_rocket.GetPosition().y)))
		{
			Engine::Instance()->SetupMatrices(&m_offScreen);
			m_offScreen.Render(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());
		}

		if (drawHUD)
		{
			Engine::Instance()->GetEffect()->SetTechnique( Engine::Instance()->GetEffect()->GetTechniqueByName( "BasicTexture" ) );

			Engine::Instance()->SetupMatrices(&m_attQ1);
			m_attQ1.Render(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());

			Engine::Instance()->SetupMatrices(&m_attQ2);
			m_attQ2.Render(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());

			Engine::Instance()->SetupMatrices(&m_attQ3);
			m_attQ3.Render(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());
		}

		Engine::Instance()->GetEffect()->SetTechnique( Engine::Instance()->GetEffect()->GetTechniqueByName( "ShadingTexture" ) );
	}
	else
	{
		Engine::Instance()->GetEffect()->SetTechnique( Engine::Instance()->GetEffect()->GetTechniqueByName( "BasicTexture" ) );

		Engine::Instance()->SetupMatrices(&m_instructions);
		m_instructions.Render(*(Engine::Instance()->GetDevice()), Engine::Instance()->GetEffect());

		Engine::Instance()->GetEffect()->SetTechnique( Engine::Instance()->GetEffect()->GetTechniqueByName( "ShadingTexture" ) );
	}
}

VOID MainGame::GoToLevel(int levelID)
{
	//clear level variables
	m_particles.GetSpriteList()->clear();

	iCurLevelID = levelID;

	char levelpath[100];
	sprintf(levelpath, "..\\PlanetWars\\levels\\level%i.txt",iCurLevelID);

	m_curLevel.Initialise(*(Engine::Instance()->GetDevice()), levelpath);
	m_ship.SetPosition(m_curLevel.GetShipPosition());
	m_ship.SetRotation(m_curLevel.GetShipRotation());
	m_enemy.SetPosition(m_curLevel.GetEnemyPosition());
	m_enemy.SetRotation(m_curLevel.GetEnemyRotation());
	
	iAttempts = 0;
	fShotPower = 0.01f;
	bEnterDown = true;
	bEscapeDown = true;
	
	RecalculateAttemptImages();

	bHasFired = false;
}

VOID MainGame::GetLevelTexture()
{
	// create texture
	(*Engine::Instance()->GetDevice())->CreateTexture(600,500,1,D3DUSAGE_RENDERTARGET,D3DFMT_R5G6B5,D3DPOOL_DEFAULT, &lpRenderTexture, NULL);
	lpRenderTexture->GetSurfaceLevel(0,&lpRenderSurface);
	D3DXMatrixPerspectiveFovLH(&mMatProjection,D3DX_PI / 0.1f,1,1,100);

	bool showInsTemp = bShowInstructions;
	bShowInstructions = false;

	HRESULT h = (*Engine::Instance()->GetDevice())->GetRenderTarget(0,&lpBackBuffer);
	(*Engine::Instance()->GetDevice())->SetRenderTarget(0,lpRenderSurface);
	(*Engine::Instance()->GetDevice())->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(100,100,100),1.0f,0);
	(*Engine::Instance()->GetDevice())->BeginScene();
	Render(false);
	(*Engine::Instance()->GetDevice())->EndScene();
	(*Engine::Instance()->GetDevice())->SetRenderTarget(0,lpBackBuffer);

	bShowInstructions = showInsTemp;
}
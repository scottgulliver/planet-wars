////////////////////////////////////////////////////////////////////////
// File: Level.cpp			Class: Level
// 
// Description:
// Represents a single level. This stores positions for the ships, and
// the planets. This data is loaded from a text file.
//
////////////////////////////////////////////////////////////////////////

#include "Level.h"
using namespace std;

// -- Object Management --

Level::Level()
{
	m_planets = std::list<Planet>();

	vShipPosition = D3DXVECTOR3(0,0,0);
	vShipRotation = D3DXVECTOR3(0,0,0);
	vEnemyPosition = D3DXVECTOR3(0,0,0);
	vEnemyRotation = D3DXVECTOR3(0,0,0);
}

Level::~Level()
{
}

// -- Get / Set Methods --

D3DXVECTOR3 Level::GetShipPosition()
{
	return vShipPosition;
}

void Level::SetShipPosition	(D3DXVECTOR3 shipPos)
{
	vShipPosition = shipPos;
}

D3DXVECTOR3 Level::GetShipRotation()
{
	return vShipRotation;
}

void Level::SetShipRotation(D3DXVECTOR3 shipRot)
{
	vShipRotation = shipRot;
}

D3DXVECTOR3	Level::GetEnemyPosition()
{
	return vEnemyPosition;
}

void Level::SetEnemyPosition(D3DXVECTOR3 enemyPos)
{
	vEnemyPosition = enemyPos;
}

D3DXVECTOR3 Level::GetEnemyRotation()
{
	return vEnemyRotation;
}

void Level::SetEnemyRotation(D3DXVECTOR3 enemyRot)
{
	vEnemyRotation = enemyRot;
}

std::list<Planet>* Level::GetPlanets()
{
	return &m_planets;
}

void Level::SetPlanets(std::list<Planet> planets)
{
	m_planets = planets;
}

// -- Methods --

HRESULT Level::Initialise(LPDIRECT3DDEVICE9 lpDevice, LPCTSTR pFilename)
{
	m_planets = std::list<Planet>();

	vShipPosition = D3DXVECTOR3(0,0,0);
	vShipRotation = D3DXVECTOR3(0,0,0);
	vEnemyPosition = D3DXVECTOR3(0,0,0);
	vEnemyRotation = D3DXVECTOR3(0,0,0);

	//load file

	string line;
	ifstream levelFile(pFilename);
	if (levelFile.is_open())
	{
		while ( levelFile.good() )
		{
			// ship xPos
			getline (levelFile,line);
			line = line.substr(0, line.find_first_of("'"));
			vShipPosition.x = atof(line.c_str());
			
			// ship yPos
			getline (levelFile,line);
			line = line.substr(0, line.find_first_of("'"));
			vShipPosition.y = atof(line.c_str());

			// ship xRot
			getline (levelFile,line);
			line = line.substr(0, line.find_first_of("'"));
			vShipRotation.x = atof(line.c_str());

			// ship yRot
			getline (levelFile,line);
			line = line.substr(0, line.find_first_of("'"));
			vShipRotation.y = atof(line.c_str());

			// ship zRot
			getline (levelFile,line);
			line = line.substr(0, line.find_first_of("'"));
			vShipRotation.z = atof(line.c_str());

			// enemy xPos
			getline (levelFile,line);
			line = line.substr(0, line.find_first_of("'"));
			vEnemyPosition.x = atof(line.c_str());
			
			// enemy yPos
			getline (levelFile,line);
			line = line.substr(0, line.find_first_of("'"));
			vEnemyPosition.y = atof(line.c_str());
			
			// enemy xRot
			getline (levelFile,line);
			line = line.substr(0, line.find_first_of("'"));
			vEnemyRotation.x = atof(line.c_str());
			
			// enemy yRot
			getline (levelFile,line);
			line = line.substr(0, line.find_first_of("'"));
			vEnemyRotation.y = atof(line.c_str());
			
			// enemy zRot
			getline (levelFile,line);
			line = line.substr(0, line.find_first_of("'"));
			vEnemyRotation.z = atof(line.c_str());
			
			// num planets
			getline (levelFile,line);
			line = line.substr(0, line.find_first_of("'"));
			int numPlanets = atoi(line.c_str());
			
			for (int i = 0; i < numPlanets; i++)
			{
				// p1 texture
				getline (levelFile,line);
				line = line.substr(0, line.find_first_of("'"));
				string texPath = "..\\PlanetWars\\planetTextures\\" + line;

				// p1 bumpmap
				string bmPath = "..\\PlanetWars\\planetTextures\\" + line.substr(0,7) + "BM.bmp";

				// p1 radius
				getline (levelFile,line);
				line = line.substr(0, line.find_first_of("'"));
				float radius = atof(line.c_str());

				// p1 xPos
				getline (levelFile,line);
				line = line.substr(0, line.find_first_of("'"));
				float xPos = atof(line.c_str());

				// p1 yPos
				getline (levelFile,line);
				line = line.substr(0, line.find_first_of("'"));
				float yPos = atof(line.c_str());
				
				// p1 xRot
				getline (levelFile,line);
				line = line.substr(0, line.find_first_of("'"));
				float xRot = atof(line.c_str());
				
				// p1 yRot
				getline (levelFile,line);
				line = line.substr(0, line.find_first_of("'"));
				float yRot = atof(line.c_str());
				
				// p1 zRot
				getline (levelFile,line);
				line = line.substr(0, line.find_first_of("'"));
				float zRot = atof(line.c_str());

				Planet* planet = new Planet();
				planet->Initialise(lpDevice, radius);
				D3DXCreateTextureFromFile(lpDevice, texPath.c_str(), planet->GetTexture());
				D3DXCreateTextureFromFile(lpDevice, bmPath.c_str(), planet->GetBumpMap());
				planet->SetPosition(D3DXVECTOR3(xPos, yPos, 0.0f));
				planet->SetRotVel(D3DXVECTOR3(xRot, yRot, zRot));
				planet->SetMass( planet->GetMass() * 0.5f );
				m_planets.push_back(*planet);
			}
		}
		levelFile.close();
	}
	else
	{
		MessageBox( NULL, "Could not find level file", "Level", MB_OK );
		return FALSE;
	}

	return S_OK;
}
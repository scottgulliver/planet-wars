////////////////////////////////////////////////////////////////////////
// File: Level.h			Class: Level
// 
// Description:
// Represents a single level. This stores positions for the ships, and
// the planets. This data is loaded from a text file.
//
////////////////////////////////////////////////////////////////////////

#ifndef HG_LEVEL
#define HG_LEVEL

#include "Planet.h"
#include <list>
#include <iostream>
#include <fstream>
#include <string>

class Level
{
private:
	// -- Variables --
	std::list<Planet>	m_planets;
	D3DXVECTOR3			vShipPosition;
	D3DXVECTOR3			vShipRotation;
	D3DXVECTOR3			vEnemyPosition;
	D3DXVECTOR3			vEnemyRotation;

public:
	// -- Object Management --
	Level	();
	~Level	();

	// -- Get / Set Methods --
	D3DXVECTOR3		GetShipPosition	();
	void			SetShipPosition	(D3DXVECTOR3 shipPos);
	D3DXVECTOR3		GetShipRotation	();
	void			SetShipRotation	(D3DXVECTOR3 shipRot);
	D3DXVECTOR3		GetEnemyPosition ();
	void			SetEnemyPosition (D3DXVECTOR3 enemyPos);
	D3DXVECTOR3		GetEnemyRotation ();
	void			SetEnemyRotation (D3DXVECTOR3 enemyRot);
	std::list<Planet>* GetPlanets	();
	void			SetPlanets		(std::list<Planet> planets);


	// -- Methods --
	HRESULT Initialise	(LPDIRECT3DDEVICE9 lpDevice, LPCTSTR pFilename);
};

#endif
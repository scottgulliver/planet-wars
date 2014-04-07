////////////////////////////////////////////////////////////////////////
// File: Game.cpp			Class: Game
// 
// Description:
// TODO.
//
////////////////////////////////////////////////////////////////////////

#include "Engine.h"

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
			Engine::Instance()->Cleanup();
            PostQuitMessage( 0 );
            return 0;

		case WM_KEYDOWN:
		case WM_KEYUP:
			return Engine::Instance()->GetInput()->HandleInput(msg, wParam);
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
    // Register the window class
    WNDCLASSEX wc =
    {
        sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L,
        GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
        "D3D Tutorial", NULL
    };
    RegisterClassEx( &wc );

    // Create the application's window
    HWND hWnd = CreateWindow( "D3D Tutorial", "Planet Wars",
                              WS_OVERLAPPEDWINDOW, 100, 100, 800, 600,
                              NULL, NULL, wc.hInstance, NULL );

    // Initialize Direct3D
    if( SUCCEEDED( Engine::Instance()->InitD3D( hWnd ) ) )
    {
		Engine::Instance()->GetCamera().SetPosition(	D3DXVECTOR3( 0.0f, 0.0f, -40.0f ),	// eye position
								D3DXVECTOR3( 0.0f, 0.0f, 10.0f )		// lookat offset
								);

		// Set effect variables as needed
		D3DXCOLOR colorMtrlDiffuse( 0.0f, 0.0f, 0.0f, 1.0f );
		D3DXCOLOR colorMtrlAmbient( 0.9f, 0.9f, 0.9f, 0.9f );
		Engine::Instance()->GetEffect()->SetValue( "g_MaterialAmbientColor", &colorMtrlAmbient, sizeof( D3DXCOLOR ) );
		Engine::Instance()->GetEffect()->SetValue( "g_MaterialDiffuseColor", &colorMtrlDiffuse, sizeof( D3DXCOLOR ) );
		

        // Show the window
        ShowWindow( hWnd, SW_SHOWDEFAULT );
        UpdateWindow( hWnd );

        // Enter the message loop
        MSG msg;
        ZeroMemory( &msg, sizeof( msg ) );
        while( msg.message != WM_QUIT )
        {
            if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
            else
			{
				Engine::Instance()->Update();
                Engine::Instance()->Render();
			}
        }
    }

    UnregisterClass( "D3D Tutorial", wc.hInstance );
    return 0;
}

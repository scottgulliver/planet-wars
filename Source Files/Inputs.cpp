////////////////////////////////////////////////////////////////////////
// File: Inputs.cpp			Class: Inputs
// 
// Description:
// This holds the  current states of the inputs from the keyboard.
//
////////////////////////////////////////////////////////////////////////

#include "Inputs.h"

bool Inputs::HandleInput(UINT msg, WPARAM wParam)
{
	switch( msg )
    {
		case WM_KEYDOWN:
            switch (wParam)
            {
                case VK_LEFT:
					leftArrowDown = true;
                    return 0;
                case VK_RIGHT:
					rightArrowDown = true;
                    return 0;
                case VK_UP:
					upArrowDown = true;
                    return 0;
                case VK_DOWN:
					downArrowDown = true;
                    return 0;

				case VK_RETURN:
					returnDown = true;
					return 0;
				case VK_ESCAPE:
					escapeDown = true;
					return 0;

				case 0x41: aKeyDown = true; return 0;
				case 0x42: bKeyDown = true; return 0;
				case 0x43: cKeyDown = true; return 0;
				case 0x44: dKeyDown = true; return 0;
				case 0x45: eKeyDown = true; return 0;
				case 0x46: fKeyDown = true; return 0;
				case 0x47: gKeyDown = true; return 0;
				case 0x48: hKeyDown = true; return 0;
				case 0x49: iKeyDown = true; return 0;
				case 0x4a: jKeyDown = true; return 0;
				case 0x4b: kKeyDown = true; return 0;
				case 0x4c: lKeyDown = true; return 0;
				case 0x4d: mKeyDown = true; return 0;
				case 0x4e: nKeyDown = true; return 0;
				case 0x4f: oKeyDown = true; return 0;
				case 0x50: pKeyDown = true; return 0;
				case 0x51: qKeyDown = true; return 0;
				case 0x52: rKeyDown = true; return 0;
				case 0x53: sKeyDown = true; return 0;
				case 0x54: tKeyDown = true; return 0;
				case 0x55: uKeyDown = true; return 0;
				case 0x56: vKeyDown = true; return 0;
				case 0x57: wKeyDown = true; return 0;
				case 0x58: xKeyDown = true; return 0;
				case 0x59: yKeyDown = true; return 0;
				case 0x5a: zKeyDown = true; return 0;

				case 0x31: n1KeyDown = true; return 0;
			}
			return 0;
		case WM_KEYUP:
            switch (wParam)
            {
                case VK_LEFT:
					leftArrowDown = false;
                    return 0;
                case VK_RIGHT:
					rightArrowDown = false;
                    return 0;
                case VK_UP:
					upArrowDown = false;
                    return 0;
                case VK_DOWN:
					downArrowDown = false;
                    return 0;

				case VK_RETURN:
					returnDown = false;
					return 0;
				case VK_ESCAPE:
					escapeDown = false;
					return 0;

				case 0x41: aKeyDown = false; return 0;
				case 0x42: bKeyDown = false; return 0;
				case 0x43: cKeyDown = false; return 0;
				case 0x44: dKeyDown = false; return 0;
				case 0x45: eKeyDown = false; return 0;
				case 0x46: fKeyDown = false; return 0;
				case 0x47: gKeyDown = false; return 0;
				case 0x48: hKeyDown = false; return 0;
				case 0x49: iKeyDown = false; return 0;
				case 0x4a: jKeyDown = false; return 0;
				case 0x4b: kKeyDown = false; return 0;
				case 0x4c: lKeyDown = false; return 0;
				case 0x4d: mKeyDown = false; return 0;
				case 0x4e: nKeyDown = false; return 0;
				case 0x4f: oKeyDown = false; return 0;
				case 0x50: pKeyDown = false; return 0;
				case 0x51: qKeyDown = false; return 0;
				case 0x52: rKeyDown = false; return 0;
				case 0x53: sKeyDown = false; return 0;
				case 0x54: tKeyDown = false; return 0;
				case 0x55: uKeyDown = false; return 0;
				case 0x56: vKeyDown = false; return 0;
				case 0x57: wKeyDown = false; return 0;
				case 0x58: xKeyDown = false; return 0;
				case 0x59: yKeyDown = false; return 0;
				case 0x5a: zKeyDown = false; return 0;

				case 0x31: n1KeyDown = false; return 0;
			}
			return 0;
    }
}
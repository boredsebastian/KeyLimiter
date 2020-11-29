#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <array>
#include <vector>

using std::cout;
using std::string;
//using std::getline;
using std::cin;
using std::endl;
using std::array;
using std::vector;

//callback function for the keyboard hook
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

//possible states for the automata
enum STATES { NOTHING, TYPED_L, TYPED_O, TYPED_V, TYPED_E };
//detects if the sequence L O V E has been typed
bool UserTypedLOVE(DWORD vkCode);
//initialization of the current state variable
STATES currentState = NOTHING;

//the hook
HHOOK keyboardHook;

//we use this variable to identify if the current window has changed
HWND prevWindow;

int main()
{
	printf("Hooking the keyboard\n");
	//Here we set the low level hook
	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, 0, 0);
	printf("%X\n", keyboardHook);

	MSG msg{ 0 };
	//a very long loop
	while (GetMessage(&msg, NULL, 0, 0) != 0);
	UnhookWindowsHookEx(keyboardHook);
	return 0;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	wchar_t title[256];
	HWND fwindow = GetForegroundWindow();
	PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
	//a key was pressed
	if (wParam == WM_KEYDOWN && nCode == HC_ACTION)
	{
		if (prevWindow != fwindow)
		{
			GetWindowText(fwindow, title, 256);
			wprintf(title);
			printf("\n");
			prevWindow = fwindow;
		}
		//if ENTER was pressed we print a line ending
		//otherwise we print the key that was pressed
		if (key->vkCode != VK_RETURN)
			printf("%c", key->vkCode);
		else
			printf("\n");
		//if the L O V E sequence was typed we print a message
		//and make a sound
		if (UserTypedLOVE(key->vkCode))
		{
			printf("\n\n\n love is the child of illusion and the parent of disillusion\n -Miguel de Unamuno-\n\n\n");
			Beep(800, 400); Beep(600, 400); Beep(400, 400);
		}
	}

	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

//automata to identify a sequence
bool UserTypedLOVE(DWORD vkCode)
{
	switch (currentState)
	{
	case NOTHING:
		if (vkCode == 'L')
			currentState = TYPED_L;
		else
			currentState = NOTHING;
		break;
	case TYPED_L:
		if (vkCode == 'O')
			currentState = TYPED_O;
		else
			currentState = NOTHING;
		break;
	case TYPED_O:
		if (vkCode == 'V')
			currentState = TYPED_V;
		else
			currentState = NOTHING;
		break;
	case TYPED_V:
		if (vkCode == 'E')
			currentState = TYPED_E;
		else
			currentState = NOTHING;
		return true;
		break;

	}
	return false;
}
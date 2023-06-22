//#include <stdio.h>
//#include <stdbool.h>
//#include <Windows.h>
//
//enum FG_COLORS
//{
//	FG_BLACK,
//	FG_DARK_BLUE,
//	FG_DARK_GREEN,
//	FG_DARK_CYAN,
//	FG_DARK_RED,
//	FG_DARK_MAGENTA,
//	FG_DARK_YELLOW,
//	FG_GREY,
//	FG_DARK_GREY,
//	FG_BLUE,
//	FG_GREEN,
//	FG_CYAN,
//	FG_RED,
//	FG_MAGENTA,
//	FG_YELLOW,
//	FG_WHITE
//};
//
//enum BG_COLORS
//{
//	BG_BLACK = 0x0000,
//	BG_DARK_BLUE = 0x0010,
//	BG_DARK_GREEN = 0x0020,
//	BG_DARK_CYAN = 0x0030,
//	BG_DARK_RED = 0x0040,
//	BG_DARK_MAGENTA = 0x0050,
//	BG_DARK_YELLOW = 0x0060,
//	BG_GREY = 0x0070,
//	BG_DARK_GREY = 0x0080,
//	BG_BLUE = 0x0090,
//	BG_GREEN = 0x00A0,
//	BG_CYAN = 0x00B0,
//	BG_RED = 0x00C0,
//	BG_MAGENTA = 0x00D0,
//	BG_YELLOW = 0x00E0,
//	BG_WHITE = 0x00F0
//};
//
//enum PIXEL_TYPE
//{
//	PIXEL_SOLID = 0x2588,
//	PIXEL_THREEQUARTERS = 0x2593,
//	PIXEL_HALF = 0x2592,
//	PIXEL_QUARTER = 0x2591
//};
//
//typedef struct
//{
//	bool pressed;
//	bool held;
//	bool released;
//} KeyState;
//
//typedef struct
//{
//	wchar_t title[128];
//
//	CHAR_INFO* screen;
//	HANDLE consoleOut;
//	HANDLE consoleIn;
//	SMALL_RECT windowRect;
//
//	HWND wnd;
//	HDC dc;
//
//	KeyState keys[256];
//	KeyState mouse[5];
//
//	short keyOldState[256];
//	short keyNewState[256];
//
//	bool mouseOldState[5];
//	bool mouseNewState[5];
//
//	int mouseX;
//	int mouseY;
//
//	int screenWidth;
//	int screenHeight;
//
//	int fontWidth;
//	int fontHeight;
//
//	int targetFPS;
//
//	bool focused;
//	bool active;
//
//	int ticks;
//
//} ConsoleGameEngine;
//
//void cgeClear(ConsoleGameEngine* cge)
//{
//	SetConsoleActiveScreenBuffer((HANDLE)0);
//	free(cge->screen);
//}
//
//bool cgeConstruct(ConsoleGameEngine* cge, int screenWidth, int screenHeight, int fontWidth, int fontHeight)
//{
//	if (screenWidth <= 0 || screenHeight <= 0 || fontWidth <= 0 || fontHeight <= 0)
//		return false;
//
//	cge->focused = true;
//	cge->active = true;
//	cge->mouseX = INT_MAX;
//	cge->mouseY = INT_MAX;
//	cge->targetFPS = 0;
//	cge->ticks = 0.0f;
//
//	for (int i = 0; i < 256; i++)
//	{
//		cge->keys[i].held = false;
//		cge->keys[i].pressed = false;
//		cge->keys[i].released = false;
//
//		cge->keyOldState[i] = 0;
//		cge->keyNewState[i] = 0;
//	}
//
//	for (int i = 0; i < 5; i++)
//	{
//		cge->mouse[i].held = false;
//		cge->mouse[i].pressed = false;
//		cge->mouse[i].released = false;
//
//		cge->mouseOldState[i] = false;
//		cge->mouseNewState[i] = false;
//	}
//	
//	cge->screenWidth = screenWidth;
//	cge->screenHeight = screenHeight;
//
//	cge->fontWidth = fontWidth;
//	cge->fontHeight = fontHeight;
//
//	cge->consoleOut = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//	cge->consoleIn = GetStdHandle(STD_INPUT_HANDLE);
//	
//	if (cge->consoleOut == INVALID_HANDLE_VALUE)
//		return false;
//
//	cge->windowRect = (SMALL_RECT){ 0, 0, 1, 1 };
//	if (!SetConsoleWindowInfo(cge->consoleOut, TRUE, &cge->windowRect)) return false;
//
//	COORD c = { .X = screenWidth, .Y = screenHeight };
//
//	if (!SetConsoleScreenBufferSize(cge->consoleOut, c)) return false;
//	if (!SetConsoleActiveScreenBuffer(cge->consoleOut)) return false;
//
//	CONSOLE_FONT_INFOEX cfi;
//	cfi.cbSize = sizeof(cfi);
//	cfi.nFont = 0;
//	cfi.dwFontSize.X = fontWidth;
//	cfi.dwFontSize.Y = fontHeight;
//	cfi.FontFamily = FF_DONTCARE;
//	cfi.FontWeight = FW_NORMAL;
//	
//	wcscpy_s(cfi.FaceName, 9, L"Consolas");
//	if (!SetCurrentConsoleFontEx(cge->consoleOut, FALSE, &cfi)) return false;
//
//	if (!SetConsoleMode(cge->consoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
//		return false;
//
//	CONSOLE_SCREEN_BUFFER_INFO csbi;
//	if (!GetConsoleScreenBufferInfo(cge->consoleOut, &csbi)) return false;
//
//	if (screenWidth > csbi.dwMaximumWindowSize.X) return false;
//	if (screenHeight > csbi.dwMaximumWindowSize.Y) return false;
//
//	cge->windowRect = (SMALL_RECT){ 0, 0, screenWidth - 1, screenHeight - 1 };
//	if (!SetConsoleWindowInfo(cge->consoleOut, TRUE, &cge->windowRect)) return false;
//
//	cge->screen = (CHAR_INFO*)malloc(screenWidth * screenHeight * sizeof(CHAR_INFO));
//	if (cge->screen == NULL) return false;
//
//	memset(cge->screen, 0, screenWidth * screenHeight * sizeof(CHAR_INFO));
//
//	swprintf_s(cge->title, 128, L"Hello, World!");
//
//	wchar_t title[256];
//	swprintf_s(title, 256, L"Console Game Engine - %s", cge->title);
//	SetConsoleTitle(title);
//
//	return true;
//}
//
//bool cgeUpdate(ConsoleGameEngine* cge)
//{
//	if (cge->active)
//	{
//		INPUT_RECORD input[32];
//		DWORD events = 0;
//		GetNumberOfConsoleInputEvents(cge->consoleIn, &events);
//		if (events > 0)
//			ReadConsoleInput(cge->consoleIn, input, events, &events);
//
//		for (DWORD i = 0; i < events; i++)
//		{
//			switch (input[i].EventType)
//			{
//			case FOCUS_EVENT:
//				cge->focused = input[i].Event.FocusEvent.bSetFocus;
//				break;
//
//			case MOUSE_EVENT:
//			{
//				switch (input[i].Event.MouseEvent.dwEventFlags)
//				{
//				case MOUSE_MOVED:
//				{
//					cge->mouseX = input[i].Event.MouseEvent.dwMousePosition.X;
//					cge->mouseY = input[i].Event.MouseEvent.dwMousePosition.Y;
//				}
//				break;
//
//				case 0:
//				{
//					for (int m = 0; m < 5; m++)
//						cge->mouseNewState[m] = (input[i].Event.MouseEvent.dwButtonState & (1 << m)) > 0;
//				}
//				break;
//			
//				}
//			}
//			break;
//
//			}
//		}
//
//		for (int i = 0; i < 256; i++)
//		{
//			cge->keyNewState[i] = GetAsyncKeyState(i);
//
//			cge->keys[i].pressed = false;
//			cge->keys[i].released = false;
//
//			if (cge->keyNewState[i] != cge->keyOldState[i])
//			{
//				if (cge->keyNewState[i] & 0x8000)
//				{
//					cge->keys[i].pressed = !cge->keys[i].held;
//					cge->keys[i].held = true;
//				}
//				else
//				{
//					cge->keys[i].released = true;
//					cge->keys[i].held = false;
//				}
//			}
//
//			cge->keyOldState[i] = cge->keyNewState[i];
//		}
//
//		for (int i = 0; i < 5; i++)
//		{
//			cge->mouse[i].pressed = false;
//			cge->mouse[i].released = false;
//
//			if (cge->mouseNewState[i] != cge->mouseOldState[i])
//			{
//				if (cge->mouseNewState[i])
//				{
//					cge->mouse[i].pressed = true;
//					cge->mouse[i].held = true;
//				}
//				else
//				{
//					cge->mouse[i].released = true;
//					cge->mouse[i].held = false;
//				}
//			}
//
//			cge->mouseOldState[i] = cge->mouseNewState[i];
//		}
//
//		if (cge->ticks < 1000 / cge->targetFPS)
//			cge->ticks++;
//		else
//		{
//			WriteConsoleOutputW(cge->consoleOut, cge->screen, (COORD){ cge->screenWidth, cge->screenHeight }, (COORD){ 0,0 }, &cge->windowRect);
//			cge->ticks = 0;
//		}
//	}
//
//	return cge->active;
//}
//
//void cgeDraw(ConsoleGameEngine* cge, int x, int y, short c, short col)
//{
//	if (x >= 0 && y >= 0 && x < cge->screenWidth && y < cge->screenHeight)
//	{
//		cge->screen[y * cge->screenWidth + x].Char.UnicodeChar = c;
//		cge->screen[y * cge->screenWidth + x].Attributes = col;
//	}
//}
//
//void cgeFillRect(ConsoleGameEngine* cge, int x, int y, int sx, int sy, short c, short col)
//{
//	for (int i = 0; i < sx; i++)
//		for (int j = 0; j < sy; j++)
//			cgeDraw(cge, x + i, y + j, c, col);
//}
//
//void cgeClearScreen(ConsoleGameEngine* cge, short c, short col)
//{
//	for (int i = 0; i < cge->screenWidth; i++)
//		for (int j = 0; j < cge->screenHeight; j++)
//			cgeDraw(cge, i, j, c, col);
//}
//
//int main()
//{
//	ConsoleGameEngine cge;
//
//	if (!cgeConstruct(&cge, 256, 200, 4, 4))
//		return 1;
//
//	cge.targetFPS = 60;
//
//	while (cgeUpdate(&cge))
//	{
//		short col;
//
//		if (cge.keys[VK_SPACE].held)
//			col = FG_RED | BG_RED;
//		else
//			col = FG_YELLOW | BG_YELLOW;
//
//		cgeClearScreen(&cge, PIXEL_SOLID, FG_WHITE | BG_WHITE);
//		cgeFillRect(&cge, cge.mouseX, cge.mouseY, 40, 40, PIXEL_SOLID, col);
//	}
//
//	cgeClear(&cge);
//
//	return 0;
//}

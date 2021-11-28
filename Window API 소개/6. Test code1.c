#include <Windows.h>
#define MY_DEF_STYLE CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
void RegMyWndClass(LPCWSTR cname);
void MakeWindow(LPCWSTR cname);
void MsgLoop();

INT APIENTRY WinMain(HINSTANCE hIns, HINSTANCE hPrev, LPSTR cmd, INT nShow)
{
	RegMyWndClass(TEXT("MyWindow"));//Register
	MakeWindow(TEXT("MyWindow"));//Make
	MsgLoop();
}

void RegMyClass(LPCWSTR cname)//Set window property, Register
{
	WNDCLASS wndclass={0};//Set window's properties
	wndclass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hInstance=GetModuleHandle(0);
	wndclass.hIcon=LoadIcon(0, IDI_APPLICATION);
	wndclass.hCursor=LoadCursor(0, IDC_ARROW);
	wndclass.lpfnWndProc=MyWndProc;
	wndclass.lpszMenuName=0;
	wndclass.lpszClassName=0;
	wndclass.style=MY_DEF_STYLE;
	
	RegisterClass(&wndclass);//Register.
}

void MakeWindow(LPCWSTR cname)//make window object, Show
{
	HWND hWnd=CreateWindow(cname,//Class Name
					TEXT("Two Timer"),//Title Name
					WS_OVERLAPPEDWINDOW,//Window Style
					100,30,700,600,//Left,Top,Width,Height
					0,//Handle of parent window
					0,//menu
					GetModuleHandle(0),//handle of module
					0);//arguemnt to WM_CREATE
	
	ShowWinodw(hWnd, SW_SHOW);//Visualization of Window Object.
}

void MsgLoop()
{
	MSG Message;
	while(GetMessage(&Message, 0, 0, 0))//Get Message to application queue
	{
		DispatchMessage(&Message);//work callback(do message)
	}
}


SYSTEMTIME st;
int ticks;
VOID CALLBACK OnClock(HWND hWnd, UINT iMessage, UINT_PTR tid, DWORD cnt)
{
	GetLocalTime(&st);
}

#define IDT_CLOCK 1
#define IDT_TICK 2

VOID CALLBACK OnClock(HWND, UINT, UINT_PTR, DWORD);
void OnTick(HWND hWnd);

void OnCreate(HWND hWnd)//SetTimer.
{
	SetTimer(hWnd, IDT_CLOCK, 1000, OnClock);//IDT_CLOCK(Timer serial number we set), per 1 second, Callback to OnClock
	SetTimer(hWnd, IDT_TICK, 1/10, NULL);//IDT_TICK(Timer serial number we set), per 1/100 second, No CallbackProc (***1/10? )
}

void OnTimer(HWND hWnd, DWORD tid)
{
	switch(tid)
	{
		case IDT_TICK:
			OnTick(hWnd);
			return;
	}
}

void OnDestroy(HWND hWnd)
{
	KillTimer(hWnd, IDT_CLOCK);//KillTimer by nIDEvent(Timer's serial number)
	KillTimer(hWnd, IDT_TICK);
	PostQuitMessage(0);//WM_QUIT
}



/*
1.	In WinMain, Set properties of window & Register -> Make window object * Visualize -> Call MsgLoop(Execute MyWindowProc)
2.	In MyWindowProc, WM_CREATE call OnCreate || WM_PAINT call OnPaint || WM_TIMER call OnTimer || WM_DESTROY call OnDestroy.
	2-1) In OnCreate, Make Timer objects by SetTimer. We set first Timer with OnClock Proc, second Timer without Proc.
		2-1-1) In OnClock, Get current system time to st(SYSTEMTIME global variable) & invaliate Rect
	2-2) In OnPaint, Make PAINTSTRUCT object for drawing, call BeginPaint(reg DC, cal unvalid) & OnDraw(drawing) & EndPaint(del DC, change to valid)
		2-2-1) In OnDraw, TextOut st, ticks that are global variable of information in first, second timer
	2-3) In OnTimer, If wParam is equals to Serial of second Timer, Call OnTick function.
		2-3-1) In OnTick, INC ticks & invalidate Rect.
	2-4) In OnDestroy, delete both Timer by calling KillTimer function & Send WM_QUIT message to window Proc.
*/

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

void RegMyWndClass(LPCWSTR cname)//Set window property, Register
{
	WNDCLASS wndclass={0};//Set window's properties
	wndclass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hInstance=GetModuleHandle(0);
	wndclass.hIcon=LoadIcon(0, IDI_APPLICATION);
	wndclass.hCursor=LoadCursor(0, IDC_ARROW);
	wndclass.lpfnWndProc=MyWndProc;//Changing value, call callbackProc
	wndclass.lpszMenuName=0;
	wndclass.lpszClassName=cname;
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
	
	ShowWindow(hWnd, SW_SHOW);//Visualization of Window Object.
}

void MsgLoop()
{
	MSG Message;
	while(GetMessage(&Message, 0, 0, 0))//Get Message to application queue
	{
		DispatchMessage(&Message);//work callback(do message)
	}
}


SYSTEMTIME st;//Make system time object
int ticks;//make ticks variable
VOID CALLBACK OnClock(HWND hWnd, UINT iMessage, UINT_PTR tid, DWORD cnt)//CALLBACK for first Timer
{
	GetLocalTime(&st);//get local time to st(SYSTEMTIME variable)
	InvalidateRect(hWnd, 0, 0);//Invalidate Rect. Reflect on Screen per this command.
	//Make Invalidate Section for redrawing. for all screen redrawing, pass 0 as argument.
}
void OnTick(HWND hWnd)//add ticks for second Timer
{
	ticks++;//INC ticks
	InvalidateRect(hWnd, 0, 0);//Invalidate Rect. Reflect on Screen per this command.
}
void OnDraw(HDC hdc)//print Timers
{
	WCHAR buf[256];
	wsprintf(buf, TEXT("Current Time: %d:%d:%d"), st.wHour, st.wMinute, st.wSecond);//print SYSTEMTIME
	TextOut(hdc, 10, 10, buf, lstrlen(buf));
	wsprintf(buf, TEXT("%8d"), ticks);//print ticks
	TextOut(hdc, 10, 40, buf, lstrlen(buf));
}
void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;//Make PAINTSTRUCT object for drawing
	BeginPaint(hWnd, &ps);//calc unvalid section, register dc.
	OnDraw(ps.hdc);//drawing
	EndPaint(hWnd, &ps);//change to valid section, delete dc.
}

#define IDT_CLOCK 1 //serial id for first Timer(uIDEvent)
#define IDT_TICK 2 //serial id for second Timer(uIDEvent)

VOID CALLBACK OnClock(HWND, UINT, UINT_PTR, DWORD);//function for first Timer's execution
void OnTick(HWND hWnd);//function for second Timer's execution

void OnCreate(HWND hWnd)//SetTimer.
{
	//Timers are made on hWnd(our Window handle.)
	SetTimer(hWnd, IDT_CLOCK, 1000, OnClock);//IDT_CLOCK(Timer serial number we set), per 1 second, Callback to OnClock
	//First Timer sends Message per 1 Second.
	SetTimer(hWnd, IDT_TICK, 1/10, NULL);//IDT_TICK(Timer serial number we set), per 1/100 second, No CallbackProc (***1/10? )
	//Second Timer sends Message per 1/100 Second.
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

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)//wParam for get serial number of Timer
{
	switch(iMessage)//I Wonder The Way Of Sending Message!!!*****_Timer Message is Sended in Timer object int SetTimer's sending message duration. in first time, call WM_CREATE Message and do work after passing one duration
	{
		case WM_CREATE://Create two Timer object.(use execute this program. This MyWndProc is Procedure of window instance we made.)
			OnCreate(hWnd);
			return 0;
			
		case WM_PAINT://when call InvalidRect in OnClock or OnTick(per Message of Timers).
			OnPaint(hWnd);
			return 0;
		case WM_TIMER://If we send null to Proc in SetTimer, make WM_TIMER message.  (p.s First timer automatcally call that's Proc by TIMERPROC OnClock once.)
			OnTimer(hWnd, wParam);
			return 0;
			
		case WM_DESTROY://KillTimer all & WM_QUIT(user click close button)
			OnDestroy(hWnd);
			return 0;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);//recalc, show
}

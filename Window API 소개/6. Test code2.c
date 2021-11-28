//시작하자 마자 현재 시각을 정상적으로 출력하기 위해서는 OnCreate에서 현재 시각을 얻어와야 하지만
//지금은 PostMessage, SendMessage로 WM_TIMER 메시지를 보내면 윈도우 콜백 프로시저에서 WM_TIMER을 처리하기에 그때까지 기다려야 한다.
//고로 Test code2에서는 OnCreate함수에서 GetLocalTime을 호출하여 바로 현재 시각을 얻을 수 있게 하였다. 

#include <Windows.h>
#define MY_DEF_STYLE CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
void RegMyWndClass(LPCWSTR cname);
void MakeWindow(LPCWSTR cname);
void MsgLoop();

INT APIENTRY WinMain(HINSTANCE hIns, HINSTANCE hPrev, LPSTR cmd, INT nSHOW)
{
	RegMyWndClass(TEXT("MyWindow"));//reg property
	MakeWindow(TEXT("MyWindow"));//use property, create
	MsgLoop();
}

void RegMyWndClass(LPCWSTR cname)
{
	WNDCLASS wndclass={0};
	wndclass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hInstance=GetModuleHandle(0);
	wndclass.hIcon=LoadIcon(0, IDI_APPLICATION);
	wndclass.hCursor=LoadCursor(0, IDC_ARROW);
	wndclass.lpfnWndProc=MyWndProc;
	wndclass.lpszMenuName=0;
	wndclass.lpszClassName=cname;
	wndclass.style=MY_DEF_STYLE;
	
	RegisterClass(&wndclass);
}

void MakeWindow(LPCWSTR cname)
{
	HWND hWnd=CreateWindow(cname,
				TEXT("Two Timers"),
				WS_OVERLAPPEDWINDOW,
				100, 30, 700, 600,
				0,
				0,
				GetModuleHandle(0),
				0);//WM_Create? maybe
	ShowWindow(hWnd, SW_SHOW);//WM_PAINT? maybe
}

void MsgLoop()
{
	MSG Message;
	while(GetMessage(&Message, 0, 0, 0))
	{
		DispatchMessage(&Message);
	}
}

SYSTEMTIME st;
int ticks;

VOID CALLBACK OnClock(HWND hWnd, UINT iMessage, UINT_PTR tid, DWORD cnt)
{
	GetLocalTime(&st);
	InvalidateRect(hWnd, 0, 0);
}

void OnTick(HWND hWnd)
{
	ticks++;
	InvalidateRect(hWnd, 0, 0);
}

void OnDraw(HDC hdc)
{
	WCHAR buf[256];
	wsprintf(buf, TEXT("Current Time: %d:%d:%d"), st.wHour, st.wMinute, st.wSecond);
	TextOut(hdc, 10, 10, buf, lstrlen(buf));
	wsprintf(buf, TEXT("%8d"), ticks);
	TextOut(hdc, 10, 40, buf, lstrlen(buf));
}

void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	OnDraw(ps.hdc);//pass PAINTSTRUCT's DC
	EndPaint(hWnd, &ps);
}

#define IDT_CLOCK 1
#define IDT_TICK 2

void OnCreate(HWND hWnd)
{
	SetTimer(hWnd, IDT_CLOCK, 1000, OnClock);//Proc: OnClock
	SetTimer(hWnd, IDT_TICK, 1/10, NULL);//if proc null, send WM_TIMER message to winProc
	GetLocalTime(&st);//GetLocalTime in Creating. for accurancy.
}

void OnTimer(HWND hWnd, DWORD tid)
{
	switch(tid)
	{
		case IDT_TICK: OnTick(hWnd); return;//check second timer's id
	}
}

void OnDestroy(HWND hWnd)
{
	KillTimer(hWnd, IDT_CLOCK);
	KillTimer(hWnd, IDT_TICK);
	PostQuitMessage(0);
}

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
		case WM_CREATE: OnCreate(hWnd); return 0;//In OnCreate, GetLocal for perfect print of time.
		
		case WM_PAINT: OnPaint(hWnd); return 0;//InvalidateRect is called
		case WM_TIMER: OnTimer(hWnd, wParam); return 0;//second call(without proc, post WM_TIMER message)
		
		case WM_DESTROY: OnDestroy(hWnd); return 0;
	}
	
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

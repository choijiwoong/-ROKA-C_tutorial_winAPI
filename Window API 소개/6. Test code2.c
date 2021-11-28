//�������� ���� ���� �ð��� ���������� ����ϱ� ���ؼ��� OnCreate���� ���� �ð��� ���;� ������
//������ PostMessage, SendMessage�� WM_TIMER �޽����� ������ ������ �ݹ� ���ν������� WM_TIMER�� ó���ϱ⿡ �׶����� ��ٷ��� �Ѵ�.
//��� Test code2������ OnCreate�Լ����� GetLocalTime�� ȣ���Ͽ� �ٷ� ���� �ð��� ���� �� �ְ� �Ͽ���. 

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

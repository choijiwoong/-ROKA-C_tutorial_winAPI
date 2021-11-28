#include <Windows.h>
#define MY_DEF_STYLE CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
void RegMyWndClass(LPCWSTR cname);
void MakeWindow(LPCWSTR cname);
void MsgLoop();

INT APIENTRY WinMain(HINSTANCE hIns, HINSTANCE hPrev, LPSTR cmd, INT nShow)
{
	RegMyWndClass(TEXT("MyWindow"));
	MakeWindow(TEXT("MyWindow"));
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
	HWND hWnd=CreateWindow(cname,//Class Name(to MyWindow)
				TEXT("Keyboard Message Printer"),//Title name
				WS_OVERLAPPEDWINDOW,//Window Style
				100, 30, 700, 600,//Left Top Width Height
				0,//handle of parent window
				0,//menu
				GetModuleHandle(0),//module handle
				0);//argument to WM_CREATE
				
	ShowWindow(hWnd, SW_SHOW);//Visualize
}

void MsgLoop()
{
	MSG Message;
	while(GetMessage(&Message, 0, 0, 0))//get message to application queue
	{
		DispatchMessage(&Message);//work callback(do message)
	}
}

void OnDestroy(HWND hWnd)
{
	PostQuitMessage(0);
}

void OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)//Virtual Key Code in wParam, Information in lParam
{
	HDC hdc=GetDC(hWnd);//Get DC
	RECT rt;//get x,y,width, height
	GetClientRect(hWnd, &rt);//GetClientRect to rt passing by pointer
	Rectangle(hdc, 0, 0, rt.right, rt.bottom);//Execute Drawing by rt's setting that we get Client Rect
	//Get DC, Get Client Rect, Execute
	
	WCHAR buf[256];
	wsprintf(buf, TEXT("Virtual Key Code: %#X"), wParam);
	TextOut(hdc, 10, 10, buf, lstrlen(buf));
	wsprintf(buf, TEXT("Scan Code: %#X"), (lParam>>16)&0xFF);//Scan code is in 16~23. extract by shift&mask
	TextOut(hdc, 10, 40, buf, lstrlen(buf));
	
	ReleaseDC(hWnd, hdc);//Delete DC
}

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
		case WM_KEYDOWN://When KEYDOWN, Call onKeyDown with handle, wParam, lParam as arguments
			OnKeyDown(hWnd, wParam, lParam);
			return 0;
		case WM_DESTROY:
			OnDestroy(hWnd);
			return 0;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

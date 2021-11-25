#include <windows.h>
#define MY_FIRST_WND (TEXT("myfirstwnd"))

void RegWindowClass();
void MessageLoop();

INT APIENTRY WinMain(HINSTANCE hIns, HINSTANCE hPrev, LPSTR cmd, INT nShow)
{
	RegWindowClass();//Set & Register Window's class setting
	
	//Make window instance
	HWND hWnd=CreateWindow(MY_FIRST_WND,
							TEXT("Test"),
							WS_OVERLAPPEDWINDOW,
							10,10,1000,800,
							0,
							0,
							hIns,
							0);
	
	ShowWindow(hWnd, nShow);
	MessageLoop();
	
	return 0;
}

void OnDestroy(HWND hWnd)
{
	PostQuitMessage(0);//post msg WM_QUIT to message queue
}

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
		case WM_DESTROY: 
			OnDestroy(hWnd);
			return 0;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

void RegWindowClass()
{
	WNDCLASS wndclass={0};
	wndclass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor=LoadCursor(0, IDC_ARROW);
	wndclass.hIcon=LoadIcon(0, IDI_APPLICATION);
	wndclass.hInstance=GetModuleHandle(0);
	wndclass.lpfnWndProc=MyWndProc;
	wndclass.lpszClassName=MY_FIRST_WND;
	wndclass.style=CS_DBLCLKS;
	
	RegisterClass(&wndclass);
}

void MessageLoop()
{
	MSG Message;
	while(GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
}

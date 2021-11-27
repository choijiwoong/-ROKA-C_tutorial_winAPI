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
	HWND hWnd=CreateWindow(cname, TEXT("Title"), WS_OVERLAPPEDWINDOW, 100,30,700,600, 0, 0, GetModuleHandle(0), 0);
	ShowWindow(hWnd, SW_SHOW);
}
void MsgLoop()
{
	MSG Message;
	while(GetMessage(&Message,0,0,0))
	{
		DispatchMessage(&Message);
	}
}

void OnDestroy(HWND hWnd)
{
	PostQuitMessage(0);
}

void OnLButtonDown(HWND hWnd, WPARAM wParam, WORD x, WORD y)
{
	HDC hdc=GetDC(hWnd);
	RECT rt;
	GetClientRect(hWnd, &rt);
	Rectangle(hdc, 0, 0, rt.right, rt.bottom);
	WCHAR buf[256];
	wsprintf(buf, TEXT("X:%d, Y:%d"), x, y);
	TextOut(hdc, 10, 10, buf, lstrlen(buf));
	wsprintf(buf, TEXT("%#X"), wParam);
	TextOut(hdc, 10, 40, buf, lstrlen(buf));
	
	if(wParam&MK_LBUTTON){
		lstrcpy(buf, TEXT("0x01 LBUTTON ON"));
	}else{
		lstrcpy(buf, TEXT("0x01 LBUTTON OFF"));
	}
	TextOut(hdc, 10, 70, buf, lstrlen(buf));
	
	if(wParam&MK_RBUTTON){
		lstrcpy(buf, TEXT("0x02 RBUTTON ON"));
	}else{
		lstrcpy(buf, TEXT("0x02 RBUTTON OFF"));
	}
	TextOut(hdc, 10, 100, buf, lstrlen(buf));
	
	if(wParam&MK_SHIFT){
		lstrcpy(buf, TEXT("0x04 Shift Key ON"));
	}else{
		lstrcpy(buf, TEXT("0x04 Shift Key OFF"));
	}
	TextOut(hdc, 10, 130, buf, lstrlen(buf));
	
	if(wParam&MK_CONTROL){
		lstrcpy(buf, TEXT("0x08 CTRL Key ON"));
	}else{
		lstrcpy(buf, TEXT("0x08 CTRL Key OFF"));
	}
	TextOut(hdc, 10, 160, buf, lstrlen(buf));
	
	if(wParam&MK_MBUTTON){
		lstrcpy(buf, TEXT("0x10 MBUTTON ON"));
	}else{
		lstrcpy(buf, TEXT("0x10 MBUTTON OFF"));
	}
	TextOut(hdc, 10, 190, buf, lstrlen(buf));
	
	if(wParam&MK_XBUTTON1){
		lstrcpy(buf, TEXT("0x20 XBUTTON1 ON"));
	}else{
		lstrcpy(buf, TEXT("0x20 XBUTTON1 OFF"));
	}
	TextOut(hdc, 10, 220, buf, lstrlen(buf));
	
	if(wParam&MK_XBUTTON2){
		lstrcpy(buf, TEXT("0x40 XBUTTON2 ON"));
	}else{
		lstrcpy(buf, TEXT("0x40 XBUTTON OFF"));
	}
	TextOut(hdc, 10, 250, buf, lstrlen(buf));
	
	
	ReleaseDC(hWnd, hdc);
}

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
		case WM_LBUTTONDOWN:
			OnLButtonDown(hWnd, wParam, LOWORD(lParam), HIWORD(lParam));
			return 0;
		case WM_DESTROY:
			OnDestroy(hWnd);
			return 0;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

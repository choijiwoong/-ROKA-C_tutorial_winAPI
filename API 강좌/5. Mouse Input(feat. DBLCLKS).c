#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="Mouse";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=(WNDPROC)WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	RegisterClass(&WndClass);
	
	hWnd=CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	
	while(GetMessage(&Message, 0, 0, 0)){
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static int x;
	static int y;
	static BOOL bnowDraw=FALSE;
	
	switch(iMessage)
	{
		//For Mouse Drawing
		case WM_LBUTTONDOWN:
			x=LOWORD(lParam);//get x, y
			y=HIWORD(lParam);
			bnowDraw=TRUE;//BoolIsNowDrawing?
			return 0;
		
		case WM_MOUSEMOVE://NO WM_PAINT! it can be removed. so we have to save drawing, but it's some complex now. 
			if(bnowDraw==TRUE)//We have to save All screen to bitmap or have to save movement of mouse by LinkedList.
			{
				hdc=GetDC(hWnd);//Handle for mouse.
				MoveToEx(hdc, x, y, NULL);
				x=LOWORD(lParam);
				y=HIWORD(lParam);
				LineTo(hdc, x, y);
				ReleaseDC(hWnd, hdc);
			}
			return 0;
			
		case WM_LBUTTONUP:
			bnowDraw=FALSE;
			return 0;
			
		//For Erase all screen by DBLCLK
		case WM_LBUTTONDBLCLK://For use This, We have to express CS_DBLCLKS to WinClass.style
			InvalidateRect(hWnd, NULL, TRUE);//For All Screen, Redraw. We don't make WM_PAINT for redrawing.
			return 0;//If we make InvalidateSection, Drawings will be removed by DefWindowsProc.
		
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);//InvalidateRect without WM_PAINT, wrase window to background color
	}
}

/*
	[Mouse Input]
1.	DC필요하다.

	[DBLCLK]
2.	보편적인 더블클릭 메시지보다 CS_DBLCLKS플레그를 사용하는 것이 DBLCLK연속 검출와 속도 면에서 좋다.
	또한 시간간격이나 마우스 포인터의 위치 따위의 규칙을 우리가 만들어 쓸 수 있기 때문이다. 
*/

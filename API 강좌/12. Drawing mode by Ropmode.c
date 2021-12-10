/*
	[��鿡���� �׸��� ���]
1.	��鿡���� �׸���� ��� ���� 0�� 1�� �־� or, copy, and, xor�������� ó���� �����ϸ�, �÷� �׷��� ȯ�濡���� �̿� ���� ������ �����ϴ�.
 
	[�׸��� ����� ����]
1.	���������� default drawing mode�� R2_COPY�̴�.(�����)
	int SetROP2(HDC hdc, int fnDrawMode);//setter
	int GetROP2(HDC hdc)//getter
	Drawing mode�� ������ �� �ִ� ������ fnDrawMode�� ������ ����_R2_BLACK, R2_WHITE, R2_NOP, R2_NOT(����), R2_COPYPEN(�����), R2_NOTCOPYPEN(�� �׸��� ����), R2_MERGEPEN(or), R2_MASKPEN(and), R2_XORPEN(xor)

	[Ropmode]
1.	���� �׸� �� ������带 ����ϴ� ������ Drawing mode�� importance�� �����غ���. 
*/

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="Drawing mode";

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
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	
	RegisterClass(&WndClass);
	
	hWnd=CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	
	while(GetMessage(&Message,0,0,0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static int sx, sy, oldx, oldy;//first location, end point of real drawing line
	int ex, ey;//fluid point of mouse pointer
	static BOOL bNowDraw=FALSE;//is drawing
	HDC hdc;
	
	switch(iMessage)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		
		case WM_LBUTTONDOWN:
			sx=LOWORD(lParam);
			sy=HIWORD(lParam);
			oldx=sx;
			oldy=sy;
			bNowDraw=TRUE;
			return 0;
		
		case WM_LBUTTONUP:
			bNowDraw=FALSE;
			hdc=GetDC(hWnd);//Get DC for draw line
			MoveToEx(hdc, sx, sy, NULL);//draw line at sx, sy
			LineTo(hdc, oldx, oldy);//to oldx, oldy
			return 0;
			
		case WM_MOUSEMOVE:
			if(bNowDraw){
				hdc=GetDC(hWnd);//Get DC
				SetROP2(hdc, R2_NOT);//****If Redraw, Erase previous line by XOR operation. if new drawing occur, previous drawing will be erased
				MoveToEx(hdc, sx, sy, NULL);//draw line at sx, sy
				LineTo(hdc, oldx, oldy);//to oldx, oldy
				ex=LOWORD(lParam);//get current location
				ey=HIWORD(lParam);
				MoveToEx(hdc, sx, sy, NULL);//draw line at sx, sy
				LineTo(hdc, ex, ey);//draw line to current location
				oldx=ex;//set old location to current location. for draw if WM_LBUTTONUP. save all previous location of last movement.
				oldy=ey;
				ReleaseDC(hWnd, hdc);//delete DC
			}
			return 0;
		
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}


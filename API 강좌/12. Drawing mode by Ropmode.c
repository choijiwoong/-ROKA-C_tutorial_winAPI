/*
	[흑백에서의 그리기 모드]
1.	흑백에서의 그리기는 흑과 백이 0과 1로 있어 or, copy, and, xor연산으로 처리가 가능하며, 컬러 그래픽 환경에서도 이와 같은 개념은 동일하다.
 
	[그리기 모드의 종류]
1.	윈도우즈의 default drawing mode는 R2_COPY이다.(덮어쓰기)
	int SetROP2(HDC hdc, int fnDrawMode);//setter
	int GetROP2(HDC hdc)//getter
	Drawing mode를 설정할 수 있는 인자인 fnDrawMode는 다음과 같다_R2_BLACK, R2_WHITE, R2_NOP, R2_NOT(반전), R2_COPYPEN(덮어쓰기), R2_NOTCOPYPEN(새 그림을 반전), R2_MERGEPEN(or), R2_MASKPEN(and), R2_XORPEN(xor)

	[Ropmode]
1.	선을 그릴 때 반전모드를 사용하는 예제로 Drawing mode의 importance를 습득해보자. 
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


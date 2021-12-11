/*
	[bitmap.dsw]
1.	복잡한 그림은 미리 그려진 비트맵을 사용한다. bmp파일로 비트맵 리소스를 만든다. 

	[메모리 DC]
1.	비트맵을 곧바로 DC로 출력한다면 비트맵은 큰 데이터 덩어리이기에 속도 이외에도 여러 문제가 발생할 수 있기에 바로 출력시키기 않고 MemDC를 이용한다.
	메모리 DC는 화면 DC와 동일한 특성을 가지기에 MemDC에 먼저 그림을 그린 후 작업이 완료되면 결과만 화면으로 고속복사하는 방법을 사용한다.
2.	비트맵은 아예 화면 DC에서 선택할 수 없게 되어있어 메모리 DC만이 비트맵을 선택하 수 있다. 메모리DC생성과 선택은 아래의 함수에 따른다.
	HDC CreateCompatibleDC(HDC hdc);//DC의 핸들을 인자로 주면 내부적으로 똑같은 특성의 DC를 메모리에 만들며 그 핸들값을 리턴한다. 
	HBITMAP LoadBitmap(HINSTANCE hInstance, LPCTSTR lpBitmapName);//비트맵을 메모리DC에 선택해주면 동일한 SelectObject를 사용한다.
	// 첫 인수는 비트맵 리소스를 가진 인스턴스의 핸들이고, 두번째는 비트맵 리소스의 이름이다. 
3.	Bitblt는 DC간의 고속 영역 복사를 수행하며 아래의 함수를 따른다.
	BOOL BitBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, HDC hdcSrc, int nXSrc, int nYSrc, DWORD dwRop); 
	복사대상DC, 복사대상의 XYWH, 복사원의 DC, 복사원의 좌표XY, 레지스터 연산방법지정
4.	레지스터 연산방법종류는 아래와 같다. 
	BLACKNESS(대상영역을 검정색으로), DSTINVERT(화면을 반전), MERGECOPY(소스비트맵과 대상을 AND), 
	MERGEPAINT(소스 비트맵과 대상화면을 OR), SRCCOPY(소스를 대상에 복사), WHITENESS(대상을 흰색으로) 
5.	비트맵 출력 완료 후 Memory DC(별도의 함수 DeleteDC로)와 Bitmap(GDI object이기에 DeleteObject로)을 해제해주어야 한다. 

	[StretchBlt]
1.	BitBlt와 마찬가지로 DC간의 복사를 수행하는데 차이점은 복사 후에 크기가 변경된다는 것이다. 복사원의 지정한 영역이 복사대상의 지정한 영역의 크기만큼 확대되어 출력된다.
	쉽게 말하면 걍 지정해준게 대상영역보다 작으면 그만큼 확대된다는 거고 대상영역이 지정한거보다 작으면 축소된다는거. 약간 실수방지느낌. 아래의 함수를 따른다.
	BOOL StretBlt(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest, HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, DWORD dwRop); 
	//복사원XY에서 복사원WYWH로 늘어남 
*/

#include <windows.h>
#include "resource.h"//bmp file's recource file

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="Bitmap";

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
	

	hWnd=CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW|WS_VSCROLL|WS_HSCROLL,
					CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
					NULL, (HMENU)NULL, hInstance, NULL);
					
	ShowWindow(hWnd, nCmdShow);
	
	while(GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, MemDC;
	PAINTSTRUCT ps;
	HBITMAP MyBitmap, OldBitmap;
	
	switch(iMessage)
	{
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
			MemDC=CreateCompatibleDC(hdc);//Initialization of MemDC by CreateCompatibleDC
			MyBitmap=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));//LoadBitmap to MyBitmap(g_hInst) & MAKEINTRESOURCE with bitmap's id
			//Load Bitmap to instance 'g_hInst' to MyBitMap
			OldBitmap=(HBITMAP)SelectObject(MemDC, MyBitmap);//Change MemDC's Bitmap and backup OldBitmap
			//SelectObject MyBitmap to MemDC. Bitmap is already loaded to MemDC. We just have to move MemDC to real DC
			
			BitBlt(hdc, 0, 0, 123, 160, MemDC, 0, 0, SRCCOPY);//copy to hdc(drawing)
			//StretchBlt(hdc, 0, 0, 246, 320, MemDc, 0, 0, 123, 160, SRCCOPY);
			
			SelectObject(MemDC, OldBitmap);//restore with initial bitmap
			DeleteObject(MyBitmap);
			DeleteDC(MemDC);
			EndPaint(hWnd, &ps);
			return 0;
			
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			
		default:
			DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

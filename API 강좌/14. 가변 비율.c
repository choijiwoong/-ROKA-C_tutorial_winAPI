/*
	[가변 비율]
1.	윈도우의 크기에 따라 작업영역도 바뀌게 하는 것은 코드나 좌표는 일정하게 유지하면서 윈도우의 확장을 변경하여 전체 좌표계의 범위를 조정하여 처리한다.(MM_ISOTROPIC, MM_ANISOTROPIC만 가능) 
2.	이때 사용되는 맵핑 모드가 MM_ISOTROPIC(가로세로비율유지)과 MM_ANISOTROPIC(자유로운 변형)이며 아래의 함수는 윈도우와 뷰포트의 확장을 설정하며  예제를 참고한다.
	BOOL SetWindowExtEx(HDC hdc, int nXExtent,  int nYExtent, LPSIZE lpSize);
	BOOL SetViewportExtEx(HDC hdc, int nXExtent, int nYExtent, LPSIZE lpSize);

*/

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="Variadic Ratio";

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
	HDC hdc;
	PAINTSTRUCT ps;
	POINT ar[]={25,10,125,10,140,30,10,30,25,10};
	HBRUSH BrR, BrB, BrY, OldBr;
	RECT rect;
	
	switch(iMessage)
	{
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
			BrR=CreateSolidBrush(RGB(255,0,0));
			BrB=CreateSolidBrush(RGB(0,0,255));
			BrY=CreateSolidBrush(RGB(255,255,0));
			
			SetMapMode(hdc, MM_ANISOTROPIC);//Mapping mode to MM_ANISOTROPIC
			//Set Mapping Mode to MM_ANISOTROPIC that can change window's extention. 
			SetWindowExtEx(hdc, 160, 100, NULL);//Extend window. window's limit is(160, 100) to (0,0)
			//Set Window's Extention to 160, 180
			GetClientRect(hWnd, &rect);
			//Get ClientRect that's in window(160, 180)
			SetViewportExtEx(hdc, rect.right, rect.bottom, NULL);//Extend Viewport. Same limit with Window because we set right&bottom by rect(that is maded by window).
			//Set Viewport's Extendtion to ClientRect's size.
			//So if window's size is changed, than reSetWindowExtEx(Logid location). In that state, Get Client Rect and set Viewport that's location.
			//(Window uses Logic location, Viewport use absolute location!)			
			OldBr=(HBRUSH)SelectObject(hdc, BrR);
			Rectangle(hdc, 20, 30, 130, 90);
			SelectObject(hdc, BrB);
			Polygon(hdc, ar, 5);
			
			SelectObject(hdc, BrY);
			Rectangle(hdc, 30, 40, 60, 70);
			Rectangle(hdc, 90, 40, 120, 70);
			Ellipse(hdc, 135, 5, 155, 25);
			
			SelectObject(hdc, OldBr);
			DeleteObject(BrR);
			DeleteObject(BrB);
			DeleteObject(BrY);
			EndPaint(hWnd, &ps);
			return 0;
			
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

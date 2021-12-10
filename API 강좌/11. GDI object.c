/*
	[GDI 오브젝트]
1.	GDI오브젝트를 모아둔것이 DC이며, GDI는 현재 DC에 선택된 GDI오브젝트를 사용한다. 고로 사용자가 그래픽 출력 전에 DC에 원하는 오브젝트를 선택하므써 다른 모양으로 변경할 수 있다.
	GDI오브젝트는 내부적으로 구조체이지만 우리는 모두 핸들로 관리한다. DC가 BeginPaint나 GetDC함수에 의해 처음 만들어졌을때 default로 선택되는 GDI오브젝트는 아래와 같다.
	(GDI 오브젝느, 핸들 타입, 디폴트)
	펜 HPEN 검정색의 가는 선, 브러시 HBRUSH 흰색, 폰트 HFONT 시스템 글꼴, 비트맵 HBITMAP 선택되지않음, 팔레트 HPALETTE 선택되지않음, 영역 HRGN 선택되지 않음
	
	[스톡 오브젝트]	 
1.	윈도우즈 기본제공 GDI오브젝트이기에 만들지 않아도 사용이 가능하며 파괴 안해도되고 HGDIOBJ getStockObject(int fnObject)로 Handle만 얻어 사용하면 된다. 
	BLACK_BRUSH, GRAY_BRUSH, NULL_BRUSH, WHITE_BRUSH, DKGRAY_BRUSH, LTGRAY_BRUSH, BLACK_PEN, WHITE_PEN, NULL_PEN, ANSI_FIXED_FONT, ANSI_VAR_FONT, DEFAULT_PALETTE

	[색상]
1.	윈도우즈에는 색상값을 표현하기 위해 COLORREF데이터형을 사용한다. typedef DWORD COLORREF(8비트씩(0~255)RGB농도, 상위8비트는 미사용) 
	0xff0000처럼 사용하는데, 주로 RGB매크로 함수를 사용한다. 아래와 같은 기본 매크로 함수가 정의되어 있다. 
*/
#define RGV(r, g, b)	( (COLORREF)(((BYTE)(r) | ((WORD)((BYTE)(g))<<8)) | (((DWORD)(BYTE)(b))<<16)) )
#define GetRValue(rgb)	((BYTE)(rgb))
#define GetGValue(rgb)	((BYTE)(((WORD)(rgb))>>8))
#define GetBValue(rgb)	((BYTE)((rgb)>>16))

/*	[펜]
1.	HPEN CreatePen(int fnPenStyle, int nWidth, COLORREF crColor);
	fnPenStyle_펜의 모양 PS_COLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT
	nWidth_선의 폭 default 굵기는 1
	crColor_선의 색상을 지정하며 COLORREF형이므로 RGB매크로 함수를 이용한다. 
2.	GDI오브젝트들은 사용 후 반드시 삭제해주어야 MemoryLeak을 막을 수 있고, 아래의 함수를 사용한다.
	BOOL DeleteObject(HGDIOBJ hObject);
	삭제하고자하는 GDI오브젝트의 핸들만 넘기면 되고, DC에 현재 선택되어있는 GDI오브젝트는 삭제할 수 없기 때문에 OldPen을 백업해두었다가 다시 세팅하고, 만들어둔 핸들을 삭제한 것이다.
3.	SelectObject(hdc, OldPen);//삭제를 위해 MyPen해제 
	DeleteObject(MyPen); //삭제
	의 코드를 아래의 한줄로 바꿀 수 있다.
	DeleteObject(SelectObjct(hdc, OldPen));//이전 핸들값을 SelectObject가 리턴하면 바로 그 핸들을 삭제
4.	GDI 오브젝트 생성의 일반적인 절차는 아래와 같다.
	핸들의 선언(HPEN MyPen, OldPen)->GDI 오브젝트 생성(MyPen=CreatePen(...))->DC에 선택&이전핸들 백업(OldPen=SelectObject(...))->사용(Rectangle, Ellipse,...)->해제&삭제(DeleteObject(SelectObject(...))) 

	[브러쉬]
1.	사용은 펜과 동일하다
	HBRUSH CreateSolidBrush(COLORREF crColor);//단색의 브러쉬 
	HBRUSH CreateHatchBrush(int fnStyle, COLORREF clrref);//색+무늬 지정 가능.
	fnStyle(무늬)의 종류: HS_BDIAGONAL, HS_CROSS, HS_DIACROSS, HS_FDIAGONAL, HS_HORIZONTAL, HS_VERTICAL
*/	

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="Practice";

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
	//HBRUSH MyBrush, OldBrush;//for saving HBRUSH
	HPEN MyPen, OldPen;//for saving HPEN
	HBRUSH MyBrush, OldBrush;
	
	switch(iMessage)
	{
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
			
			MyBrush=CreateHatchBrush(HS_BDIAGONAL, RGB(255,255,0));
			OldBrush=(HBRUSH)SelectObject(hdc, MyBrush);
		
			//MyBrush=(HBRUSH)GetStockObject(GRAY_BRUSH);//Get handle of GRAY_BRUSH
			//OldBrush=(HBRUSH)SelectObject(hdc, MyBrush);//GDI오브젝트를 DC에 선택할 때. DC의 핸들, GDI 오브젝트의 핸들로 DC에 해당 오브젝트를 선택한다. 
			//HGDIOBJ를 리턴한다.(바꾸기 이전에 있던 값을 리턴하여 OldBrush로 저장. 백업을 위함) 
			MyPen=CreatePen(PS_SOLID, 5, RGB(0,0,255));
			OldPen=(HPEN)SelectObject(hdc, MyPen);//set new pen and backup old pen
			
			Rectangle(hdc, 50, 50, 300, 200);//Draw by new pen
			
			SelectObject(hdc, OldPen);//restore to oldpen
			DeleteObject(MyPen);//delete MyPen
			DeleteObject(SelectObject(hdc, OldBrush));//restore & delete at once
			
			//SelectObject(hdc, OldBrush);//hdc에 OldBrush전달 복구작업 
			EndPaint(hWnd, &ps);
			return 0;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

/*
	[Create Font]
1.	폰트도 GDI오브젝트이다. default로 시스템 폰트를 사용하지만, 폰트 제작 후 DC로 전송하여 문자열 출력하면 적용된다.
2.	폰트 제작은 아래의 함수를 따르며, 리턴값(핸들)을 HFONT형의 변수에 대입해주면 된다.
	HFONT CreateFont(int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight, 
	DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision, 
	DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily, LPCTSTR lpszFace);
	(높이, 폭, 전체문자의 각도, 개별문자의각도, 굵기, 기울임, 밑줄, 관통선, 문자셋(ANSI_CHARSET 등), 출력 정확도, 클리핑 정확도, 
	논리적 폰트를 물리적 폰트에 얼마나 근접시킬 지, 피치&그룹설정, 글꼴) 

	[Font.dsw]
1.	CreateFont함수로 폰트의 모든 설정을 다 전달해 줄수도 있고, LOGFONT구조체를 사용하여 폰트를 정의하고 CreateFontIndirect함수로 폰트 제작하는 방법도 있다.
	LOGFONT는 아래의 구조체를 따른다.
	typedef struct tagLOGFONT{
		LONG lfHeight;
		LONG lfWidth;
		LONG lfEscapement;
		LONG lfOrientation;
		LONG lfWeight;
		BYTE lfItalic;
		BYTE lfUnderline;
		BYTE lfStrikeOut;
		BYTE lfCharSet;
		BYTE lfOutPrecision;
		BYTE lfClipPrecision;
		BYTE lfQuality;
		BYTE lfPitchAndFamily;
		TCHAR lfFaceName[LF_FACESIZE];
	} LOGFONT;
2.	LOGFONT는 CreateFont의 인수 전체를 멤버변수로 가지는 구조체이며, 인자값들을 대입후 아래의 CreateFontIndirect함수로 생성하면 된다.
	HFONT CreateFontIndirect(CONST LOGFONT *lplf); 
	 LOGFONT를 사용하며 얻는 장점은 부분적인 목록의 변경, 사용할 폰트의 LIST화 등이다.
	Win32API에서 CreateFontIndirect처럼 Indirect가 붙은 함수들은 공통적으로 구조체의 포인터를 인수로 취한다.(CreatePenIndirect, CreateBrushIndirect) 

	[문자열의 색상]
1.	폰트 오브젝트 외에도 문자열 정렬 상태를 지정하는 SetTextAlign함수 외에도 아래의 세가지 함수가 있다.
	COLORREF SetTextColor(HDC hdc, COLORREF crColor);
	COLORREF SetBkColor(HDC hdc, COLORREF crColor);//글자뒤쪽 배경색. COLORREF는 RGB매크로함수 이용하면 된다. 
	int SetBkMode(HDC hdc, int iBkMode);//배경색상을 사용할 방법으로 OPAQUE(불투명한 배경_default), TRANSPARENT(투명한 배경)을 제공한다.
2.	당연히 GetTextColor, GetBkColor함수도 있다.	 

	[글자 회전시키기]
1.	CreateFont의 세번째 인수인 nEscapeMent를 변경시키면 된다. 
*/ 

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="First";

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
	HDC hdc;
	PAINTSTRUCT ps;
	HFONT font, oldfont;
	char str[]="폰트 Test 1234 !@#$";
	LOGFONT lf;//way2)
	HBRUSH MyBrush, OldBrush;
	int i;//for rotate
	
	switch(iMessage)
	{
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
			
			MyBrush=CreateHatchBrush(HS_CROSS, RGB(0,0,255));
			OldBrush=(HBRUSH)SelectObject(hdc, MyBrush);
			Rectangle(hdc, 50, 50, 400, 200);//Just Drawing Rectangle
			SelectObject(hdc, OldBrush);
			
			//way1) Make FONT and Select
			//font=CreateFont(50,0,0,0,0,0,0,0,HANGEUL_CHARSET,0,0,0,0,"궁서");//same way with HPEN, HBRUSH 
			//oldfont=(HFONT)SelectObject(hdc, font);
			
			//way2) Make LOGFONT and Make FONT by it, and Select
			lf.lfHeight=25;
			lf.lfWidth=0;
			lf.lfEscapement=0;
			lf.lfOrientation=0;
			lf.lfWeight=0;
			lf.lfItalic=0;
			lf.lfUnderline=0;
			lf.lfStrikeOut=0;
			lf.lfCharSet=HANGEUL_CHARSET;
			lf.lfOutPrecision=0;
			lf.lfClipPrecision=0;
			lf.lfQuality=0;
			lf.lfPitchAndFamily=0;
			strcpy(lf.lfFaceName,"궁서");//set lfFaceName to char* by using strcpy
			
			//Set Font
			font=CreateFontIndirect(&lf);//Make Font
			oldfont=(HFONT)SelectObject(hdc, font);//Set Font with backup
			
			//Change Color of Text&Background
			SetTextColor(hdc, RGB(255,0,0));//Set Text&Back color by COLORREF
			SetBkColor(hdc, RGB(255,255,0));
			TextOut(hdc, 100, 100, str, strlen(str));//default BkMode is OPAQUE
			
			SetBkMode(hdc, TRANSPARENT);
			TextOut(hdc, 100, 150, str, strlen(str));
			
			//Rotate string
			SetTextColor(hdc, RGB(0,0,0));
			for(i=0; i<900; i+=100)
			{
				font=CreateFont(40, 0, i, 0, FW_NORMAL, FALSE, FALSE, FALSE,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, VARIABLE_PITCH|FF_SWISS, "Times New Roman");
				oldfont=(HFONT)SelectObject(hdc, font);
				TextOut(hdc, 0, 450, str, strlen(str));
				SelectObject(hdc, oldfont);
				DeleteObject(font);
			}
			
			
			SelectObject(hdc, oldfont);//restore Font
			DeleteObject(MyBrush);
			//DeleteObject(font);//for for loop
			EndPaint(hWnd, &ps);
			return 0;
			
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

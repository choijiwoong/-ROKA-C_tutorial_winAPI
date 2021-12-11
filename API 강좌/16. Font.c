/*
	[Create Font]
1.	��Ʈ�� GDI������Ʈ�̴�. default�� �ý��� ��Ʈ�� ���������, ��Ʈ ���� �� DC�� �����Ͽ� ���ڿ� ����ϸ� ����ȴ�.
2.	��Ʈ ������ �Ʒ��� �Լ��� ������, ���ϰ�(�ڵ�)�� HFONT���� ������ �������ָ� �ȴ�.
	HFONT CreateFont(int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight, 
	DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision, 
	DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily, LPCTSTR lpszFace);
	(����, ��, ��ü������ ����, ���������ǰ���, ����, �����, ����, ���뼱, ���ڼ�(ANSI_CHARSET ��), ��� ��Ȯ��, Ŭ���� ��Ȯ��, 
	���� ��Ʈ�� ������ ��Ʈ�� �󸶳� ������ų ��, ��ġ&�׷켳��, �۲�) 

	[Font.dsw]
1.	CreateFont�Լ��� ��Ʈ�� ��� ������ �� ������ �ټ��� �ְ�, LOGFONT����ü�� ����Ͽ� ��Ʈ�� �����ϰ� CreateFontIndirect�Լ��� ��Ʈ �����ϴ� ����� �ִ�.
	LOGFONT�� �Ʒ��� ����ü�� ������.
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
2.	LOGFONT�� CreateFont�� �μ� ��ü�� ��������� ������ ����ü�̸�, ���ڰ����� ������ �Ʒ��� CreateFontIndirect�Լ��� �����ϸ� �ȴ�.
	HFONT CreateFontIndirect(CONST LOGFONT *lplf); 
	 LOGFONT�� ����ϸ� ��� ������ �κ����� ����� ����, ����� ��Ʈ�� LISTȭ ���̴�.
	Win32API���� CreateFontIndirectó�� Indirect�� ���� �Լ����� ���������� ����ü�� �����͸� �μ��� ���Ѵ�.(CreatePenIndirect, CreateBrushIndirect) 

	[���ڿ��� ����]
1.	��Ʈ ������Ʈ �ܿ��� ���ڿ� ���� ���¸� �����ϴ� SetTextAlign�Լ� �ܿ��� �Ʒ��� ������ �Լ��� �ִ�.
	COLORREF SetTextColor(HDC hdc, COLORREF crColor);
	COLORREF SetBkColor(HDC hdc, COLORREF crColor);//���ڵ��� ����. COLORREF�� RGB��ũ���Լ� �̿��ϸ� �ȴ�. 
	int SetBkMode(HDC hdc, int iBkMode);//�������� ����� ������� OPAQUE(�������� ���_default), TRANSPARENT(������ ���)�� �����Ѵ�.
2.	�翬�� GetTextColor, GetBkColor�Լ��� �ִ�.	 

	[���� ȸ����Ű��]
1.	CreateFont�� ����° �μ��� nEscapeMent�� �����Ű�� �ȴ�. 
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
	char str[]="��Ʈ Test 1234 !@#$";
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
			//font=CreateFont(50,0,0,0,0,0,0,0,HANGEUL_CHARSET,0,0,0,0,"�ü�");//same way with HPEN, HBRUSH 
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
			strcpy(lf.lfFaceName,"�ü�");//set lfFaceName to char* by using strcpy
			
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

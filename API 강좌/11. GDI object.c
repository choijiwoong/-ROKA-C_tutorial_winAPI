/*
	[GDI ������Ʈ]
1.	GDI������Ʈ�� ��Ƶа��� DC�̸�, GDI�� ���� DC�� ���õ� GDI������Ʈ�� ����Ѵ�. ��� ����ڰ� �׷��� ��� ���� DC�� ���ϴ� ������Ʈ�� �����Ϲǽ� �ٸ� ������� ������ �� �ִ�.
	GDI������Ʈ�� ���������� ����ü������ �츮�� ��� �ڵ�� �����Ѵ�. DC�� BeginPaint�� GetDC�Լ��� ���� ó�� ����������� default�� ���õǴ� GDI������Ʈ�� �Ʒ��� ����.
	(GDI ��������, �ڵ� Ÿ��, ����Ʈ)
	�� HPEN �������� ���� ��, �귯�� HBRUSH ���, ��Ʈ HFONT �ý��� �۲�, ��Ʈ�� HBITMAP ���õ�������, �ȷ�Ʈ HPALETTE ���õ�������, ���� HRGN ���õ��� ����
	
	[���� ������Ʈ]	 
1.	�������� �⺻���� GDI������Ʈ�̱⿡ ������ �ʾƵ� ����� �����ϸ� �ı� ���ص��ǰ� HGDIOBJ getStockObject(int fnObject)�� Handle�� ��� ����ϸ� �ȴ�. 
	BLACK_BRUSH, GRAY_BRUSH, NULL_BRUSH, WHITE_BRUSH, DKGRAY_BRUSH, LTGRAY_BRUSH, BLACK_PEN, WHITE_PEN, NULL_PEN, ANSI_FIXED_FONT, ANSI_VAR_FONT, DEFAULT_PALETTE

	[����]
1.	��������� ������ ǥ���ϱ� ���� COLORREF���������� ����Ѵ�. typedef DWORD COLORREF(8��Ʈ��(0~255)RGB��, ����8��Ʈ�� �̻��) 
	0xff0000ó�� ����ϴµ�, �ַ� RGB��ũ�� �Լ��� ����Ѵ�. �Ʒ��� ���� �⺻ ��ũ�� �Լ��� ���ǵǾ� �ִ�. 
*/
#define RGV(r, g, b)	( (COLORREF)(((BYTE)(r) | ((WORD)((BYTE)(g))<<8)) | (((DWORD)(BYTE)(b))<<16)) )
#define GetRValue(rgb)	((BYTE)(rgb))
#define GetGValue(rgb)	((BYTE)(((WORD)(rgb))>>8))
#define GetBValue(rgb)	((BYTE)((rgb)>>16))

/*	[��]
1.	HPEN CreatePen(int fnPenStyle, int nWidth, COLORREF crColor);
	fnPenStyle_���� ��� PS_COLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT
	nWidth_���� �� default ����� 1
	crColor_���� ������ �����ϸ� COLORREF���̹Ƿ� RGB��ũ�� �Լ��� �̿��Ѵ�. 
2.	GDI������Ʈ���� ��� �� �ݵ�� �������־�� MemoryLeak�� ���� �� �ְ�, �Ʒ��� �Լ��� ����Ѵ�.
	BOOL DeleteObject(HGDIOBJ hObject);
	�����ϰ����ϴ� GDI������Ʈ�� �ڵ鸸 �ѱ�� �ǰ�, DC�� ���� ���õǾ��ִ� GDI������Ʈ�� ������ �� ���� ������ OldPen�� ����صξ��ٰ� �ٽ� �����ϰ�, ������ �ڵ��� ������ ���̴�.
3.	SelectObject(hdc, OldPen);//������ ���� MyPen���� 
	DeleteObject(MyPen); //����
	�� �ڵ带 �Ʒ��� ���ٷ� �ٲ� �� �ִ�.
	DeleteObject(SelectObjct(hdc, OldPen));//���� �ڵ鰪�� SelectObject�� �����ϸ� �ٷ� �� �ڵ��� ����
4.	GDI ������Ʈ ������ �Ϲ����� ������ �Ʒ��� ����.
	�ڵ��� ����(HPEN MyPen, OldPen)->GDI ������Ʈ ����(MyPen=CreatePen(...))->DC�� ����&�����ڵ� ���(OldPen=SelectObject(...))->���(Rectangle, Ellipse,...)->����&����(DeleteObject(SelectObject(...))) 

	[�귯��]
1.	����� ��� �����ϴ�
	HBRUSH CreateSolidBrush(COLORREF crColor);//�ܻ��� �귯�� 
	HBRUSH CreateHatchBrush(int fnStyle, COLORREF clrref);//��+���� ���� ����.
	fnStyle(����)�� ����: HS_BDIAGONAL, HS_CROSS, HS_DIACROSS, HS_FDIAGONAL, HS_HORIZONTAL, HS_VERTICAL
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
			//OldBrush=(HBRUSH)SelectObject(hdc, MyBrush);//GDI������Ʈ�� DC�� ������ ��. DC�� �ڵ�, GDI ������Ʈ�� �ڵ�� DC�� �ش� ������Ʈ�� �����Ѵ�. 
			//HGDIOBJ�� �����Ѵ�.(�ٲٱ� ������ �ִ� ���� �����Ͽ� OldBrush�� ����. ����� ����) 
			MyPen=CreatePen(PS_SOLID, 5, RGB(0,0,255));
			OldPen=(HPEN)SelectObject(hdc, MyPen);//set new pen and backup old pen
			
			Rectangle(hdc, 50, 50, 300, 200);//Draw by new pen
			
			SelectObject(hdc, OldPen);//restore to oldpen
			DeleteObject(MyPen);//delete MyPen
			DeleteObject(SelectObject(hdc, OldBrush));//restore & delete at once
			
			//SelectObject(hdc, OldBrush);//hdc�� OldBrush���� �����۾� 
			EndPaint(hWnd, &ps);
			return 0;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

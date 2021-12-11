/*
	[bitmap.dsw]
1.	������ �׸��� �̸� �׷��� ��Ʈ���� ����Ѵ�. bmp���Ϸ� ��Ʈ�� ���ҽ��� �����. 

	[�޸� DC]
1.	��Ʈ���� ��ٷ� DC�� ����Ѵٸ� ��Ʈ���� ū ������ ����̱⿡ �ӵ� �̿ܿ��� ���� ������ �߻��� �� �ֱ⿡ �ٷ� ��½�Ű�� �ʰ� MemDC�� �̿��Ѵ�.
	�޸� DC�� ȭ�� DC�� ������ Ư���� �����⿡ MemDC�� ���� �׸��� �׸� �� �۾��� �Ϸ�Ǹ� ����� ȭ������ ��Ӻ����ϴ� ����� ����Ѵ�.
2.	��Ʈ���� �ƿ� ȭ�� DC���� ������ �� ���� �Ǿ��־� �޸� DC���� ��Ʈ���� ������ �� �ִ�. �޸�DC������ ������ �Ʒ��� �Լ��� ������.
	HDC CreateCompatibleDC(HDC hdc);//DC�� �ڵ��� ���ڷ� �ָ� ���������� �Ȱ��� Ư���� DC�� �޸𸮿� ����� �� �ڵ鰪�� �����Ѵ�. 
	HBITMAP LoadBitmap(HINSTANCE hInstance, LPCTSTR lpBitmapName);//��Ʈ���� �޸�DC�� �������ָ� ������ SelectObject�� ����Ѵ�.
	// ù �μ��� ��Ʈ�� ���ҽ��� ���� �ν��Ͻ��� �ڵ��̰�, �ι�°�� ��Ʈ�� ���ҽ��� �̸��̴�. 
3.	Bitblt�� DC���� ��� ���� ���縦 �����ϸ� �Ʒ��� �Լ��� ������.
	BOOL BitBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, HDC hdcSrc, int nXSrc, int nYSrc, DWORD dwRop); 
	������DC, �������� XYWH, ������� DC, ������� ��ǥXY, �������� ����������
4.	�������� ������������ �Ʒ��� ����. 
	BLACKNESS(��󿵿��� ����������), DSTINVERT(ȭ���� ����), MERGECOPY(�ҽ���Ʈ�ʰ� ����� AND), 
	MERGEPAINT(�ҽ� ��Ʈ�ʰ� ���ȭ���� OR), SRCCOPY(�ҽ��� ��� ����), WHITENESS(����� �������) 
5.	��Ʈ�� ��� �Ϸ� �� Memory DC(������ �Լ� DeleteDC��)�� Bitmap(GDI object�̱⿡ DeleteObject��)�� �������־�� �Ѵ�. 

	[StretchBlt]
1.	BitBlt�� ���������� DC���� ���縦 �����ϴµ� �������� ���� �Ŀ� ũ�Ⱑ ����ȴٴ� ���̴�. ������� ������ ������ �������� ������ ������ ũ�⸸ŭ Ȯ��Ǿ� ��µȴ�.
	���� ���ϸ� �� �������ذ� ��󿵿����� ������ �׸�ŭ Ȯ��ȴٴ� �Ű� ��󿵿��� �����Ѱź��� ������ ��ҵȴٴ°�. �ణ �Ǽ���������. �Ʒ��� �Լ��� ������.
	BOOL StretBlt(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest, HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, DWORD dwRop); 
	//�����XY���� �����WYWH�� �þ 
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

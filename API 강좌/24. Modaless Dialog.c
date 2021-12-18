/*
	[�𵨸����� ��ȭ����]
1.	�𵨸������� ��ȭ���ڸ� ����ΰ� ���� ������ ������ �����ϱ� ������ �����찡 ����� ���� �ﰢ������ ������ �� �ֵ��� �ؾ��ϱ⿡ �� �����ϴ�.
2.	�𵨸������� ���������� �������� �ʴ� Ư���� ���� DialogBox�Լ��� ������� �ʴ´�. ��� CreateDialog�Լ��� ����Ѵ�. ����� �����ϳ� �̴� ���� ��� ��ȭ������ �ڵ鰪�� �����Ѵ�.
	��ȭ���ڿ� �⺻������ WS_VISIBLE��Ÿ���� ���� ������ ���� �� ShowWindow�� ���� ȭ�鿡 ��Ÿ���� �ؾ��Ѵ�. 
3.	���������� �����쿡�� �ٽ� ��ȭ���ڸ� ȣ���� ����̴�. ��� WM_LBUTTONDOWN���� ��ȭ���ڸ� ����� ���� hDlg�� ��ȿ�� �ڵ����� ���� �����Ѵ�. ����� �Ʒ��� �Լ��� ������.
	BOOL IsWindow(HWND hWnd);
4.	�������� ����� ��ȭ���� ��ư�� ������ ������ ����. (OK, ID_CHANGE, Change), (Cancel, ID_CLOSE, Close) 
*/

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="Mless";

int x, y;
char str[128];
HWND hDlg;

HWND hMainWnd;//for access to main window on CreateDialog&DialogBox

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
	hMainWnd=hWnd;//Set Main Window's Handle
	
	while(GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

BOOL CALLBACK MlessDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)//Callback Procedure.
{
	switch(iMessage)
	{
		case WM_INITDIALOG:
			SetDlgItemText(hDlg, IDC_STR, str);
			SetDlgItemInt(hDlg, IDC_X, x, FALSE);
			SetDlgItemInt(hDlg, IDC_Y, y, FALSE);
			return TRUE;
			
		case WM_COMMAND:
			switch(wParam)
			{
				case ID_CHANGE:
					GetDlgItemText(hDlg, IDC_STR, str, 128);
					x=GetDlgItemInt(hDlg, IDC_X, NULL, FALSE);
					y=GetDlgItemInt(hDlg, IDC_Y, NULL, FALSE);
					InvalidateRect(hMainWnd, NULL, TRUE);
					return TRUE;
					
				case ID_CLOSE:
					DestroyWindow(hDlg);//CreateDialog-DestroyWindow
					hDlg=NULL;
					return true;
			}
			breaK;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
		case WM_LBUTTONDOWN:
			if(!IsWindow(hDlg))
			{
				hDlg=CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, MlessDlgProc);
				ShowWindow(hDlg, SW_SHOW);
			}
			return 0;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

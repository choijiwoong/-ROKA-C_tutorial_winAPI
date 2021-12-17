/*
	[��Ʈ���� �� �б�]
1.	��ȭ���ڿ� �θ� �����찣�� ���� ��ȯ ������� �Ʒ��� �Լ��� ������, ���ڿ��� ������ �����͸� ��ȯ�Ѵ�.
	UINT GetDlgItemText(HWND hDlg, int nlDDlgItem, LPTSTR lpString, int nMaxCount);
	BOOL SetDlgItemText(HWND hDlg, int nlDDlgItem, LPCTSTR lpString);
	������ �ڵ�, ��Ʈ���� ID, �����ϰ��� ���ڿ�(Ȥ�� ���� ����), �б� �� nMaxCount�� ������ ũ�� ����
	
	UINT GetDlgItemInt(HWND hDlg, int nlDDlgItem, BOOL *lpTranslated, BOOL bSigned);
	BOOL SetDlgItemInt(HWND hDlg, int nlDDigItem, UINT uValue, BOOL bSigned);
	������ �ڵ�, ��Ʈ�� ID, �����˻�(�ʿ������ NULL), �������Ȯ�� 
	
	[InfoDlg]
1.	�����̸��̴�. �������� x, y, str�� �̿��Ͽ� ���� �ְ�޴� Dialog�� ����� ���� ��ǥ���� �Է��ϴ� Dialog(Insert/Resource/Dialog)�� ����� InfoDig.rc�� ���Խ�Ų��.
 
*/ 

#include <windows.h>
#include <resource.h>//Dialog_InfoDlg

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="InfoDlg";

int x, y;//for get, set(Dialog value)
char str[128];

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

BOOL CALLBACK InfoDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
		case WM_INITDIALOG://like WM_CREATE
			SetDlgItemText(hDlg, IDC_STR, str);//set initial value by defined value(str, x, y)
			SetDlgItemInt(hDlg, IDC_X, x, FALSE);
			SetDlgItemInt(hDlg, IDC_Y, y, FALSE);
			//show current set value
			return TRUE;
			
		case WM_COMMAND:
			switch(wParam)
			{
				case IDOK://push OK button
					GetDlgItemText(hDlg, IDC_STR, str, 128);//Get IDC_STR value to str
					x=GetDlgItemInt(hDlg, IDC_X, NULL, FALSE);//Get IDC_X value to x
					y=GetDlgItemInt(hDlg, IDC_Y, NULL, FALSE);
					EndDialog(hDlg, 1);//end with return value TRUE
					return TRUE;
					
				case IDCANCEL://push Cancel button
					EndDialog(hDlg, 0);//end with return value FALSE
					return TRUE;
			}
			break;
	}
	return FALSE;//else, FALSE wrong use of InfoDlgProc
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	
	switch(iMessage)
	{
		case WM_CREATE:
			x=100;
			y=100;
			strcpy(str, "String");
			return 0;
			
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
			TextOut(hdc, x, y, str, strlen(str));//print str to (x, y)
			EndPaint(hWnd, &ps);
			return 0;
			
		case WM_LBUTTONDOWN:
			if(DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, InfoDlgProc)==1)//Call DialogBox with InfoDlgProc. if TRUE(Well work)
				InvalidateRect(hWnd, NULL, TRUE);//redraw by x, y, str
			return 0;
			
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

/*
	[����ڿ��� ��ȭ]
1.	����ڿ��� ��ȭ�� ���� ���ϰ� �ϱ� ���� ��Ʈ�ѵ��� ��ġ�Ǵ� ��ȭ���ڸ� �̿��Ѵ�. 
	Modal���� Ÿ ��������ȯ�� �Ұ��ϸ�, Modaless�� ��ȯ�� �����ϴ�(���α׷����� �� ��ٷӴ�).

	[About]
1.	��ȭ���ڸ� ����� ���ؼ� ���ø��� ���ν����� �ʿ��ϴ�. ������ Insert/Resource/Dialog���� �����Ѵ�. rc������ ������ �� ���ν����� �ۼ��Ѵ�.
	WndProc���� ��ȭ���� ���ν����� �����ϹǷ� WndProc���� ��������. 
2.	��ȭ���� ȣ�� �� �Ʒ��� �Լ��� ������.
	int DialogBox(HINSTANCE hInstance, LPCTSTR lpTemplate, HWND hWndParent, DLGPROC lpDialogFunc); 
	_��ȭ���� ���ҽ��� ���� �ν��Ͻ��� �ڵ�, ���ø��� ���ҽ�ID, �θ� ������, ��ȭ���� ���ν��� 

	[��ȭ���� ���ν���]
1.	��ȭ���� ���ν����� ������ ���ν����� �������� ���ϰ��� ����(LRESULT(long), BOOL), �޾Ƶ��̴� �޽���(WM_CREATE, WM_INITDIALOG)�̴�. 
	�ַ� Dialog���� WM_COMMAND�� ���� ó���ϴµ�, LOWORD(wParam)�� ��Ʈ���� ID, HIWORD(wParam)�� notification code�� ���޵ȴ�.
2.	OK��ư�̳� CANCEL��ư�� ������� EndDialog�Լ��� ȣ���ϴµ�, ������ �Ʒ��� ����.
	BOOL EndDialog(HWND hDlg, int nResult);
	 nResult�� DialogBox�Լ��� ���ϰ����� ���޵ȴ�.(�����Լ�) 
	 
	
	[��Ʈ���� ����]
-	��ȭ���� �����⿡ ������(���콺 ��), �׸����� ���� Ʈ��, ����Ʈ �� ���� ��Ʈ���� �ֱ⿡ �̵�(���α׷���, ��Ű, Ʈ�� ��)�� ������ ���θ� �ؾ� ����� ����� �� �ִ�. 
	[��ġ]
-	�巡���Ͽ� ��ġ�� ũ�⸦ �����ϸ� �ȴ�. Ctrl�� ����ä�� ��Ʈ���� �����ϸ� ���� ��Ʈ���� ���� �� ��ġ�� �� �ִ�.
	[����] [ũ�� ���� �� �̵�]
-	���ڸ� ���� �����̳� ũ�� ������ ���ϰ� �� �� �ִ�. �̴� Layout/Guide Settings�� ����Ѵ�.
	[����] [�Ӽ� ����]
-	�Ӽ� ������ ���ϴ� ��Ʈ�� �˾� �޴��� Properties�� �����Ͽ� ID, ����Ű ���� ������ �� �ִ�.
	[����] [�� ���� ����]
-	Layout/Tab Order�� ��Ʈ���� TabOrder�� ������ �� �ִ�. 
	[�׽�Ʈ]
-	����� ���� ������ �׽�Ʈ�� ���� Layout/Test�� �ش� ��ȭ���ڸ� ������ �� �ִ�. 
*/

#include <windows.h>
#include "resource.h"//Dialog

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="Modal, Modaless";

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

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
		case WM_INITDIALOG:
			return TRUE;
		
		case WM_COMMAND:
			switch(wParam)
			{
				case IDOK:
				case IDCANCEL:
					EndDialog(hDlg, 0);
					return TRUE;
			}
			break;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
		case WM_LBUTTONDOWN:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc);
			//Make Dialog by IDD_DIALOG1 template that is defined g_hInst, with AboutDlgProc procdure.
			return 0;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

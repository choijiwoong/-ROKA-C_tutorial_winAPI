/*
	[모델리스형 대화상자]
1.	모델리스형은 대화상자를 열어두고 메인 윈도우 조작이 가능하기 때문에 윈도우가 변경된 값을 즉각적으로 인지할 수 있도록 해야하기에 더 복잡하다.
2.	모델리스형은 종료전까지 리턴하지 않는 특성을 가진 DialogBox함수를 사용하지 않는다. 대신 CreateDialog함수를 사용한다. 사용은 동일하나 이는 만든 즉시 대화상자의 핸들값을 리턴한다.
	대화상자에 기본적으로 WS_VISIBLE스타일이 없기 때문에 생성 후 ShowWindow로 직접 화면에 나타나게 해야한다. 
3.	주의할점은 윈도우에서 다시 대화상자를 호출한 경우이다. 고로 WM_LBUTTONDOWN에서 대화상자를 만들기 전에 hDlg이 유효한 핸들인지 먼저 조사한다. 조사는 아래의 함수를 따른다.
	BOOL IsWindow(HWND hWnd);
4.	예제에서 사용할 대화상자 버튼의 구성은 다음과 같다. (OK, ID_CHANGE, Change), (Cancel, ID_CLOSE, Close) 
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

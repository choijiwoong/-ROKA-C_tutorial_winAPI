/*
	[ApiStart]
1.	사용하는게 거의 동일하니 아예 기본 코드를 복붙해서 사용하자. 이는 CD_ROM의 ApiPrj디렉토리에 ApiStart.txt로 존재한다.

	[AppWizard]
1.	이는 Visual C++의 사용자의 주문을 받아 원하는 골격을 만들어주는 프로젝트 생성툴이다. 
2.	A typical "Hello World!" application항목을 선택하면 생성되는 골격은 아래와 같다. 
	그냥 참고만 하자... 나 어차피 vs없자나.. 
*/

#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADING];
TCHAR szWindowClass[MAX_LOADING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;
	
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_AWPRJ, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	
	if(!InitInstance(hInstance, nCmdShow))
		return FALSE:
	
	hAccelTable=LoadAccelerator(hInstance, (LPCTSTR)IDC_AWPRJ);
	
	while(GetMessage(&msg, NULL, 0, 0))
	{
		if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	
	wcex.cbSize=sizeof(WNDCLASSEX);
	wcex.style=CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc=(WNDPROC)WndProc;
	wcex.cbClsExtra=0;
	wcex.cbWndExtra=0;
	wcex.hInstance=hInstance;
	wcex.hIcon=LoadIcon(hInstance, (LPCTSTR)IDI_AWPRJ);
	wcex.hCursor=LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName=(LPCSTR)IDC_AWPRJ;
	wcex.lpszClassName=szWindowClass;
	wcex.hIconSm=LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);
	
	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst=hInstance;
	
	hWnd=CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
			NULL, NULL, hInstance, NULL);
	
	if(!hWnd)
		return FALSE;
		
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmld, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	
	switch(message)
	{
		case WM_COMMAND:
			wmld=LOWORD(wParam);
			wmEvent=HIWORD(wParam);
			
			switch(wmid)
			{
				case IDM_ABOUT:
					DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
					break;
				
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
	}
	
	return 0;
}

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG:
			return TRUE;
		
		case WM_COMMAND:
			if(LOWORD(wParam)==IDOK||LOWORD(wParam)==IDCANCEL)
			{
				EndDialog(hDig, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
	return FALSE;
}

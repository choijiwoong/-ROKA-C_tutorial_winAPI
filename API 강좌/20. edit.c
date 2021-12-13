/*
	[����Ʈ]
1.	�Ʒ��� ���� ��Ÿ�ϰ� �Բ� edit������ Ŭ������ ����� �� �ִ�. �Ʒ��� ��Ÿ���� CreateWindow�� ����° �μ��� �����Ѵ�.
	ES_AUTOHSCROLL, ES_AUTOVSCROLL, ES_LEFT, ES_CENTER, ES_RIGHT, ES_LOWERCASE, ES_UPPERCASE, ES_NULTILINE, ES_NOHIDESEL, ES_READONLY
2.	�� ����Ʈ child window�� �θ� ������� �Ʒ��� ���� notification msg�� �����ش�.
	EN_CHANGE(�����), EN_ERRSPACE, EN_HSCROLL, EN_VSCROLL, EN_KILLFOCUS, EN_SETFOCUS, EN_MAXTEXT, EN_UPDATE(����Ǳ� �����̴�.)
	(����Ʈ�� ���ڿ��� ����Ǹ� EN_UPDATE�� ������, ȭ�鿡 redrawing�� �ڿ� EN_CHANGE�� ������. �� ���̴� ������ EN_CHANGE�� ���� ����Ѵ�.) 
	(�ִ� 32k���� ������ �����ϴ�) 
3.	BOOL SetWindowText(HWND hWnd, LPCTSTR lpString);//������&��Ʈ���� �ڵ�, �ٲ� ���ڿ� 
	int GetWindowText(HWND hWnd, LPTSTR lpString, int nMaxCount);//������&��Ʈ���� �ڵ�, ����, ����ũ�� 
	
	[��Ʈ�ѵ� �������]
1.	Ÿ��Ʋ �ٰ� ���⿡ �̵� �Ұ����� ���ϵ� ������� ������ �޽����� ó���� �� �ִ�(��ư������ ������ Ƽ��). WM_PAINTó���Ͽ� �����θ� �����Ҽ��ִ�. 
	���� üũ�ڽ��� BM_SETCHECK�޽����� ������ wParam�� ���� �ڽ��� üũ���¸� ������ �����Ų��.
2.	��� Window�� ���밡���� �Լ����� ��κ��� ��Ʈ�ѿ� ������ �� �ִ�. 
*/

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="Edit";

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

#define ID_EDIT 100
HWND hEdit;
char str[128];

int nTop=10;//for set dinamically edit's location
BOOL bShow=TRUE;//for set hide or show of edit
 
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	char str[]="���� Ŭ��: ����Ʈ �̵�, ������ Ŭ��: ����/����";
	
	switch(iMessage)
	{
		case WM_CREATE:
			hEdit=CreateWindow("edit", NULL, WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL,
					10,10,200,25, hWnd, (HMENU)ID_EDIT, g_hInst, NULL);//id is set to ID_EDIT(100)
			SetWindowText(hEdit, "����Ʈ�� �������.");//Set Text to hEdit
			return 0;
		
		case WM_COMMAND://string input to edit
			switch(LOWORD(wParam))//for get id of WM_COMMAND(event)
			{
				case ID_EDIT://id of hEdit(100)
					switch(HIWORD(wParam))//for get Message content
					{
						case EN_CHANGE://content change
							GetWindowText(hEdit, str, 128);//Get Text to str from hEdit. 
							//Because Control is also window, so we can use function of window management
							SetWindowText(hWnd, str);//set to str's content
							//Get Text to str from hEdit & Set str to hWnd?
					}
			}
			return 0;
		
		case WM_LBUTTONDOWN:
			nTop+=10;
			MoveWindow(hEdit, 10, nTop, 200, 25, TRUE);
			return 0;
		
		case WM_RBUTTONDOWN:
			if(bShow){
				bShow=FALSE;
				ShowWindow(hEdit, SW_HIDE);
			}else{
				bShow=TRUE;
				ShowWindow(hEdit, SW_SHOW);
			}
			return 0;
			
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
			TextOut(hdc, 200, 100, str, strlen(str));
			EndPaint(hWnd, &ps);
			return 0;
			
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

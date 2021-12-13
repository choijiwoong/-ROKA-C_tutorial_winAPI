/*
	[에디트]
1.	아래와 같은 스타일과 함께 edit윈도우 클래스를 사용할 수 있다. 아래의 스타일은 CreateWindow의 세번째 인수로 지정한다.
	ES_AUTOHSCROLL, ES_AUTOVSCROLL, ES_LEFT, ES_CENTER, ES_RIGHT, ES_LOWERCASE, ES_UPPERCASE, ES_NULTILINE, ES_NOHIDESEL, ES_READONLY
2.	이 에디트 child window는 부모 윈도우로 아래와 같은 notification msg를 보내준다.
	EN_CHANGE(변경됨), EN_ERRSPACE, EN_HSCROLL, EN_VSCROLL, EN_KILLFOCUS, EN_SETFOCUS, EN_MAXTEXT, EN_UPDATE(변경되기 직전이다.)
	(에디트는 문자열이 변경되면 EN_UPDATE를 보내고, 화면에 redrawing한 뒤에 EN_CHANGE를 보낸다. 별 차이는 없으나 EN_CHANGE를 많이 사용한다.) 
	(최대 32k까지 편집이 가능하다) 
3.	BOOL SetWindowText(HWND hWnd, LPCTSTR lpString);//윈도우&컨트롤의 핸들, 바꿀 문자열 
	int GetWindowText(HWND hWnd, LPTSTR lpString, int nMaxCount);//윈도우&컨트롤의 핸들, 버퍼, 버퍼크기 
	
	[컨트롤도 윈도우다]
1.	타이틀 바가 없기에 이동 불가능한 차일드 윈도우며 스스로 메시지를 처리할 수 있다(버튼누르면 누른거 티냄). WM_PAINT처리하여 스스로를 복구할수있다. 
	예로 체크박스는 BM_SETCHECK메시지를 받으면 wParam에 따라 자신의 체크상태를 스스로 변경시킨다.
2.	고로 Window에 적용가능한 함수들은 대부분의 컨트롤에 적용할 수 있다. 
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
	char str[]="왼쪽 클릭: 에디트 이동, 오른쪽 클릭: 보임/숨김";
	
	switch(iMessage)
	{
		case WM_CREATE:
			hEdit=CreateWindow("edit", NULL, WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL,
					10,10,200,25, hWnd, (HMENU)ID_EDIT, g_hInst, NULL);//id is set to ID_EDIT(100)
			SetWindowText(hEdit, "에디트도 윈도우다.");//Set Text to hEdit
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

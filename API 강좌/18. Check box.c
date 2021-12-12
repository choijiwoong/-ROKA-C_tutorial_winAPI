/*
	[체크 박스의 종류]
1.	일반 버튼과 마찬가지로 차일드윈도우이며 클래스 이름도 button을 사용하되 스타일에 BS_PUSHBUTTON대신 체크박스 스타일을 지정해준다ㅣ.
	BS_CHECKBOX와 BS_3STATE(선택, 비선택, Grayed)가 있다. 
	 자동 체크박스와 수동 체크박스로 구분되는데, 체크박스의 상태가 변경될 때 마다 어떤 처리를 해주어야 한다면 수동체크박스를, 필요할때 조사만 할 것이라면 자동 체크박스를 사용한다. 

	[컨트롤의 메시지]
1.	컨트롤 변화시에 부모 윈도우로 BN_CLICKED같은 notification message를 보낸다. 반대로 부모윈도우가 체크박스를 세팅하거나 알아볼땐 차일드윈도우로 메시지를 보낸다.
	BM_GETCHECK, BM_SETCHECK메시지가 있고, BM_GETCHECK로 인해 리턴되는 값에는 BST_CHECKED, BST_UNCHECKED, BST_INDETERMINATE상태가 있다.
2.	체크박스가 클릭되면 BN_CLICKED notification MSG가 WM_COMMAND를 통해 전달되므로 LOWORD(wParam)을 통해 id를 얻는다.
3.	Button 0의 경우 체크상태 변경시마다 ELLIPSE변수값 변경과 redraw해야되기에 자동 체크 스타일을 부적당하다. 
*/

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="Check Button";

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
	static HWND c1, c2, c3, c4;
	static BOOL ELLIPSE=FALSE;
	
	switch(iMessage)
	{
		case WM_CREATE:
			c1=CreateWindow("button", "Draw Ellipse?", WS_CHILD|WS_VISIBLE|BS_CHECKBOX,//BS_CHECKBOX
					20,20,160,25, hWnd, (HMENU)0, g_hInst, NULL);
			c2=CreateWindow("button", "Good bye Message?", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,//BS_AUTOCHECKBOX
					20,50,160,25, hWnd, (HMENU)1, g_hInst, NULL);
			c3=CreateWindow("button", "3State", WS_CHILD|WS_VISIBLE|BS_3STATE,//BS_3STATE
					20,80,160,25, hWnd, (HMENU)2, g_hInst, NULL);
			c4=CreateWindow("button", "Auto 3State", WS_CHILD|WS_VISIBLE|BS_AUTO3STATE,//BS_AUTO3STATE
					20,110,160,25, hWnd, (HMENU)3, g_hInst, NULL);
			return 0;
			
		case WM_COMMAND://button's command
			switch(LOWORD(wParam))//we don't have to hand about autobutton
			{
				case 0:
					if(SendMessage(c1, BM_GETCHECK,0,0)==BST_UNCHECKED){//button 0 is unchecked
						SendMessage(c1, BM_SETCHECK, BST_CHECKED, 0);//set check
						ELLIPSE=TRUE;
					} else{//button 0 is checked
						SendMessage(c1, BM_SETCHECK, BST_UNCHECKED, 0);//set unchecked
						ELLIPSE=FALSE;
					}
					InvalidateRect(hWnd, NULL, TRUE);//redrawing
					break;
					
				case 2:
					if(SendMessage(c3, BM_GETCHECK,0,0)==BST_UNCHECKED)//button 2 is unchecked
						SendMessage(c3, BM_SETCHECK, BST_CHECKED, 0);//set check
					else if(SendMessage(c3, BM_GETCHECK,0,0)==BST_INDETERMINATE)//button 2 is indeterminate
						SendMessage(c3, BM_SETCHECK, BST_UNCHECKED, 0);//set unchecked
					else//button 2 is checked
						SendMessage(c3, BM_SETCHECK, BST_INDETERMINATE, 0);//set indeterminate
					break;
			}
			return 0;
		
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
			if(ELLIPSE==TRUE)//check ELLIPSE is true(button 0 set check)
				Ellipse(hdc, 200,100,400,200);
			else
				Rectangle(hdc, 200,100,400,200);
			EndPaint(hWnd, &ps);
			return 0;
			
		case WM_DESTROY:
			if(SendMessage(c2, BM_GETCHECK,0,0)==BST_CHECKED)//if button 1 is checked
				MessageBox(hWnd, "Good bye", "Check", MB_OK);//Make MessageBox
			PostQuitMessage(0);
			return 0;
			
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

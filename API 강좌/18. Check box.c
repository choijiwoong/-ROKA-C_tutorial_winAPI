/*
	[üũ �ڽ��� ����]
1.	�Ϲ� ��ư�� ���������� ���ϵ��������̸� Ŭ���� �̸��� button�� ����ϵ� ��Ÿ�Ͽ� BS_PUSHBUTTON��� üũ�ڽ� ��Ÿ���� �������ش٤�.
	BS_CHECKBOX�� BS_3STATE(����, ����, Grayed)�� �ִ�. 
	 �ڵ� üũ�ڽ��� ���� üũ�ڽ��� ���еǴµ�, üũ�ڽ��� ���°� ����� �� ���� � ó���� ���־�� �Ѵٸ� ����üũ�ڽ���, �ʿ��Ҷ� ���縸 �� ���̶�� �ڵ� üũ�ڽ��� ����Ѵ�. 

	[��Ʈ���� �޽���]
1.	��Ʈ�� ��ȭ�ÿ� �θ� ������� BN_CLICKED���� notification message�� ������. �ݴ�� �θ������찡 üũ�ڽ��� �����ϰų� �˾ƺ��� ���ϵ�������� �޽����� ������.
	BM_GETCHECK, BM_SETCHECK�޽����� �ְ�, BM_GETCHECK�� ���� ���ϵǴ� ������ BST_CHECKED, BST_UNCHECKED, BST_INDETERMINATE���°� �ִ�.
2.	üũ�ڽ��� Ŭ���Ǹ� BN_CLICKED notification MSG�� WM_COMMAND�� ���� ���޵ǹǷ� LOWORD(wParam)�� ���� id�� ��´�.
3.	Button 0�� ��� üũ���� ����ø��� ELLIPSE������ ����� redraw�ؾߵǱ⿡ �ڵ� üũ ��Ÿ���� �������ϴ�. 
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

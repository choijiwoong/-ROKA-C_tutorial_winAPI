/*
	[���� ��ư]
1.	buttonŬ������ BS_RADIOBUTTON, BS_AUTORADIOBUTTON���� ������ �� �ִ�. ���� ��ư�� ���� ����(�׷�)�� �ϳ��� ������ �� ����Ѵ�.
2.	ù��° ��ư���� WS_GROUP��Ÿ���� �ְ� �������� �� ��Ÿ���� ���� ������ �ȴ�. �׷�ڽ��� BS_GROUPBOX��Ÿ���� ���� ��ư������ �������⸸ �Ѵ�.
3.	�Ʒ��� �������� ������ r1, r4�� WS_GROUP��Ÿ���� �����Ǿ� ������ �����Ѵ�. ó�� ���õ� ���� ��ư�� �����ϴ� ���� �Ʒ��� �Լ��� ������.
	BOOL CheckRadioButton(HWND hDlg, int nlDFirstButton, int nlDLastbutton, int nlDCheckButton);
	ó���� ���� ��ư�� ������ �θ� �������� �ڵ�, �ι�° ����°�� �� �׷��� ���� ��ư�� �� ��ư�̸�, 4��° ��ư�� ���õ� ��ư�� ID�̴�. 
4.	�Ʒ��� ���� ���� ��ư�� �ڵ� ���� ��ư�̱⿡ GRAPH, COLOR�������� WM_COMMAND���� �� �ƴ϶� WM_COMMAND������ �׳� InvalidateRect�� ȣ���ϰ�
	WM_PAINT���� �׸��� ������ ���� ��ư�� ���¸� �����ϴ� ����� ����� ���� �ִ�. 
*/ 

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="Radio Button";

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

#define ID_R1 101
#define ID_R2 102
#define ID_R3 103
#define ID_R4 104
#define ID_R5 105
#define ID_R6 106

HWND r1,r2,r3,r4,r5,r6;
int GRAPH=0;
int COLOR=0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH MyBrush, OldBrush;

	switch(iMessage)
	{
		case WM_CREATE:
			CreateWindow("button", "Graph", WS_CHILD|WS_VISIBLE|BS_GROUPBOX, //make GroupBox
					5,5,120,110, hWnd, (HMENU)0, g_hInst, NULL);
			CreateWindow("button", "Color", WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
					145,5,120,110, hWnd, (HMENU)1, g_hInst, NULL);
					
			r1=CreateWindow("button", "Color", WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|WS_GROUP,//to first GroupBox
					10,20,100,30, hWnd, (HMENU)ID_R1, g_hInst, NULL);
			r2=CreateWindow("button", "Ellipse", WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,
					10,50,100,30, hWnd, (HMENU)ID_R2, g_hInst, NULL);
			r3=CreateWindow("button", "Line", WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,
					10,80,100,30, hWnd, (HMENU)ID_R3, g_hInst, NULL);
			r4=CreateWindow("button", "Black", WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|WS_GROUP,//to Second GroupBox
					150,20,100,30, hWnd, (HMENU)ID_R4, g_hInst, NULL);
			r5=CreateWindow("button", "Red", WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,
					150,50,100,30, hWnd, (HMENU)ID_R5, g_hInst, NULL);
			r6=CreateWindow("button", "Blue", WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,
					150,80,100,30, hWnd, (HMENU)ID_R6, g_hInst, NULL);
			
			CheckRadioButton(hWnd, ID_R1, ID_R3, ID_R1);//Make CheckRadioButton with R1,2,3
			CheckRadioButton(hWnd, ID_R4, ID_R6, ID_R4);//Start_R4, END_R6, checked_R4
			break;
			
		case WM_COMMAND://by checkbox in checkradiobutton
			if(HIWORD(wParam)==BN_CLICKED){//if clicked
				switch(LOWORD(wParam))//find id of button
				{
					case ID_R1:
						GRAPH=0;//Rectangle
						break;
					
					case ID_R2:
						GRAPH=1;//Ellipse
						break;
						
					case ID_R3:
						GRAPH=2;//Line
						break;
						
					case ID_R4:
						COLOR=0;//Black
						break;
					
					case ID_R5:
						COLOR=1;//Red
						break;
					
					case ID_R6:
						COLOR=2;//Blue
						break;
				}
				InvalidateRect(hWnd, NULL, TRUE);//redraw. in drawing part, change drawing by COLOR, GRAPH
			}
			return 0;
			
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);//Make DC
			
			switch(COLOR)//Make MyBrush by COLOR
			{
				case 0:
					MyBrush=CreateSolidBrush(RGB(0,0,0));
					break;
				
				case 1:
					MyBrush=CreateSolidBrush(RGB(255,0,0));
					break;
				
				case 2:
					MyBrush=CreateSolidBrush(RGB(0,0,255));
					break;
			}
			OldBrush=(HBRUSH)SelectObject(hdc, MyBrush);//select & backup
			
			switch(GRAPH)//Make Figure by GRAPH
			{
				case 0:
					Rectangle(hdc, 10, 200, 200, 300);
					break;
				
				case 1:
					Ellipse(hdc, 10, 200, 200, 300);
					break;
				
				case 2:
					MoveToEx(hdc, 10, 200, NULL);
					LineTo(hdc, 200, 300);
					break;
			}
			
			SelectObject(hdc, OldBrush);
			DeleteObject(MyBrush);
			EndPaint(hWnd, &ps);
			return 0;
			
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

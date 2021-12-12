/*
	[라디오 버튼]
1.	button클래스에 BS_RADIOBUTTON, BS_AUTORADIOBUTTON으로 설정할 수 있다. 라디오 버튼은 여러 사항(그룹)중 하나만 선택할 때 사용한다.
2.	첫번째 버튼에만 WS_GROUP스타일을 주고 나머지는 이 스타일을 주지 않으면 된다. 그룹박스도 BS_GROUPBOX스타일을 갖는 버튼이지만 보여지기만 한다.
3.	아래의 구현에서 선두의 r1, r4에 WS_GROUP스타일이 설정되어 있음을 주의한다. 처음 선택될 라디오 버튼을 지정하는 것은 아래의 함수를 따른다.
	BOOL CheckRadioButton(HWND hDlg, int nlDFirstButton, int nlDLastbutton, int nlDCheckButton);
	처음은 라디오 버튼을 가지는 부모 윈도우의 핸들, 두번째 세번째는 각 그룹의 시작 버튼과 끝 버튼이며, 4번째 버튼은 선택될 버튼의 ID이다. 
4.	아래의 예제 라디오 버튼은 자동 라디오 버튼이기에 GRAPH, COLOR변수값을 WM_COMMAND에서 뿐 아니라 WM_COMMAND에서는 그냥 InvalidateRect만 호출하고
	WM_PAINT에서 그리기 직전에 라디오 버튼의 상태를 조사하는 방법을 사용할 수도 있다. 
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

/*
	[스크롤 바]
1.	scrollbar윈도우 클래스로 생성하며 수평 스크롤 바는 SBS_HORZ, 수직 스크롤 바는 SBS_VERT스타일을 지정한다.
	스크롤바의 범위와 위치값 지정은 아래의 함수를 따른다.
	BOOL SetScrollRange(HWND hWnd, int nBar, int nMinPos, int nMaxPos, BOOL bRedraw);
	스크롤바의 핸들, 메인에 부착된 스크롤 바 혹은 별도(CBC_STL)의 스크롤바, 최소, 최대, repaint할지여부 
	
	int SetScrollPos(HWND hWnd, int nBar, int nPos, BOOL bRedraw); 
	스크롤 핸들, ,현재 위치응 nPos에 지정, repaint여부 
2.	이벤트가 있어야 부모 윈도우로 메시지를 보내는 보통의 컨트롤과 달리, WM_HSCROLL(수평), WM_VSCROLL(수직)일 경우를 별도의 메시지로 보낸다.
	LOWORD(wParam)_스크롤 바 내의 어디를 눌렀는지, HIWORD(wParam)_현재 위치, lParam(스크롤 바 핸들)
	ㄴLOWORD(wParam)의 값은 다음과 같다. SB_LINELEFT(SB_LINE_UP), SB_LINERIGHT(SB_LINEDOWN), SB_PAGELEFT(SB_PAGEUP), SB_PAGERIGHT(SB_PAGEDOWN),
	SB_THUMBPOSITION, SB_THUMBTRACK
	(<(SB_LINELEFT)----(SB_PAGELEFT)----ㅁ----(SB_PAGERIGHT)--->(SB_LINERIGHT)) 
	
	[스태틱]
1.	가장 간단한 컨트롤로, 문자열을 보여주기만 한다. 윈도우 클래스를 static으로 설정해주면 된다.
	여러개의 스태틱 컨트롤의 아이디를 같게 해도 된다. 잘 사용되지 않지만 스타일의 종류는 아래와 같다. default는 SS_LEFT이다.
	SS_LEFT, SS_LEFTNOWORDWRAP, SS_CENTER, SS_RIGHT, SS_SIMPLE, SS_WHILEFRAME, SS_WHITERECTm SS_BLACKFRAME, SS_BLACKRECT, SS_GRAYFRAME, SS_GRAYRECT, SS_ICON, SS_NOPREFIX
2.	일반 TextOut은 언제든지 지워질 수 있기에 WM_PAINT에서 계속 출력해야하는데, 스태틱은 스스로 메시지 처리가능한 윈도우이기에 신경쓰지 않아도 된다.	 
*/

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="scroll bar with RGB";

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

#define ID_SCRRED 100
#define ID_SCRGREEN 101
#define ID_SCRBLUE 102
HWND hRed, hGreen, hBlue;
int Red, Green, Blue;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH MyBrush, OldBrush;
	int TempPos;
	
	switch(iMessage)
	{
		case WM_CREATE:
			hRed=CreateWindow("scrollbar", NULL, WS_CHILD|WS_VISIBLE|SBS_HORZ,//SBS_HORZ
					10,10,200,20, hWnd, (HMENU)ID_SCRRED, g_hInst, NULL);
			hGreen=CreateWindow("scrollbar", NULL, WS_CHILD|WS_VISIBLE|SBS_HORZ,
					10,40,200,20, hWnd, (HMENU)ID_SCRGREEN, g_hInst, NULL);
			hBlue=CreateWindow("scrollbar", NULL, WS_CHILD|WS_VISIBLE|SBS_HORZ,
					10,70,200,20, hWnd, (HMENU)ID_SCRBLUE, g_hInst, NULL);
					
			SetScrollRange(hRed, SB_CTL, 0, 255, TRUE);//0~255
			SetScrollPos(hRed, SB_CTL, 0, TRUE);//to 0
			SetScrollRange(hGreen, SB_CTL, 0, 255, TRUE);
			SetScrollPos(hGreen, SB_CTL, 0, TRUE);
			SetScrollRange(hBlue, SB_CTL, 0, 255, TRUE);
			SetScrollPos(hBlue, SB_CTL, 0, TRUE);
			
			//static
			CreateWindow("static", "Only Text", WS_CHILD|WS_VISIBLE,
					120,260,100,25, hWnd, (HMENU)-1, g_hInst, NULL);
			
			return 0;
			
		case WM_HSCROLL://after created
			if((HWND)lParam==hRed) TempPos=Red;//Get ID
			if((HWND)lParam==hGreen) TempPos=Green;
			if((HWND)lParam==hBlue) TempPos=Blue;
			
			switch(LOWORD(wParam))
			{
				case SB_LINELEFT:
					TempPos=max(0, TempPos-1);
					break;
				
				case SB_LINERIGHT:
					TempPos=min(255, TempPos+1);
					break;
				
				case SB_PAGELEFT:
					TempPos=max(0, TempPos-10);
					break;
				
				case SB_PAGERIGHT:
					TempPos=min(255, TempPos+10);
					break;
				
				case SB_THUMBTRACK:
					TempPos=HIWORD(wParam);
					break;
			}
			
			if((HWND)lParam==hRed) Red=TempPos;//Little Spaghetti..
			if((HWND)lParam==hGreen) Green=TempPos;
			if((HWND)lParam==hBlue) Blue=TempPos;
			
			SetScrollPos((HWND)lParam, SB_CTL, TempPos, TRUE);//set scroll with one of many events
			InvalidateRect(hWnd, NULL, FALSE);//refraw
			
			return 0;
			
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
			MyBrush=CreateSolidBrush(RGB(Red, Green, Blue));//We just use global variables on RGB for BRUSH color
			OldBrush=(HBRUSH)SelectObject(hdc, MyBrush);
			Rectangle(hdc, 10, 100, 210, 200);
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

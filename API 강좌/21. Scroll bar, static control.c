/*
	[��ũ�� ��]
1.	scrollbar������ Ŭ������ �����ϸ� ���� ��ũ�� �ٴ� SBS_HORZ, ���� ��ũ�� �ٴ� SBS_VERT��Ÿ���� �����Ѵ�.
	��ũ�ѹ��� ������ ��ġ�� ������ �Ʒ��� �Լ��� ������.
	BOOL SetScrollRange(HWND hWnd, int nBar, int nMinPos, int nMaxPos, BOOL bRedraw);
	��ũ�ѹ��� �ڵ�, ���ο� ������ ��ũ�� �� Ȥ�� ����(CBC_STL)�� ��ũ�ѹ�, �ּ�, �ִ�, repaint�������� 
	
	int SetScrollPos(HWND hWnd, int nBar, int nPos, BOOL bRedraw); 
	��ũ�� �ڵ�, ,���� ��ġ�� nPos�� ����, repaint���� 
2.	�̺�Ʈ�� �־�� �θ� ������� �޽����� ������ ������ ��Ʈ�Ѱ� �޸�, WM_HSCROLL(����), WM_VSCROLL(����)�� ��츦 ������ �޽����� ������.
	LOWORD(wParam)_��ũ�� �� ���� ��� ��������, HIWORD(wParam)_���� ��ġ, lParam(��ũ�� �� �ڵ�)
	��LOWORD(wParam)�� ���� ������ ����. SB_LINELEFT(SB_LINE_UP), SB_LINERIGHT(SB_LINEDOWN), SB_PAGELEFT(SB_PAGEUP), SB_PAGERIGHT(SB_PAGEDOWN),
	SB_THUMBPOSITION, SB_THUMBTRACK
	(<(SB_LINELEFT)----(SB_PAGELEFT)----��----(SB_PAGERIGHT)--->(SB_LINERIGHT)) 
	
	[����ƽ]
1.	���� ������ ��Ʈ�ѷ�, ���ڿ��� �����ֱ⸸ �Ѵ�. ������ Ŭ������ static���� �������ָ� �ȴ�.
	�������� ����ƽ ��Ʈ���� ���̵� ���� �ص� �ȴ�. �� ������ ������ ��Ÿ���� ������ �Ʒ��� ����. default�� SS_LEFT�̴�.
	SS_LEFT, SS_LEFTNOWORDWRAP, SS_CENTER, SS_RIGHT, SS_SIMPLE, SS_WHILEFRAME, SS_WHITERECTm SS_BLACKFRAME, SS_BLACKRECT, SS_GRAYFRAME, SS_GRAYRECT, SS_ICON, SS_NOPREFIX
2.	�Ϲ� TextOut�� �������� ������ �� �ֱ⿡ WM_PAINT���� ��� ����ؾ��ϴµ�, ����ƽ�� ������ �޽��� ó�������� �������̱⿡ �Ű澲�� �ʾƵ� �ȴ�.	 
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

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="Key";

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
	
	hWnd=CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
				NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	
	while(GetMessage(&Message,0,0,0)){
		TranslateMessage(&Message);//Make MSG WM_CHAR 
		DispatchMessage(&Message);
	}
	
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	//for WM_CHAR
	static char str[256];//Save WM_CHARstatic variable because it's WndProc's local variable
	int len;//str's length
	//for WM_KEYDOWN
	static int x=100;
	static int y=100;
	
	switch(iMessage)
	{
		case WM_CHAR:
			//Ver1. Simple
			//len=strlen(str);
			//str[len]=(TCHAR)wParam;//CHAR is in wParam. we can use it by casting to TCHAR
			//str[len+1]=0;//expression of \n
			//InvalidateRect(hWnd, NULL, FALSE);//redrawing. make WM_PAINT MSG
			//return 0;
			
			//Ver2. handling Spacebar
			if((TCHAR)wParam==32)//if wParam is space
				str[0]=0;//erase str
			else{
				len=strlen(str);
				str[len]=(TCHAR)wParam;
				str[len+1]=0;
			}
			InvalidateRect(hWnd, NULL, TRUE);//TRUE for redrawing all.
			
			return 0;
		
		case WM_KEYDOWN://We can also use WM_KEYUP
			switch(wParam)//for reflection of location
			{
				case VK_LEFT:
					x-=8;
					break;
				case VK_RIGHT:
					x+=8;
					break;
				case VK_UP:
					y-=8;
					break;
				case VK_DOWN:
					y+=8;
					break;
			}
			InvalidateRect(hWnd, NULL, TRUE);//if FALSE, we can see trajectory of A
			return 0;
		
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
			//For WM_CHAR test
			TextOut(hdc, 100, 100, str, strlen(str));//print content of str
			//For WM_KEYDOWN test
			TextOut(hdc, x, y, "A", 1);
			EndPaint(hWnd, &ps);
			return 0;
			
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

/*
	[키보드 입력] 
1.	키보드 입력과 동시에 str없이 출력할 수 있지만 키보드의 입력을 WM_CHAR에서 str로, 출력을 WM_PAINT로 나누어 처리하는 
	이유는 항상 화면을 다시 그릴 준비를 해두어야 하는데 그러한 화면 복구와 저장은 WM_PAINT에서 이루어지기 때문이다.
	만약 WM_CHAR에서 바로 출력을 해버리면 window가 가려지는 순간 화면 복구를 한다하더라도 글자 복구가 불가능하다. 
2.	wParam은 아스키값을 받고, lParam은 총 32비트의 공간에 스캔코드, 반복 카운트 등 기타 정보를 갖고 있다.
	lParam중 필요한 정보가 있으면 사용하면 되고, 없으면 wParam만 사용하면 된다. lParam을 사용하는 경우는 드물다. 

	[무효화 영역]
1.	WM_PAINT가 호출되는 것은 다시 그려져야할 필요가 있다는 것으로 다른말로 Invalid Region(원래의 모습과 다른 모습)이 있다는 것이다. 
	즉, 화면이 가려졌다가 가려진게 치워지면, 그 영역을 무효로 만든 뒤 다시 그려지게 해준다. 
	우리는 InvalidateRect로 변경된 부분을 강제로 무효화 시켜 WM_PAINT를 호출할 수 있다. 원형은 아래와 같다.
	
	BOOL InvalidateRect(HWND hWnd, CONST RECT *lpRect, BOOL bErase);
	첫 인수로는 무효화 대상이 되는 윈도우의 핸들이며, 기존의 윈도우를 다시 그릴 경우 WndProc에서 받은 hWnd를 그대로 사용하면 된다.
	lpRect는 무효화의 대상이 되는 사각 영역인데, NULL값이면 모든 영역이 무효화(속도느려짐) 된다.
	bErase는 background Erase느낌으로다가 배경도 지우고 다시 그릴건지 배경은 냅두고 그릴건지 전달하는데 FALSE면 배경안지우고 그린다. 
	(Space바 입력시 문자열을 초기화하는 경우, InvalidateRect의 세번째 인자를 TRUE로 설정해야지 다 지워진다.)

	[WM_KEYDOWN]
1.	WM_CHAR외의 키는 WM_KEYDOWN메시지를 사용한다. VK_BACK, VK_TAB, VK_RETURN, VK_SHIFT, VK_CONTROL, CK_MENU
	VK_PAUSE, VK_CAPITAL, VK_ESCAPE, VK_SPACE, VK_PRIOR, VK_NEXT, VK_END, WK_HOME, VK_LEFT, VK_UP 등이 있다.
	
	[TraslateMessage]
1.	키보드에서 A를 눌렀다 떼는 메시지의 발생 과정은 WM_KEYDOWN->WM_CHAR->WM_KEYUP인데, 키보드는 DOWN, UP메시지만 발생시키고
	WM_CHAR은 MsgLoop에서 TraslateMessage(&Message)에서 인위적으로 생성된다. TranslateMessage는 키가 CHAR이면 WM_CHAR을 MSG queue에 덧붙인다.
	이는 DispatchMessage에 의해 WndProc으로 보내진다. 고로 TranslateMessage가 없으면 WM_CHAR은 WndProc에 전달되지 않는다. 
*/

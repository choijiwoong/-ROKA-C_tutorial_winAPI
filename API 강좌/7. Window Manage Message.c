#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="Now, Do Memory.";

HANDLE hTimer1, hTimer2;
char *str;

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
	
	while(GetMessage(&Message,0,0,0))
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
	static RECT rt;
	
	switch(iMessage)
	{
		//move code to  WM_SIZE
		//case WM_CREATE://To WM_SIZE 
			//GetClientRect(hWnd, &rt);//Get Client information to &rt
			//return 0;
			
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
			SetTextAlign(hdc, TA_CENTER);//Set Flags.
			TextOut(hdc, rt.right/2, rt.bottom/2, "Center String in Client Area", 28);
			EndPaint(hWnd, &ps);
			return 0;
			
		case WM_SIZE:
			GetClientRect(hWnd, &rt);
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		
		//No GetClientRect, Use lParam Version
		//case WM_SIZE:
			//rt.right=LOWORD(lParam);
			//rt.bottom=HIWORD(lParam); 
			//InvalidateRect(hWnd, NULL, TRUE);
			//return 0;
			
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			
		default:
			DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

/*
	[생성 및 파괴]
1.	기존에 WM_CREATE, WM_DESTROY를 사용하여 초기화와 종료처리를 하는 방법 외에도, WinMain에서 직업 초기화와 종료처리를 할 수 있다. 
	WM_CREATE는 CreateWindow에 의해 생성된 후 발생하기에 그 뒤에 넣고, WM_DESTROY가 발상하면 MessageLoop가 끝나기에 Loop뒤에 종료처리를 해주면 된다. 
	 고로 WM_CREATE&WM_DESTROY는 특정 윈도우에 관한 초기/종료처리에 좋고, 메인윈도우에서 처리하는 것은 전역적인 조기/종료처리를 하는 것이 권장된다.
	
	[작업 영역]
1.	Client Area와 Non Client Area로 윈도우가 구성되어 있는데, 프로그래밍의 대상은 Client Area에 한정된다. Non Client Area는 운영체제가 관리하기 때문이다.
	고로 원하는 위치에 출력하려면 전체 윈도우의 좌표가 아닌 작업 영역의 좌표를 조사해야하는데 이때 사용되는 함수의 원형은 아래와 같다.
	
	BOOL GetClientRect(HWND hWnd, LPRECT lpRect);//window handle, pointer of RECT for getting return
	
	[WM_SIZE]
1.	윈도우 크기가 변경되었을 때 보내지면 lParam의 하위워드엔 변경 후의 폭, 상위엔 높이가 전달되며
	wParam에는 메시지 발생 이유를 나타내는 플레그가 전달된다.
	wParam에 전달되는 플래그는 다음과 같다. SIZE_MAXHIDE, SIZE_MAIMIZED, SIZE_MAXSHOW, SIZE_MINIMIZED, SIZE_RESTORED
	근데 사실 사용 잘 안한다. 
2.	WM_CREATE할떄도 윈도우가 새로 뜨는거기에 GetClientRect계산 및 WM_PAINT호출을 아예 WM_SIZE에서 할 수도 있다. 
3.	GetClientRect로 rt를 설정할 수도 있고, 어차피 lParam에 변경된 작업 영역 크기가 전달되기 때문에 GetClientRect함수 없이 lParam으로 rt를 바로 세팅해도 된다. 

	[WM_MOVE]
1.	WM_SIZE와 유사한 메시지로 윈도우가 이동될 때 마다 lParam의 하위에 X, 상위에 Y를 담아 전달하는데, 이동할때 특별한 일을 하지 않는이상 사용안한다. 
*/

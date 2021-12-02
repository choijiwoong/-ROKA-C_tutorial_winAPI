#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="MyTimer";

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
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	
	return Message.wParam;
}

#include <time.h> //For Function about Time
LRESULT FAR PASCAL WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	time_t mytime;
	static HANDLE hTimer;
	static char *str;
	
	static RECT rt={100, 100, 400, 120};//for reduce time for redrawing
	//We have to calculate Rectangle Section for perfection.
	
	switch(iMessage)
	{
		case WM_CREATE://When Window is created. for initialization(like window constructor...?ㅋ) 
			hTimer=(HANDLE)SetTimer(hWnd, 1, 1000, NULL);//Make Tiemr
			str="";//For prevention of print garbage value in first time
			SendMessage(hWnd, WM_TIMER, 1, 0);//For print Time to screen befor making MW_TIMER automatically.
			//WM_TIMER's wParam is ID of Timer. 
			return 0;
		
		case WM_TIMER:
			time(&mytime);//Get time(WM_TIMER's called)
			str=ctime(&mytime);//for convert to String
			InvalidateRect(hWnd, &rt, TRUE);//REDRAWING just on designed section in rt
			return 0;
		
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
			TextOut(hdc, 100, 100, str, strlen(str)-1);
			EndPaint(hWnd, &ps);
			return 0;
		
		case WM_DESTROY:
			KillTimer(hWnd, 1);//Remove Timer 
			PostQuitMessage(0);
			return 0;
			
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

/*
	[Timer]
1.	사용자의 동작으로부터 유발되는 메시지가 아닌 것중 대표적인 것이 WM_TIMER이다. 이는 한번 지정하면
	일정한 시간간격을 두고 연속적으로 계속 발생한다.(주기를 나누어 같은 동작은 반복해야하거나 나누어 해야할 일이 있을 때) Timer을 만드는 SetTimer의 원형은 아래와 같다. 

	UINT SetTimer(HWND hWnd, UINT nIDEvent, UINT uElapse, TIMERPROC, lpTimerFunc)
	hWnd는 타이머 메시지르 받을 윈도우이다. 두번쨰는 타이머의 번호를 지정하고(WM_TIMER에서 여러 타이머를 구분하기 위해. 하나면 NULL로 사용안하면 됨)
	uElapase는 1/1000단위로 타이머의 주기를 설정하며, lpTimerFunc는 타이머 메시지가 발생할 때마다 호출될 함수를 지정한다. 사용하지 않으면 NULL로 설정한다.
	 WM_TIMER은 wParam에 Timer's ID, lParma에 TimerProc가 저장되어 전달된다. time와 ctime함수의 원형은 아래와 같다.
	 
	time_t time(tiem_t *timer);
	char *ctime(const time_t *timer); 
2.	타이머는 전역 시스템 자원이기에 한번 설정하면 윈도우가 파괴되어도 계속 남아있는다. 고로 WM_DESTROY시 타이머를 직접 파괴해주어야 한다.
	
	BOOL KillTimer(HWND hWnd, UINT uIDEvent);
	hWnd는 Timer을 소유한 윈도우 핸들이고, uIDEvent는 타이머 ID이다.
	(참고로 SetTimer의 리턴값은 타이머가 윈도우 소유없이 만들어졌을때_SetTimer의 첫번째 인자가 NULL일때 사용된다.) 

	[SendMessage]
1.	위의 프로그램은 WM_TIMER최초호출시간이 WM_CREATE후 1초후이므로 시작하자마자 시간이 안보인다는 문제가 있어,
	고로 CM_CREATE에서 SendMessage로 WM_TIMER을 강제로 발생시켜 문제가 해결되게 하자. SendMessage의 원형은 아래와 같다.
	
	LRESULT SendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	세번째, 네번째 인자는 메시지의 추가정보인 wParam, lParam이다. 어떤 정보인지는 MSG에 따라 다른데,
	WM_TIMER의 경우 wParam으로 ID를 넘기기에 1을 넘겨주자. 
2.	두번째 문제는 화면의 깜빡임이 보인다는 건데, 이는 InvalidateRect(hWnd, NULL, TRUE)에서 NULL설정때문에 모든
	윈도우를 다시 수정하기에 그렇다. 고로 무효화 영역을 최소화하여 Redrawing시간을 줄이자. 
*/

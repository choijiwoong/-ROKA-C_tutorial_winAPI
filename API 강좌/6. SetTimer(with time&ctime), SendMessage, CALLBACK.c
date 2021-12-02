#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="MyTimer & RandGrp";

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

void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	HDC hdc;
	int i;
	hdc=GetDC(hWnd);
	for(i=0; i<3000; i++)
		SetPixel(hdc, rand()%1300, rand()%1000, RGB(rand()%256, rand()%256, rand()%256));
	ReleaseDC(hWnd, hdc);
}

#include <time.h> //For Function about Time(ctime)
LRESULT FAR PASCAL WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	time_t mytime;
	static HANDLE hTimer, hTimer2, hTimer3;
	static char *str;
	
	static RECT rt={100, 100, 400, 120};//for reduce time for redrawing
	//We have to calculate Rectangle Section for perfection.
	
	switch(iMessage)
	{
		case WM_CREATE://When Window is created. for initialization(like window constructor...?ㅋ) 
			hTimer=(HANDLE)SetTimer(hWnd, 1, 1000, NULL);//Make Tiemr
			hTimer2=(HANDLE)SetTimer(hWnd, 2, 5000, NULL);//Second Timer
			hTimer3=(HANDLE)SetTimer(hWnd, 3, 100, (TIMERPROC)TimerProc);//Third Timer with TimerProcedure
			str="";//For prevention of print garbage value in first time
			SendMessage(hWnd, WM_TIMER, 1, 0);//For print Time to screen befor making MW_TIMER automatically.
			//WM_TIMER's wParam is ID of Timer. 
			return 0;
		
		case WM_TIMER://We don't have to make case for hTimer3! It's WM_TImer is passed to it's procedure(TimerProc)
			switch(wParam)
			{
				case 1:
					time(&mytime);//Get time(WM_TIMER's called)
					str=ctime(&mytime);//for convert to String
					InvalidateRect(hWnd, &rt, TRUE);//REDRAWING just on designed section in rt
					break;
				case 2:
					MessageBeep(MB_OK);//Make Beep per 5 seconds
					break;
			}
			return 0;
		
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
			TextOut(hdc, 100, 100, str, strlen(str)-1);
			EndPaint(hWnd, &ps);
			return 0;
		
		case WM_DESTROY:
			KillTimer(hWnd, 1);//Remove Timer 
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
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

	[두 개의 타이머]
1.	WM_TIMER하나로 처리하기에 wParam의 ID로 구분하여 작업을 처리하게 하자. 
2.	16비트 윈도우즈에서는 최대 16개까지 설치가 가능하다. 

	이 강의 너무 마음에 드네.. 예제 연결될 수 있게 해놨어..
	
	[Timer CALLBACK Procedure]
1.	SetTimer의 TIMERPROC lpTimerFunc가 NULL이면 첫번째 인수인 hWnd로 WM_TIMER이 전달되는데, 이를 설정해두면 매 시간마다 이 함수가 대신 호출된다.
	
	VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
	윈도우핸들, WM_TIMER, ID, 실행후 경과시간이 인자인데 사용 잘안한다. 
2.	만약 SetPixel에서 무한 루프를 돌려버리면 도트는 잘 찍히지만 그 외에 다른 작업을 아무것도 못한다. WM_PAINT에서 다른 어떤 메시지도 받을 수 없는 상태가 되었기 때문이다. 
	(win95부터 선점형 멀티태스킹으로 다른작업 전환 가능. procedure없이 WM_TIMER로도 구현이 가능하지만 콜백이 더 정확한 시간에 호출이 된다. 
3.	CALLBACK function은 보통의 API함수들은 application에서 필요할 때 운영체제의 함수(TextOut, SetTimer...)를 사용하는 것과 달리
	운영체제가 필요할때 application에서 호출하는 함수이다. 즉 호출방식이 거꾸로기에 callback function인 것이다.
	callback fucntion은 운영체제에 의해 호출되는 프로그램 내부의 함수인데, WndProc은 운용체제만이 이 함수를 호출한다. 
*/

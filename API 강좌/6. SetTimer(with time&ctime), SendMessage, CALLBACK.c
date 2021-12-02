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
		case WM_CREATE://When Window is created. for initialization(like window constructor...?��) 
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
1.	������� �������κ��� ���ߵǴ� �޽����� �ƴ� ���� ��ǥ���� ���� WM_TIMER�̴�. �̴� �ѹ� �����ϸ�
	������ �ð������� �ΰ� ���������� ��� �߻��Ѵ�.(�ֱ⸦ ������ ���� ������ �ݺ��ؾ��ϰų� ������ �ؾ��� ���� ���� ��) Timer�� ����� SetTimer�� ������ �Ʒ��� ����. 

	UINT SetTimer(HWND hWnd, UINT nIDEvent, UINT uElapse, TIMERPROC, lpTimerFunc)
	hWnd�� Ÿ�̸� �޽����� ���� �������̴�. �ι����� Ÿ�̸��� ��ȣ�� �����ϰ�(WM_TIMER���� ���� Ÿ�̸Ӹ� �����ϱ� ����. �ϳ��� NULL�� �����ϸ� ��)
	uElapase�� 1/1000������ Ÿ�̸��� �ֱ⸦ �����ϸ�, lpTimerFunc�� Ÿ�̸� �޽����� �߻��� ������ ȣ��� �Լ��� �����Ѵ�. ������� ������ NULL�� �����Ѵ�.
	 WM_TIMER�� wParam�� Timer's ID, lParma�� TimerProc�� ����Ǿ� ���޵ȴ�. time�� ctime�Լ��� ������ �Ʒ��� ����.
	 
	time_t time(tiem_t *timer);
	char *ctime(const time_t *timer); 
2.	Ÿ�̸Ӵ� ���� �ý��� �ڿ��̱⿡ �ѹ� �����ϸ� �����찡 �ı��Ǿ ��� �����ִ´�. ��� WM_DESTROY�� Ÿ�̸Ӹ� ���� �ı����־�� �Ѵ�.
	
	BOOL KillTimer(HWND hWnd, UINT uIDEvent);
	hWnd�� Timer�� ������ ������ �ڵ��̰�, uIDEvent�� Ÿ�̸� ID�̴�.
	(����� SetTimer�� ���ϰ��� Ÿ�̸Ӱ� ������ �������� �����������_SetTimer�� ù��° ���ڰ� NULL�϶� ���ȴ�.) 

	[SendMessage]
1.	���� ���α׷��� WM_TIMER����ȣ��ð��� WM_CREATE�� 1�����̹Ƿ� �������ڸ��� �ð��� �Ⱥ��δٴ� ������ �־�,
	��� CM_CREATE���� SendMessage�� WM_TIMER�� ������ �߻����� ������ �ذ�ǰ� ����. SendMessage�� ������ �Ʒ��� ����.
	
	LRESULT SendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	����°, �׹�° ���ڴ� �޽����� �߰������� wParam, lParam�̴�. � ���������� MSG�� ���� �ٸ���,
	WM_TIMER�� ��� wParam���� ID�� �ѱ�⿡ 1�� �Ѱ�����. 
2.	�ι�° ������ ȭ���� �������� ���δٴ� �ǵ�, �̴� InvalidateRect(hWnd, NULL, TRUE)���� NULL���������� ���
	�����츦 �ٽ� �����ϱ⿡ �׷���. ��� ��ȿȭ ������ �ּ�ȭ�Ͽ� Redrawing�ð��� ������. 

	[�� ���� Ÿ�̸�]
1.	WM_TIMER�ϳ��� ó���ϱ⿡ wParam�� ID�� �����Ͽ� �۾��� ó���ϰ� ����. 
2.	16��Ʈ ����������� �ִ� 16������ ��ġ�� �����ϴ�. 

	�� ���� �ʹ� ������ ���.. ���� ����� �� �ְ� �س���..
	
	[Timer CALLBACK Procedure]
1.	SetTimer�� TIMERPROC lpTimerFunc�� NULL�̸� ù��° �μ��� hWnd�� WM_TIMER�� ���޵Ǵµ�, �̸� �����صθ� �� �ð����� �� �Լ��� ��� ȣ��ȴ�.
	
	VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
	�������ڵ�, WM_TIMER, ID, ������ ����ð��� �����ε� ��� �߾��Ѵ�. 
2.	���� SetPixel���� ���� ������ ���������� ��Ʈ�� �� �������� �� �ܿ� �ٸ� �۾��� �ƹ��͵� ���Ѵ�. WM_PAINT���� �ٸ� � �޽����� ���� �� ���� ���°� �Ǿ��� �����̴�. 
	(win95���� ������ ��Ƽ�½�ŷ���� �ٸ��۾� ��ȯ ����. procedure���� WM_TIMER�ε� ������ ���������� �ݹ��� �� ��Ȯ�� �ð��� ȣ���� �ȴ�. 
3.	CALLBACK function�� ������ API�Լ����� application���� �ʿ��� �� �ü���� �Լ�(TextOut, SetTimer...)�� ����ϴ� �Ͱ� �޸�
	�ü���� �ʿ��Ҷ� application���� ȣ���ϴ� �Լ��̴�. �� ȣ������ �Ųٷα⿡ callback function�� ���̴�.
	callback fucntion�� �ü���� ���� ȣ��Ǵ� ���α׷� ������ �Լ��ε�, WndProc�� ���ü������ �� �Լ��� ȣ���Ѵ�. 
*/

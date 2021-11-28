//WindowsAPI에서는 SetTimer, KillTimer함수를 통해 주기적인 작업을 할 수 있게 타이머를 제공해준다.

//1. SetTimer
UINT_PTR WINAPI SetTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
//	hWnd_타이머 메시지를 처리할 윈도우, nIDEvent_타이머 일련 번호(개발자가 정의), uElapse_타이머 메시지를 발생할 주기(ms)
//	lpTimerFunc_타이머 메시지를 수행할 함수. NULL을 전달하면 hWnd 콜백 프로시저에 WM_TIMER 발생.
typedef VOID(CALLBACK* TIMERPROC)(HWND, UINT, UINT_PTR, DWORD); 

//	타이머 메시지는 SetTimer을 호출하였을 때가 아닌, 세번째로 전달한 시간이 흘렀을 때 처음으로 타이머 메시지가 발생한다.
//	4번쨰 인자에 타이머 콜백 프로시저를 전달하면 주기적으로 타이머 콜백 프로시저가 동작하는데, 
//	NULL일 시 hWnd의 WindowCallbackProc에 WM_TIMER 메시지가 전달된다.

//2. KillTimer
BOOL WINAPI KillTimer(HWND hWnd, UINT_PTR uIDEvent); 
//	hWnd_SetTimer에서 설정한 hWnd, nIDEvent_타이머 일련 번호(SetTimer에서 전달한 값) 

//3. Test Code
//	두 개의 타이머를 이용하는데, 첫 타이머는 1초주기로 발생하며 콜백 프로시저에서 현재 시각을 출련한다.
//	두번째 타이머는 1/100초 주기로 발생하고 프로그램실행으로부터 얼마나 흘렀는지 틱(여기선 1/100)수를 출력한다.
//	윈도우 생성 메시지 처리에서 두 개의 타이머를 생성하는데, 연습 목적으로 첫번쨰는 타이머 콜백 프로시저를 사용하고
//	두번째는 NULL을 전달하여 처리해보자. 

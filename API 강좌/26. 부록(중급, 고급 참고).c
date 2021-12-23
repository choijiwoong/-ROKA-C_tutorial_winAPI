//1. 유용한 매크로 정의문들_SendMessage함수로 직접 메시지를 보낼 때 유용.. 
//For seperate each information to lParam
#define LOWORD(l)	((WORD)(l))
#define HIWORD(l)	((WORD)(((DWORD)(l)>>16) & 0xFFFF))
#define LOBYTE(w)	((BYTE)(w))
#define HIBYTE(w)	((BYTE)(((WORD)(w)>>8) & 0xFF))
//For use 32bit value by 16bit value
#define MAKEWORD(a, b)	((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b)))<<8))
#define MAKELONG(a, b)	((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b)))<<16))


//2. 훅(Hock)은 메시지가 목표 윈도우로 전달되기 전에 메시지를 가로채는 특수한 프로시저로, 지역훅(Thread Specific)과 시스템 전역 훅(System Wide)로 나뉘어진다.
//	 응용 프로그램이 훅을 관리하기에 여러 개의 훅 프로시저가 존재할 수 있으며, 이를 훅 체인(Hook Chain)으로 관리한다.(함수포인터 배열)
//	예시코드는 아래와 같다. 아래응 Keyboard Hook의 예시이다. 더 깊은 예시는 알아보지 않고 단지 훅이 어떤 개념인지 정도만 알아두자. 
char Mes[]="Keyboard Hooking Test Program.";
char Mes2[128], Mes3[128];
int Count=0;

HHOOK hKeyHook;

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode>=0)
	{
		InvalidateRect(hWndMain, NULL, TRUE);
		if(wParam==VK_F2)
		{
			wsprintf(Mes2, "F2 key is denied.");
			return 1;
		} else
		{
			wsprintf(Mes2, "nCode=%dm wParam=%u, lParam=%08x, Count=%d", nCode, wParam, lParam, Count++);
		}
	}
	
	return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch(iMessage)
	{
		case WM_CREATE:
			hKeyHook=SetWindowsHookEx(WH_KEYBOARD, KeyHookProc, NULL, GetCurrentThreadId());//add Hook procedure that's type is WM_KEYBOARD
			return 0;
			
		case WM_KEYDOWN:
			wsprintf(Mes3, "Real Key: %u", wParam);
			InvalidateRect(hWnd, NULL, true);
			return 0;
			
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
			TextOut(hdc, 100, 10, Mes, strlen(Mes));
			TextOut(hdc, 100, 30, Mes2, strlen(Mes2));
			TextOut(hdc, 100, 50, Mes3, strlen(Mes3));
			EndPaint(hWnd, &ps);
			return 0;
			
		case WM_DESTROY:
			UnhookWindowsHookEx(hKeyHook);
			PostQuitMessage(0);
			return 0;
			
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

//3. 깜빡임 문제에 대해 무효영역을 최소화하거나, 새 그림을 덮퍼쓰는 방법으로 해결할 수 있지만, 완전한 해경 방법은 더블 버퍼링(Double Buffering)을 사용한다.
//	내부 계산버퍼와 화면출력 버퍼를 따로 유지하는 것으로 Bitmap출력과 유사한 방법이다. 단점은 내부 버퍼에 그림을 그린 후 외부 버퍼로 전송하기에 전반적인 속도가 느려진다는 것이다. 


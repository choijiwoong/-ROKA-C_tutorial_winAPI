//1. ������ ��ũ�� ���ǹ���_SendMessage�Լ��� ���� �޽����� ���� �� ����.. 
//For seperate each information to lParam
#define LOWORD(l)	((WORD)(l))
#define HIWORD(l)	((WORD)(((DWORD)(l)>>16) & 0xFFFF))
#define LOBYTE(w)	((BYTE)(w))
#define HIBYTE(w)	((BYTE)(((WORD)(w)>>8) & 0xFF))
//For use 32bit value by 16bit value
#define MAKEWORD(a, b)	((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b)))<<8))
#define MAKELONG(a, b)	((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b)))<<16))


//2. ��(Hock)�� �޽����� ��ǥ ������� ���޵Ǳ� ���� �޽����� ����ä�� Ư���� ���ν�����, ������(Thread Specific)�� �ý��� ���� ��(System Wide)�� ����������.
//	 ���� ���α׷��� ���� �����ϱ⿡ ���� ���� �� ���ν����� ������ �� ������, �̸� �� ü��(Hook Chain)���� �����Ѵ�.(�Լ������� �迭)
//	�����ڵ�� �Ʒ��� ����. �Ʒ��� Keyboard Hook�� �����̴�. �� ���� ���ô� �˾ƺ��� �ʰ� ���� ���� � �������� ������ �˾Ƶ���. 
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

//3. ������ ������ ���� ��ȿ������ �ּ�ȭ�ϰų�, �� �׸��� ���۾��� ������� �ذ��� �� ������, ������ �ذ� ����� ���� ���۸�(Double Buffering)�� ����Ѵ�.
//	���� �����ۿ� ȭ����� ���۸� ���� �����ϴ� ������ Bitmap��°� ������ ����̴�. ������ ���� ���ۿ� �׸��� �׸� �� �ܺ� ���۷� �����ϱ⿡ �������� �ӵ��� �������ٴ� ���̴�. 


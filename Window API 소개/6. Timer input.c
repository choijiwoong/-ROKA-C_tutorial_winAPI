//WindowsAPI������ SetTimer, KillTimer�Լ��� ���� �ֱ����� �۾��� �� �� �ְ� Ÿ�̸Ӹ� �������ش�.

//1. SetTimer
UINT_PTR WINAPI SetTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
//	hWnd_Ÿ�̸� �޽����� ó���� ������, nIDEvent_Ÿ�̸� �Ϸ� ��ȣ(�����ڰ� ����), uElapse_Ÿ�̸� �޽����� �߻��� �ֱ�(ms)
//	lpTimerFunc_Ÿ�̸� �޽����� ������ �Լ�. NULL�� �����ϸ� hWnd �ݹ� ���ν����� WM_TIMER �߻�.
typedef VOID(CALLBACK* TIMERPROC)(HWND, UINT, UINT_PTR, DWORD); 

//	Ÿ�̸� �޽����� SetTimer�� ȣ���Ͽ��� ���� �ƴ�, ����°�� ������ �ð��� �귶�� �� ó������ Ÿ�̸� �޽����� �߻��Ѵ�.
//	4���� ���ڿ� Ÿ�̸� �ݹ� ���ν����� �����ϸ� �ֱ������� Ÿ�̸� �ݹ� ���ν����� �����ϴµ�, 
//	NULL�� �� hWnd�� WindowCallbackProc�� WM_TIMER �޽����� ���޵ȴ�.

//2. KillTimer
BOOL WINAPI KillTimer(HWND hWnd, UINT_PTR uIDEvent); 
//	hWnd_SetTimer���� ������ hWnd, nIDEvent_Ÿ�̸� �Ϸ� ��ȣ(SetTimer���� ������ ��) 

//3. Test Code
//	�� ���� Ÿ�̸Ӹ� �̿��ϴµ�, ù Ÿ�̸Ӵ� 1���ֱ�� �߻��ϸ� �ݹ� ���ν������� ���� �ð��� ����Ѵ�.
//	�ι�° Ÿ�̸Ӵ� 1/100�� �ֱ�� �߻��ϰ� ���α׷��������κ��� �󸶳� �귶���� ƽ(���⼱ 1/100)���� ����Ѵ�.
//	������ ���� �޽��� ó������ �� ���� Ÿ�̸Ӹ� �����ϴµ�, ���� �������� ù������ Ÿ�̸� �ݹ� ���ν����� ����ϰ�
//	�ι�°�� NULL�� �����Ͽ� ó���غ���. 

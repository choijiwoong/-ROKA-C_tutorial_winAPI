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
	[���� �� �ı�]
1.	������ WM_CREATE, WM_DESTROY�� ����Ͽ� �ʱ�ȭ�� ����ó���� �ϴ� ��� �ܿ���, WinMain���� ���� �ʱ�ȭ�� ����ó���� �� �� �ִ�. 
	WM_CREATE�� CreateWindow�� ���� ������ �� �߻��ϱ⿡ �� �ڿ� �ְ�, WM_DESTROY�� �߻��ϸ� MessageLoop�� �����⿡ Loop�ڿ� ����ó���� ���ָ� �ȴ�. 
	 ��� WM_CREATE&WM_DESTROY�� Ư�� �����쿡 ���� �ʱ�/����ó���� ����, ���������쿡�� ó���ϴ� ���� �������� ����/����ó���� �ϴ� ���� ����ȴ�.
	
	[�۾� ����]
1.	Client Area�� Non Client Area�� �����찡 �����Ǿ� �ִµ�, ���α׷����� ����� Client Area�� �����ȴ�. Non Client Area�� �ü���� �����ϱ� �����̴�.
	��� ���ϴ� ��ġ�� ����Ϸ��� ��ü �������� ��ǥ�� �ƴ� �۾� ������ ��ǥ�� �����ؾ��ϴµ� �̶� ���Ǵ� �Լ��� ������ �Ʒ��� ����.
	
	BOOL GetClientRect(HWND hWnd, LPRECT lpRect);//window handle, pointer of RECT for getting return
	
	[WM_SIZE]
1.	������ ũ�Ⱑ ����Ǿ��� �� �������� lParam�� �������忣 ���� ���� ��, ������ ���̰� ���޵Ǹ�
	wParam���� �޽��� �߻� ������ ��Ÿ���� �÷��װ� ���޵ȴ�.
	wParam�� ���޵Ǵ� �÷��״� ������ ����. SIZE_MAXHIDE, SIZE_MAIMIZED, SIZE_MAXSHOW, SIZE_MINIMIZED, SIZE_RESTORED
	�ٵ� ��� ��� �� ���Ѵ�. 
2.	WM_CREATE�ҋ��� �����찡 ���� �ߴ°ű⿡ GetClientRect��� �� WM_PAINTȣ���� �ƿ� WM_SIZE���� �� ���� �ִ�. 
3.	GetClientRect�� rt�� ������ ���� �ְ�, ������ lParam�� ����� �۾� ���� ũ�Ⱑ ���޵Ǳ� ������ GetClientRect�Լ� ���� lParam���� rt�� �ٷ� �����ص� �ȴ�. 

	[WM_MOVE]
1.	WM_SIZE�� ������ �޽����� �����찡 �̵��� �� ���� lParam�� ������ X, ������ Y�� ��� �����ϴµ�, �̵��Ҷ� Ư���� ���� ���� �ʴ��̻� �����Ѵ�. 
*/

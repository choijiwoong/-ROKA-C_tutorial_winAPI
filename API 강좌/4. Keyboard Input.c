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
	[Ű���� �Է�] 
1.	Ű���� �Է°� ���ÿ� str���� ����� �� ������ Ű������ �Է��� WM_CHAR���� str��, ����� WM_PAINT�� ������ ó���ϴ� 
	������ �׻� ȭ���� �ٽ� �׸� �غ� �صξ�� �ϴµ� �׷��� ȭ�� ������ ������ WM_PAINT���� �̷������ �����̴�.
	���� WM_CHAR���� �ٷ� ����� �ع����� window�� �������� ���� ȭ�� ������ �Ѵ��ϴ��� ���� ������ �Ұ����ϴ�. 
2.	wParam�� �ƽ�Ű���� �ް�, lParam�� �� 32��Ʈ�� ������ ��ĵ�ڵ�, �ݺ� ī��Ʈ �� ��Ÿ ������ ���� �ִ�.
	lParam�� �ʿ��� ������ ������ ����ϸ� �ǰ�, ������ wParam�� ����ϸ� �ȴ�. lParam�� ����ϴ� ���� �幰��. 

	[��ȿȭ ����]
1.	WM_PAINT�� ȣ��Ǵ� ���� �ٽ� �׷������� �ʿ䰡 �ִٴ� ������ �ٸ����� Invalid Region(������ ����� �ٸ� ���)�� �ִٴ� ���̴�. 
	��, ȭ���� �������ٰ� �������� ġ������, �� ������ ��ȿ�� ���� �� �ٽ� �׷����� ���ش�. 
	�츮�� InvalidateRect�� ����� �κ��� ������ ��ȿȭ ���� WM_PAINT�� ȣ���� �� �ִ�. ������ �Ʒ��� ����.
	
	BOOL InvalidateRect(HWND hWnd, CONST RECT *lpRect, BOOL bErase);
	ù �μ��δ� ��ȿȭ ����� �Ǵ� �������� �ڵ��̸�, ������ �����츦 �ٽ� �׸� ��� WndProc���� ���� hWnd�� �״�� ����ϸ� �ȴ�.
	lpRect�� ��ȿȭ�� ����� �Ǵ� �簢 �����ε�, NULL���̸� ��� ������ ��ȿȭ(�ӵ�������) �ȴ�.
	bErase�� background Erase�������δٰ� ��浵 ����� �ٽ� �׸����� ����� ���ΰ� �׸����� �����ϴµ� FALSE�� ��������� �׸���. 
	(Space�� �Է½� ���ڿ��� �ʱ�ȭ�ϴ� ���, InvalidateRect�� ����° ���ڸ� TRUE�� �����ؾ��� �� ��������.)

	[WM_KEYDOWN]
1.	WM_CHAR���� Ű�� WM_KEYDOWN�޽����� ����Ѵ�. VK_BACK, VK_TAB, VK_RETURN, VK_SHIFT, VK_CONTROL, CK_MENU
	VK_PAUSE, VK_CAPITAL, VK_ESCAPE, VK_SPACE, VK_PRIOR, VK_NEXT, VK_END, WK_HOME, VK_LEFT, VK_UP ���� �ִ�.
	
	[TraslateMessage]
1.	Ű���忡�� A�� ������ ���� �޽����� �߻� ������ WM_KEYDOWN->WM_CHAR->WM_KEYUP�ε�, Ű����� DOWN, UP�޽����� �߻���Ű��
	WM_CHAR�� MsgLoop���� TraslateMessage(&Message)���� ���������� �����ȴ�. TranslateMessage�� Ű�� CHAR�̸� WM_CHAR�� MSG queue�� �����δ�.
	�̴� DispatchMessage�� ���� WndProc���� ��������. ��� TranslateMessage�� ������ WM_CHAR�� WndProc�� ���޵��� �ʴ´�. 
*/

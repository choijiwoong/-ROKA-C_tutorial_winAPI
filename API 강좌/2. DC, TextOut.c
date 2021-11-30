#include <windows.h>//3-2-�� DrawText�������� 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="TextOut";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, HPSTR lpszCmdParam, int nCmdShow)
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
	WndClass.style=CS_HREDRAW|CS_VREDRAW;
	
	RegisterClass(&WndClass);
	
	hWnd=CreateWindow(lpszClass, "First DC Program", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	
	while(GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;//for BeginPaint. it contains information for drawing
	
	switch(iMessage)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			
		case WM_LBUTTONDOWN:
			hdc=GetDC(hWnd);
			TextOut(hdc, 100, 100, "Beginsssss", 15);//100, 100 location. 15 is enough size to String , But trash value is printed
			//Problem.1 print of garbage value
			//Problem.2 erase printed string in repainting(Because Windows doesn't automatically save or restore screen 
			//So we have to change code that can print when it is erased not depending on LBUTTONDOWN
			ReleaseDC(hWnd, hdc);
			return 0;
			
		case WM_PAINT://***New Version of WM_LBUTTONDOWN case***. In resizing, call WM_PAINT for restore screen automatically.
			hdc=BeginPaint(hWnd, &ps);//more efficent way for drawing than GetDC. it can be used in WM_PAINT only. 
			
			SetTextAlign(hdc, TA_CENTER);//We can use Cp by set TA_UPDATECP flag.
			TextOut(hdc, 200, 60, "Welcome", 7);//Think 200 is centor location.
			TextOut(hdc, 200, 80, "to",2);
			TextOut(hdc, 200, 100, "Sinmyeong", 9);
			EndPaint(hWnd, &ps);
			return 0;
			
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

/*
1.	��������� �޸𸮸� �����ϰ� ���α׷��� �����Ű�� KERNEL, ���� �������̽��� �����츦 �����ϴ� USER,
	ȭ�� ó���� �׷����� ����ϴ� GDI(Graphic Device Interface) �� �� ���� ���� ���̺귯���� �����Ǿ� �ִ�.	 
	 DC�� �ʿ伺�� �츮�� ���� �ߴ°͸� �ϴ��� �� ���� ��ǥ, ���� ��, ���� ����, ���� �� ���� ������ �����ؾ��ϴµ�,
	�̸� ���ڷ� �ϳ��ϳ� �����ϸ� ��ȿ�����̴� �̸� ����ϴ� ����ü�� ����� hWnd�� �����Ͽ� ó���ϴ� ���̰�, �� ����ü�� DC�̴�.
	(��ġ C���� �Լ��� �����͸� �����Ͽ� ó���ϴ� �� ó��) DC(Device Contect)�� ��������� ������ ����ü�� GDI��⿡ ���� �����ȴ�.
	 �Ǵٸ� �ʿ伺�� ������� ���� ���α׷��� ���ÿ� ����Ǵ� ��Ƽ �½�ŷ �ý����̱⿡ �� �����찡 �������� ǥ�� ������ hWnd�� �����ϸ�
	��� ��ü�� �׸��� �� ��Ȯ������.(������ǥ���� hWnd�� ���ѵ� ���������ǥ����..)  DC�� ���Ͽ� ���� ���� ���� ��µǰ��ִ� ���α׷��� ��ġ�� �κ���
	Unvalid�������� ������Ű��, DC��� �� ����� �㰡�� �������� ����� ��Ų��.
2.	��� �������� ���ڿ��� ����� ���ΰ����� ��������� WM_PAINT�޽����̴�. ���������� �Ϻΰ� �������ٸ� WM_PAINT MSG�� �����Ǹ�, �̸� ����Ͽ�
	�����ϰԲ� �ؾ��Ѵ�. 
3.	ȭ�鿡 ����ϱ� ���ؼ��� DC�� �־�� �ϴµ�, GetDC(HWND hWnd) & ReleaseDC(HWND hWnd, HDC hDC), 
	BeginPaint(HWND hWnd, LPPAINTSTRUCT lpPaint) & EndPaint(HWND hWnd, CONST PAINTSTRUCT *lpPaint) in WM_PAINT�� �ΰ��� ������� DC�� ���� �� �ִ�.
	 DC�� �ַ� �ϳ��� �����쿡 �����Ǵ� ��������� �����⿡ �μ��� � �����쿡 ���� DC������ ����(HWND)
	�� ������ �ȿ��� ������ DC�� �߱��Ѵ�. �̴� �� �����Ǿ�� �ϴ°� DC�� �ش� Window�ȿ��� �޸𸮸� �����ϱ� �����̴�.	 
	(�ᱹ DC�� �ڵ��ε� �츮�� ���� ������ ���α׷� �ڵ鿡 ���ѵǾ ���Ǵ� �ణ ���� �ڵ� ������ ����. �޸� ������ ���� �߱ް� ������ �ϴ°Ű�)
4.	TextOut(hdc_����� �ڵ�, nXStart, nYStart, lpszString_����� ���ڿ��� ������, cbString_����)�� ȣ���ϸ�
	��¿� �ʿ��� ������ hdc�� ������ ����Ѵ�. ��� �츰 hdc�� ������ �����ϸ� ������ ���, ũ��, ��, ���Ļ���, ��ǥ�ؼ�������� ������ �����ѵ�,
	 UINT SetTextAlign(HDC hdc, UINT fMode)�� ����Ͽ� ���� ����� ������ �� �ִ�. �� ���̻��� �÷��׸� OR�� �����Ͽ� ����Ѵ�. default�� TA_TOP|TA_LEFT�̴�. 
	TA_TOP(�����ǥȭ), TA_BOTTOM(�ϴ���ǥȭ), TA_CENTER(�߽���ǥȭ), TA_LEFT(���������ǥȭ) , TA_RIGHT(�����������ǥȭ),
	TA_UPDATECP(������ǥ ��� CP�� ����Ͽ� ��������� CP�� ����), TA_NOUPDATECP(CP����� ������ ������ǥ�� ����Ѵ�.) 
5.	CP(Current Position)�� �ؽ�Ʈ��忡�� ��µ� ��ġ�� Ŀ���� ǥ���ϵ��� �׷��ȸ�忡�� ���� �׷����� ��µ� ��ġ�� CP�� ������ �ִ�.
	ȭ�鿡 ������ ������ TA_UPDATECP�� ����ϸ� �����ġ��ǥ�� ������� CP������ġ�� ����ϰ� ���� CP�� ++�Ѵ�. 
*/ 

/*
typedef struct tagPAINTSTRUCT{
	HDC hdc;//User use
	BOOL fErase;
	RECT rcPaint;
	
	BOOL fRestore;//Windows use innerly. We don't touch it.
	BOOL fIncUpdate;
	BYTE rgbReserved[16];
}PAINTSTRUCT;
*/

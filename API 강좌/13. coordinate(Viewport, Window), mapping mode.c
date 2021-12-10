/*
	[���������� ��ǥü��]
1.	GUI�ü���� ���������� ��� ����� picture Element(�ȼ�) �� ȭ��, �������� �̷������.
	�̴� �����Ϳ� ���� ������ �ػ� ���̿� ���� ������ �߻��Ǵµ�, 72dpi�� ���� ���ػ󵵿��� 100�ȼ��� ���� �׸������� 4800dpi�� �ʰ��ػ󵵿��� 100�ȼ��� ���� �׸����� ũ�Ⱑ �ξ� �۴�.
	 �Ǵٸ� ������ ��ǥü�迡�� �߻��Ѵ�. ����ʹ� �»���� (0,0)�� �Ǳ⿡ �ϻ����� ��ǥ��(��и�)�� �ٸ���. ��� ��ȯ�� �ʿ��� ��쿡�� ����Լ��� Y��ǥ�� -1�� �����־� 
	������ �ٲٰų�, �����̵���Ű�� �� ���������� �Űܾ� �Ѵ�.
2.	��� ��������� ���� ����� ��Ŀ�������� ��ǥ ü�踦 �ٲ۴� ����� �ü�� �ܿ��� ���������ν� ���α׷����� �δ��� �����ش�. 

	[���� ���]
1.	�־��� ��ǥ�� ȭ����� ���� ��� �ش��ϴ����� �����ϴ� �������, ����ǥ(�������� ���ο��� ���Ǵ� ��ǥ)�� ������ǥ(���� ȭ�鿡�� �ȼ��� ����Ͽ� ���)�� �ִ�. 
	����������� default(MM_TEXT) ���� ��忡���� ����ǥ�� ������ǥ�� ��ġ�Ǿ� �ֱ�� TextOut(100,100)�� ���� �Ϳ��� �Ȱ��� �۵��ߴ� ���̴�.
2.	��������� �����ϴ� ���� ���� ������ ����. (���θ��, ����, x�� ��������, y�� ��������) 
	MM_TEXT(�ȼ�), MM_LOMERTIC(0.1mm), MM_HIMETRIC(0.01mm), MM_LOENGLISH(0.01inch), MM_HIENGLISH(0.001inch), MM_TWIPS(1/1440inch), MM_ISOTROPIC(����), MM_ANISOTROPIC(����) 
3.	���� ����� ������ �Ʒ��� �Լ��� �̿��Ѵ�. DC�� �ڵ��� �������ش�. 
	int SetMapMode(HDC hdc, int fnMapMode);//setter 
	int GetMapMode(HDC hdc); //getter
	
	[������� ����Ʈ]
1.	����Ʈ(Viewport)�� ������ǥ�� ���Ǵ� ������ ���Ѵ�.(������� ���� ���̴� ȭ��) ������� ����ǥ�� ����ϴ� �����̴�.
	 ����(Origin)�� ��ǥ�� ������ �Ǵ� ������ default�� �»���̴�. ������ ������ �Ʒ��� �Լ��� ������.
	BOOL SetViewportOrgEx(HDC hdc, int X, int Y, LPPOINT lpPoint);//Setter
	BOOL SetWindowOrgEx(HDC hdc, int X, int Y, LPPOINT lpPoint);//Getter
	 lpPoint�� ������ �������� �ޱ� ���� ����ϸ� �ʿ������ NULL�� ����Ѵ�. �� �� �ϳ��� �Ű� ó���ϴ� ����� (���� 2��..�ϳ� �������� ��� �����ϳ� �ű��) �������̸�
	���� ����Ʈ�� ������ �̵���Ų��.(������_������, ����Ʈ_�ȼ� ����)
2.	�Ʒ��� ���ô� ������ǥ��� ������ ��ǥ�踦 ������ �� ���ΰ�� �׸���. 
*/ 

#include <math.h>
#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="Sin";

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
	double f;
	int y;
	
	switch(iMessage)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
			SetMapMode(hdc, MM_LOENGLISH);//Get Math's coordinate system(0.1inch)
			//SetMapMode(hdc, MM_HIGHENGLISH);//0.001inch it's become more smaller!
			SetViewportOrgEx(hdc, 400, 300, NULL);//and set origin to 200, 150 in Math's coordinate system.(x=200 right, y=150 up)
			//Let's set origin anywhere!
			MoveToEx(hdc, -2000, 0, NULL);//draw standard line
			LineTo(hdc, 2000, 0);
			MoveToEx(hdc, 0, -2000, NULL);
			LineTo(hdc, 0, 2000);
			
			for(f=-500; f<1000; f++)
			{
				y=(int)(sin(f*3.14/180)*100);//radian in sin
				//y=(int)(ctan(f*3.14/180)*100);
				SetPixel(hdc, (int)f, y, RGB(0,0,0));
			}
			EndPaint(hWnd, &ps);
			return 0;
			
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

/*
	[��Ʈ���� ����]
1.	����ڿ��� �������̽��� �ٷ�� ������ ��ư, ����Ʈ, üũ, ����, ����Ʈ �ڽ�, �޺� �ڽ�, ��ũ�� ��, ����ƽ, ...OCX��Ʈ�ѱ��� �پ��ϴ�.
 	��Ʈ���� ȭ����� ������ ������ �����ϸ� �ڽ��� �����޽����� ó���ϴ� �ϳ��� �������̴�. ���� �ƿ� ���������� �������� �ʰ� ���ϵ� ������� �����Ѵ�.
	�Ϲ������� WNDCLASS�� RegisterClass, CreateWindow�� ����� �Ͱ��� �޸� ��Ʈ���� ������� �ü�� �������� �������ֱ⿡ ���ǵǾ��ִ� Ŭ������ ����ϸ�ȴ�. Ŭ������ ������ �Ʒ��� ����.
	 ������ Ŭ����, ��Ʈ��: button ��ư,üũ,����	static �ؽ�Ʈ	scrollbar ��ũ�ѹ�	edit ����Ʈ	listbox ����Ʈ�ڽ�	combobox �޺��ڽ�
	����� CreateWindow�Լ��� ù��° �μ��� ������ Ŭ������ �ָ� �ȴ�.	

	[��ư �����]
1.	�ݵ�� �θ� �������� ���ϵ�� �����ؾ��ϸ� ��Ʈ���� �ַ� WM_CREATE�߻��� �����.  
	CreateWindow("button", "Click Me", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 20,20,100,25, hWnd, (HMENU)0, g_hInst, NULL);
	 ù �μ��� ��������ϴ� �������� Ŭ������, ��Ʈ���� �̹� ���ǵǾ��ֱ⿡ ������ Ŭ���� �� ���´�.
	�ι�° �μ��� ���� ������ �����ε� ��Ʈ�ѿ����� ĸ����ġ�� �޶�����. �ʿ������ NULL����. 
	����° �μ��� �������� �Ӽ� ������ ��Ʈ���� WM_CHILD�� �ʼ��̴�. ��ư�� ��Ÿ�� ���� �Ʒ��� ����.
	 BS_PUSHBUTTON, BS_DEFPUSHBUTTON, BS_CHECKBOX, BS_3STATE, BS_AUTOCEHCKBOX, BS_AUTO3STATE, BS_RADIOBUTTON, BS_GROUPBOX 
	4567�� �θ𒩵����� Client Section�� ���������� ��ǥ�̴�. 8��° �μ��� �θ������츦 �������ָ�, ��Ʈ�ѿ� �̺�Ʈ �߻� �� hWnd�� ������, hWnd�� �ı��� �� ���� �ı��ȴ�. 
	9�� �����쿡�� ����� �޴��� �ڵ��ε� CHILD��Ʈ���� �޴��� ������ �̸� ��Ʈ���� ID�������� ����Ѵ�. ������ �޴��� �ڵ��̱⿡ (HMENU)����ȯ�� ���� ID�� �����Ѵ�.(�����츦 ����� �ν��Ͻ��� �ڵ�) 
	10�� MDI���� ����ϴ� ����ü�̴�. ���� �ȴ�. 
2.	CreateWindow�� ������ ���ϵ� ��Ʈ���� ������ �ڵ��� �����ϴµ�, �츮�� ���� �ʴ´�. 

	[�θ���� ���]
1.	��Ʈ�ѿ��� �̺�Ʈ �߻� �� �θ� ������� WM_COMMAND�޽����� ���� Notification Message�� �����ش�. ���޵Ǵ� ������ �Ʒ��� ����.
	HIWORD(wParam)_�����ڵ�, LOWORD(wParam)_��Ʈ���� ID, lParam(�޽����� ���� ���ϵ� �������� ������ �ڵ�)
	��Ʈ���� ID�� CreateWindow�� 9��° �μ��� ���� ���̸� � ��Ʈ���� �������´����� ��Ÿ����.
	��ư�� ��� �����ڵ�� BN_CLICKED�̹Ƿ� Ư���� �˻��� �ʿ�� ���µ�, �����ڵ尡 ���� ���� ��Ʈ�ѿ� ���ؼ��� �����Ѵ�. 
2.	WM_COMMAND�޽����� ��Ʈ���� ��� �޽��� �� �ƴ϶� �޴��׸�, �����������͵��� ����� ó���ϴ� �߿��� ���� �Ѵ�. 
	�̷��� ��Ʈ���� ID, �޴� ID, ������������ ID���� ��� LOWORD(wParam)���� ���޵ǹǷ� 0~65535�������� �ߺ����� �ʴ� ID�� ������ �Ѵ�.
3.	�Ʒ��� �Ϲ����� WM_COMMAND�޽����� ����̴�. 
	case WM_COMMAND:
		switch(LOWORD(wParam)){
			case menu1:
			case menu2:
			case accelerator:
			case control:
				switch(HIWORD(wParam))
				{
					case BN_CLICKED:
						~
					case ~
				}
		}
	�� ����������. �̰� �Ϲ����� ����� �����滧�� 
*/

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="Control";

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
	

	hWnd=CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW|WS_VSCROLL|WS_HSCROLL,
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
	switch(iMessage)
	{
		case WM_CREATE:
			CreateWindow("button", "Click Me", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 20,20,100,25, 
					hWnd, (HMENU)0, g_hInst, NULL);
			CreateWindow("button", "Me too", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 20,50,100,25,
					hWnd, (HMENU)1, g_hInst, NULL);
			return 0;
			
		case WM_COMMAND:
			switch(LOWORD(wParam))//Get Control's ID 
			{
				case 0:
					MessageBox(hWnd, "First Button Clicked", "Button", MB_OK);
					break;
					
				case 1:
					MessageBox(hWnd, "Second Button Clicked", "Button", MB_OK);
					break;
				
				return 0;
			}
		
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

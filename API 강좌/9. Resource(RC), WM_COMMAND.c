/*
	[���ҽ� �и�] 
1.	�ڵ��� ���� ������ �����͸� ���Ҹ��� �ϸ� �̴� ��������� ������ �۾����� �и��Ǿ� �ִµ� �����Ͻÿ� �������Ͽ� ���յȴ�. 
	�̴� �����̳ʿ� ���α׷��Ӱ� �д��ϱ� ���ϰ�, ���ҽ� ������ ���������� ������������ �ʾƵ� �Ǹ� ���ҽ� ��Ȱ�� �� ��ü�� ����������.
	ex.) �޴�, ��Ʈ��, ����Ű, ���ڿ�, ������, Ŀ��
	[���ҽ� ������]
1.	���ҽ��� �ҽ��� RC������ txt���Ϸ� �̸� �����Ϸ��� �������ϸ� RES��� ���������� �����ȴ�. RC���Ͽ��� ������ ������ ����Ǹ� ���� �Ʒ��� ����. 
*/
MYMENU MENU
BEGIN
	POPUP "&File"
	BEGIN
		MENUITEM "&New", 101
		MENUITEM "&OPEN", 102
	END
END

MYBITMAP BITMAP "ksh.bmp"

MYACCLERATORS ACCELERATORS
BEGIN
	"A", 1, VIRTKEY, NOINVERT
	"Z", 9, VIRTKEY, NOINVERT
END

/*���ſ��� �̸� ������ ���ҽ� �����Ϸ��� ������������ ������ �������� ���ҽ� ��ũ��Ʈ ������ ���� ���ҽ� ������ �ۼ��ϰԲ� ������ ������ �����Ѵ�. �� �Ű�Ƚᵵ �ȴ�.

	[���ҽ� �ۼ�]
1.	�ַ� Visual C++�� ����Ͽ� �����. ������� RC������ ����� �߰���Ų �� WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);ó�� �������ָ� �ȴ�. �Ʒ��� ���ô� File/Nex����� ����� ������ �Ʒ��� ����. 
*/

#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="Menu";//

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=GetStockObject(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpFnWndProc=(WNDPROC)WndProc;
	WndClass.lpszClassName=lpszClass;//
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);//
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	
	RegisterClass(&WndClass);
}

long FAR PASCAL WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
		case WM_COMMAND:
			switch(LOWORD(wParam))//Get Menu's ID
			{
				case ID_FILE_MENU1:
					MessageBox(hWnd, "Choose first menu.", "Menu Demo", MB_OK);
					break;
				case ID_FILE_MENU2:
					MessageBox(hWnd, "Choose second menu.", "Menu Demo", MB_OK);
					break;
				case ID_FILE_EXIT:
					PostQuitMessage(0);
					break;
			}
			return 0;
		
		case WM_DESTROY:
			PostQuitmessage(0);
			return 0;
		
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

/*
1.	�޴� �׸��� ������ WM_COMMAND�޽����� ���޵Ǹ�, WPARAM�� LOWORD�� ��� �޴��� �����ߴ��� ������ �� �ִ�. 
2.	���ҽ� �������: �� ���ҽ� ����(Visual C++)->���ҽ� ���� ����(MENU)->���ҽ� �ۼ�(���ҽ� ������ �̿�)->��ũ��Ʈ ����(������Ʈ�� �����̸�����)->������Ʈ�� ���ҽ� ����(include, setting to WndClass)

	[WM_COMMAND]
1.	WM_COMMAND�޽����� �޴� �׸��� ������ �� �Ӹ� �ƴ϶� �����������͸� ���� ���� �߻��ϸ�, ��ư&����Ʈ �ڽ����� ��Ʈ���� �θ� ������� �����޽����� ������ �߻��Ѵ�.
2.	WM_COMMAND�� �߰������� �Ʒ��� ���� ������ ��� �ִ�.
	lParam_���� �޽����� �߻���Ų ��Ʈ���� ������ �ڵ�(��� �� �޽��������� �ڵ��)
	LOWORD(wParam)_�޴��� ������������, ��Ʈ���� ID
	HIWORD(wParam)_��Ʈ���� ������ ���� �޽����� �޴����ý� 0, ������������ ���ý� 1 

	[�޴� �������� ����]
1.	 
*/

/*
	[������, Ŀ��]
1.	���������� Visual C++�� Insert/Resource���� Icon, Cursor�� �����Ͽ� ����� �߰���Ų��. 
	���� ����� ���� ������� RC������ ���ô� �Ʒ��� ����. 
*/

//MENU.RC

#include "resource.h"
#define APSTUDIO_READONLY_SYMBOLS
#include "afxres.h"
#undef APSTUDIO_READONLY_SYMBOLS

#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_KOR)
#ifdef _WIN32
LANGUAGE LANG_KOREAN, SUBLANG_DEFAULT
#pragma code_page(949)
#endif //_WIN32

//Menu
IDR_MENU1 MENU DISCARDABLE
BEGIN
	POPUP "&File"
	BEGIN
		MENUITEM "Menu1",	ID_FILE_MENU1
		MENUITEM "Menu2",	ID_FILE_MENU2
		MENUITEM "Exit",	ID_FILE_EXIT
	END
END

#ifdef APSTUDIO_INVOKED

//TEXTINCLUDE
1 TEXTINCLUDE DISCARDABLE
BEGIN 
	"resource.h\0"
END

2 TEXTINCLUDE DISCARDABLE
BEGIN
	"#include ""afxres.h""\r\n"
	"\0"
END

3 TEXTINCLUDE DISCARDABLE
BEGIN
	"\r\n"
	"\0"
END

#endif //APSTUDIO_INVOKED

//Icon
IDI_ICON1	ICON	DISCARDABLE "Menu.ico"

//Cursor
IDC_CURSOR1	CURSOR	DISCARDABLE	"Menu.cur"

#endif//Korean resources

#ifndef APSTUDIO_INVOKED
//Generated from the TEXTINCLUDE 3 resource.

#endif//not APSTUDIO_INVOKED

/*
	[������������(Accelerator)]
1.	����Ű�ε�, ��������� ����Ű(ShortBut_Alt�� �Բ� �޴��� �׸��� Ű����� ������ ��)��� ���� �ٸ� �ǹ̷� ����ϱ⿡ �ٸ��� �θ���.
	�츮�� �޴��� ������� �������� ����� �� �ֱ⿡ �����������Ͷ� �θ���. 
2.	����ϴ� ����� Caption �Ӽ��� �����ϸ� �ȴ�.
	Menu1->Menu&\tCtrl+A   ,Menu2->Menu&2\tCtrl+B   ,Exit->&Exit\tCtrl+C
	ĸ���� ������ &���ڷ� ����Ű�� �����Ѵ�.  \t�� ������ ĭ�� ��� Ű������ ����Ѵ�. 
	File�޴��� F���ڰ� ����Ű�̹Ƿ� Alt+f�� ������ 1�� ������ Menu1�� ���õǸ�, E�� ������ Exit�� ���õȴ�. 
3.	2���� ������� Caption�Ӽ��� ����Ű�� �����߾ �̴� �׳� ���ڷ� ǥ���� ���� ���̰� ���� ������ ������ ���ҽ��� �����.
	Visual c++�� Insert/Resource���� Accelerator�� ID, Key, Modifier, Type�� �Է��Ͽ� RC�� �ۼ��Ѵ�. 
4.	���� �ڵ忡 �ݿ��� �����ε�, �������������� ID�� Menu�� ID�� �����صξ��⿡ WndProc�� WM_COMMAND�� ������ �ʿ� ����
	WinMain���ο� hAccel������ �����ϰ� �޽��� ������ �Ʒ��� ���� �����ϸ�ȴ�. 
*/
HACCEL hAccel;

hAccel=LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
while(getMessage(&Message, 0, 0, 0)){
	if(!TranslateAccelerator(hWnd, hAccel, &Message)){
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
}

HACCEL LoadAccelerators(HINSTNACE hInstance, LPCTSTR lpTableName);
/*
5.	IDR_ACCELERATOR1�� �������̱⿡ MAKEINTRESOURCE�� �̿��Ͽ� ���ҽ��� ���������� ���̺��� �ڵ鰪�� �������ش�. ���� hAccel�� ������������ ���̺��� ���� �� �ִ�. 
	!TranslateAccerlator�� Keyboard�޽����� WM_COMMAND�� ��ȯ�Ͽ� ���������Ͱ� ������ �� �ֵ��� ó�����ش�.  
	���� !TranslateAccerlator�� ���ٸ� WM_KEYDOWN�� ���� �� ���� ó���� ���̱⿡ TranslateAccelerator�� ���� �� Ű���� ������������ ���̺� �ִ��� Ȯ���ϰ� 
	�ִٸ� WM_COMMAND�� �߻���Ű�� TRUE�� �����Ѵ�. ��� �Ʒ��� TranslateMessage, DispatchMessage�� ������� ���ϰ� ���ƹ�����. 

	[���ڿ� ���̺�]
1.	��������� ���ڿ��鵵 ���ҽ��� �������� ��޵ȴ�. Insert/Resource->String Table���� ID�� default�� IDS_STRING1�� �ְ� ������ ���ڿ��� Caption�� ������ �ȴ�.
2.	���ڿ� ���ĵ� �ҽ��� ������������ �ʾƵ� �ȴ�. 
*/
#incluse "resource.h"//���ҽ� ID�� �ν��ϱ� ���� 
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	char str[256];//="String Resource Test"ó�� ����� �� ������, �������� �ڵ��� �и��� ���� ������ ������ ������ �ִ�. 
	
	switch(iMessage){
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
			LoadString(g_hInst, IDS_STRING1, str, 256);//g_hInst�����쿡 ������� IDC_STRING1 ���ڿ��� str�� 256������� �ε��ϰ� 
			TextOut(hec, 10, 10, str, strlen(str));//��� 
			EndPaint(hWnd, &ps);
			return 0;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

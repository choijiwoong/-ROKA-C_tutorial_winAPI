/*
	[리소스 분리] 
1.	코드의 논리와 무관한 데이터를 리소르라 하며 이는 윈도우즈에는 별도의 작업으로 분리되어 있는데 컴파일시에 실행파일에 결합된다. 
	이는 디자이너와 프로그래머가 분담하기 편리하고, 리소스 수정시 실행파일을 재컴파일하지 않아도 되며 리소스 재활용 및 교체가 용이해진다.
	ex.) 메뉴, 비트맵, 단축키, 문자열, 아이콘, 커서
	[리소스 편집기]
1.	리소스의 소스인 RC파일은 txt파일로 이를 컴파일러로 컴파일하면 RES라는 이진파일이 생성된다. RC파일에는 고유의 문법이 적용되며 예는 아래와 같다. 
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

/*과거에는 이를 별도의 리소스 컴파일러로 컴파일했지만 지금의 개발툴은 리소스 스크립트 문법을 몰라도 리소스 파일을 작성하게끔 위지윅 편집을 지원한다. 즉 신경안써도 된다.

	[리소스 작성]
1.	주로 Visual C++을 사용하여 만든다. 만들어진 RC파일은 헤더로 추가시킨 후 WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);처럼 포함해주면 된다. 아래의 예시는 File/Nex명령을 사용한 것으로 아래와 같다. 
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
1.	메뉴 항목을 누르면 WM_COMMAND메시지가 전달되며, WPARAM의 LOWORD로 어느 메뉴를 선택했는지 구분할 수 있다. 
2.	리소스 사용절차: 새 리소스 삽입(Visual C++)->리소스 종류 선택(MENU)->리소스 작성(리소스 편집기 이용)->스크립트 저장(프로젝트와 같은이름으로)->프로젝트에 리소스 포함(include, setting to WndClass)

	[WM_COMMAND]
1.	WM_COMMAND메시지는 메뉴 항목을 선택할 때 뿐만 아니라 엑셀러레이터를 누를 떄도 발생하며, 버튼&에디트 박스등의 컴트롤이 부모 윈도우로 통지메시지를 보낼때 발생한다.
2.	WM_COMMAND의 추가정보는 아래와 같은 구조를 띠고 있다.
	lParam_통지 메시지를 발생시킨 컨트롤의 윈도우 핸들(어디서 온 메시지인지를 핸들로)
	LOWORD(wParam)_메뉴나 엑셀러레이터, 컨트롤의 ID
	HIWORD(wParam)_컨트롤이 보내는 통지 메시지로 메뉴선택시 0, 엑셀러레이터 선택시 1 

	[메뉴 편집기의 사용법]
1.	 
*/

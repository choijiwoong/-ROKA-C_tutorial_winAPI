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
	switch(iMessage){
		
	}
}

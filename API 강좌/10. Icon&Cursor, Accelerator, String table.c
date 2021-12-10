/*
	[아이콘, 커서]
1.	마찬가지로 Visual C++의 Insert/Resource에서 Icon, Cursor을 선택하여 만들고 추가시킨다. 
	위의 방식을 통해 만들어진 RC파일의 예시는 아래와 같다. 
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
	[엑셀러레이터(Accelerator)]
1.	단축키인데, 윈도우즈에서 단축키(ShortBut_Alt와 함께 메뉴의 항목을 키보드로 누르는 것)라는 말을 다른 의미로 사용하기에 다르게 부른다.
	우리는 메뉴와 상관없이 언제든지 사용할 수 있기에 엑셀러레이터라 부른다. 
2.	사용하는 방법은 Caption 속성을 편집하면 된다.
	Menu1->Menu&\tCtrl+A   ,Menu2->Menu&2\tCtrl+B   ,Exit->&Exit\tCtrl+C
	캡션을 포함한 &문자로 단축키를 지정한다.  \t로 적당히 칸을 띄워 키조합을 사용한다. 
	File메뉴는 F문자가 단축키이므로 Alt+f를 누르고 1을 누르면 Menu1이 선택되며, E를 누르면 Exit가 선택된다. 
3.	2번의 방법으로 Caption속성에 단축키를 포함했어도 이는 그냥 문자로 표현한 것일 뿐이고 실제 구현은 별도의 리소스를 만든다.
	Visual c++의 Insert/Resource에서 Accelerator로 ID, Key, Modifier, Type를 입력하여 RC를 작성한다. 
4.	이제 코드에 반영할 차례인데, 엑셀러레이터의 ID를 Menu의 ID와 같게해두었기에 WndProc의 WM_COMMAND는 수정할 필요 없이
	WinMain선두에 hAccel변수를 선언하고 메시지 루프를 아래와 같이 수정하면된다. 
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
5.	IDR_ACCELERATOR1은 정수값이기에 MAKEINTRESOURCE를 이용하여 리소스의 엑셀레이터 테이블의 핸들값을 리턴해준다. 이제 hAccel로 엑셀러레이터 테이블을 읽을 수 있다. 
	!TranslateAccerlator는 Keyboard메시지를 WM_COMMAND로 변환하여 엑셀레이터가 동작할 수 있도록 처리해준다.  
	만약 !TranslateAccerlator이 없다면 WM_KEYDOWN이 먼저 이 값을 처리할 것이기에 TranslateAccelerator로 먼저 이 키값이 엑셀러레이터 테이블에 있는지 확인하고 
	있다면 WM_COMMAND를 발생시키며 TRUE를 리턴한다. 고로 아래의 TranslateMessage, DispatchMessage가 실행되지 못하게 막아버린다. 

	[문자열 테이블]
1.	윈도우즈에서 문자열들도 리소스의 일종으로 취급된다. Insert/Resource->String Table에서 ID는 default인 IDS_STRING1을 넣고 적당한 문자열을 Caption에 넣으면 된다.
2.	문자열 고쳐도 소스를 재컴파일하지 않아도 된다. 
*/
#incluse "resource.h"//리소스 ID를 인식하기 위해 
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	char str[256];//="String Resource Test"처럼 사용할 순 있지만, 가독성과 코드의 분리로 인한 별도의 관리의 장점이 있다. 
	
	switch(iMessage){
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
			LoadString(g_hInst, IDS_STRING1, str, 256);//g_hInst윈도우에 헤더안의 IDC_STRING1 문자열을 str에 256사이즈로 로드하고 
			TextOut(hec, 10, 10, str, strlen(str));//출력 
			EndPaint(hWnd, &ps);
			return 0;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

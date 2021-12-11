/*
	[컨트롤의 정의]
1.	사용자와의 인터페이스를 다루는 도구로 버튼, 에디트, 체크, 라디오, 리스트 박스, 콤보 박스, 스크롤 바, 스태틱, ...OCX컨트롤까지 다양하다.
 	컨트롤은 화면상의 일정한 영역을 차지하며 자신의 고유메시지를 처리하는 하나의 윈도우이다. 보통 아예 독립적으로 사용되지는 않고 차일드 윈도우로 존재한다.
	일반적으로 WNDCLASS와 RegisterClass, CreateWindow로 만드는 것과는 달리 컨트롤은 윈도우즈가 운영체제 차원에서 제공해주기에 정의되어있는 클래스를 사용하면된다. 클래스의 종류는 아래와 같다.
	 윈도우 클래스, 컨트롤: button 버튼,체크,라디오	static 텍스트	scrollbar 스크롤바	edit 에디트	listbox 리스트박스	combobox 콤보박스
	사용은 CreateWindow함수의 첫번째 인수로 윈도우 클래스를 주면 된다.	

	[버튼 만들기]
1.	반드시 부모 윈도우의 차일드로 존재해야하며 컨트롤은 주로 WM_CREATE발생시 만든다.  
	CreateWindow("button", "Click Me", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 20,20,100,25, hWnd, (HMENU)0, g_hInst, NULL);
	 첫 인수는 만들고자하는 윈도우의 클래스로, 컨트롤은 이미 정의되어있기에 윈도우 클래스 명만 적는다.
	두번째 인수는 원랜 윈도우 제목인데 컨트롤에따라 캡션위치가 달라진다. 필요없으면 NULL전달. 
	세번째 인수는 윈도우의 속성 값으로 컨트롤은 WM_CHILD가 필수이다. 버튼의 스타일 값은 아래와 같다.
	 BS_PUSHBUTTON, BS_DEFPUSHBUTTON, BS_CHECKBOX, BS_3STATE, BS_AUTOCEHCKBOX, BS_AUTO3STATE, BS_RADIOBUTTON, BS_GROUPBOX 
	4567은 부모뮝도우의 Client Section을 기준으로한 좌표이다. 8번째 인수는 부모윈도우를 지정해주며, 컨트롤에 이벤트 발생 시 hWnd에 보내며, hWnd가 파괴될 때 같이 파괴된다. 
	9는 윈도우에서 사용할 메뉴의 핸들인데 CHILD컨트롤은 메뉴가 없으니 이를 컨트롤의 ID지정으로 사용한다. 본래는 메뉴의 핸들이기에 (HMENU)형변환을 통해 ID를 지정한다.(윈도우를 만드는 인스턴스의 핸들) 
	10은 MDI에서 사용하는 구조체이다. 몰라도 된다. 
2.	CreateWindow는 생성된 차일드 컨트롤의 윈도우 핸들을 리턴하는데, 우리는 쓰지 않는다. 

	[부모와의 통신]
1.	컨트롤에서 이벤트 발생 시 부모 윈도우로 WM_COMMAND메시지를 통해 Notification Message를 보내준다. 전달되는 정보는 아래와 같다.
	HIWORD(wParam)_통지코드, LOWORD(wParam)_컨트롤의 ID, lParam(메시지를 보낸 차일드 윈도우의 윈도우 핸들)
	컨트롤의 ID는 CreateWindow의 9번째 인수로 정한 값이며 어떤 컨트롤이 통지보냈는지를 나타낸다.
	버튼의 경우 통지코드는 BN_CLICKED이므로 특별히 검사할 필요는 없는데, 통지코드가 여러 개인 컨트롤에 대해서만 조사한다. 
2.	WM_COMMAND메시지는 컨트롤의 통시 메시지 뿐 아니라 메뉴항목, 엘셀러레이터등의 명령을 처리하는 중요한 일을 한다. 
	이러한 컨트롤의 ID, 메뉴 ID, 엑셀러레이터 ID등은 모두 LOWORD(wParam)으로 전달되므로 0~65535범위에서 중복되지 않는 ID를 가져야 한다.
3.	아래는 일반적인 WM_COMMAND메시지의 모양이다. 
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
	즉 난잡해진다. 이게 일반적인 모양임 쿠쿠루삥빵뽕 
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

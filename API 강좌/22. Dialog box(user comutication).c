/*
	[사용자와의 대화]
1.	사용자와의 대화를 보다 편하게 하기 위해 컨트롤들이 배치되는 대화상자를 이용한다. 
	Modal형은 타 윈도우전환이 불가하며, Modaless는 전환이 가능하다(프로그래밍이 더 까다롭다).

	[About]
1.	대화살자를 만들기 위해선 템플릿과 프로시저가 필요하다. 제작은 Insert/Resource/Dialog에서 선택한다. rc파일을 포함한 뒤 프로시저를 작성한다.
	WndProc에서 대화상자 프로시저를 참조하므로 WndProc위에 정의하자. 
2.	대화상자 호출 시 아래의 함수를 따른다.
	int DialogBox(HINSTANCE hInstance, LPCTSTR lpTemplate, HWND hWndParent, DLGPROC lpDialogFunc); 
	_대화상자 리소스를 가진 인스턴스의 핸들, 템플릿의 리소스ID, 부모 윈도우, 대화상자 프로시저 

	[대화상자 프로시저]
1.	대화상자 프로시저와 윈도우 프로시저의 차이점은 리턴값의 차이(LRESULT(long), BOOL), 받아들이는 메시지(WM_CREATE, WM_INITDIALOG)이다. 
	주로 Dialog에서 WM_COMMAND를 자주 처리하는데, LOWORD(wParam)에 컨트롤의 ID, HIWORD(wParam)에 notification code가 전달된다.
2.	OK버튼이나 CANCEL버튼을 누를경우 EndDialog함수를 호출하는데, 원형은 아래와 같다.
	BOOL EndDialog(HWND hDlg, int nResult);
	 nResult는 DialogBox함수의 리턴값으로 전달된다.(생성함수) 
	 
	
	[컨트롤의 종류]
-	대화상자 편집기에 포인터(마우스 툴), 그림상자 부터 트리, 리스트 등 여러 컨트롤이 있기에 이들(프로그래스, 핫키, 트리 등)은 별도의 공부를 해야 제대로 사용할 수 있다. 
	[배치]
-	드래그하여 위치와 크기를 조절하면 된다. Ctrl을 누른채로 컨트롤을 선택하면 같은 컨트롤을 여러 개 배치할 수 있다.
	[선택] [크기 조정 및 이동]
-	격자를 눌러 정렬이나 크기 조절을 편하게 할 수 있다. 이는 Layout/Guide Settings를 사용한다.
	[삭제] [속성 조정]
-	속성 변경을 원하는 컨트롤 팝업 메뉴서 Properties를 선택하여 ID, 단축키 등을 선택할 수 있다.
	[정렬] [탭 순서 변경]
-	Layout/Tab Order로 컨트롤의 TabOrder를 지정할 수 있다. 
	[테스트]
-	모양을 보며 간단한 테스트를 위해 Layout/Test로 해당 대화상자만 실행할 수 있다. 
*/

#include <windows.h>
#include "resource.h"//Dialog

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="Modal, Modaless";

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

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
		case WM_INITDIALOG:
			return TRUE;
		
		case WM_COMMAND:
			switch(wParam)
			{
				case IDOK:
				case IDCANCEL:
					EndDialog(hDlg, 0);
					return TRUE;
			}
			break;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
		case WM_LBUTTONDOWN:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc);
			//Make Dialog by IDD_DIALOG1 template that is defined g_hInst, with AboutDlgProc procdure.
			return 0;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

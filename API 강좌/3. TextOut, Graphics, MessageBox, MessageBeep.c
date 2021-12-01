//1. DrawText
//BOOL TextOut(hdc, nXStart, nYStart, lpszString, cbString);//보다 조금 더 기능이 많은 DrawText가 있다.
//int DrawText(HDC, hDC, LPCTSTR lpString, int nCount, LPRECT lpRect, UINT uFormat);//RECT사각영역을 정의하여 영역안에 문자열 출력이 가능하며, 여러 포멧설정이 가능하다.

//DrawText의 세번째 인자의 구조는 아래와 같다. 
//typedef struct _RECT_practice{
	//LONG left;
	//LONG top;
	//LONG right;
	//LONG bottom;
//} RECT_practice; 

//uFormat으로는 DT_LEFT, DT_RIGHT, DT_CENTOR, DT_BOTTOM, DT_VCENTER,
//				DT_WORDBREAK(사각영역의 오른쪽 끝에서 자동 개행\n되게), DT_SINGLELINE(한줄출력), DT_NOCLIP(경계를 벗어나도 그대로 출력) 


//기존의 WM_PAINT의 코드를 DrawText를 이용하여 아래와 같이 바꿀 수 있다. 
#include <windows.h>

long FAR PASCAL WndProc(HWND, UINT, WPARAM, LPARAM);
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
	
	hWnd=CreateWindow(lpszClass, "Second DC program", WS_OVERLAPPEDWINDOW,
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

long FAR PASCAL WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rt={300, 100, 600, 300};//Rectangle Section
	char str[]="The immature will be characterized in hoping to die for ther great cause"
				"and the mature to live close for such."
				"The distinctive feature of the immature implies that they want to die for the greatcause"
				"while that of the mature denotes to live bumble for such.";
	
	switch(iMessage)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);//DC is important
			
			DrawText(hdc, str, -1, &rt, DT_CENTER|DT_WORDBREAK);//Use DrawText
			
			MoveToEx(hdc, 150, 150, NULL);
			LineTo(hdc, 180, 200);//Draw Line
			Rectangle(hdc, 200, 100, 250, 180);//Draw Rectangle
			Ellipse(hdc, 200, 200, 250, 280);//Draw Ellipse
			
			EndPaint(hWnd, &ps);//release
			return 0;
			
		case WM_LBUTTONDOWN:
			MessageBeep(MB_OK);//Make Beep sound
			
			hdc=GetDC(hWnd);//GetDC
			if(MessageBox(hWnd, "Mouse Left Button is Pushed", "Message Box Title", MB_ABORTRETRYIGNORE|MB_ICONHAND)==IDRETRY){
				TextOut(hdc, 50, 50, "Retry is returned to MessageBox!",50);
			}//Use return value of MessageBox like ID_OK, ID_RETRY
			ReleaseDC(hWnd, hdc);//ReleaseDC
			return 0;
		
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

//2. Graphic Print
//COLORREF SetPixel(hdc, nXPos, nYPos, clrref)//점 출력(clrref는 색상) 
//DWORD MoveToEx(hdc, x, y, lpPoint);//선을그릴땐 이 두 함수를 사용한다. GDI는 CP를 항상 유지시켜주기 때문에 LineTo함수는 CP에서부터 지정한 좌표까지 선을 그으며 CP를 끝점으로 이동시킨다.
//BOOL LineTo(hdc, xEnd, yEnd);//고로 이를 여러번 호출하면 연속된 선을 그을수있다. MoveTo는 CP를 지정한 좌표로 이동시키며, 그 CP의 좌표를 lpPoint에 대입해준다.(NULL값가능) 
//BOOL Rectangle(hdc, nLeftRect, nTopRect, nRightRect, nBottomRect)
//BOOL Ellipse(hdc, nLeftRect, nTopRect, nRightRect, nBottomRect)
//마찬가지로 DC의 핸들을 받아 사용한다. 

//(A, B) - (C, D)까지의 선을 그리는 작업은 아래와 같다.
//MoveToEx(hdc, A, B, NULL) ;
//LineTo(hdc, C, D);

//Rectangle함수는 사각형의 내부를 채운다는 점만 유의하여 사용하면 된다. 

//3. MessageBox
//int MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
//hWnd는 오너 윈도우로 MessageBox activate동안 오너 윈도우는 사용할 수 없는 상태가 된다.(닫거나 확인누르거나..등등 없애야 됨) 
//hpText는 출력할 문자열, lpCaption은 타이틀바의 TitleName, uTupe은 메시지상자 종류 플레그들이다. 
//MB_ABORTRETRYIGNORE, MB_OK, MB_OKCANCEL, MB_RETRYCANCEL, MB_YESNO, MB_YESNOCANCEL로 버튼을 설정할 수 있고, 아래의 플레그로 버튼에 아이콘을 출력할 수 있다. 
//MB_ICONEXCLAMATION & MB_ICONWARNING(경고!), MB_ICONINFORMATION & MB_ICONASTERISK(말풍선!), MB_ICONQUESTION(말풍선?), MB_ICONSTOP & MB_ICONEERROR & MB_ICONHAND(빨간x동그라미)

//코드는 위의 코드의 WM_LBUTTONDOWN을 참고하자. 

//MessageBox의 리턴값은 누른 버튼값으로 아래와 같다. 
//IDABORT, IDCANCEL, IDIGNORE, IDNO, IDOK, IDRETRY, IDYES
//MessageBox의 리턴값을 이용하는 코드는 아래와 같이 작성하면 된다.
//if(MessageBox(hWnd, "Do you want to continue?", "Queation", MB_YESNO)==IDYES){
	//Keep code
//} else{
	//Stop code
//}


//4. MessageBeep
//비프음을 내는 들리는 형태의 출력중 간단한 출력은 아래의 함수를 사용한다.
//BOOL MessageBeep(UINT uType);
//uType의 종류로는 0xFFFFFFFF(PC의 스피커이용), MB_ICONASTERISK(Asterisk 비프음), MB_ICONEXCLAMATION(Exclamation비프음),
//					MB_ICONHAND(Hand 비프음), MB_ICONQUESTION(Question 비프음), MB_OK(system default 비프음)

//WM_LBUTTON메시지 처리 루틴에 MessageBeep를 넣으면 마우스 왼쪽 버튼을 누를 때마다 비프음이 들릴 것이다.(시스템에 사운드카드와 스피터가 있다는 전제하에..) 
//코드는 위의 LBUTTONDOWN코드를 참고하자. 

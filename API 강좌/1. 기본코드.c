#include <windows.h>
//막상 차근차근하니 재밌네 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="First";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);//we can also change setting (like BLACK_BRUSH, LTGRAY_BRUSH)
	//We can get handle of Brush, Pen(that is supported by windows produce) by GetStockObject function.
	WndClass.hCursor=LoadCursor(NULL, IDC_ARROW);//NULL: use default cursor, IDC_ARROW: name of cursor in first argument.(like IDC_CROSS, IDC_IBEAN, IDC_NO, IDC_WAIT)
	//LoadCursor returns HCURSOR(Handle of cursor)
	WndClass.hIcon=LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=(WNDPROC)WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	
	RegisterClass(&WndClass);
	
	//Make information on memory, return window handle. hWnd will be used as argument of all functions that reference window
	hWnd=CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW|WS_VSCROLL|WS_HSCROLL,//second arguemnt is title of window. We can change it by string like "My First Program"
	//third argument os int value for propertu of windows. we can add property by OR operator
	//WS_CAPTION(get title bar), WS_HSCROLL(get Horizontal bar), WS_VSCROLL(get vertical bar), WS_MAXIMIZEBOX(max button), 
	//WS_MINMIZEBOX(min button), WS_SYSMENU(get system menu), WS_TICKFRAME(get boundary that can change size), WD_OVERLAPPED(0_common default style)
					CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,//Size of window_X, Y, width, Height 
					NULL, (HMENU)NULL, hInstance, NULL);
					
	ShowWindow(hWnd, nCmdShow);//Visualize_pass hWnd directly. In nCmdShow, mecro statics that design printing way to screen are defiend.
	
	while(GetMessage(&Message, 0, 0, 0))//Get message in message queue of system. GetMessage returns MSG struct.
	{//If returned MSG's value is WM_QUIT, return False, Else True.
		TranslateMessage(&Message);//Convert WM_KEYDOWN & WM_KEYUP to WM_CHAR. (Manufacture input message on keyboard)
		DispatchMessage(&Message);//MSG(that is popped out by system message queue) is passed to WndProc
	}//This called 'MessageLoop' that just pass to WndProc with little work like Translate, Dispatch
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)//not called in WinMain, just called in window
{//iMessage(kind of MSG(MSG.message)), 
	switch(iMessage)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			DefWindowProc(hWnd, iMessage, wParam, lParam);//Other Message we don't handle. (Like changing size, full screen...etc)
	}
}

//Definition of WndClass->CreateWindow->ShowWindow->MsgLoop
/*Struct of MSG
typedef struct tagMSG{
	HWND hwnd;//Window Handle that will be received MSG
	UINT message;//Kind of MSG(Important)
	WPARAM wParam;//Additional information(32bit)
	LPARAM lParam;//Additional information(32bit)
	DWORD time;//Time that occur MSG
	POINT pt;//Location of mouse when MSG occurs.
}MSG;*/
//WM_QUIT(End of Program), WM_LBUTTONDOWN, WM_CHAR, WM_PAINT(when we have to redraw screen), 
//WM_DESTROY(When window destroy on memory), WM_CREATE(First birth of Window)

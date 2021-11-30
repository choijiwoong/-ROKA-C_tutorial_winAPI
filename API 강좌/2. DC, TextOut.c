#include <windows.h>//3-2-나 DrawText부터하자 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
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
	
	hWnd=CreateWindow(lpszClass, "First DC Program", WS_OVERLAPPEDWINDOW,
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
	HDC hdc;
	PAINTSTRUCT ps;//for BeginPaint. it contains information for drawing
	
	switch(iMessage)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			
		case WM_LBUTTONDOWN:
			hdc=GetDC(hWnd);
			TextOut(hdc, 100, 100, "Beginsssss", 15);//100, 100 location. 15 is enough size to String , But trash value is printed
			//Problem.1 print of garbage value
			//Problem.2 erase printed string in repainting(Because Windows doesn't automatically save or restore screen 
			//So we have to change code that can print when it is erased not depending on LBUTTONDOWN
			ReleaseDC(hWnd, hdc);
			return 0;
			
		case WM_PAINT://***New Version of WM_LBUTTONDOWN case***. In resizing, call WM_PAINT for restore screen automatically.
			hdc=BeginPaint(hWnd, &ps);//more efficent way for drawing than GetDC. it can be used in WM_PAINT only. 
			
			SetTextAlign(hdc, TA_CENTER);//We can use Cp by set TA_UPDATECP flag.
			TextOut(hdc, 200, 60, "Welcome", 7);//Think 200 is centor location.
			TextOut(hdc, 200, 80, "to",2);
			TextOut(hdc, 200, 100, "Sinmyeong", 9);
			EndPaint(hWnd, &ps);
			return 0;
			
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

/*
1.	윈도우즈는 메모리를 관리하고 프로그램을 실행시키는 KERNEL, 유저 인터페이스와 윈도우를 관리하는 USER,
	화면 처리와 그래픽을 담당하는 GDI(Graphic Device Interface) 이 세 가지 동적 라이브러리로 구성되어 있다.	 
	 DC의 필요성은 우리가 선을 긋는것만 하더라도 두 점의 좌표, 선의 색, 선의 종류, 굵기 등 여러 정보를 전달해야하는데,
	이를 인자로 하나하나 전달하면 비효율적이니 이를 담당하는 구조체를 만들어 hWnd을 전달하여 처리하는 것이고, 이 구조체가 DC이다.
	(마치 C언어에서 함수에 포인터를 전달하여 처리하는 것 처럼) DC(Device Contect)는 출력정보를 가지는 구조체로 GDI모듈에 의해 관리된다.
	 또다른 필요성은 윈도우즈가 여러 프로그램이 동시에 실행되는 멀티 태스킹 시스템이기에 두 윈도우가 겹쳤을때 표시 정보를 hWnd로 전달하면
	어느 객체에 그릴지 더 명확해진다.(절대좌표보단 hWnd에 국한된 상대절대좌표랄까..)  DC를 통하여 현재 가장 위에 출력되고있는 프로그램에 겹치는 부분을
	Unvalid영역으로 지정시키고, DC출력 시 출력이 허가된 영역에만 출력을 시킨다.
2.	어느 시점에서 문자열을 출력할 것인가에서 어느시점이 WM_PAINT메시지이다. 윈도우즈의 일부가 지워졌다면 WM_PAINT MSG가 생성되며, 이를 사용하여
	복구하게끔 해야한다. 
3.	화면에 출력하기 위해서는 DC가 있어야 하는데, GetDC(HWND hWnd) & ReleaseDC(HWND hWnd, HDC hDC), 
	BeginPaint(HWND hWnd, LPPAINTSTRUCT lpPaint) & EndPaint(HWND hWnd, CONST PAINTSTRUCT *lpPaint) in WM_PAINT총 두가지 방법으로 DC를 얻을 수 있다.
	 DC는 주로 하나의 윈도우에 연관되는 출력정보를 가지기에 인수로 어떤 윈도우에 대한 DC인지를 밝혀(HWND)
	그 윈도우 안에서 적당한 DC를 발급한다. 이는 꼭 해제되어야 하는게 DC가 해당 Window안에서 메모리를 차지하기 떄문이다.	 
	(결국 DC도 핸들인데 우리가 만든 윈도우 프로그램 핸들에 국한되어서 사용되는 약간 지역 핸들 느낌인 것임. 메모리 관리를 위해 발급과 해제를 하는거고)
4.	TextOut(hdc_출력할 핸들, nXStart, nYStart, lpszString_출력할 문자열의 포인터, cbString_길이)로 호출하며
	출력에 필요한 정보는 hdc의 정보를 사용한다. 고로 우린 hdc의 정보를 변경하면 문자의 모양, 크기, 색, 정렬상태, 좌표해석방법등의 변형이 가능한데,
	 UINT SetTextAlign(HDC hdc, UINT fMode)를 사용하여 정렬 방법을 변경할 수 있다. 두 개이상의 플레그를 OR로 연결하여 사용한다. default는 TA_TOP|TA_LEFT이다. 
	TA_TOP(상단좌표화), TA_BOTTOM(하단좌표화), TA_CENTER(중심좌표화), TA_LEFT(수평왼쪽좌표화) , TA_RIGHT(수평오른쪽좌표화),
	TA_UPDATECP(지정좌표 대신 CP를 사용하여 문자출력후 CP를 변경), TA_NOUPDATECP(CP변경과 사용없이 지정좌표를 사용한다.) 
5.	CP(Current Position)은 텍스트모드에서 출력될 위치를 커서가 표시하듯이 그래픽모드에선 다음 그래픽이 출력될 위치를 CP가 가지고 있다.
	화면에 보이진 않지만 TA_UPDATECP를 사용하면 출력위치좌표에 상관없이 CP다음위치에 출력하고 다음 CP를 ++한다. 
*/ 

/*
typedef struct tagPAINTSTRUCT{
	HDC hdc;//User use
	BOOL fErase;
	RECT rcPaint;
	
	BOOL fRestore;//Windows use innerly. We don't touch it.
	BOOL fIncUpdate;
	BYTE rgbReserved[16];
}PAINTSTRUCT;
*/

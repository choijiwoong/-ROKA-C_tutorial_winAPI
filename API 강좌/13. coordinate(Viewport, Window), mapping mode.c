/*
	[윈도우즈의 좌표체계]
1.	GUI운영체제인 윈도우즈의 모든 출력은 picture Element(픽셀) 즉 화소, 점단위로 이루어진다.
	이는 프린터와 같은 곳에서 해상도 차이에 따른 문제가 발생되는데, 72dpi의 낮은 저해상도에서 100픽셀로 원을 그릴때보다 4800dpi의 초고해상도에서 100픽셀로 원을 그릴때의 크기가 훨씬 작다.
	 또다른 문제는 좌표체계에서 발생한다. 모니터는 좌상단이 (0,0)이 되기에 일상적인 좌표계(사분면)과 다르다. 고로 변환이 필요한 경우에는 출력함수에 Y좌표에 -1을 곱해주어 
	방향을 바꾸거나, 평행이동시키는 등 인위적으로 옮겨야 한다.
2.	고로 윈도우즈에서 맵핑 모드라는 메커니즘으로 좌표 체계를 바꾼느 방법은 운영체제 단에서 제공함으로써 프로그래머의 부담을 덜어준다. 

	[맵핑 모드]
1.	주어진 좌표가 화면상의 실제 어디에 해당하는지를 결정하느 방법으로, 논리좌표(윈도우즈 내부에서 사용되는 좌표)와 물리좌표(실제 화면에서 픽셀을 사용하여 출력)가 있다. 
	윈도우즈에서의 default(MM_TEXT) 맵핑 모드에서는 논리좌표와 물리좌표가 일치되어 있기어 TextOut(100,100)과 같은 것에서 똑같이 작동했던 것이다.
2.	윈도우즈에서 지원하는 맵핑 모드는 다음과 같다. (맵핑모드, 단위, x축 증가방향, y축 증가방향) 
	MM_TEXT(픽셀), MM_LOMERTIC(0.1mm), MM_HIMETRIC(0.01mm), MM_LOENGLISH(0.01inch), MM_HIENGLISH(0.001inch), MM_TWIPS(1/1440inch), MM_ISOTROPIC(가변), MM_ANISOTROPIC(가변) 
3.	맵핑 모드의 변경은 아래의 함수를 이용한다. DC의 핸들을 전달해준다. 
	int SetMapMode(HDC hdc, int fnMapMode);//setter 
	int GetMapMode(HDC hdc); //getter
	
	[윈도우와 뷰포트]
1.	뷰포트(Viewport)는 물리좌표가 사용되는 영역을 말한다.(사용자의 눈에 보이는 화면) 윈도우는 논리좌표를 사용하는 영역이다.
	 원점(Origin)은 좌표의 기준이 되는 점으로 default는 좌상단이다. 원점의 변경은 아래의 함수를 따른다.
	BOOL SetViewportOrgEx(HDC hdc, int X, int Y, LPPOINT lpPoint);//Setter
	BOOL SetWindowOrgEx(HDC hdc, int X, int Y, LPPOINT lpPoint);//Getter
	 lpPoint는 기존의 원점값을 받기 위해 사용하며 필요없으면 NULL을 사용한다. 둘 중 하나만 옮겨 처리하는 방법이 (굳이 2개..하나 기준으로 잡고 남은하나 옮기기) 보편적이며
	보통 뷰포트의 원점을 이동시킨다.(윈도우_논리단위, 뷰포트_픽셀 단위)
2.	아래의 예시는 수학좌표계와 동일한 좌표계를 세팅한 후 사인곡선을 그린다. 
*/ 

#include <math.h>
#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="Sin";

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
	
	hWnd=CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	
	while(GetMessage(&Message,0,0,0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	double f;
	int y;
	
	switch(iMessage)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			
		case WM_PAINT:
			hdc=BeginPaint(hWnd, &ps);
			SetMapMode(hdc, MM_LOENGLISH);//Get Math's coordinate system(0.1inch)
			//SetMapMode(hdc, MM_HIGHENGLISH);//0.001inch it's become more smaller!
			SetViewportOrgEx(hdc, 400, 300, NULL);//and set origin to 200, 150 in Math's coordinate system.(x=200 right, y=150 up)
			//Let's set origin anywhere!
			MoveToEx(hdc, -2000, 0, NULL);//draw standard line
			LineTo(hdc, 2000, 0);
			MoveToEx(hdc, 0, -2000, NULL);
			LineTo(hdc, 0, 2000);
			
			for(f=-500; f<1000; f++)
			{
				y=(int)(sin(f*3.14/180)*100);//radian in sin
				//y=(int)(ctan(f*3.14/180)*100);
				SetPixel(hdc, (int)f, y, RGB(0,0,0));
			}
			EndPaint(hWnd, &ps);
			return 0;
			
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

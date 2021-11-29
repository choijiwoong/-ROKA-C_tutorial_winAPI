/*원하는 프로그램 조건은 아래와 같다. 
1. 주어진 공간 안에서 도형을 이동시킨다.
2. 방향키를 누르면 도형의 방향이 바뀐다.
3. space를 누르면 도형이 멈춘다.
4. 현재 방향과 도형의 좌표(논리 좌표)를 출력한다.
5. 마우스 왼쪽 버튼을 클릭하면 도형의 종류가 바뀐다(사각형->원->사각형.. 등)
6. 도형이 움직일 수 있는 공간을 표시하고 논리 좌표에 맞게 모눈을 그린다.
7. 도형은 움직일 수 있는 공간 경계 밖으로 이동이 불가능하다. 
*/

#include <Windows.h>
#define MY_DEF_STYLE CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS

//definition of functions we will use.
LRESULT CALLBACK MyWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
void RegMyWndClass(LPCWSTR cname);
void MakeWindow(LPCWSTR cname);
void MsgLoop();

INT APIENTRY WinMain(HINSTANCE hIns, HINSTANCE hPrev, LPSTR cmd, INT nShow)
{
	RegMyWndClass(TEXT("MyWindow"));
	MakeWindow(TEXT("MyWindow"));
	MsgLoop();//Execute Message
}

void RegMyWndClass(LPCWSTR cname)
{
	WNDCLASS wndclass={0};
	wndclass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hInstance=GetModuleHandle(0);
	wndclass.hIcon=LoadIcon(0, IDI_APPLICATION);
	wndclass.hCursor=LoadCursor(0, IDC_ARROW);
	wndclass.lpfnWndProc=MyWndProc;
	wndclass.lpszMenuName=0;
	wndclass.lpszClassName=cname;
	wndclass.style=MY_DEF_STYLE;
	
	RegisterClass(&wndclass);
}

void MakeWindow(LPCWSTR cname)
{
	HWND hWnd=CreateWindow(cname, 
				TEXT("Figure move"),
				WS_OVERLAPPEDWINDOW,
				100, 30, 700, 600,
				0,
				0,
				GetModuleHandle(0),
				0);
	ShowWindow(hWnd, SW_SHOW);
}

void MsgLoop()
{
	MSG Message;
	while(GetMessage(&Message, 0, 0, 0))
	{
		DispatchMessage(&Message);
	}
}

//Function Definitions
#define SX 200
#define SY 10
#define MY_WIDTH 15
#define MY_HEIGHT 15
#define BOARD_WIDTH 20
#define BOARD_HEIGHT 20
#define SCX(x) (SX+(x)*MY_WIDTH)//logic coordinate to screen coordinate
#define SCY(y) (SY+(y)*MY_HEIGHT)

int dir=0;
BOOL is_rect;
POINT now;
const WCHAR dirs[5][2]={TEXT("◎"), TEXT("↑"), TEXT("←") , TEXT("→"), TEXT("↓")};
const int xs[5]={50,50,10,90,50};
const int ys[5]={40,10,40,40,70};

//*****LBUTTONDOWN******
void OnLButtonDown(HWND hWnd)
{
	is_rect^=true;//true->false->true->false
	InvalidateRect(hWnd, 0, TRUE);//make Invalid Section for redrawing
}

//******KEYDOWN******
VOID CALLBACK MoveProc(HWND hWnd, UINT iMessage, UINT_PTR tid, DWORD cnt)
{
	switch(dir)
	{
		case 1: if(now.y>0) now.y--; break;//if unsigned y, --
		case 2: if(now.x>0) now.x--; break;
		case 3: if(now.x<BOARD_WIDTH-1) now.x++; break;
		case 4: if(now.y<BOARD_HEIGHT-1) now.y++; break;
	}
	InvalidateRect(hWnd, 0, TRUE);//ReDrawing
}
void OnKeyDown(HWND hWnd, DWORD vkey)
{
	switch(vkey)
	{
		case VK_LEFT: dir=2; break;
		case VK_RIGHT: dir=3; break;
		case VK_UP: dir=1; break;
		case VK_DOWN: dir=4; break;
		case VK_SPACE: dir=0; break;
		default: return;
	}
	
	static BOOL is_alive=false;
	if(dir==0)//space
	{
		KillTimer(hWnd, 0);
		is_alive=false;
	}
	else
	{//Direction Key Push
		if(is_alive==false)
		{
			SetTimer(hWnd,0,300,MoveProc);//Make Timer with MoveProc
			is_alive=true;
		}
	}
}

//*****ONPAINT******
void DrawDirection(HDC hdc)//OnDraw/DrawInfo/DrawDirection
{//For different style of Current direction with other directions
	SetTextColor(hdc, RGB(0xFF,0,0));
	SetBkColor(hdc, RGB(0, 0xFF, 0xFF));
	TextOut(hdc, xs[dir], ys[dir], dirs[dir], 1);
}
void DrawInfo(HDC hdc)//OnDraw/DrawInfo
{
	DrawDirection(hdc);
	SetTextColor(hdc, RGB(0,0,0));//For ther directions
	SetBkColor(hdc, RGB(0xFF, 0xFF, 0xFF));
	
	for(int i=0; i<5; i++)//For All Keys
	{
		if(i!=dir)//if different with dir value,
		{
			TextOut(hdc, xs[i], ys[i], dirs[i], 1);//print
		}
	}
	WCHAR buf[256];
	wsprintf(buf, TEXT("%3d, %3d"), now.x, now.y);//print current coordinate
	TextOut(hdc, 25, 100, buf, lstrlen(buf));
}

void DrawBoard(HDC hdc)//OnDraw/DrawBoard
{
	HPEN hPen=CreatePen(PS_SOLID, 4, RGB(0,0,0));//Make Solid Pen object
	HGDIOBJ oPen=SelectObject(hdc, hPen);//choose Solid object for drawing
	Rectangle(hdc, SX, SY, SCX(BOARD_WIDTH), SCY(BOARD_HEIGHT));//Drawing
	
	hPen=CreatePen(PS_DOT, 1, RGB(0x7f, 0x7f, 0x7f));//Make Dot Pen object.
	SelectObject(hdc, hPen);//choose object for drawing to PS_DOT
	for(int i=1; i<BOARD_WIDTH; i++)
	{
		MoveToEx(hdc, SCX(i), SCY(0), 0);
		LineTo(hdc, SCX(i), SCY(BOARD_HEIGHT));
	}
	for(int i=1; i<BOARD_HEIGHT; i++)
	{
		MoveToEx(hdc, SCX(0), SCY(i), 0);
		LineTo(hdc, SCX(BOARD_WIDTH), SCY(i));
	}
	SelectObject(hdc, oPen);
}

void DrawBall(HDC hdc)
{
	HBRUSH hBrush=CreateSolidBrush(RGB(0xFF, 0x00, 0x00));
	HGDIOBJ oBrush=SelectObject(hdc, hBrush);
	if(is_rect)
	{
		Rectangle(hdc, SCX(now.x), SCY(now.y), SCX(now.x+1)-1, SCY(now.y+1)-1);
	}
	else
	{
		Ellipse(hdc, SCX(now.x), SCY(now.y), SCX(now.x+1)-1, SCY(now.y+1));
	}
	SelectObject(hdc, oBrush);
}


void OnDraw(HDC hdc)
{
	DrawInfo(hdc);//information print
	DrawBoard(hdc);//Board print
	DrawBall(hdc);//Ball print
}
void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	OnDraw(ps.hdc);//OnDraw is added
	EndPaint(hWnd, &ps);
}


//****ONDESTROY****
void OnDestroy(HWND hWnd)
{
	PostQuitMessage(0);
}

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
		case WM_LBUTTONDOWN:
			OnLButtonDown(hWnd);
			return 0;
			
		case WM_KEYDOWN:
			OnKeyDown(hWnd, wParam);
			return 0;
		
		case WM_PAINT:
			OnPaint(hWnd);
			return 0;
			
		case WM_DESTROY:
			OnDestroy(hWnd);
			return 0;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

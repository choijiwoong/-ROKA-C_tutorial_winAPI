#include <Windows.h>
#define MY_DRAW_WND (TEXT("ex_drawing"))//for class name

void RegWindowClass();
void MessageLoop();

INT APIENTRY WinMain(HINSTANCE hIns, HINSTANCE hPrev, LPSTR cmd, INT nShow)
{
	RegWindowClass();//SET window class property, Register
	
	//Make window instance
	HWND hWnd=CreateWindow(MY_DRAW_WND, //Class Name
						TEXT("Example of Drawing"), //Caption Name
						WS_OVERLAPPEDWINDOW, //Window Style
						10,10,520,420, 
						0, //Handle of Parent window
						0, //Handle of Menu
						hIns, //Handle of instance
						0);//argument with creation
	ShowWindow(hWnd, nShow);//Virtualize(SW_SHOW & SW_HIDE)
	MessageLoop();//Message Loop
	
	return 0;
}

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);//definition
void RegWindowClass()//Window class property set&register
{
	WNDCLASS wndclass={0};
	wndclass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);//Handle of white brush
	wndclass.hCursor=LoadCursor(0, IDC_ARROW);//Handle of mouse cursor
	wndclass.hIcon=LoadIcon(0, IDI_APPLICATION);//Handle of icon
	wndclass.hInstance=GetModuleHandle(0);//Handle of instance in self module
	wndclass.lpfnWndProc=MyWndProc;//already defined! but not created
	wndclass.lpszClassName=MY_DRAW_WND;//Class Name
	wndclass.style=CS_DBLCLKS;//Class kind
	
	RegisterClass(&wndclass);//Register
}

void MessageLoop()
{
	MSG Message;
	while(GetMessage(&Message, 0, 0, 0))//WM_QUIT_Return false; escape
	{
		TranslateMessage(&Message);//WM_KEYDOWN with char key_Return WM_CHAR
		DispatchMessage(&Message);//for callbackproc.
	}
}

void OnPaint(HWND hWnd);
void OnDestroy(HWND hWnd);
LRESULT CALLBACK MyWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)//creation
{
	switch(iMessage)
	{
		case WM_PAINT:
			OnPaint(hWnd);
			return 0;
		case WM_DESTROY:
			OnDestroy(hWnd);
			return 0;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

#define STEP_WIDTH 100//print width
#define STEP_HEIGHT 100//print height
#define LEFT(pi) (pi*STEP_WIDTH)//left x per pen index
#define RIGHT(pi) ((pi+1)*STEP_WIDTH-20)//upper y per brush index
#define TOP(bi) (bi*STEP_HEIGHT+30)//right x per pen index
#define BOTTOM(bi) ((bi+1)*STEP_HEIGHT-30)//bottom y per brush index
enum EN_PEN {HP_RED, HP_GREEN, HP_BLUE, HP2_RED, HPD_RED, HP_MAX};//Pen style enum
enum EH_BRUSH {HB_RED, HB_GREEN, HB_BLUE, HBS_RED, HB_MAX};//Brush style enum

LPCTSTR pstrs[HP_MAX]={TEXT("R"), TEXT("G"), TEXT("B"), TEXT("T R"), TEXT("DD R")};//StringArray of pen print
LPCTSTR bstrs[HB_MAX]={TEXT("R"), TEXT("G"), TEXT("B"), TEXT("C R")};//StringArray of brush print

//Drawing work
void DrawDiagram(HWND hWnd, HDC hdc, HPEN hPen, HBRUSH hBrush, LPRECT prt);
void OnDraw(HWND hWnd, HDC hdc)
{
	HPEN hPens[HP_MAX];
	//Create Pen object for Drawing
	hPens[HP_RED]=CreatePen(PS_SOLID, 1, RGB(255,0,0));
	hPens[HP_GREEN]=CreatePen(PS_SOLID, 1, RGB(0,255,0));
	hPens[HP_BLUE]=CreatePen(PS_SOLID, 1, RGB(0,0,255));
	hPens[HP2_RED]=CreatePen(PS_SOLID, 4, RGB(255,0,0));
	hPens[HPD_RED]=CreatePen(PS_DASHDOT, 1, RGB(255,0,0));
	
	HBRUSH hBrushes[HB_MAX];
	//Create Brush object for Drawing
	hBrushes[HB_RED]=CreateSolidBrush(RGB(255,0,0));
	hBrushes[HB_GREEN]=CreateSolidBrush(RGB(0,255,0));
	hBrushes[HB_BLUE]=CreateSolidBrush(RGB(0,0,255));
	hBrushes[HBS_RED]=CreateHatchBrush(HS_CROSS, RGB(255,0,0));
	
	TCHAR buf[256]=TEXT("");
	RECT rt;
	for(int pi=0; pi<HP_MAX; pi++)
	{
		for(int bi=0; bi<HB_MAX; bi++)
		{
			//set style of Pen & Brush
			wsprintf(buf, TEXT("%s %s"), pstrs[pi], bstrs[bi]);
			//Set Point for print
			rt.left=LEFT(pi);//mecro
			rt.top=TOP(bi);
			rt.right=RIGHT(pi);
			rt.bottom=BOTTOM(bi);
			//printString style of Pen & Brush
			TextOut(hdc, pi*STEP_WIDTH, bi*STEP_HEIGHT, buf, lstrlen(buf));
			//print Figure by special Pen & Brush
			DrawDiagram(hWnd, hdc, hPens[pi], hBrushes[bi], &rt);
		}
	}
	
	//Delete Pen
	for(int pi=0; pi<HP_MAX; pi++)
	{
		DeleteObject(hPens[pi]);
	}
	//Delete Brush
	for(int bi=0; bi<HB_MAX; bi++)
	{
		DeleteObject(hBrushes[bi]);
	}
}

//Draw Figure
void DrawDiagram(HWND hWnd, HDC hdc, HPEN hPen, HBRUSH hBrush, LPRECT prt)
{
	HPEN oPen;//for remember initial handle of Pen
	oPen=(HPEN)SelectObject(hdc, hPen);//save
	
	HBRUSH oBrush;//for remember initial handle of Brush
	oBrush=(HBRUSH)SelectObject(hdc, hBrush);//save
	
	Rectangle(hdc, prt->left, prt->top, prt->right, prt->bottom);//draw rectangle
	Ellipse(hdc, prt->left, prt->top, prt->right, prt->bottom);//dwar ellipse
	
	SelectObject(hdc, oPen);//select initial pen to DC
	SelectObject(hdc, oBrush);//select initial brush to DC
}

//Handling part of message WM_PAINT
void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);//Cal unvalid section & Register DC
	OnDraw(hWnd, ps.hdc);//Wrok Drawwing
	EndPaint(hWnd, &ps);//Change to valud & Delete DC
}

//Handling part of message WM_DESTROY
void OnDestroy(HWND hWnd)
{
	PostQuitMessage(0);//pass WM_QUIT to message queue
}

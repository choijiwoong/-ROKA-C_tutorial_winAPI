/*
	[테트리스]
1.	WM_TIMER메시지로 게임 전체의 속도를 조절한다.  WndProc에서 사용된 일반함수 11개는 아래와 같은 기능을 갖는다.
	makenewblock_새 벽돌 만들기, 게임 끝처리
	drawscreen_화면다시그리기 
	printscore_점수출력 
	drawnextbrick_다음에 나올 벽돌을 출력
	brick_이동중인 벽돌을 그리거나 삭제
	downmove, leftmove, rightmove_움직임 제어
	rotate_벽돌 회전
	whataround_지정한 좌표 주변을 검사
	isfull_한줄이 다 찼는지 
	
	[벽돌 그리기]
1.	 
*/

#include <windows.h>
#include <stdlib.h>
#include "resource.h"

#define random(n) (rand()%n)//macro for making random

//Origin of Functions
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void brick(int flag);
void leftmove();
void rightmove();
int downmove();
void rotate();
int whataround(int, int);
void isfull();
void printstage();
void makenewblock();
void drawscreen();
void printscore();
void drawnextbrick(); 

int pat[7][32]={
  {0,0,1,0,2,0,-1,0,0,0,0,1,0,-1,0,-2,},
  {0,0,1,0,0,1,1,1,},
  {0,0,-1,0,0,-1,1,-1,0,0,0,1,-1,0,-1,-1,},
  {0,0,-1,-1,0,-1,1,0,0,0,-1,0,-1,1,0,-1,},
  {0,0,0,-1,1,0,2,0,0,0,-1,0,0,-1,0,-2,0,0,-1,0,- 2,0,0,1,0,0,1,0,0,1,0,2},
  {0,0,-1,0,-2,0,0,-1,0,0,-1,0,0,1,0,2,0,0,0,1,1,0,2,0,0,0,1,0,0,-1,0,-2},
  {0,0,-1,0,1,0,0,1,0,0,0,-1,0,1,1,0,0,0,-1,0,1,0,0,-1,0,0,-1,0,0,-1,0,1}
};//Brick's pattern on array.
int brickpatnum[7]={2,1,2,2,4,4,4};//pattern count of each brick
int tet[14][23];//board (game)
int i,j,k;//temp variables on loof
int score;
int speed;
char imsistr[128];//buffer for image
int nowbrick, nowrot;//current pattern of brick, rotate shape
int nextbrick;
int nowx, nowy;
BOOL GameStart;//is game working 

HDC hDC, hMemDC, hMemDC2;
HBITMAP hbrick, hbrick2;
HINSTANCE g_hInst;
HWND hWnd;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon=LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=(WNDPROC)WndProc;
	WndClass.lpszClassName="Shtet3";
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style=CS_HREDRAW|CS_VREDRAW;
	RegisterClass(&WndClass);
	
	hWnd=CreateWindow("Shtet3", "Sh tetris 3", WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
			50,50,470,460, NULL, NULL, g_hInst, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	while(GetMessage(&Message,0,0,0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static FARPROC lpfnAboutDlgProc;
	PAINTSTRUCT ps;
	
	switch(iMessage)
	{
		case WM_CREATE:
			hDC=GetDC(hWnd);
			hMemDC=CreateCompatibleDC(hDC);//
			hMemDC2=CreateCompatibleDC(hDC);//
			ReleaseDC(hWnd, hDC);//
			
			hbrick=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BRICK));//brick shape
			SelectObject(hMemDC, hbrick);
			hbrick2=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BLACK));//space for erasing bricks
			SelectObject(hMemDC2, hbrick2);
			
			GameStart=FALSE;//not TRUE yet
			return 0;
			
		case WM_KEYDOWN:
			switch(wParam)//which key down?
			{
				case VK_LEFT:
					leftmove();
					break;
					
				case VK_RIGHT:
					rightmove();
					break;
					
				case VK_UP://rotate block by VK_UP
					rotate();
					break;
					
				case WK_DOWN:
					while(downmove());//place fixing.
					makenewblock();
					break;
			}
			return 0;
			
		case WM_TIMER://if timer is over
			if(downmove()==0)//if block is fixed
				makenewblock();
			return 0;
				
		case WM_PAINT:
			hDC=BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			
			drawscreen();
			if(GameStart)
				brick(1);//
			return 0;
		
		case WM_COMMAND://parse command
			switch(wParam)
			{
				case ID_START:
					
					for(i=0; i<23; i++)
						for(j=0; j<14; j++)
							if(i==21||j==0||j==13)
								tet[j][i]=2;
							else//?
								tet[j][i]=0;
					
					speed=500;
					score=0;
					SetTimer(hWnd, 1, speed, NULL);
					nextbrick=random(7);
					
					makenewblock();
					EnableMenuItem(GetMenu(hWnd), ID_START, MF_BYCOMMAND|MF_DISABLED|MF_GRAVED);//
					GameStart=TRUE;
					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);
					break;
			}
			return 0;
		
		case WM_DESTROY:
			DeleteDC(hMemDC);
			DeleteDC(hMemDC2);
			KillTimer(hWnd, 1);
			DeleteObject(hbrick);
			DeleteObject(hbricks);
			PostQuitMessage(0);
			return 0;
		
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

//normal logic functions
void makenewblock()
{
	nowbrick=nextbrick;
	nextbrick=random(7);
	nowx=8;
	nowy=3;
	nowrot=0;
	brick(1);
	drawnextbrick();
	
	if(whataround(nowx,nowy)!=0)//another bricks is already existing around (nowx, nowy)
	{
		KillTimer(hWnd, 1);
		GameStart=FALSE;
		
		if(MessageBox(hWnd, "Game Over!", "Tetris", MB_YESNO|MB_ICONHAND)==IDYES)//make popup for choice regamge or not to user.
			SendMessage(hWnd, WM_COMMAND, ID_START, NULL);
		else
			EnableMenuItem(GetMenu(hWnd), ID_START, MF_BYCOMMAND|MF_ENABLED);
	}
}

void drawscreen()
{
	HDC c;
	hDC=GetDC(hWnd);
	Rectangle(hDC, 8,8,252,412);
	Rectangle(hDC, 260,8,460,150);
	TextOut(hDC, 320,20, "Shtet 3", 7);
	
	printscore();
	
	Rectangle(hDC, 260,300,460,412);
	for(i=1; i<=20; i++)
	{
		for(j=1; j<=12; j++)
		{
			if(tet[j][i]==1)
				c=hMemDC;
			else
				c=hMemDC2;
			
			BitBlt(hDC, (j-1)*20+10, (i-1)*20+10, 20, 20, c, 0, 0, SRCCOPY);
		}
	}
	ReleaseDC(hWnd, hDC);
	drawnextbrick();
}

void drawnextbrick()
{
	hDC=GetDC(hWnd);
	RECT rect={261, 301, 459, 399};
	FillRect(hDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	
	for(i=0; i<4; i++)
		BitBlt(hDC, (17+pat[nextbrick][i*2]-1)*20, (18+pat[nextbrick][i*2+1]-1)*20, 20, 20, hMemDC, 0, 0, SRCCOPY);
	ReleaseDC(hWnd, hDC);
}

void printscore()
{
	wsprintf(imsistr, "Score: %d", score);
	TextOut(hDC, 260,200, imsistr, strlen(imsistr));
}

void brick(int flag)
{
	HDC c;
	hDC=GetDC(hWnd);
	
	if(flag==0)
		c=hMemDC2;
	else
		c=hMemDC;
	
	for(i=0; i<4; i++)
		BitBlt(hDC, (nowx+pat[nowbrick][nowrot*8+i*2]-1)*20+10, (nowy+pat[nowrot*8+i*2+1]-1)*20+10, 20,20,c,0,0,SRCCOPY);
	
	ReleaseDC(hWnd, hDC);
}

int downmove()
{
	if(whataround(nowx, nowy+1)!=0)
	{
		isfull();
		return 0;
	}
	
	brick(0);
	nowy++;
	brick(1);//
	return 1;
}

int leftmove()
{
	if(whataround(nowx-1, nowy)!=0)
		return;
		
	brick(0);
	nowx-=1;
	brick(1);
}

void rightmove()
{
	if(whataround(nowx+1, nowy)!=0)
		return;
		
	brick(0);
	nowx+=1;
	brick(1);
}

void rotate()
{
	int imsi, imsi2;
	imsi=nowrot;
	nowrot++;
	
	if(nowrot==brickpatnum[nowbrick])//if rotate is imposssible, no rotate.
		nowrot=0;
	
	if(whataround(nowx, nowy)!=0)
	{
		nowrot=imsi;
		return;
	}
	
	imsi2=nowrot;
	nowrot=imsi;
	brick(0);
	nowrot=imsi2;
	brick(1);
}

int whataround(int x, int y)//return_0: empty, 1: block by brick, 2: block by wall
{
	k=0;
	for(i=0; i<4; i++)
	{
		j=tet[x+pat[nowbrick][nowrot*8+i*2]] [u+pat[nowbrick][nowrot*8+i*2+1]];
		
		if(j>k)
			k=j;
	}
	
	return k;
}

void isfull()
{
	int check, check2;
	score++;
	printscore();
	
	for(i=0; i<4; i++)
		tet[nowx+pat[nowbrick][nowrot*8+i*2]] [nowy+pat[nowbrick][nowrot*8+i*2+1]]=1;
	check2=0;
	
	for(i=20; i>=0; i--)
	{
		check=0;
		for(j=1; j<13; j++)
			if(tet[j][i]==0)
				check=1;
		
		if(check==0)
		{
			check2=1;
			for(j=i-1; k>0; k--)
				for(j=1; j<13; j++)
					tet[j][k+1]=tet[j][k];
			i++;
		}
	}
	
	if(check2==0)
		return 0;
	
	score+=10;
	KillTimer(hWnd, 1);
	speed-=10;
	
	if(speed<100)
		speed=100;
		
	SetTimer(hWnd, 1, speed, NULL);
	drawscreen();
}

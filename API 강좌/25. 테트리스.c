/*
	[��Ʈ����]
1.	WM_TIMER�޽����� ���� ��ü�� �ӵ��� �����Ѵ�.  WndProc���� ���� �Ϲ��Լ� 11���� �Ʒ��� ���� ����� ���´�.
	makenewblock_�� ���� �����, ���� ��ó��
	drawscreen_ȭ��ٽñ׸��� 
	printscore_������� 
	drawnextbrick_������ ���� ������ ���
	brick_�̵����� ������ �׸��ų� ����
	downmove, leftmove, rightmove_������ ����
	rotate_���� ȸ��
	whataround_������ ��ǥ �ֺ��� �˻�
	isfull_������ �� á���� 
	
	[���� �׸���]
1.	pat�迭�� ������ ����� �����صθ�, ��ü ������ ����� 7��, �ִ� 4���� ȸ�����, �� ���� ����� ���� 8����Ʈ�� ����ϱ⿡
	pat[7][4*8]�� �Ǿ���. �ϳ��� ���簢�� ��ǥ�� ���� (x, y)��ǥ���� �ʿ��ϱ⿡ 4(���簢������)*2(x,y)=8����Ʈ�� �ϳ��� ����� ��Ÿ����. 
	(�ణ ���� ���� AI���鶧 ��ü �񱳳׸� 5x5ĭ�� �������� �Ѱ�ó�� ����� ����5*����4�� �׸��� ������ǥ�� �߽����� ��� ���ϳ�) 
2.	��Ī���� �� ������ ����� ��������� ��� 4���� ȸ�����¸� �� ������ �ʱ⿡ �ʱⰪ�� 0���� ó���ߴ�. 
3.	��������� IDB_BRICK�̶�� ID ��Ʈ�� ���ҽ���, ���� ������ ���� ������ IDB_BLANK��Ʈ������ ����Ǿ�������, memDC�� �����صд�.
4.	brick�Լ��� flag�� ���� ���� ������ ����ϰų�, �����Ѵ�. 
5.	tet�迭�� ���� �̹� �ٴڿ� ��ġ�Ǿ��ִ� ������ ������ ����Ѵ�. �������� �ʺ�14*����23�̹Ƿ� [14][23]�� ũ�⸦ ������, 0��ĭ 1���� 2������ �����Ѵ�.
6.	drawscreen�� ���� WM_PAINT�� ȣ���ϸ� tet�迭�� �����Ͽ� default ������ �׸��� printscore�� ������ ���, drawnextbrick���� ���� ������ �׸���. 

	[Ű���� �Է�ó��]
*/

#include <windows.h>
#include <stdlib.h>
#include "resource.h"

#define random(n) (rand()%n)//macro for making random

//Origin of Functions
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//Window Callback Procedure 
void brick(int flag);//print or erase brick by flag value. tool function
void leftmove();
void rightmove();
int downmove();
void rotate();
int whataround(int, int);//check around brick. is there disable
void isfull();//game over
void printstage();//
void makenewblock();//make next brick
void drawscreen();//WM_PAINT call
void printscore();
void drawnextbrick();//draw next bricks after calling drawscreen, printscore.

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
			SelectObject(hMemDC, hbrick);//real brick to hMemDC
			hbrick2=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BLACK));//space for erasing bricks
			SelectObject(hMemDC2, hbrick2);//blank brick to hMemDC2
			
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
			//Call downmove per 0.5 second and if it's done(return 0), make new block.
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
				case ID_START://handle start of game 
					
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
	
	//handle end of game here. if new bricks can't be placed around it.
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
	
	if(flag==0)//choose IDB_BLANK
		c=hMemDC2;
	else//choose IDB_BRICK
		c=hMemDC;
	
	for(i=0; i<4; i++)
		BitBlt(hDC, (nowx+pat[nowbrick][nowrot*8+i*2]-1)*20+10, (nowy+pat[nowrot*8+i*2+1]-1)*20+10, 20,20,c,0,0,SRCCOPY);
	//get coordinate with nowbrick(shape of brick) & nowrot(state of rotate)
	//We can get start address of brick data by multiplying 8 to nowrot.
	//By for loop, get 4 bricks's data(coordinate_x, y)
	//Use it Like vararray in C++
	//draw depending on nowx&nowy , C that can contain hMemDC or hMemDC2
	
	ReleaseDC(hWnd, hDC);
}

int downmove()
{
	if(whataround(nowx, nowy+1)!=0)
	{
		isfull();//is reach to bottom?
		return 0;
	}
	
	brick(0);//draw by IDB_BRICK
	nowy++;//move nowx, nowy to last point that will be erased
	brick(1);//erase by IDB_BLANK
	return 1;
}

int leftmove()
{
	if(whataround(nowx-1, nowy)!=0)
		return;//if work success. no disable around brick
		
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
	//Rotate First, Check around Last because nowrot is global.
	
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
	
	return k;//empty 0, wall 2, bricks 1
}

void isfull()//erase brick. it's called when new bricks is fixed to bottom.
{
	int check, check2;
	score++;
	printscore();
	
	for(i=0; i<4; i++)
		tet[nowx+pat[nowbrick][nowrot*8+i*2]] [nowy+pat[nowbrick][nowrot*8+i*2+1]]=1;
	check2=0;
	
	for(i=20; i>=0; i--)//is line full all?
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
		//if fulled line is detected, erase and copy upper line to down.
		//for rechecking that moved line, it should stay that location.
	}
	
	if(check2==0)//any line is not removed.
		return 0;
	
	score+=10;
	KillTimer(hWnd, 1);
	speed-=10;
	
	if(speed<100)
		speed=100;
		
	SetTimer(hWnd, 1, speed, NULL);
	drawscreen();
}

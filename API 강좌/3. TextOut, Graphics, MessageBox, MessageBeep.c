//1. DrawText
//BOOL TextOut(hdc, nXStart, nYStart, lpszString, cbString);//���� ���� �� ����� ���� DrawText�� �ִ�.
//int DrawText(HDC, hDC, LPCTSTR lpString, int nCount, LPRECT lpRect, UINT uFormat);//RECT�簢������ �����Ͽ� �����ȿ� ���ڿ� ����� �����ϸ�, ���� ���伳���� �����ϴ�.

//DrawText�� ����° ������ ������ �Ʒ��� ����. 
//typedef struct _RECT_practice{
	//LONG left;
	//LONG top;
	//LONG right;
	//LONG bottom;
//} RECT_practice; 

//uFormat���δ� DT_LEFT, DT_RIGHT, DT_CENTOR, DT_BOTTOM, DT_VCENTER,
//				DT_WORDBREAK(�簢������ ������ ������ �ڵ� ����\n�ǰ�), DT_SINGLELINE(�������), DT_NOCLIP(��踦 ����� �״�� ���) 


//������ WM_PAINT�� �ڵ带 DrawText�� �̿��Ͽ� �Ʒ��� ���� �ٲ� �� �ִ�. 
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
//COLORREF SetPixel(hdc, nXPos, nYPos, clrref)//�� ���(clrref�� ����) 
//DWORD MoveToEx(hdc, x, y, lpPoint);//�����׸��� �� �� �Լ��� ����Ѵ�. GDI�� CP�� �׻� ���������ֱ� ������ LineTo�Լ��� CP�������� ������ ��ǥ���� ���� ������ CP�� �������� �̵���Ų��.
//BOOL LineTo(hdc, xEnd, yEnd);//��� �̸� ������ ȣ���ϸ� ���ӵ� ���� �������ִ�. MoveTo�� CP�� ������ ��ǥ�� �̵���Ű��, �� CP�� ��ǥ�� lpPoint�� �������ش�.(NULL������) 
//BOOL Rectangle(hdc, nLeftRect, nTopRect, nRightRect, nBottomRect)
//BOOL Ellipse(hdc, nLeftRect, nTopRect, nRightRect, nBottomRect)
//���������� DC�� �ڵ��� �޾� ����Ѵ�. 

//(A, B) - (C, D)������ ���� �׸��� �۾��� �Ʒ��� ����.
//MoveToEx(hdc, A, B, NULL) ;
//LineTo(hdc, C, D);

//Rectangle�Լ��� �簢���� ���θ� ä��ٴ� ���� �����Ͽ� ����ϸ� �ȴ�. 

//3. MessageBox
//int MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
//hWnd�� ���� ������� MessageBox activate���� ���� ������� ����� �� ���� ���°� �ȴ�.(�ݰų� Ȯ�δ����ų�..��� ���־� ��) 
//hpText�� ����� ���ڿ�, lpCaption�� Ÿ��Ʋ���� TitleName, uTupe�� �޽������� ���� �÷��׵��̴�. 
//MB_ABORTRETRYIGNORE, MB_OK, MB_OKCANCEL, MB_RETRYCANCEL, MB_YESNO, MB_YESNOCANCEL�� ��ư�� ������ �� �ְ�, �Ʒ��� �÷��׷� ��ư�� �������� ����� �� �ִ�. 
//MB_ICONEXCLAMATION & MB_ICONWARNING(���!), MB_ICONINFORMATION & MB_ICONASTERISK(��ǳ��!), MB_ICONQUESTION(��ǳ��?), MB_ICONSTOP & MB_ICONEERROR & MB_ICONHAND(����x���׶��)

//�ڵ�� ���� �ڵ��� WM_LBUTTONDOWN�� ��������. 

//MessageBox�� ���ϰ��� ���� ��ư������ �Ʒ��� ����. 
//IDABORT, IDCANCEL, IDIGNORE, IDNO, IDOK, IDRETRY, IDYES
//MessageBox�� ���ϰ��� �̿��ϴ� �ڵ�� �Ʒ��� ���� �ۼ��ϸ� �ȴ�.
//if(MessageBox(hWnd, "Do you want to continue?", "Queation", MB_YESNO)==IDYES){
	//Keep code
//} else{
	//Stop code
//}


//4. MessageBeep
//�������� ���� �鸮�� ������ ����� ������ ����� �Ʒ��� �Լ��� ����Ѵ�.
//BOOL MessageBeep(UINT uType);
//uType�� �����δ� 0xFFFFFFFF(PC�� ����Ŀ�̿�), MB_ICONASTERISK(Asterisk ������), MB_ICONEXCLAMATION(Exclamation������),
//					MB_ICONHAND(Hand ������), MB_ICONQUESTION(Question ������), MB_OK(system default ������)

//WM_LBUTTON�޽��� ó�� ��ƾ�� MessageBeep�� ������ ���콺 ���� ��ư�� ���� ������ �������� �鸱 ���̴�.(�ý��ۿ� ����ī��� �����Ͱ� �ִٴ� �����Ͽ�..) 
//�ڵ�� ���� LBUTTONDOWN�ڵ带 ��������. 

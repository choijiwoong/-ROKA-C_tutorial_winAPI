/*
	[리스트 박스]
1.	선택 가능한 여러 항목 중에 하나 혹은 여러개를 선택하게 하는 것이다. listbox윈도우 클래스를 사용하며 스타일은 아래와 같다.
	LBS_MULTIPLESEL, LBS_NOTIFY, LBS_SORT, LBS_OWNERDRAW, LBS_STANDARO(LBS_NOTIFY|LBS_SORT|WS_BORDER)
2.	부모 윈도우로 notification msg를 보내는 것이 일반적이기에 LBS_NOTIFY가 많이 사용되며 부모 윈도우가 리스트 박스를 조작하는데 사용되는 리스트 박스 메시지는 다음과 같다.
	LB_ADDSTRING(string_lParam), LB_DELETESTRING(id_wParam), LB_GETCURSEL(search index of checked), LB_GETTEXT(id_wParam, lParam_buffer), LB_GETCOUNT(count), LB_SETCURSEL(select item_wParam)
3.	리스트 박스에서 event발생시 부모 윈도우로 보내는 메시지는 아래와 같다.
	LBN_DBLCLK, LBN_ERRSPACE, LBN_KILLFOCUS, LBN_SELCANCEL, LBN_SELCHANGE, LBN_SETFOCUS
	 
	[콤보 박스]
1.	에디트 컨트롤과 리스트 박스를 결합시킨 것으로, 목록중에 하나를 선택할 수도 있고, 없으면 에디트에 직접 항목을 넣을 수도 있다.(화살표 누르지 말고 네모칸을 에디트처럼 직접 입력가능) 
	리스트 박스와 달리 필요할 경우에만 목록을 열어 선택하기에 화면 공간을 적게 차지한다.
2.	combobox윈도우 클래스를 사용하며 다음의 스타일(종류)를 가진다.(CBX_SIMPLE(에디트만), CBS_DROPDOWN(에디트+리스트박스), CBS_DROPDOWNLIST(리스트박스, 에디트 입력불가) 
*/

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="list box";

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

#define ID_LISTBOX 100//ID FOR LISTBOX
char Items[][15]={"Apple", "Orange", "Melon", "Graph", "Strawberry"};
char str[128];
HWND hList;

#define ID_COMBOBOX 101
HWND hCombo; 

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)//,,(msg, id), content
{
	int i;
	switch(iMessage)
	{
		case WM_CREATE:
			hList=CreateWindow("listbox", NULL, WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_NOTIFY,
					10,10,100,200, hWnd, (HMENU)ID_LISTBOX, g_hInst, NULL);//Make ListBox with LBS_NOTIFY
			for(i=0; i<5; i++)
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)Items[i]);//Add items to hList by using LB_ADDSTRING Message(to Parent Window)
				
			hCombo=CreateWindow("combobox", NULL, WS_CHILD|WS_VISIBLE|CBS_DROPDOWN,
					150,10,100,200, hWnd, (HMENU)ID_COMBOBOX, g_hInst, NULL);//Create Combo box
			for(i=0; i<5; i++)
				SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)Items[i]);//sendMessage to combobox Items by LPARAM CB_ADDSTRING
							
			return 0;
			
		case WM_COMMAND:
			switch(LOWORD(wParam))//get List
			{
				case ID_LISTBOX:
					switch(HIWORD(wParam))//get MSG(listbox->parent when eventing)
					{
						case LBN_SELCHANGE://if we get LBN_SELCHANGE MSG to ListBox
							i=SendMessage(hList, LB_GETCURSEL, 0, 0);//get index of clicked item for access to that element in Items[][]
							//get index of current clicked
							SendMessage(hList, LB_GETTEXT, i, (LPARAM)str);//send msg to hList "LB_GETTEXT" that is in i, to str
							//send message for getting string by using index that we got
							SetWindowText(hWnd, str);//set windowText to str that we get by SendMessage function
							//set window's text to string that we got
							break;
					}
					break;
				
				case ID_COMBOBOX://for combo box. check id by wParam's LOWORD
					switch(HIWORD(wParam))//check msg by wParam's HIWORD
					{
						case CBN_SELCHANGE://if Listbox is changed
							i=SendMessage(hCombo, CB_GETCURSEL,0,0);//get choiced id to i of hCombo by message; CB_GETCURSEL
							SendMessage(hCombo, CB_GETLBTEXT, i, (LPARAM)str);//get TEXT of hCombo's list by using i(choiced id) to str
							SetWindowText(hWnd, str);//set window text to str
							break;
						
						case CBN_EDITCHANGE://if Edit is changed
							GetWindowText(hCombo, str, 128);//get hCombo's text to str
							SetWindowText(hWnd, str);//and set
							break;
					}
					break;
			}
			return 0;
			
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

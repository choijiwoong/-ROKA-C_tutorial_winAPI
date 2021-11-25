//1.윈도우즈 프로그램의 기본 흐름: 윈도우 클래스 속성 설정(WNDCLASS), 윈도우 클래스 등록(RegisterClass), 윈도우 인스턴스 생성(CreateWindow), 
//	윈도우 인스턴스 시각화(ShowWindow), 메시지 루프(MSG message; while(GetMessage) DispatchMessage) 
	
/*
2.	윈도우 클래스 속성은 WNDCLASS를 사용하여 설정하는데,
	style은 CS_CREDRAW & CS_HREDRAW(크기변할때 화면영역 다시 설정), CS_DBLCLKS(마우스 더블 클릭 메시지 발생가능하게 설정)..등으로 설정이 가능하다.
	
	인스턴스에 마우스클릭, 키보드클릭 등의 event를 처리할 프로세스의 응용 메시지 큐에 보내거나 윈도우 콜백 프로시저를 호출하므로 이(lpfnWndProc)를 윈도우 클래스 등록 시 설정해야 한다. 
	(typedef LRESULT (CALLBACK* WNDPROC)(HWND, UNIT, WPARAM, LPARAM);//와 같이 윈도우 콜백 프로시저의 함수 포인터 형식을 타입 재지정할 수 있다.
	위의 예시와 같이 윈도우 프로시저의 입력 매개변수는 4개인데, (윈도우 인스턴스 핸들, 윈도우 메시지 ID, 메시지 처리에 필요한 메타정보1,2)로 구성된다.
	
	hInstance는 윈도우 클래스를 등록하는 모듈의 인스턴스 핸들로, 운영체제에서 누가 등록하였는지 저장하다가 모듈이 사라질 때 등록된 윈도우 클래스도 해제한다.

	hIcon은 윈도우 인스턴스 좌측 상단 모서리에 작게 뜨는 이미지에 대한 핸들이다.
	
	hCursor은 윈도우 인스턴스에 마우스 좌표가 위치할 때, 보이는 커서에 대한 핸들이다.
	
	hbrCackground는 윈도우 인스턴스 배경색에 대한 브러쉬 핸들이다.
	
	lpszMenuName은 메뉴 핸들을 문자열로 변형한 값이고, lpszClassName은 윈도우 클래스 구분자인 클래스 이름이다.
*/
struct WNDCLASS{
	UNIT 		style;//(unsigned int)윈도우 클래스 스타일
	WNDPROC 	lpfnWndProc;//()콜백 프로시저
	int 		cbClsExtra;//윈도우 클래스의 여분 메모리 
	int 		cbWndExtra;//윈도우 인스턴스의 여분 메모리 
	
	HINSTANCE	hInstance;//윈도우 클래스를 등록하는 모듈의 인스턴스 핸들_push as argument, not in Class setting
	HICON		hIcon;//아이콘 핸들 
	HCURSOR		hCursor;//커서 핸들 
	HBRUSH		hbrBackground;//배경 브러쉬 
	
	LPCWSTR		lpszMenuName;//(포인터)메뉴 이름
	LPCWSTR		lpszClassName;//(포인터)클래스 이름 _push as argument, not in Class setting
};
//아래는 윈도우 클래스의 설정 예시이다. hInstance, lpszClassName을 제외한 항목을 세팅하며 이 2개의 항목은 생성시 인자로 세팅한다.
WNDCLASS wndclass={0};

wndclass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);//배경핸들 세팅
wndclass.hCursor=LoadCursor(0, IDC_ARROW);//커서 핸들 세팅
wndclass.hIcon=LoadIcon(0, IDI_APPLICATION);//아이콘 핸들 세팅
wndclass.hInstance=GetModuleHandle(0);//인스턴스 핸들 세팅(of 윈도우 클래스를 등록하는 모듈)
wndclass.lpfnWndProc=MyWndProc;//콜백프로시저 세팅
wndclass.lpszClassName=MY_FIRST_WND;//메뉴이름 세팅
wndclass.style=CS_DBLCLKS;//클래스 종류 세팅(더블클릭가능)

//설정 후 등록은 RegisterClass를 호출한다.
ATOM WINAPI RegisterClass(WNDCLASS *lpWndClass);//성공은 1, 실패는 0반환
//ATOM WINAPI RegisterClass(WNDCLASS *wndclass);

//윈도우 인스턴스(윈도우, 관리하는 정보) 생성은 등록 상태의 윈도우 클래스를 CreateWindow함수를 통해 생성한다.
HWND WINAPI CreateWindow(LPCWSTR lpClassName, //윈도우 클래스 이름(고유 이름)__
						 LPCWSTR lpWindowName,//윈도우 인스턴스 캡션 명(타이틀)
						 DWORD dwStyle,
						 int X,//좌상단 X, Y
						 int Y,
						 int nWidth,//너비
						 int nHeight,//높이
						 HWND hWndParent,//부모 윈도우 핸들_다른 윈도우의 클라이언트 영역에 배치할 경우 부모 윈도우 핸들도 같이 전달.
						 HMENU hMenu,//메뉴 핸들
						 HINSTANCE hInstance,//모듈의 인스턴스 핸들__프로세스 자신의 인스턴스 행들. 운영체제가 누가 생성을 요청했는지 구분하기 위해 사용한다.
						 LPVOID lpParam);//생성할 때 전달할 인자

//생성 후 화면에 시각화하기 위해서는 ShowWindow함수를 호출시킨다.
ShowWindow(hWnd, nShow);//이제 진짜 출력!! 시각화!! (SH_CHOW는 시각화, SH_HIDE는 비시각화를 나타낸다.)


//3. 윈도우즈 프로그램은 window instance위의 사용자의 컨트롤, Click, type등에 따라 동작하는데, 이러한 동작을 운영체제에서 프로세스의 응용 메시지 큐에 전달하고, 응용메시지 큐에 도착한 메시지를 하나씩 콜백 프로시저에 넣어 처리한다.
MSG Message;
while(GetMessage(&Message, 0, 0, 0))
{
	TranslateMessage(&Message);//WM_KEYDOWN이고 키가 문자일때, WM_CHAR발생(키가 눌렸는데 문자면 WM_CHAR발생)
	DispatchMessage(&Message);//콜백 프로시저가 수행할 수 있게 전달(Dispatch)한다.
}

//이때 MSG 구조체는 아래와 같이 생겼다.
struct MSG{//아래의 구조체를 통하여 이벤트처리에 필요한 정보를 콜백 프로시저에 전달
	HWND hwnd;//Event가 일어난 윈도우의 핸들
	UNIT message;//메시지 구분자?
	WPARAM wParam;//메시지 처리에 필요한 인자1, 2
	LPARAM lParam;
	DWORD time;//Event 발생 시각
	POINT pt;//Event 발생 시 마우스 좌표
};

//4. 전달받은 Message를 처리하기 위해 메시지 루프에서 먼저 GetMessage를 통해 메시지 큐의 메시지를 꺼낸다.
BOOL WINAPI GetMessage(LPMSG lpMsg, HWND hWnd, UNIT wMsgFilterMin, UNIT wMsgFilterMax);
//lpMsg를 통해 msg를 설정하고, hWnd & wMsgFilterMin & wMsgFilterMax를 통해 원하는 메시지를 필터링한다.
//ex) 특정 윈도우에 발생한 메시지만 원하면 두번째 인자에 유효한 윈도우 핸들을 전달하고, 세번째와 네번째로 메시지 번호의 구간을 전달한다.
//주의사항은 GetMessage의 반환값은 메시지큐에 메시지가 없을때 FALSE가 아니라 WM_QUIT MSG를 꺼낼 때 FALSE이기에 처리할 메시지가 없을 때 프로세스를 종료하는 것이 아닌, 종료 메시지를 수신할때인 것에 유의해야한다.

//WM_QUIT을 메시지 큐에 전달할 때는 PostQuitMessage를 호출하여 전달한다.
//1.�������� ���α׷��� �⺻ �帧: ������ Ŭ���� �Ӽ� ����(WNDCLASS), ������ Ŭ���� ���(RegisterClass), ������ �ν��Ͻ� ����(CreateWindow), 
//	������ �ν��Ͻ� �ð�ȭ(ShowWindow), �޽��� ����(MSG message; while(GetMessage) DispatchMessage) 
	
/*
2.	������ Ŭ���� �Ӽ��� WNDCLASS�� ����Ͽ� �����ϴµ�,
	style�� CS_CREDRAW & CS_HREDRAW(ũ�⺯�Ҷ� ȭ�鿵�� �ٽ� ����), CS_DBLCLKS(���콺 ���� Ŭ�� �޽��� �߻������ϰ� ����)..������ ������ �����ϴ�.
	
	�ν��Ͻ��� ���콺Ŭ��, Ű����Ŭ�� ���� event�� ó���� ���μ����� ���� �޽��� ť�� �����ų� ������ �ݹ� ���ν����� ȣ���ϹǷ� ��(lpfnWndProc)�� ������ Ŭ���� ��� �� �����ؾ� �Ѵ�. 
	(typedef LRESULT (CALLBACK* WNDPROC)(HWND, UNIT, WPARAM, LPARAM);//�� ���� ������ �ݹ� ���ν����� �Լ� ������ ������ Ÿ�� �������� �� �ִ�.
	���� ���ÿ� ���� ������ ���ν����� �Է� �Ű������� 4���ε�, (������ �ν��Ͻ� �ڵ�, ������ �޽��� ID, �޽��� ó���� �ʿ��� ��Ÿ����1,2)�� �����ȴ�.
	
	hInstance�� ������ Ŭ������ ����ϴ� ����� �ν��Ͻ� �ڵ��, �ü������ ���� ����Ͽ����� �����ϴٰ� ����� ����� �� ��ϵ� ������ Ŭ������ �����Ѵ�.

	hIcon�� ������ �ν��Ͻ� ���� ��� �𼭸��� �۰� �ߴ� �̹����� ���� �ڵ��̴�.
	
	hCursor�� ������ �ν��Ͻ��� ���콺 ��ǥ�� ��ġ�� ��, ���̴� Ŀ���� ���� �ڵ��̴�.
	
	hbrCackground�� ������ �ν��Ͻ� ������ ���� �귯�� �ڵ��̴�.
	
	lpszMenuName�� �޴� �ڵ��� ���ڿ��� ������ ���̰�, lpszClassName�� ������ Ŭ���� �������� Ŭ���� �̸��̴�.
*/
struct WNDCLASS{
	UNIT 		style;//(unsigned int)������ Ŭ���� ��Ÿ��
	WNDPROC 	lpfnWndProc;//()�ݹ� ���ν���
	int 		cbClsExtra;//������ Ŭ������ ���� �޸� 
	int 		cbWndExtra;//������ �ν��Ͻ��� ���� �޸� 
	
	HINSTANCE	hInstance;//������ Ŭ������ ����ϴ� ����� �ν��Ͻ� �ڵ�_push as argument, not in Class setting
	HICON		hIcon;//������ �ڵ� 
	HCURSOR		hCursor;//Ŀ�� �ڵ� 
	HBRUSH		hbrBackground;//��� �귯�� 
	
	LPCWSTR		lpszMenuName;//(������)�޴� �̸�
	LPCWSTR		lpszClassName;//(������)Ŭ���� �̸� _push as argument, not in Class setting
};
//�Ʒ��� ������ Ŭ������ ���� �����̴�. hInstance, lpszClassName�� ������ �׸��� �����ϸ� �� 2���� �׸��� ������ ���ڷ� �����Ѵ�.
WNDCLASS wndclass={0};

wndclass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);//����ڵ� ����
wndclass.hCursor=LoadCursor(0, IDC_ARROW);//Ŀ�� �ڵ� ����
wndclass.hIcon=LoadIcon(0, IDI_APPLICATION);//������ �ڵ� ����
wndclass.hInstance=GetModuleHandle(0);//�ν��Ͻ� �ڵ� ����(of ������ Ŭ������ ����ϴ� ���)
wndclass.lpfnWndProc=MyWndProc;//�ݹ����ν��� ����
wndclass.lpszClassName=MY_FIRST_WND;//�޴��̸� ����
wndclass.style=CS_DBLCLKS;//Ŭ���� ���� ����(����Ŭ������)

//���� �� ����� RegisterClass�� ȣ���Ѵ�.
ATOM WINAPI RegisterClass(WNDCLASS *lpWndClass);//������ 1, ���д� 0��ȯ
//ATOM WINAPI RegisterClass(WNDCLASS *wndclass);

//������ �ν��Ͻ�(������, �����ϴ� ����) ������ ��� ������ ������ Ŭ������ CreateWindow�Լ��� ���� �����Ѵ�.
HWND WINAPI CreateWindow(LPCWSTR lpClassName, //������ Ŭ���� �̸�(���� �̸�)__
						 LPCWSTR lpWindowName,//������ �ν��Ͻ� ĸ�� ��(Ÿ��Ʋ)
						 DWORD dwStyle,
						 int X,//�»�� X, Y
						 int Y,
						 int nWidth,//�ʺ�
						 int nHeight,//����
						 HWND hWndParent,//�θ� ������ �ڵ�_�ٸ� �������� Ŭ���̾�Ʈ ������ ��ġ�� ��� �θ� ������ �ڵ鵵 ���� ����.
						 HMENU hMenu,//�޴� �ڵ�
						 HINSTANCE hInstance,//����� �ν��Ͻ� �ڵ�__���μ��� �ڽ��� �ν��Ͻ� ���. �ü���� ���� ������ ��û�ߴ��� �����ϱ� ���� ����Ѵ�.
						 LPVOID lpParam);//������ �� ������ ����

//���� �� ȭ�鿡 �ð�ȭ�ϱ� ���ؼ��� ShowWindow�Լ��� ȣ���Ų��.
ShowWindow(hWnd, nShow);//���� ��¥ ���!! �ð�ȭ!! (SH_CHOW�� �ð�ȭ, SH_HIDE�� ��ð�ȭ�� ��Ÿ����.)


//3. �������� ���α׷��� window instance���� ������� ��Ʈ��, Click, type� ���� �����ϴµ�, �̷��� ������ �ü������ ���μ����� ���� �޽��� ť�� �����ϰ�, ����޽��� ť�� ������ �޽����� �ϳ��� �ݹ� ���ν����� �־� ó���Ѵ�.
MSG Message;
while(GetMessage(&Message, 0, 0, 0))
{
	TranslateMessage(&Message);//WM_KEYDOWN�̰� Ű�� �����϶�, WM_CHAR�߻�(Ű�� ���ȴµ� ���ڸ� WM_CHAR�߻�)
	DispatchMessage(&Message);//�ݹ� ���ν����� ������ �� �ְ� ����(Dispatch)�Ѵ�.
}

//�̶� MSG ����ü�� �Ʒ��� ���� �����.
struct MSG{//�Ʒ��� ����ü�� ���Ͽ� �̺�Ʈó���� �ʿ��� ������ �ݹ� ���ν����� ����
	HWND hwnd;//Event�� �Ͼ �������� �ڵ�
	UNIT message;//�޽��� ������?
	WPARAM wParam;//�޽��� ó���� �ʿ��� ����1, 2
	LPARAM lParam;
	DWORD time;//Event �߻� �ð�
	POINT pt;//Event �߻� �� ���콺 ��ǥ
};

//4. ���޹��� Message�� ó���ϱ� ���� �޽��� �������� ���� GetMessage�� ���� �޽��� ť�� �޽����� ������.
BOOL WINAPI GetMessage(LPMSG lpMsg, HWND hWnd, UNIT wMsgFilterMin, UNIT wMsgFilterMax);
//lpMsg�� ���� msg�� �����ϰ�, hWnd & wMsgFilterMin & wMsgFilterMax�� ���� ���ϴ� �޽����� ���͸��Ѵ�.
//ex) Ư�� �����쿡 �߻��� �޽����� ���ϸ� �ι�° ���ڿ� ��ȿ�� ������ �ڵ��� �����ϰ�, ����°�� �׹�°�� �޽��� ��ȣ�� ������ �����Ѵ�.
//���ǻ����� GetMessage�� ��ȯ���� �޽���ť�� �޽����� ������ FALSE�� �ƴ϶� WM_QUIT MSG�� ���� �� FALSE�̱⿡ ó���� �޽����� ���� �� ���μ����� �����ϴ� ���� �ƴ�, ���� �޽����� �����Ҷ��� �Ϳ� �����ؾ��Ѵ�.

//WM_QUIT�� �޽��� ť�� ������ ���� PostQuitMessage�� ȣ���Ͽ� �����Ѵ�.
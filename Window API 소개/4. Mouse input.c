/*
1.	WindowsAPI���� ���콺 ���� �̺�Ʈ�� ũ�� Client�������� ���콺�� �����ų� ���� �� �߻��ϴ� �޽���, 
	Noc Client�������� ���콺 ������~, �� ���� �޽���(���콺 ��, ���콺 ��ġ ���� ��)���� �����Ѵ�. 
2.	Client Section: WM_LBUTTONDOWN, WM_LBUTTONUP, WM_BUTTONDBLCLK, WM-RBUTTONDOWN, WM_RBUTTONUP, WM_RBUTTONDBLCLK
					WM_MBUTTONDOWN, WM_MBUTTONUP, WM_BUTTONDBLCLK, WM_MOUSEMOVE
	   Non  Client: WM_NCLBUTTONDOWN, WM_NCLBUTTONUP, WM_NCLBUTTONDBLCLK, WM_NCRBUTTONDOWN, WM_NCRBUTTONUP, WM_NCRBUTTONDBLCLK
	   				WM_NCMBUTTONDOWN, WM_NCMBUTTONUP, WM_NCMBUTTONDBLCLK, WM_NCMOUSEMOVE
	   		etc	  : WM_NCHITTEST, WM_MOUSEWHEEL 
3.	NC�������� �߻��� �޽����� �ַ� DefWindowsProc�� ���� defaultó���� �ϴ� ���� ��κ��̴�.
	DBLCLK�޽����� window class�� register�� �� CS_DBLCLS��Ÿ���� ������ �����쿡 �߻��Ѵ�. 
*/

//4. ���콺 ���� �޽������� LPARAM�� ���� 2����Ʈ�� Y����, ���� 2����Ʈ�� X��ǥ�̴�. 
int x=LOWORD(lParam);
int y=HIWORD(lParam);
//POINT pt=MAKEPOINTS(lParam);

//5. ���콺 ���� �޽������� WPARAM�� �Ʒ��� ���� Ű����� ���콺�� ���� ����(��ũ��)�� �����Ѵ�. 
//MK_LBUTTON(0x01)_���� ���콺�� ���ȴ���, MK_RBUTTON(0x02), MK_SHIFT(0x04), MK_CONTROL(0x08), 
//MK_MBUTTON(0x10)_��� ���콺 ��ư�� ���ȴ���, MK_XBUTTON1(0x20)_ù��° X��ư�� ����������, MK_XBUTTON2(0x40)

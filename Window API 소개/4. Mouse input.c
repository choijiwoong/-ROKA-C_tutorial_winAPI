/*
1.	WindowsAPI에선 마우스 관련 이벤트를 크게 Client영역에서 마우스를 누르거나 땠을 때 발생하는 메시지, 
	Noc Client영역에서 마우스 누르땠~, 그 외의 메시지(마우스 휠, 마우스 위치 조사 등)으로 구분한다. 
2.	Client Section: WM_LBUTTONDOWN, WM_LBUTTONUP, WM_BUTTONDBLCLK, WM-RBUTTONDOWN, WM_RBUTTONUP, WM_RBUTTONDBLCLK
					WM_MBUTTONDOWN, WM_MBUTTONUP, WM_BUTTONDBLCLK, WM_MOUSEMOVE
	   Non  Client: WM_NCLBUTTONDOWN, WM_NCLBUTTONUP, WM_NCLBUTTONDBLCLK, WM_NCRBUTTONDOWN, WM_NCRBUTTONUP, WM_NCRBUTTONDBLCLK
	   				WM_NCMBUTTONDOWN, WM_NCMBUTTONUP, WM_NCMBUTTONDBLCLK, WM_NCMOUSEMOVE
	   		etc	  : WM_NCHITTEST, WM_MOUSEWHEEL 
3.	NC영역에서 발생한 메시지는 주로 DefWindowsProc에 의해 default처리를 하는 것이 대부분이다.
	DBLCLK메시지는 window class를 register할 때 CS_DBLCLS스타일을 지정한 윈도우에 발생한다. 
*/

//4. 마우스 관련 메시지에서 LPARAM의 상위 2바이트는 Y좌포, 하위 2바이트는 X좌표이다. 
int x=LOWORD(lParam);
int y=HIWORD(lParam);
//POINT pt=MAKEPOINTS(lParam);

//5. 마우스 관련 메시지에서 WPARAM은 아래와 같은 키보드와 마우스의 상태 조합(매크로)을 제공한다. 
//MK_LBUTTON(0x01)_왼쪽 마우스가 눌렸는지, MK_RBUTTON(0x02), MK_SHIFT(0x04), MK_CONTROL(0x08), 
//MK_MBUTTON(0x10)_가운데 마우스 버튼이 눌렸는지, MK_XBUTTON1(0x20)_첫번째 X버튼이 눌러졌는지, MK_XBUTTON2(0x40)

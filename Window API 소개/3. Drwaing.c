/*
1.	������ Drawing�� ���� ���� ���ڵ��� �־�� �߱⿡(����, �� ��, �β�, ���� ��, ���� �� ��..) DC�� ����Ѵ�. 
	WindowsAPI�� DC(Device Content)�� ��¿� �ʿ��� ���� ������ ������ ����ü�� ��, �귯������ ������ ���� �ִ�.
	 �������� �帧�� ������ ����. DC�߱�(BeginPaint or GetDC)->�׸��� ��ü ����(CreatePen, CreateSolidBrush)
	->DC�� �׸��� ��ü ����(SelectObject)->�׸����۾�(DrawText, Rectangle, Ellipse..etc)->DC�� �׸��� ��ü ����(SelectObject)
	->�׸��� ��ü �Ҹ�(DeleteObject)->DC����(EndPaint, ReleaseDC)
2.	WM_PAINT�޽����� �����찡 �ּ�ȭ, �ִ�ȭ ���� �۾��� ���� �ٸ� �����쿡 �������� �κ��� Ŭ���� �������� �����صΰ�,
	�ٽ� �������� �� �簢 ������ ��ȿȭ ������ �߻��� ������ ó���� �� �߻��ϴ� ������ �޽����̴�.
	 �������� ���α׷��� �׸��� �켱������ ����.(���� �޽��� ť�� ó���� ������ �޽����� ���� ��ȿȭ ������ ���� ���� �߻�) 
3.	WM_PAINT�޽��� ó���⿡�� ��ȿȭ ������ ����ϰ� DC�� �߱޹޾ƾ� �Ѵ�. �� �� DC�� �����ϰ� ��ȿȭ������ ��ȿȭ�������� ����(���ϸ� ��� WM_PAINT�߻��Ͽ� ���� ������)���ָ� �ȴ�.  
*/
HDC WINAPI BeginPaint(HWND hWnd, LPPAINTSTRUCT lpPaint);//Calculate unvalidated section & Register DC at once.
BOOL WINAPI EndPaint(HWND hWnd, CONST PAINTSTRCT *lpPaint);//Validate unvalidated section & Delete DC
//Start to BeginPaint, End to EndPaint

struct PAINTSTRUCT{//DC
	HDC		hdc;
	BOOL	fErase;
	RECT	rcPaint;//�ٽ� �׷��� ������ ����� �� 
	BOOL	fRestore;
	BOOL	fIncUpdate;
	BYTE	egbReserved[32];
};

//4. Skeleton Example by using BeginPaint(), EndPaint()
void OnDraw(HWND hWnd, HDC hdc)
{
	//draw work
}
void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;//Make PAINTSTUCT object
	BeginPaint(hWnd, &ps);//cal unvalid section, register DC. We just pass HWND only.
	OnDraw(hWnd, ps.hdc);//Do Drawing
	EndPaint(hWnd, &ps);//cal valid section, delete DC
}
LRESULT CALLBACK MyWndProc(HWND hWnd, UNIT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
		case WM_PAINT: 
			OnPaint(hWnd);
			return 0;
		//...
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

//5. ��ȿȭ ������ ����� �ٽ� �׸��� �� ���� Invalidate�� �����ϴ� �Լ��� ȣ���Ѵ�.(ex. InvalidateRect)
BOOL WINAPI InvalidateRect(HWND hWnd, CONST RECT *lpRect, BOOL bErase); 
//*lpRect�� �ٽ� �׷��� �簢 ������ ������ ������ �ּ�, bErase�� ����� �ٽ� �׸��� �����̴�. 
//*lpRect�� ��ü �����츦 �ٽ� �׸��Ÿ� 0��(ȭ���� �����Ÿ�������), Ư�� ������ �ٽ� �׸����Ҷ��� �簢 ������ ����Ͽ� �����ϸ� �ȴ�. 


//6. �׸��� ����. BeginPaint, GetDC->CreatePen, CreateSolidBrush->SelectObject->DrawText, Rectangle, Ellipse->SelectObject->DeleteObject->EndPaint, ReleaseDc


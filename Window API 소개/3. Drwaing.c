/*
1.	기존의 Drawing은 정말 많은 인자들을 넣어야 했기에(패턴, 두 점, 두께, 선의 색, 면의 색 등..) DC를 사용한다. 
	WindowsAPI의 DC(Device Content)는 출력에 필요한 여러 정보를 가지는 구조체로 펜, 브러쉬등의 정보를 갖고 있다.
	 전반적인 흐름은 다음과 같다. DC발급(BeginPaint or GetDC)->그리기 개체 생석(CreatePen, CreateSolidBrush)
	->DC에 그리기 개체 선택(SelectObject)->그리기작업(DrawText, Rectangle, Ellipse..etc)->DC에 그리기 개체 선택(SelectObject)
	->그리기 개체 소멸(DeleteObject)->DC해제(EndPaint, ReleaseDC)
2.	WM_PAINT메시지는 윈도우가 최소화, 최대화 등의 작업을 통해 다른 윈도우에 가려져진 부분을 클리핑 영역으로 저장해두고,
	다시 보여지면 그 사각 영역을 무효화 영역이 발생한 것으로 처리할 때 발생하는 윈도우 메시지이다.
	 윈도우즈 프로그램의 그리기 우선순위는 낮다.(응용 메시지 큐에 처리할 윈도우 메시지도 없고 무효화 영역이 있을 때만 발생) 
3.	WM_PAINT메시지 처리기에선 무효화 영역을 계산하고 DC를 발급받아야 한다. 그 후 DC를 해제하고 무효화영역을 유효화영역으로 변경(안하면 계속 WM_PAINT발생하여 성능 떨어짐)해주면 된다.  
*/
HDC WINAPI BeginPaint(HWND hWnd, LPPAINTSTRUCT lpPaint);//Calculate unvalidated section & Register DC at once.
BOOL WINAPI EndPaint(HWND hWnd, CONST PAINTSTRCT *lpPaint);//Validate unvalidated section & Delete DC
//Start to BeginPaint, End to EndPaint

struct PAINTSTRUCT{//DC
	HDC		hdc;
	BOOL	fErase;
	RECT	rcPaint;//다시 그려줄 영역을 계산한 값 
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

//5. 무효화 영역을 만들어 다시 그리게 할 때는 Invalidate로 시작하는 함수를 호출한다.(ex. InvalidateRect)
BOOL WINAPI InvalidateRect(HWND hWnd, CONST RECT *lpRect, BOOL bErase); 
//*lpRect는 다시 그려줄 사각 영역을 설정한 변수의 주소, bErase는 배경을 다시 그릴지 여부이다. 
//*lpRect에 전체 윈도우를 다시 그릴거면 0을(화면이 깜빡거릴수있음), 특정 영역만 다시 그리게할때는 사각 영역을 계산하여 전달하면 된다. 


//6. 그리기 예제. BeginPaint, GetDC->CreatePen, CreateSolidBrush->SelectObject->DrawText, Rectangle, Ellipse->SelectObject->DeleteObject->EndPaint, ReleaseDc


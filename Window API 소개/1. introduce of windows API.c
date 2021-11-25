#include <Windows.h>//For convenience, almost header is included in Windows.h

INT APIENTRY WinMain(HINSTANCE hIns, HINSTANCE hprev, LPSTR cmd, INT nshow)//automatically inputted with execution. (handle, ,getStringPart, printLocation)
{
	MessageBox(0, TEXT("First Windows Program."), TEXT("Message window"), MB_OK);//return button click(handle, m, m, buttonType)
	return 0;
}

/*
1.	INT반환형, 진입점은 WinMain. 반환형과 진입점 사이에 함수호출규약(default는 __cdecl)을 설정할 수 있는데, Windows API가 아닌 다른 언어일 경우 알아서 default호출규약에 따라 만들기에 생략 가능하다. 
	다만, Windows API사용 시, 미리 약속한 함수 호출 규약에 맞게 작성해야하기에 #define APIENTRY __stdcall을 통하여 표기한다.(CALLBACK이나 WINAPI도 같은 __stdcall을 나타내는 매크로이다 for 상황에 따른 가독성)
2.	hIns와 hPrev는 실행 모듈 자체에 부여한 핸들이다. 프로세스에서 윈도우 클래스를 등록하거나 인스턴스 생성 시, 자신의 모듈을 운영체제에게 알려주어야 하는데, 이때 hIns를 사용한다.
	두번째 인자는 16비트 API호환을 위한 부분으로 아무 의미가 없다.
3.	cmd는 command에서 전달한 문자열을 받는 부분이며, nShow는 window창을 어디에 띄울것인지(전체화면이라던가..)에 대한 값으로 사용자가 제어판에 설정한 값이다.
	nShow는 왠만하면 WinMain에서 전달받은 값으로 시각화해야 정확하다. 제어판설정값은 사용자의 설정에 따라 매번 바뀔 수 있기 때문이다. 
4.	MessageBox는 MessageBoxW(Unicode_WCHAR), MessageBoxA(MultiByte_CHAR)중에서 프로젝트에 설정한 문자 집합에 맞게 함수를 변경하여 사용한다.
	int WINAPI MessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UNIT uType);
	int WINAPI MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
	 hWnd는 메시지 창을 띄울때 기준으로 삼을 창의 윈도우 핸들 형식이다. 유효한 핸들을 전달하면 중앙에 메시지를 띄우며, 0은 default location에 창을 띄운다.
	lpText는 출력한 메시지며, lpCaption은 캡션명이다. uType은 메시지 창에 자식 버튼의 종류이며, MB_OK, MB_OKCANCEL, MB_ABORTRETRYIGNORE, MB_YESNO, MB_RETRYCANCEL이 있다. 
	 MessageBox의 return값은 어느 버튼을 눌렀는지를 의미하며, IDIK, IDCANCEL, IDABORT, IDRETRY, IDIGNORE, IDYES, IDNO, IDCLOSE가 1~8로 할당되어 있다. 
*/

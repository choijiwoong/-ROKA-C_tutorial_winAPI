/*
1.	WindowsAPI에서 키보드 입력에 관한 주요 메시지는 WM_KEYDOWN, WM_KEYUP, WM_CHAR(default로 발생하지는 않음)이다.
	윈도우 운영체제에서 키보드 포커스를 소유한 윈도우는 전체 시스템에서 유일하게끔 포커스를 소유한 윈도우에게만 발생한다.
1-1.MsgLoop에서 TranslateMessage호출 시 WM-KEYDOWN중 문자에 관한 것일 때 WM_CHAR을 발생시켜준다.
	(이때 edit 컴트롤이라는 개념이 필요한데, 문자를 입력할 때 입력기에 따라 입력 문자열을 표시하는 것이다.
	한글로 예를 들면 초성까지 입력한 것인지 중성까지 입력한건지 처리하는게 까다롭기에 edit 컨트롤에서 WM_CHAR발생시
	내부적으로 입력기를 이용해 화면에 표시해준다)
2.	WM_KEYDOWN에서 WPARAM은 가상 키 코드, LPARAM은 상태정보를 제공한다.
	[LPARAM]: 0~15(메시지 반복 횟수), 16~23(스캔 코드), 24(확장키여부), 25~28(undefined), 29(Context코드_WM_KEYDOWN에선 0)
			  ,30(이전 키 상태), 31(전환 상태)
*/

typedef struct tagRECT{//RECT 구조체 
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECT, *PRECT, *NPRECT, *LPRECT;

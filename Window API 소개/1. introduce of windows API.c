#include <Windows.h>//For convenience, almost header is included in Windows.h

INT APIENTRY WinMain(HINSTANCE hIns, HINSTANCE hprev, LPSTR cmd, INT nshow)//automatically inputted with execution. (handle, ,getStringPart, printLocation)
{
	MessageBox(0, TEXT("First Windows Program."), TEXT("Message window"), MB_OK);//return button click(handle, m, m, buttonType)
	return 0;
}

/*
1.	INT��ȯ��, �������� WinMain. ��ȯ���� ������ ���̿� �Լ�ȣ��Ծ�(default�� __cdecl)�� ������ �� �ִµ�, Windows API�� �ƴ� �ٸ� ����� ��� �˾Ƽ� defaultȣ��Ծ࿡ ���� ����⿡ ���� �����ϴ�. 
	�ٸ�, Windows API��� ��, �̸� ����� �Լ� ȣ�� �Ծ࿡ �°� �ۼ��ؾ��ϱ⿡ #define APIENTRY __stdcall�� ���Ͽ� ǥ���Ѵ�.(CALLBACK�̳� WINAPI�� ���� __stdcall�� ��Ÿ���� ��ũ���̴� for ��Ȳ�� ���� ������)
2.	hIns�� hPrev�� ���� ��� ��ü�� �ο��� �ڵ��̴�. ���μ������� ������ Ŭ������ ����ϰų� �ν��Ͻ� ���� ��, �ڽ��� ����� �ü������ �˷��־�� �ϴµ�, �̶� hIns�� ����Ѵ�.
	�ι�° ���ڴ� 16��Ʈ APIȣȯ�� ���� �κ����� �ƹ� �ǹ̰� ����.
3.	cmd�� command���� ������ ���ڿ��� �޴� �κ��̸�, nShow�� windowâ�� ��� ��������(��üȭ���̶����..)�� ���� ������ ����ڰ� �����ǿ� ������ ���̴�.
	nShow�� �ظ��ϸ� WinMain���� ���޹��� ������ �ð�ȭ�ؾ� ��Ȯ�ϴ�. �����Ǽ������� ������� ������ ���� �Ź� �ٲ� �� �ֱ� �����̴�. 
4.	MessageBox�� MessageBoxW(Unicode_WCHAR), MessageBoxA(MultiByte_CHAR)�߿��� ������Ʈ�� ������ ���� ���տ� �°� �Լ��� �����Ͽ� ����Ѵ�.
	int WINAPI MessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UNIT uType);
	int WINAPI MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
	 hWnd�� �޽��� â�� ��ﶧ �������� ���� â�� ������ �ڵ� �����̴�. ��ȿ�� �ڵ��� �����ϸ� �߾ӿ� �޽����� ����, 0�� default location�� â�� ����.
	lpText�� ����� �޽�����, lpCaption�� ĸ�Ǹ��̴�. uType�� �޽��� â�� �ڽ� ��ư�� �����̸�, MB_OK, MB_OKCANCEL, MB_ABORTRETRYIGNORE, MB_YESNO, MB_RETRYCANCEL�� �ִ�. 
	 MessageBox�� return���� ��� ��ư�� ���������� �ǹ��ϸ�, IDIK, IDCANCEL, IDABORT, IDRETRY, IDIGNORE, IDYES, IDNO, IDCLOSE�� 1~8�� �Ҵ�Ǿ� �ִ�. 
*/

/*
1.	WindowsAPI���� Ű���� �Է¿� ���� �ֿ� �޽����� WM_KEYDOWN, WM_KEYUP, WM_CHAR(default�� �߻������� ����)�̴�.
	������ �ü������ Ű���� ��Ŀ���� ������ ������� ��ü �ý��ۿ��� �����ϰԲ� ��Ŀ���� ������ �����쿡�Ը� �߻��Ѵ�.
1-1.MsgLoop���� TranslateMessageȣ�� �� WM-KEYDOWN�� ���ڿ� ���� ���� �� WM_CHAR�� �߻������ش�.
	(�̶� edit ��Ʈ���̶�� ������ �ʿ��ѵ�, ���ڸ� �Է��� �� �Է±⿡ ���� �Է� ���ڿ��� ǥ���ϴ� ���̴�.
	�ѱ۷� ���� ��� �ʼ����� �Է��� ������ �߼����� �Է��Ѱ��� ó���ϴ°� ��ٷӱ⿡ edit ��Ʈ�ѿ��� WM_CHAR�߻���
	���������� �Է±⸦ �̿��� ȭ�鿡 ǥ�����ش�)
2.	WM_KEYDOWN���� WPARAM�� ���� Ű �ڵ�, LPARAM�� ���������� �����Ѵ�.
	[LPARAM]: 0~15(�޽��� �ݺ� Ƚ��), 16~23(��ĵ �ڵ�), 24(Ȯ��Ű����), 25~28(undefined), 29(Context�ڵ�_WM_KEYDOWN���� 0)
			  ,30(���� Ű ����), 31(��ȯ ����)
*/

typedef struct tagRECT{//RECT ����ü 
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECT, *PRECT, *NPRECT, *LPRECT;

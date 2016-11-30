#include <stdio.h>
#include "Form.h"

using namespace CommonSettings;
//---------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);//�E�B���h�E���[�h�ŋN��//FALSE���ƃt���X�N���[��
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);//�E�B���h�E�T�C�Y
	SetWaitVSyncFlag(FALSE);
	if (DxLib_Init() == -1) {
		return -1;
	}

	Form Fmain;
	// �`���𗠉�ʂɕύX
	SetDrawScreen(DX_SCREEN_BACK);
	while (1) {
		if (ProcessMessage() != 0) {//���b�Z�[�W����
			break;//�E�B���h�E�́~�{�^���������ꂽ�烋�[�v�𔲂���
		}
		ClearDrawScreen();

		Sleep((int)((1.0 / FRAME_RATE) * 1000));
		Fmain.Update();
		ScreenFlip();//����ʂ𔽉f
	}

	DxLib_End();
	return 0;
}
//---------------------------------------------------------------------------
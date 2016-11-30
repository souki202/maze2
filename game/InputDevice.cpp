#include "InputDevice.h"

InputDevice::Keyboard::Keyboard()
{
	for (auto &x : m_frame) {
		x = 0;
	}
	for (auto &x : m_time) {
		x = 0;
	}
	for (auto &x : m_lastUpdateTime) {
		x = 0;
	}
	m_interval = 100;
}

InputDevice::Keyboard::~Keyboard()
{
}

// �L�[�̓��͏�ԍX�V
void InputDevice::Keyboard::Update()
{             
	timer.Update();

	GetHitKeyStateAll(m_isPushKey);  // �S�ẴL�[�̓��͏�Ԃ𓾂�

	for (int i = 0; i < m_numOfKey; i++) {
		if (m_isPushKey[i]) { // i�Ԃ̃L�[�R�[�h�ɑΉ�����L�[��������Ă�����
			m_frame[i]++;   // ���Z
			m_time[i] += timer.GetDeltaTime();
			if (m_isIgnore[i]) m_isIgnore[i] = false;
		}
		else {              // ������Ă��Ȃ����
			m_frame[i] = 0; // 0�ɂ���
			m_time[i] = 0;
		}
	}
}

bool InputDevice::Keyboard::IsRelease(int keyCode)
{
	if (GetPressFrame(keyCode) > 0) {
		m_isPress[keyCode] = true;
		return false;
	}
	else if (GetPressFrame(keyCode) == 0 &&
		m_isPress[keyCode] == true) {
		m_isPress[keyCode] = false;
		return true;
	}

	return false;
}

bool InputDevice::Keyboard::GetIsUpdate(int keyCode)
{
	if (!m_time[keyCode]) {
		m_lastUpdateTime[keyCode] = 0;
		return false;
	}
	else if (m_time[keyCode] && m_lastUpdateTime[keyCode] == 0) { //����t���[��
		m_lastUpdateTime[keyCode] += m_interval * 3;
		return true;
	}
	else if (m_time[keyCode] && //�����Ă��āA����true���Ԃ�ׂ����Ԃ𒴂��Ă��鎞
		m_time[keyCode] >= m_lastUpdateTime[keyCode] + m_interval) {
		m_lastUpdateTime[keyCode] += m_interval;
		return true;
	}

	return false;
}

void InputDevice::Keyboard::SetupIgnoreKey()
{
	Update();
	for (int i = 0; i < m_numOfKey; i++) {
		if (GetPressFrame(i)) {
			m_isIgnore[i] = true;
		}
		else m_isIgnore[i] = false;
	}
}


//��������}�E�X
InputDevice::Mouse::Mouse()
{
	m_leftPress.first = 0;
	m_leftPress.second = 0;
	m_rightPress.first = 0;
	m_rightPress.second = 0;
	m_middlePress.first = 0;
	m_middlePress.second = 0;
}

InputDevice::Mouse::~Mouse()
{
}

void InputDevice::Mouse::Update()
{
	timer.Update();

	//�͋Z�����ǎd���Ȃ� MOUSE_INPUT_8�Ƃ�0x0080��ť��
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
		m_leftPress.first++;
		m_leftPress.second += static_cast<int>(timer.GetDeltaTime());
	}
	else {
		m_leftPress.first = 0;
		m_leftPress.second = 0;
	}

	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) {
		m_rightPress.first++;
		m_rightPress.second += static_cast<int>(timer.GetDeltaTime());
	}
	else {
		m_rightPress.first = 0;
		m_rightPress.second = 0;
	}


	if ((GetMouseInput() & MOUSE_INPUT_MIDDLE) != 0) {
		m_middlePress.first++;
		m_middlePress.second += static_cast<int>(timer.GetDeltaTime());
	}
	else {
		m_middlePress.first = 0;
		m_middlePress.second = 0;
	}
}


InputDevice::JoyPad::JoyPad()
{
}

InputDevice::JoyPad::~JoyPad()
{
}
//JoyKey�Ɋւ���֐�
/*
�g����
JOYPAD�P���g�������{�^���̖��O�������Ƃ��Ă킽��
JOYPAD1�@�́@�Pp
JOYPAD2�@�́@ 2p

��:����JOYPAD1�̍����������肪�ق������

	if(Joypad_Get(JOYPAD1*BUTTON_LEFT) == 0){
		//����
	}

*/
void InputDevice::JoyPad::Update()
{
	//Joypad1���g��

	//��
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_LEFT) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_LEFT] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_LEFT]++;
	}
	//��
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_UP) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_UP] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_UP]++;
	}
	//�E
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_RIGHT) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_RIGHT] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_RIGHT]++;
	}
	//��
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_DOWN) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_DOWN] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_DOWN]++;
	}

	//1
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_1) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_1] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_1]++;
	}
	//2
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_2) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_2] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_2]++;
	}
	//3
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_3) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_3] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_3]++;
	}
	//4
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_4) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_4] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_4]++;
	}
	//5
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_5) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_5] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_5]++;
	}
	//6
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_6) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_6] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_6]++;
	}
	//7
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_7) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_7] = 0;
	}
	else {
		m_JoyKey[BUTTON_7]++;
	}
	//8
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_8) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_8] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_8]++;
	}
	//9
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_9) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_9] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_9]++;
	}
	//10
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_10) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_10] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_10]++;
	}
	//11
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_11) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_11] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_11]++;
	}
	//12
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_12) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_12] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_12]++;
	}
	//13
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_13) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_13] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_13]++;
	}
	//14
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_14) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_14] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_14]++;
	}
	//15
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_15) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_15] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_15]++;
	}
	//16
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_16) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_16] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_16]++;
	}
	//17
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_17) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_17] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_17]++;
	}
	//18
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_18) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_18] = 0;
	}
	else {
		m_JoyKey[BUTTON_18]++;
	}
	//19
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_19) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_19] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_19]++;
	}
	//20
	if ((GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_20) == 0) {
		m_JoyKey[JOYPAD1*BUTTON_20] = 0;
	}
	else {
		m_JoyKey[JOYPAD1*BUTTON_20]++;
	}
	/*

	Joypad2�R��

	*/

	//��
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_LEFT) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_LEFT] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_LEFT]++;
	}
	//��
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_UP) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_UP] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_UP]++;
	}
	//�E
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_RIGHT) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_RIGHT] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_RIGHT]++;
	}
	//��
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_DOWN) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_DOWN] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_DOWN]++;
	}

	//1
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_1) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_1] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_1]++;
	}
	//2
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_2) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_2] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_2]++;
	}
	//3
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_3) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_3] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_3]++;
	}
	//4
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_4) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_4] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_4]++;
	}
	//5
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_5) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_5] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_5]++;
	}
	//6
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_6) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_6] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_6]++;
	}
	//7
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_7) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_7] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_7]++;
	}
	//8
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_8) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_8] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_8]++;
	}
	//9
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_9) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_9] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_9]++;
	}
	//10
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_10) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_10] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_10]++;
	}
	//11
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_11) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_11] = 0;
	}
	else {
		m_JoyKey[BUTTON_11]++;
	}
	//2
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_12) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_12] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_12]++;
	}
	//3
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_13) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_13] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_13]++;
	}
	//4
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_14) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_14] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_14]++;
	}
	//5
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_15) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_15] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_15]++;
	}
	//6
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_16) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_16] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_6]++;
	}
	//7
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_17) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_17] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_17]++;
	}
	//8
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_18) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_18] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_18]++;
	}
	//9
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_19) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_19] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_19]++;
	}
	//10
	if ((GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_20) == 0) {
		m_JoyKey[JOYPAD2*BUTTON_20] = 0;
	}
	else {
		m_JoyKey[JOYPAD2*BUTTON_20]++;
	}
}
//Joypad�̃{�^�����オ������
bool InputDevice::JoyPad::Release(int KeyCode)
{
	if (GetPressFrame(KeyCode) > 0) {
		upon = true;
	}
	else if (GetPressFrame(KeyCode) == 0 &&
		upon == true) {
		upon = false;
		return true;
	}
	return false;
}
//�W���C�p�b�h�̃{�^����������Ă����Ԃ�Ԃ�
int InputDevice::JoyPad::GetPressFrame(int KeyCode)
{
	return m_JoyKey[KeyCode];
}


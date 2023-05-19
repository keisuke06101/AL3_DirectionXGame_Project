#pragma once
class Engine 
{
public:
	Engine();  // �R���X�g���N�^
	~Engine(); // �f�X�g���N�^

	void up();
	void down();

	void changeState(EngineState* newState);
	void showNowState();

private:
	EngineState* state;
}; // Engine�N���X�̑O���錾

class EngineState
{
public:
	// �������z�֐��@���h���N���X�Ɏ�������������
	virtual void up(Engine* pEngine) = 0;
	virtual void down(Engine* pEngine) = 0;
	virtual void showState() = 0;
};

// EngineIdle1�N���X
class EngineIdle : public EngineState
{
public:
	void up(Engine* pEngine);
	void down(Engine* pEngine);
	void showState();
};

// EngineLow�N���X
class EngineLow : public EngineState {
public:
	void up(Engine* pEngine);
	void down(Engine* pEngine);
	void showState();
};

// EngineHigh�N���X
class EngineHigh : public EngineState {
public:
	void up(Engine* pEngine);
	void down(Engine* pEngine);
	void showState();
};

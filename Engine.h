#pragma once
class Engine 
{
public:
	Engine();  // コンストラクタ
	~Engine(); // デストラクタ

	void up();
	void down();

	void changeState(EngineState* newState);
	void showNowState();

private:
	EngineState* state;
}; // Engineクラスの前方宣言

class EngineState
{
public:
	// 純粋仮想関数　※派生クラスに実装を強制する
	virtual void up(Engine* pEngine) = 0;
	virtual void down(Engine* pEngine) = 0;
	virtual void showState() = 0;
};

// EngineIdle1クラス
class EngineIdle : public EngineState
{
public:
	void up(Engine* pEngine);
	void down(Engine* pEngine);
	void showState();
};

// EngineLowクラス
class EngineLow : public EngineState {
public:
	void up(Engine* pEngine);
	void down(Engine* pEngine);
	void showState();
};

// EngineHighクラス
class EngineHigh : public EngineState {
public:
	void up(Engine* pEngine);
	void down(Engine* pEngine);
	void showState();
};

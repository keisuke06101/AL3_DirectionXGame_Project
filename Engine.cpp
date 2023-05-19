#include "Engine.h"
#include <iostream>

// ----- Engineクラスの実装
Engine::Engine()       //コンストラクタ
{
	state = new EngineIdle();
}

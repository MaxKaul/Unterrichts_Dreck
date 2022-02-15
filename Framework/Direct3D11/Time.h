#pragma once
#include <Windows.h>
#include <chrono>

using namespace std::chrono;

#define TIMEPOINT high_resolution_clock::time_point // okay, but not nice for data types
#define NOW high_resolution_clock::now()
typedef high_resolution_clock::time_point TIME_POINT;
typedef duration<float> DURATION;

class Time
{
public:
	INT init();
	void update();
	void deInit();

	FLOAT getDeltaTime() { return _deltaTime; }

private:
	high_resolution_clock::time_point _lastTimestamp = {};
	FLOAT _deltaTime = 0.0f;
	FLOAT _totalTime = 0.0f; // time since application start
	UINT _fps = 0;
	UINT _lastFPS = 0;
	FLOAT _fpsTime = 0.0f;
};


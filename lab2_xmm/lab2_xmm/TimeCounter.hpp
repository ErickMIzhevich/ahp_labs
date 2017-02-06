#pragma once
#include <iostream>

class TimeCounter
{
private:
	double PCFreq;
	__int64 CounterStart;
public:
	TimeCounter() {}
	TimeCounter(_int64 a, double b) {}
	void StartCounter() {
		LARGE_INTEGER li;
		if (!QueryPerformanceFrequency(&li)) {
			std::cout << "QueryPerformanceFrequency failed!\n";
		}
		this->PCFreq = double(li.QuadPart) / 1.0;
		QueryPerformanceCounter(&li);
		this->CounterStart = li.QuadPart;
	}
	void SetCounter(_int64 a, double b) {
		this->PCFreq = b;
		this->CounterStart = a;
	}
	double GetCounter()  {
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		return double(li.QuadPart - this->CounterStart) / this->PCFreq;
	}
	~TimeCounter() {}
};

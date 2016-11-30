#pragma once

#include "DxLib.h" //Çbíºâ∫ìùàÍÇ≈Ç®äËÇ¢Å@|É÷ÅE`Åj¡◊Ø
#include "CommonSettings.h"
#include "InputDevice.h"
#include <vector>
#include <fstream>
#include <queue>
#include <stack>
#include <functional>
#include <list>
#include <map>

class Form
{
public:
	Form();
	~Form();

	void Update();
	bool LoadMap(std::string file); //ì«Ç›çûÇ›ê¨å˜Ç≈true é∏îsÇ≈false
	void DrawMap();
	void DrawChip(int x, int y);
	std::vector<std::pair<int, int>> BreadthFirstSearch();
	std::stack<std::pair<int, int>> DepthFirstSearch();
	template <typename T>
	std::vector<T> StackToVector(std::stack<T> s);
	std::vector<std::pair<int, int>> AStar();
private:
	InputDevice::Keyboard kInput;
	InputDevice::Mouse mInput;
	Timer timer;
	int time;
	int w, h;
	std::vector<std::vector<int>> map;
	std::vector<std::vector<int>> savedmap;
	std::pair<int, int> start, goal;
	int numOfTimes;
	int searchType = 0;

	bool isGoal;
	int trouble;
	int box;
	static constexpr int chipSize = 2;
};
//---------------------------------------------------------------------------

template<typename T>
inline std::vector<T> Form::StackToVector(std::stack<T> s)
{
	std::vector<T> v;
	v.resize(s.size());
	int pos = s.size() - 1;
	while (!s.empty()) {
		v[pos--] = s.top();
		s.pop();
	}
	return v;
}

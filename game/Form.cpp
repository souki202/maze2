#include "Form.h"
Form::Form()
{
	SetMainWindowText(CommonSettings::GAME_TITLE);//ウィンドウのタイトル
	LoadMap("Assets/Map/0004.txt");

	box = MakeScreen(800, 600, true);
	SetDrawScreen(box);
	DrawMap();
	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, 0, box, true);
	ScreenFlip();

	//ENTERキーで探索開始
	while(1){
		Sleep(32);
		kInput.Update();
		if (kInput.GetPressFrame(KEY_INPUT_RETURN)) break;
	}

	isGoal = false;
	timer.Reset();
	auto route = BreadthFirstSearch();
	//auto r = DepthFirstSearch(); auto route = StackToVector<std::pair<int, int>>(r);
	//auto route = AStar();
	timer.Update();
	time = timer.GetElapsedTime();

	//通過した部分を記入
	for (auto itr = route.begin() + 1; itr != route.end(); ++itr) {
		map[itr->second][itr->first] = 5;
	}
}

Form::~Form()
{
}

void Form::Update()
{
	//キーボード, マウスの状態を更新
	kInput.Update();
	mInput.Update();

	//終了
	if (kInput.GetPressFrame(KEY_INPUT_ESCAPE)) {
		DxLib_End();
		exit(-1);
	}

	//ここからプログラムを書く
	DrawFormatString(100, 4, 0xffffff, "所要時間 : %d\t探索量 : %d", time, trouble);
	DrawMap();
}

bool Form::LoadMap(std::string file)
{
	/*
	条件
	wは迷路の幅, hは迷路の高さ
	startは開始地点, goalは終了地点
	start != goal
	w >= 2, h >= 2
	*/
	std::ifstream ifs(file);
	if (ifs.fail()) { return false; }
	ifs >> w >> h;
	map.resize(h, std::vector<int>(w, 0));
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			ifs >> map[i][j];
			if (map[i][j] == 2) {
				start = std::make_pair(j, i);
			}
			if (map[i][j] == 3) {
				goal = std::make_pair(j, i);
			}
		}
	}
	savedmap = map;
	return true;
}

void Form::DrawMap()
{
	int color;
	for (size_t i = 0; i < map.size(); i++) {
		for (size_t j = 0; j < map[i].size(); j++) {
			switch (map[i][j])
			{
			case 0: color = 0xffffff; break; //passable
			case 1: color = 0x444444; break; //inpassable
			case 2: color = GetColor(64, 255, 64); break; //start
			case 3: color = GetColor(255, 64, 64); break; //goal
			case 4: color = GetColor(255, 255, 0); break; //searched
			case 5: color = GetColor(255, 128, 255); break; //correct route
			default: color = 0; break;
			}
			DrawBox(2 + j * chipSize, 32 + i * chipSize,
				2 + (j + 1) * chipSize, 32 + (i + 1) * chipSize, color, true);
		}
	}
}

void Form::DrawChip(int x, int y)
{
	DrawBox(2 + x * chipSize, 32 + y * chipSize,
		2 + (x + 1) * chipSize, 32 + (y + 1) * chipSize, GetColor(255, 255, 0), true);
}

std::vector<std::pair<int, int>> Form::BreadthFirstSearch()
{
	std::queue<std::vector<std::pair<int, int>>> q; //コスト, 場所
	q.push(std::vector<std::pair<int, int>>(1, start));
	int dx[] = {0, 1, 0, -1};
	int dy[] = {-1, 0, 1, 0};
	while (1) {
		//ゴールに辿りつけない
		if (q.empty()) {
			return std::vector<std::pair<int, int>>();
		}
		auto node = q.front(); q.pop();
		auto next = node.back();
		for (int i = 0; i < 4; i++) { //各方向
			//進める
			next.first += dx[i];
			next.second += dy[i];
			//場外
			if (next.first < 0 || next.first >= w ||
				next.second < 0 || next.second >= h) {
				next.first -= dx[i];
				next.second -= dy[i];
				continue; //今回は諦め
			}
			//ゴール
			if (map[next.second][next.first] == 3) {
				return node;
			}
			//進めない (進める先は0のみ)
			if (map[next.second][next.first]) {
				next.first -= dx[i];
				next.second -= dy[i];
				continue; //今回は諦め
			}
			//進める
			node.push_back(next);
			q.push(node);
			node.pop_back();
			map[next.second][next.first] = 4;
			//アニメーション用
			DrawChip(next.first, next.second);
			ScreenFlip();//裏画面を反映
			next.first -= dx[i];
			next.second -= dy[i];
			trouble++;
		}
	}
	return std::vector<std::pair<int, int>>();
}

std::stack<std::pair<int, int>> Form::DepthFirstSearch()
{
	int dx[] = { 0, 1, 0, -1 };
	int dy[] = { -1, 0, 1, 0 };
	std::stack<std::pair<int, int>> stack;
	stack.push(start);
	bool hasChild = false;
	while (!stack.empty()) {
		auto node = stack.top();
		hasChild = false;
		//次に進む場所を見つける
		for (int i = 0; i < 4; i++) {
			node.first += dx[i];
			node.second += dy[i];
			//ゴール
			if (node == goal) {
				return stack;
			}
			//場外
			if (node.first < 0 || node.first >= w ||
				node.second < 0 || node.second >= h) {
				node.first -= dx[i];
				node.second -= dy[i];
				continue;
			}
			//進めない (進める先は0のみ)
			if (map[node.second][node.first]) {
				node.first -= dx[i];
				node.second -= dy[i];
				continue; //今回は諦め
			}

			//進める
			hasChild = true;
			break;
		}
		if (!hasChild) stack.pop();
		else {
			stack.push(node);
			map[node.second][node.first] = 4;
			//アニメーション用
			DrawChip(node.first, node.second);
			ScreenFlip();//裏画面を反映
			trouble++;
		}
	}
	return std::stack<std::pair<int, int>>();
}

std::vector<std::pair<int, int>> Form::AStar()
{
	auto f = [](const std::pair<int, int>& start, const std::vector<std::pair<int, int>>& now, const std::pair<int, int>& goal) -> double {
		double g = now.size() - 1
			 , h = std::abs(goal.first - now.back().first) + std::abs(goal.second - now.back().second);
		return 1.0 / (g + h);
	};
	typedef std::pair<double, std::vector<std::pair<int, int>>> nodetype;
	//std::priority_queue<nodetype, std::vector<nodetype>> q; //コスト, 次の場所
	std::multimap<double, std::vector<std::pair<int, int>>, std::greater<double>> q;
	int dx[] = { 0, 1, 0, -1 };
	int dy[] = { -1, 0, 1, 0 };
	//q.push(std::make_pair(f(start, std::vector<std::pair<int, int>>(1, start), goal), std::vector<std::pair<int, int>>(1, start)));
	q.insert(std::make_pair(f(start, std::vector<std::pair<int, int>>(1, start), goal), std::vector<std::pair<int, int>>(1, start)));
	bool first = true;
	while (1) {
		//ゴールに辿りつけない
		if (q.empty()) {
			return std::vector<std::pair<int, int>>();
		}
		//auto node = q.top(); q.pop();
		auto node = *(q.begin()); q.erase(q.begin());
		auto next = node.second.back();
		if (!first) {
			map[next.second][next.first] = 4;
			//アニメーション用
			DrawChip(next.first, next.second);
			ScreenFlip();//裏画面を反映
		}
		first = false;
		//次の場所を探す
		for (int i = 0; i < 4; i++) {
			//進める
			next.first += dx[i];
			next.second += dy[i];
			//場外
			if (next.first < 0 || next.first >= w ||
				next.second < 0 || next.second >= h) {
				next.first -= dx[i];
				next.second -= dy[i];
				continue; //今回は諦め
			}
			//ゴール
			if (map[next.second][next.first] == 3) {
				return node.second;
			}
			//進めない (進める先は0のみ)
			if (map[next.second][next.first]) {
				next.first -= dx[i];
				next.second -= dy[i];
				continue; //今回は諦め
			}
			//次の場所の評価値を確認して登録
			node.second.push_back(next);
			double eval = f(start, node.second, goal);
			//q.push(std::make_pair(eval, node.second));
			q.insert(std::make_pair(eval, node.second));
			node.second.pop_back();
			next.first -= dx[i];
			next.second -= dy[i];
			trouble++;
		}
	}
	return std::vector<std::pair<int, int>>();
}

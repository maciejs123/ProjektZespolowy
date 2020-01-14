#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <deque>

using namespace std;
using namespace cv;



class Wall
{
public:
	float limbocornery;
	float cornery;
	float cornerx;
	Mat texture;

private:
	Wall();

public:
	void update_position();
	void flipper();
	friend class WallArray;
};

class WallArray
{

	deque<Wall> wallers;
	int counter = 0;
	float empty = true;

public:
	WallArray();

	void wall_countdown();
	void check_cornery();
	void update_position();
	void check_empty();
	bool get_empty();

	deque<Wall>::iterator begin();
	deque<Wall>::iterator end();

};
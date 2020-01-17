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
	int checksum;
	float limbocornery;
	float cornery;
	float cornerx;


private:
	Wall(float x);

public:
	void update_position();
	friend class WallArray;
};

class WallArray
{
public:
	Mat teks[8];
	deque<Wall> wallers;
	int counter = 0;
	float empty = true;
	int sizex = 180, sizey = 240;

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
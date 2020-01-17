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

class Obstacle
{
public:
	float limbocornery;
	float cornerx;
	float cornery;
	int width;
	int height;
	bool flipper;
	int checksum;

private:
	Obstacle();

public:
	bool check_position(int x, int y);
	void update_position();
	friend class ObstacleArray;
};

class ObstacleArray
{
public:
	Mat teks[9];
	int counter = 0;
	float empty = true;

	ObstacleArray();

	deque<Obstacle> things;
	void check_empty();
	bool get_empty();
	void obstacle_countdown();
	void check_cornery();
	void update_position();
	bool check_postion(int x, int y);
	deque<Obstacle>::iterator begin();
	deque<Obstacle>::iterator end();


};
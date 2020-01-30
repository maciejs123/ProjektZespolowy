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

///Klasa pojedynczej przeszkody
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
	void update_position();
	friend class ObstacleArray;
};

///Klasa zbioru przeszkod
class ObstacleArray
{
public:
	Mat teks[9];
	Mat mask[9];
	int counter = 100;
	float difficultymeter = 0;
	float empty = true;

	ObstacleArray();

	deque<Obstacle> things;
	void check_empty();
	bool get_empty();
	void obstacle_countdown();
	void check_cornery();
	void update_position();
	deque<Obstacle>::iterator begin();
	deque<Obstacle>::iterator end();


};
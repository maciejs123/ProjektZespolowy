#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <deque>
#include "Obstacle.h"

using namespace std;
using namespace cv;


Obstacle::Obstacle()
{
	checksum = rand();

	if ((checksum % 3) == 1)
	{
		height = 200;
		width = 200;
		flipper = false;

		checksum = (checksum / 3) % 3;
		cornerx = 180;
	}
	else if ((checksum % 3) == 2)
	{
		height = 200;
		width = 200;
		flipper = true;

		checksum = (checksum / 3) % 3;
		cornerx = 1020 - width;
	}
	else
	{
		height = 100;
		width = 100;

		checksum = ((checksum / 3) % 6) + 3;
		cornerx = 600 - (width / 2);
	}
	limbocornery = 0;
	cornery = limbocornery - 200;
}

bool Obstacle::check_position(int x, int y)
{
	if (((x >= cornerx) && (x <= (cornerx + width))) && ((y >= cornery) && (y <= (cornery + height))))
		return true;
	else
		return false;

}
void Obstacle::update_position()
{
	limbocornery += 40;
	cornery += 40;
}

ObstacleArray::ObstacleArray()
{
	teks[0] = imread("Assets/Obstacles/BlueFlag.png");
	teks[1] = imread("Assets/Obstacles/RedFlag.png");
	teks[2] = imread("Assets/Obstacles/Balcony.png");
	teks[3] = imread("Assets/Obstacles/BlackCat.png");
	teks[4] = imread("Assets/Obstacles/GingerCat.png");
	teks[5] = imread("Assets/Obstacles/GrayCat.png");
	teks[6] = imread("Assets/Obstacles/WhiteCat.png");
	teks[7] = imread("Assets/Obstacles/BlackFlowerPot.png");
	teks[8] = imread("Assets/Obstacles/BrownFlowerPot.png");

	Size size_one(200, 200);
	for (int i = 0; i < 3; i++)
		resize(teks[i], teks[i], size_one);
	Size size_two(100, 100);
	for (int i = 3; i < 9; i++)
		resize(teks[i], teks[i], size_two);
}

void ObstacleArray::check_empty()
{
	if (things.size() == 0)
		empty = true;
	else
		empty = false;

}

bool ObstacleArray::get_empty()
{
	return empty;
}
void ObstacleArray::obstacle_countdown()
{
	if (counter == 0)
	{
		things.push_back(Obstacle());
		counter = 20 + (rand() % 80);
	}
	else
		counter--;

}
void ObstacleArray::check_cornery()
{
	if (things[0].cornery > 1200)
	{
		things.pop_front();
	}
}
void ObstacleArray::update_position()
{
	for (int i = 0; i < things.size(); i++)
	{
		things[i].update_position();
	}
}

bool ObstacleArray::check_postion(int x, int y)
{
	for (Obstacle ob : things)
		if (ob.check_position(x, y))
			return true;
	return false;
}
deque<Obstacle>::iterator ObstacleArray::begin()
{
	return this->things.begin();
}
deque<Obstacle>::iterator ObstacleArray::end()
{
	return this->things.end();
}
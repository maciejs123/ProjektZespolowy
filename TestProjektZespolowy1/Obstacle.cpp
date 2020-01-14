#include "stdafx.h"
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
	int checksum = rand();

	width = 50 + (checksum % 150);
	height = 50 + (checksum % 150);
	if ((checksum % 3) == 1)
	{
		switch ((checksum / 3) % 3)
		{
		case 0:
			texture = imread("Assets/Obstacles/BlueFlag.png");
			break;
		case 1:
			texture = imread("Assets/Obstacles/RedFlag.png");
			break;
		case 2:
			texture = imread("Assets/Obstacles/Balcony.png");
			break;
		}
		height = texture.rows;
		width = texture.cols;

		cornerx = 80;
	}
	else if ((checksum % 3) == 2)
	{
		switch ((checksum / 3) % 3)
		{
		case 0:
			texture = imread("Assets/Obstacles/BlueFlag.png");
			break;
		case 1:
			texture = imread("Assets/Obstacles/RedFlag.png");
			break;
		case 2:
			texture = imread("Assets/Obstacles/Balcony.png");
			break;
		}
		flip(texture, texture, 1);
		height = texture.rows;
		width = texture.cols;

		cornerx = 1120 - width;
	}
	else
	{
		switch ((checksum / 3) % 6)
		{
		case 0:
			texture = imread("Assets/Obstacles/BlackCat.png");
			break;
		case 1:
			texture = imread("Assets/Obstacles/GingerCat.png");
			break;
		case 2:
			texture = imread("Assets/Obstacles/GrayCat.png");
			break;
		case 3:
			texture = imread("Assets/Obstacles/WhiteCat.png");
			break;
		case 4:
			texture = imread("Assets/Obstacles/BlackFlowerPot.png");
			break;
		case 5:
			texture = imread("Assets/Obstacles/BrownFlowerPot.png");
			break;
		}
		height = texture.rows;
		width = texture.cols;

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
	limbocornery += 25;
	cornery += 25;
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
	for (int i = 0; i<things.size(); i++)
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
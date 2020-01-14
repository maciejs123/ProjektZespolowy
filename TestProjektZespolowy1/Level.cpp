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
#include "Level.h"

using namespace std;
using namespace cv;

Wall::Wall()
{
	int checksum = rand();
	limbocornery = 75;
	cornery = -125;
	cornerx = 0;
	switch (checksum % 8)
	{
	case 0:
		texture = imread("Assets/Walls/Wall1a.png");
		break;
	case 1:
		texture = imread("Assets/Walls/Wall1b.png");
		break;
	case 2:
		texture = imread("Assets/Walls/Wall1c.png");
		break;
	case 3:
		texture = imread("Assets/Walls/Wall1d.png");
		break;
	case 4:
		texture = imread("Assets/Walls/Wall1e.png");
		break;
	case 5:
		texture = imread("Assets/Walls/Wall1f.png");
		break;
	case 6:
		texture = imread("Assets/Walls/Wall1g.png");
		break;
	case 7:
		texture = imread("Assets/Walls/Wall1h.png");
		break;
	}
}

void Wall::update_position()
{
	limbocornery += 25;
	cornery += 25;
}

void Wall::flipper()
{
	cornerx = 1120;
	flip(texture, texture, 1);
	
}

void WallArray::update_position()
{
	for (int i = 0; i < wallers.size(); i++)
	{
		wallers[i].update_position();
	}
}

WallArray::WallArray()
{
	for (int i = 0; i < 10; i++)
	{
		wallers.push_back(Wall());
		wallers.push_back(Wall());
		empty = false;
		for (int i = 0; i < wallers.size(); i++)
		{
			for (int j=0; j<5; j++)
				wallers[i].update_position();
		}
	}
	for (int i = 1; i < wallers.size(); i += 2)
		wallers[i].flipper();
}



void WallArray::check_cornery()
{
	if (wallers[0].cornery > 1200)
	{
		wallers.pop_front();
		wallers.pop_front();
	}
}

deque<Wall>::iterator WallArray::begin()
{
	return this->wallers.begin();
}
deque<Wall>::iterator WallArray::end()
{
	return this->wallers.end();
}

void WallArray::wall_countdown()
{
	if (counter == 0)
	{
		wallers.push_back(Wall());
		wallers.push_back(Wall());
		wallers[wallers.size() - 1].flipper();
		counter = 4;
	}
	else
		counter--;
}
void WallArray::check_empty()
{
	if (wallers.size() == 0)
		empty = true;
	else
		empty = false;

}

bool WallArray::get_empty()
{
	return empty;
}
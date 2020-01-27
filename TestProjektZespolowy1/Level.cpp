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





Wall::Wall(float x)
{
	checksum = rand();
	checksum %= 8;

	limbocornery = 50;
	cornery = -350;
	cornerx = x;

}

void Wall::update_position()
{
	limbocornery += 20;
	cornery += 20;
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
	teks[0] = imread("Assets/Walls/Wall1a.png", IMREAD_UNCHANGED);
	teks[1] = imread("Assets/Walls/Wall1b.png", IMREAD_UNCHANGED);
	teks[2] = imread("Assets/Walls/Wall1c.png", IMREAD_UNCHANGED);
	teks[3] = imread("Assets/Walls/Wall1d.png", IMREAD_UNCHANGED);
	teks[4] = imread("Assets/Walls/Wall1e.png", IMREAD_UNCHANGED);
	teks[5] = imread("Assets/Walls/Wall1f.png", IMREAD_UNCHANGED);
	teks[6] = imread("Assets/Walls/Wall1g.png", IMREAD_UNCHANGED);
	teks[7] = imread("Assets/Walls/Wall1h.png", IMREAD_UNCHANGED);
	Size size(sizex, sizey);
	for (int i = 0; i < 8; i++)
		resize(teks[i], teks[i], size);
	for (int i = 0; i < 8; i++)
	{
		vector<Mat> rgbLayer;

		split(teks[i], rgbLayer);
		Mat cs[3] = { rgbLayer[0],rgbLayer[1],rgbLayer[2] };
		merge(cs, 3, teks[i]);
		mask[i] = rgbLayer[3];
	}
	for (int i = 0; i < 5; i++)
	{
		wallers.push_back(Wall(0));
		wallers.push_back(Wall(1200 - sizex));
		empty = false;
		for (int i = 0; i < wallers.size(); i++)
		{
			for (int j = 0; j<12; j++)
				wallers[i].update_position();
		}
	}
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
		wallers.push_back(Wall(0));
		wallers.push_back(Wall(1200 - sizex));
		counter = 10;
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
#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <chrono>
#include <windows.h>
#include <WinUser.h>
#include <thread>
#include "Obstacle.h"
#include "Level.h"
#include "Player.h"
#include <time.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include<math.h>


//void callBackFunc(int event, int x, int y, int flags, void* userdata);
//void callBackFunc2(int event, int x, int y, int flags, void* userdata);

class menu {
public:
	Mat3b canvas;
	string buttonText;// = "Play";
	string buttonText2;// = "Highscores";
	string buttonText3;// = "Exit";
	string buttonText4;// = "Return";
	string buttonText5;// = "Exit";
	string winName;// = "Wallrunner";
	Rect button, button2, button3, button4, button5, button6;
	
	void callBackFunc(int event, int x, int y, int flags, void* userdata);
	void callBackFunc2(int event, int x, int y, int flags, void* userdata);

	menu();
	string intToStr(int n);
	Mat glowneMenu(Mat limbo);
	void wyniki();
	
};
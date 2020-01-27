#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <math.h>
#include <iostream>
#include <chrono>
#include <windows.h>
#include <WinUser.h>
#include <thread>
#include <time.h>
#include <cstdlib>
#include "Obstacle.h"

class player {
private:
	Mat climb2; 
	Mat climb3;
	Mat jump1; 
	Mat jump2; 
	Mat fly1;
	Mat fly2;
	Mat death;
	Mat mask1, mask2, mask3, maskj1, maskj2, maskf1, maskf2, maskd;
	bool isInAir;
	bool doubleJump;
	int posX;
	int posY;
public:
	bool sideLeft;
	bool isDead;
	Mat climb1;
	player();
	player(string c1, string c2, string c3, string j1, string j2, string f1, string f2, string d, int x, int y, bool air, bool side, bool djump);

	void switchSide();
	void jump();
	void flight(double a, double p, double q, int speed, int frameNo, int ceiling, int floor, int y, int& x, int lWall, int rWall);
	void land(int leftWall, int rightWall);
	void climbUp(int amount, int ceiling);
	void climbDown(int amount, int floor);

	void flipSprites();
	void drawSprites(Mat image, int wyborS);
	int selectSprite(int frameNo, int wyborS);

	int getPosX();
	int getPosY();
	bool getAirVal();
	bool getDJumpVal();
};
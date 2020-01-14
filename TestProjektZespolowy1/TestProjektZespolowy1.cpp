#include "stdafx.h"
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




using namespace cv;
using namespace std;

int main()
{
	srand(time(NULL));
	bool bKey[3]; //tablica wcisnietych klawiszy
	player p;
	int xSkok = 0;
	int ySkok = 0;

	//Zmienne sluzace do okreslenia czasu
	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	//Rysowanie grafiki przy starcie
	Mat whole, pom, limbo;
	Mat displayed = Mat::zeros(1200, 1200, CV_8UC3);

	//inicjalizacja przeszkód
	ObstacleArray obs;
	//inicjalizacja planszy
	WallArray ws;

	whole = imread("Assets/Walls/Walls2.png");
	limbo = imread("Assets/Walls/Limbo.png");

	//Wyswietlanie glownej planszy
	pom = whole(Rect(0, 0, 1200, 1200));
	pom.copyTo(displayed(Rect(0, 0, 1200, 1200)));
	imshow("Fragment", displayed);

	int i = 0;
	int nrKlatki = 1;
	int wyborS = 0;
	waitKey(1);


	//Petla gry
	while (i < 5)
	{
		//Uzaleznienie funkcji waitKey() od czasu

		tp2 = chrono::system_clock::now();
		chrono::duration<double> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		double fElapsedTime = elapsedTime.count();

		//STEROWANIE GRACZEM

		//przemieszczanie sie gracza do gory
		if ((0x8000 & GetAsyncKeyState((unsigned char)'\x26')))
			p.climbUp(10, 25);
		//przemieszczanie sie gracza do dolu
		if ((0x8000 & GetAsyncKeyState((unsigned char)'\x28')))
			p.climbDown(10, 500);

		//Skok gracza
		if ((0x8000 & GetAsyncKeyState((unsigned char)'\x20')) && nrKlatki >= 12 && p.getDJumpVal() == true) {
			if (p.getAirVal() == false)
				nrKlatki = 0;
			else if (p.getDJumpVal() == true) {
				nrKlatki = 6;
			}
			xSkok = p.getPosX();
			ySkok = p.getPosY();
			p.jump();
		}

		p.flight(0.0006, 80, 900, 40, nrKlatki, 0, 500, ySkok, xSkok, 80, 900);

		p.land(80, 900);

		//RYSOWANIE
		// reset limbo
		limbo = imread("Assets/Walls/Limbo.png");
		// rysowanie planszy
		for (deque<Wall>::iterator it = ws.begin(); it != ws.end(); ++it)
		{
			it->texture.copyTo(limbo(Rect(it->cornerx, it->limbocornery, it->texture.cols, it->texture.rows)));
		}
		//rysowanie przeszkód
		for (deque<Obstacle>::iterator it = obs.begin(); it != obs.end(); ++it)
		{
			it->texture.copyTo(limbo(Rect(it->cornerx, it->limbocornery, it->texture.cols, it->texture.rows)));
		}
		Mat croppedlimbo = limbo(Rect(0, 200, 1200, 1200));
		croppedlimbo.copyTo(displayed(Rect(0, 0, 1200, 1200)));


		//rysowanie gracza
		wyborS = p.selectSprite(nrKlatki, wyborS);
		p.drawSprites(displayed, wyborS);
		//Tworzenie przeszkód

		obs.obstacle_countdown();
		obs.check_empty();
		if (!obs.get_empty())
		{
			obs.update_position();
			obs.check_cornery();
		}
		//Aktualizacja planszy
		ws.wall_countdown();
		ws.check_empty();
		if (!ws.get_empty())
		{
			ws.update_position();
			ws.check_cornery();
		}


		imshow("Fragment", displayed);
		//waitKey(1 / fElapsedTime);
		waitKey(33);


		i++;
		nrKlatki++;
		if (i == 5)
			i = 0;
	}

	//	imshow("Test", whole);
	waitKey(0);
	return 0;
}
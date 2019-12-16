#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <chrono>
#include <windows.h>
#include <WinUser.h>
#include <thread>


using namespace cv;
using namespace std;

class player {
private:
	bool isInAir;
	bool sideLeft;
public:
	int selectSprite(int frameNo);
	int posX;
	int posY;
	player() { posX = 80; posY = 300; isInAir = false; sideLeft = true; }
	player(int x, int y, bool air, bool side) { posX = x; posY = y; isInAir = air; sideLeft = side; }
	int getPosX() { return posX; }
	int getPosY() { return posY; }
	bool getAirVal() { return isInAir; }
};

int player::selectSprite(int frameNo) //funkcja dopasowuje wybor sprite w zaleznosci na ktorej klatce jestesmy
{
	if (isInAir == false) {
		if (sideLeft == true) {
			if (frameNo % 7 == 0)
			{
				frameNo /= 7;
				return frameNo % 4;		// 0 1 2 3 - sprite'y wchodzenia na lewa sciane

			}
			else
				;
		}
	}
	else if (isInAir == true)
		;
	else return 0;

}

int main()
{
	bool bKey[3]; //tablica wcisnietych klawiszy
	player p;

	//Zmienne sluzace do okreslenia czasu
	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	//Rysowanie grafiki przy starcie
	Mat whole, postac, postac2, postac3, pom;
	Mat displayed = Mat::zeros(1200,1200, CV_8UC3);
	vector<Mat> rgbLayer1,rgbLayer2,rgbLayer3;

	whole = imread("Assets/Walls/Walls2.png");
	postac = imread("Assets/Postac/ClimbLeft.png", IMREAD_UNCHANGED);
	postac2 = imread("Assets/Postac/ClimbMiddle.png", IMREAD_UNCHANGED);
	postac3 = imread("Assets/Postac/ClimbRight.png",IMREAD_UNCHANGED);
	cv::resize(postac2, postac2, cv::Size(), 0.13, 0.13);
	cv::resize(postac, postac, cv::Size(), 0.13, 0.13);
	cv::resize(postac3, postac3, cv::Size(), 0.13, 0.13);

	//ustawianie masek z kanalu alfa
	Mat mask1, mask2, mask3;

	split(postac, rgbLayer1);
	Mat cs[3] = { rgbLayer1[0],rgbLayer1[1],rgbLayer1[2] };
	merge(cs, 3, postac);
	mask1 = rgbLayer1[3];
	
	split(postac2, rgbLayer2);
	cs[0] = rgbLayer2[0];
	cs[1] = rgbLayer2[1];
	cs[2] = rgbLayer2[2];
	merge(cs, 3, postac2);
	mask2 = rgbLayer2[3];

	split(postac3, rgbLayer3);
	cs[0] = rgbLayer3[0];
	cs[1] = rgbLayer3[1];
	cs[2] = rgbLayer3[2];
	merge(cs, 3, postac3);
	mask3 = rgbLayer3[3];


	//Wyswietlanie glownej planszy
	pom = whole(Rect(0, 0, 1200, 1200));

	pom.copyTo(displayed(Rect(0, 0, 1200, 1200)));
	
	postac.copyTo(displayed(Rect(80, 300, postac.cols, postac.rows)),mask1);
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

		//	bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x26\x28\x20")));	//UP, DOWN, SPACEBAR*/

		if ((0x8000 & GetAsyncKeyState((unsigned char)'\x26')) && p.getPosY() > 25 && p.getAirVal() == false)
			p.posY = p.posY - 5;


		if ((0x8000 & GetAsyncKeyState((unsigned char)'\x28')) && p.getPosY() < 500 && p.getAirVal() == false)
			p.posY = p.posY + 5;

		//rysowanie grafik postaci w pętli

		pom = whole(Rect(0, 275 - (i * 55), 1200, 1200));
		pom.copyTo(displayed(Rect(0, 0, 1200, 1200)));

		if (nrKlatki % 7 == 0)
			wyborS = p.selectSprite(nrKlatki);

		if (wyborS == 0)
			postac.copyTo(displayed(Rect(p.getPosX(), p.getPosY() + (i * 55 * 0), postac.cols, postac.rows)), mask1);
		else if (wyborS == 1)
			postac2.copyTo(displayed(Rect(p.getPosX(), p.getPosY() + (i * 55 * 0), postac.cols, postac.rows)),mask2);
		else if (wyborS == 2) 
			postac3.copyTo(displayed(Rect(p.getPosX(), p.getPosY() + (i * 55 * 0), postac.cols, postac.rows)), mask3);
		else if (wyborS == 3)
			postac2.copyTo(displayed(Rect(p.getPosX(), p.getPosY() + (i * 55 * 0), postac.cols, postac.rows)), mask2);


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
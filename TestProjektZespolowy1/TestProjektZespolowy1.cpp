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
#include <string>
#include <fstream>
#include<math.h>

using namespace cv;
using namespace std;

//Zmienne do menu
Mat3b canvas;
string buttonText = "Play";
string buttonText2 = "Highscores";
string buttonText3 = "Exit";
string buttonText4 = "Continue";
string buttonText5 = "Exit to Menu";
string winName = "Wallrunner";
Rect button, button2, button3, button4, button5, button6;
bool start = false;
bool pauseContinue = false;
bool pauseExit = false;

string intToStr(int n)
{
	string tmp, ret;
	if (n < 0) {
		ret = "-";
		n = -n;
	}
	do {
		tmp += n % 10 + 48;
		n -= n % 10;
	} while (n /= 10);
	for (int i = tmp.size() - 1; i >= 0; i--)
		ret += tmp[i];
	return ret;
}

//rysowanie menu
Mat glowneMenu(Mat limbo) {
	//guziki

	canvas = limbo(Rect(0, 400, 1200, 1200));

	button = Rect(400, 200, canvas.cols / 3, 50);
	button2 = Rect(400, 300, canvas.cols / 3, 50);
	button3 = Rect(400, 400, canvas.cols / 3, 50);

	// rysowanie guzikow
	canvas(button) = Vec3b(200, 200, 200);
	canvas(button2) = Vec3b(200, 200, 200);
	canvas(button3) = Vec3b(200, 200, 200);
	putText(canvas(button), buttonText, Point(button.width*0.43, button.height*0.7), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0));
	putText(canvas(button2), buttonText2, Point(button.width*0.28, button.height*0.7), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0));
	putText(canvas(button3), buttonText3, Point(button.width*0.43, button.height*0.7), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0));
	putText(canvas, "Wall Runner", Point(420, 100), FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Scalar(255, 255, 255), 2);

	return canvas;
}

//rysowanie menu pauzy
Mat pausaMenu(Mat limbo) {
	canvas = limbo(Rect(0, 0, 1200, 1200));
	button4 = Rect(400, 220, canvas.cols / 3, 50);
	button5 = Rect(400, 300, canvas.cols / 3, 50);

	// rysowanie guzikow

	canvas(button4) = Vec3b(100, 100, 100);
	canvas(button5) = Vec3b(100, 100, 100);
	putText(canvas(button4), buttonText4, Point(button4.width*0.315, button4.height*0.7), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0));
	putText(canvas(button5), buttonText5, Point(button5.width*0.235, button5.height*0.7), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0));
	putText(limbo, "GAME IS PAUSED", Point(335, 60), 1, 4, Scalar(255, 255, 255), 5);
	return canvas;
}

Mat endScreen(Mat limbo, int punkty) {
	canvas = limbo(Rect(0, 0, 1200, 1200));
	button5 = Rect(400, 300, canvas.cols / 3, 50);

	// rysowanie guzikow

	canvas(button5) = Vec3b(100, 100, 100);
	putText(canvas(button5), buttonText5, Point(button5.width*0.235, button5.height*0.7), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0));
	putText(limbo, "SCORE:" + intToStr(punkty), Point(400, 60), 1, 4, Scalar(255, 255, 255), 5);
	return canvas;
}

void callBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (button.contains(Point(x, y)))
			start = true;
		else if (button2.contains(Point(x, y)))
		{
			cout << "Wczytanie najwyzszych wynikow" << endl;
			rectangle(canvas(button2), button, Scalar(0, 0, 255), 2);
			Mat img = Mat::zeros(600, 550, CV_8UC3);
			imshow("Highscores", img);

			int wyniki[10];
			ifstream plik;
			plik.open("highscores.txt");
			if (!plik.good()) {
				cout << "Otwarcie pliku nie powiodlo się";
			}

			int licznik = 0;
			while (licznik < 10) {
				plik >> wyniki[licznik++];
			}
			plik.close();

			putText(img, "Best results", Point(100, 50), FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Scalar(255, 255, 255), 2);
			int x = 150;
			int y = 100;
			//putText(img, intToStr(wyniki[0]), Point(200, 200), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 250, 250), 2);


			for (int i = 0; i < 10; i++) {
				string wynik;
				if (i<9) wynik = " " + intToStr(i + 1) + ". " + intToStr(wyniki[i]);
				else wynik = intToStr(i + 1) + ". " + intToStr(wyniki[i]);
				putText(img, wynik, Point(x, y), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
				y = y + 50;
			}
			//namedWindow("Image Window image", CV_WINDOW_AUTOSIZE);
			imshow("Highscores", img);
		}
		else if (button3.contains(Point(x, y)))
		{
			exit(0);
		}
	}
}

void callBackFunc2(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (button4.contains(Point(x, y)))
			pauseContinue = true;
		else if (button5.contains(Point(x, y))) {
			pauseContinue = true;
			pauseExit = true;
		}
	}
	//imshow(winName, canvas);
}

void callBackFunc3(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (button5.contains(Point(x, y))){
			pauseContinue = true;
			pauseExit = true;
		}
	}
	//imshow(winName, canvas);
}

int main()
{
	while (1) {
		start = false;
		pauseContinue = false;
		pauseExit = false;

		const double FPS = 30.0;
		const double frameDelay = 1000.0 / FPS;
		int frameStart;

		srand(time(NULL));
		bool bKey[3]; //tablica wcisnietych klawiszy
		player p;
		int xSkok = 0;
		int ySkok = 0;
		int punkty = 0;
		int invurnelabityTime = 30;
		int number_of_lives = 3;
		int przebiegGry = 0;
		int czasPoKolizji = 0;
		bool czyKolizja = false;
		int xPostaci, yPostaci, xPrzeszkody, yPrzeszkody, rPostaci, rPrzeszkody;

		//sciezki plikow dzwiekowych
		const wchar_t* jumpSound = L"C:/Users/Core-i3/source/repos/TestProjektZespolowy1/TestProjektZespolowy1/Assets/Sounds/jump.wav";
		const wchar_t* progressSound = L"C:/Users/Core-i3/source/repos/TestProjektZespolowy1/TestProjektZespolowy1/Assets/Sounds/progress.wav";
		const wchar_t* hitSound = L"C:/Users/Core-i3/source/repos/TestProjektZespolowy1/TestProjektZespolowy1/Assets/Sounds/hit.wav";
		const wchar_t* loseSound = L"C:/Users/Core-i3/source/repos/TestProjektZespolowy1/TestProjektZespolowy1/Assets/Sounds/lose.wav";

		float colorValue = 255.0;
		int colorCounter = 25;

		int wyniki[11];
		ifstream plik;
		plik.open("highscores.txt");
		if (!plik.good()) {
			cout << "Otwarcie pliku nie powiodlo się";
		}

		int licznik = 0;
		while (licznik < 10) {
			plik >> wyniki[licznik++];
		}
		plik.close();

		//Zmienne sluzace do okreslenia czasu
		auto tp1 = chrono::system_clock::now();
		auto tp2 = chrono::system_clock::now();

		//Rysowanie grafiki przy starcie
		Mat whole, pom, limbo;
		Mat displayed = Mat::zeros(1200, 1200, CV_8UC3);

		//Ikonka zycia
		Mat healthIcon, healthMask;
		healthIcon = imread("Assets/Icons/Health.png", IMREAD_UNCHANGED);
		cv::resize(healthIcon, healthIcon, cv::Size(), 0.5, 0.5);

		vector<Mat> rgbLayerHealth;
		split(healthIcon, rgbLayerHealth);
		Mat csH[3] = { rgbLayerHealth[0],rgbLayerHealth[1],rgbLayerHealth[2] };
		merge(csH, 3, healthIcon);
		healthMask = rgbLayerHealth[3];

		//inicjalizacja przeszkód
		ObstacleArray obs;
		//inicjalizacja planszy
		WallArray ws;

		whole = imread("Assets/Walls/Walls2.png");
		limbo = Mat::zeros(2000, 1200, CV_8UC3);
		/*limbo = Scalar(colorValue, colorValue/1.01, colorValue/2);*/

		//wyswietlanie menu
		canvas = glowneMenu(limbo);
		namedWindow(winName);
		setMouseCallback(winName, callBackFunc);
		canvas.copyTo(displayed(Rect(0, 0, 1200, 1200)));

		imshow(winName, displayed);
		while(start == false)
			waitKey(1);

		//Wyswietlanie glownej planszy
		pom = whole(Rect(0, 0, 1200, 1200));
		pom.copyTo(displayed(Rect(0, 0, 1200, 1200)));
		imshow(winName, displayed);

		int i = 0;
		int nrKlatki = 1;
		int wyborS = 0;
		cv::waitKey(1);

		//Petla gry
		while (i < 5)
		{
			//Uzaleznienie funkcji waitKey() od czasu

			/*
			tp2 = chrono::system_clock::now();
			chrono::duration<double> elapsedTime = tp2 - tp1;
			tp1 = tp2;
			double fElapsedTime = elapsedTime.count();*/

			frameStart = clock();

				
			//STEROWANIE GRACZEM

			//przemieszczanie sie gracza do gory
			if ((0x8000 & GetAsyncKeyState((unsigned char)'\x26')))
				p.climbUp(10, 25);
			//przemieszczanie sie gracza do dolu
			if ((0x8000 & GetAsyncKeyState((unsigned char)'\x28')))
				p.climbDown(10, 899);

			//Skok gracza
			if ((0x8000 & GetAsyncKeyState((unsigned char)'\x20')) && nrKlatki >= 12 && p.getDJumpVal() == true && number_of_lives > 0) {
				PlaySound(jumpSound, NULL, SND_ASYNC | SND_FILENAME);
				if (p.getAirVal() == false)
					nrKlatki = 0;
				else if (p.getDJumpVal() == true) {
					nrKlatki = 6;
				}
				xSkok = p.getPosX();
				ySkok = p.getPosY();
				//xSkok += 100;
				p.jump();
			}


			//smierc postaci,
			if (p.isDead == false && number_of_lives == 0) {
				p.isDead = true;
				PlaySound(loseSound, NULL, SND_ASYNC | SND_FILENAME);
				if (p.getAirVal() == false) {
					p.jump();
					nrKlatki = 8;
				}
				else {
					p.switchSide();
					nrKlatki = 6;
				}
				xSkok = p.getPosX();
				ySkok = p.getPosY();
			}

			//Dynamika skoku
			if (number_of_lives > 0)
				p.flight(0.0006, 80, 900, 40, nrKlatki, 0, 899, ySkok, xSkok, 75, 915);
			else {
				if (p.sideLeft == false)
					p.flight(0.007, 80, 300, 11, nrKlatki, 0, 900, ySkok, xSkok, 75, 915);
				else
					p.flight(0.007, 680, 900, 11, nrKlatki, 0, 900, ySkok, xSkok, 75, 915);
				wyborS = 8;
			}

			if (number_of_lives > 0)
				p.land(75, 915);

			//RYSOWANIE
			// reset limbo
			/*if (colorValue > 40 && colorCounter == 0)
			{
			colorValue -= 1.0;
			colorCounter = 25;
			}
			else
			colorCounter -= 1;
			limbo = Scalar(colorValue, colorValue/1.01, colorValue/2);*/
			limbo = Mat::zeros(2000, 1200, CV_8UC3);

			// rysowanie planszy
			for (deque<Wall>::iterator it = ws.begin(); it <= ws.end() - 1; it++)
			{

				ws.teks[it->checksum].copyTo(limbo(Rect(it->cornerx, it->limbocornery, ws.teks[it->checksum].cols, ws.teks[it->checksum].rows)));
				++it;
				Mat flipa, maskflipa;
				flip(ws.teks[it->checksum], flipa, 1);
				flip(ws.mask[it->checksum], maskflipa, 1);
				flipa.copyTo(limbo(Rect(it->cornerx, it->limbocornery, ws.teks[it->checksum].cols, ws.teks[it->checksum].rows)), maskflipa);
			}
			//rysowanie przeszkód
			for (deque<Obstacle>::iterator it = obs.begin(); it != obs.end(); ++it)
			{
				if (it->flipper == 0)
					obs.teks[it->checksum].copyTo(limbo(Rect(it->cornerx, it->limbocornery, obs.teks[it->checksum].cols, obs.teks[it->checksum].rows)), obs.mask[it->checksum]);
				else
				{
					Mat obflipa, maskflipa;
					flip(obs.teks[it->checksum], obflipa, 1);
					flip(obs.mask[it->checksum], maskflipa, 1);
					obflipa.copyTo(limbo(Rect(it->cornerx, it->limbocornery, obs.teks[it->checksum].cols, obs.teks[it->checksum].rows)), maskflipa);
				}
			}
			Mat croppedlimbo = limbo(Rect(0, 400, 1200, 1200));
			croppedlimbo.copyTo(displayed(Rect(0, 0, 1200, 1200)));


			//rysowanie gracza
			wyborS = p.selectSprite(nrKlatki, wyborS);
			if (invurnelabityTime > 30)
				p.drawSprites(displayed, wyborS);
			else if (invurnelabityTime % 10 == 0 || invurnelabityTime % 10 == 1 || invurnelabityTime % 10 == 2 || invurnelabityTime % 10 == 3 || invurnelabityTime % 10 == 4)
				p.drawSprites(displayed, wyborS);

			//rysowanie zycia
			for (int i = 0; i < number_of_lives; i++)
				healthIcon.copyTo(displayed(Rect(0, (0 + (healthIcon.rows + 5) * i), healthIcon.cols, healthIcon.rows)), healthMask);

			//rysowanie punktow
			putText(displayed, intToStr(punkty), Point(1025, 60), 1, 4, Scalar(255, 255, 255), 5);

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


			imshow(winName, displayed);
			//waitKey(1 / fElapsedTime);

			int frameTime = clock() - frameStart;
			if (frameDelay > frameTime)
			{
				if (frameDelay - frameTime >= 1)
					cv::waitKey(frameDelay - frameTime);
				else
					cv::waitKey(1);
			}
			else
				cv::waitKey(1);

			//cv::waitKey(33);


			//kolizja

			deque<Obstacle>::iterator it = obs.things.begin();

			if (obs.things.size() > 0) {
				while (it != obs.things.end() && invurnelabityTime > 30) {
					//if (p.getPosX() < ((*it).cornerx + (*it).width) && (p.getPosX() + p.climb1.cols > (*it).cornerx) && (p.getPosY()+0.6*p.climb1.rows < (*it).cornery + (*it).height) && (p.getPosY() + p.climb1.rows-0.6*p.climb1.rows > (*it).cornery)) {
					if ((p.sideLeft == true && wyborS >= 0 && wyborS <= 4) || (p.sideLeft == false && wyborS >= 5 && wyborS <= 8)) {
						xPostaci = p.getPosX() + 0.75f * p.climb1.cols;
						yPostaci = p.getPosY() + 0.5f * p.climb1.rows;
						xPrzeszkody = (*it).cornerx + 0.5f * (*it).width;
						yPrzeszkody = (*it).cornery + 0.5f * (*it).height - 200;
						rPostaci = 0.25f * p.climb1.cols;
						rPrzeszkody = 0.5f * (*it).width;
					}
					else if ((p.sideLeft == false && wyborS >= 0 && wyborS <= 4) || (p.sideLeft == true && wyborS >= 5 && wyborS <= 8)) {
						xPostaci = p.getPosX() + 0.25f * p.climb1.cols;
						yPostaci = p.getPosY() + 0.5f * p.climb1.rows;
						xPrzeszkody = (*it).cornerx + 0.5f * (*it).width;
						yPrzeszkody = (*it).cornery + 0.5f * (*it).height - 200;
						rPostaci = 0.25f * p.climb1.cols;
						rPrzeszkody = 0.3f * (*it).width;
					}

					if (sqrt((xPrzeszkody - xPostaci) * (xPrzeszkody - xPostaci) + (yPrzeszkody - yPostaci) * (yPrzeszkody - yPostaci)) < (rPostaci + rPrzeszkody)) {
						cout << " Nastapila kolizja " << endl;
						PlaySound(hitSound, NULL, SND_ASYNC | SND_FILENAME);
						invurnelabityTime = 0;
						czyKolizja = true;
						number_of_lives--;
						punkty = punkty / 2;
						if (number_of_lives == 0) {
							cout << "Koniec gry" << endl;
							wyniki[10] = punkty;

							//sortowanie tablicy wynikow
							for (int i = 1; i < 11; i++)
								for (int j = 11 - 1; j >= i; j--)
									if (wyniki[j - 1] > wyniki[j])
									{
										int tmp = wyniki[j - 1];
										wyniki[j - 1] = wyniki[j];
										wyniki[j] = tmp;
									}

							//aktualizacja tablicy wynikow
							fstream plik("highscores.txt", ios::out);
							for (int i = 10; i > 0; i--) {
								plik << wyniki[i] << "\n";
							}
							plik.close();

							/*
							canvas = glowneMenu(limbo);
							namedWindow(winName);
							setMouseCallback(winName, callBackFunc);
							canvas.copyTo(displayed(Rect(0, 0, 1200, 1200)));

							imshow(winName, displayed);
							waitKey();

							*/
						}
					}
					*it++;
				}
			}
			if (number_of_lives > 0) {
				if (przebiegGry <= 1000 && przebiegGry % 5 == 0) punkty++;
				else if (przebiegGry > 1000 && przebiegGry <= 2000 && przebiegGry % 4 == 0) punkty++;
				else if (przebiegGry > 2000 && przebiegGry <= 4000 && przebiegGry % 3 == 0) punkty++;
				else if (przebiegGry > 4000 && przebiegGry <= 8000 && przebiegGry % 2 == 0) punkty++;
				else if (przebiegGry > 8000) punkty++;
			}
			putText(limbo, intToStr(punkty), Point(whole.cols * 0.25 - 10, 60), 1, 4, Scalar(255, 255, 255), 5);

			if (punkty % 200 == 0)
				PlaySound(progressSound, NULL, SND_ASYNC | SND_FILENAME);

			i++;
			nrKlatki++;
			invurnelabityTime++;
			if (i == 5)
				i = 0;
			cout << "Liczba punktow: " << punkty << " Liczba żyć: " << number_of_lives << '\t'<< p.getPosY() << endl;
			if (number_of_lives <= 0 && (p.getPosY() >= 900)) {
				canvas = endScreen(displayed, punkty);
				namedWindow(winName);
				setMouseCallback(winName, callBackFunc3);
				canvas.copyTo(displayed(Rect(0, 0, canvas.cols, canvas.rows)));
				imshow(winName, displayed);
				while (!pauseContinue)
					waitKey(1);
			}

			//Pausa gry
			if ((0x8000 & GetAsyncKeyState((unsigned char)'\x1B'))) {
				canvas = pausaMenu(displayed);
				namedWindow(winName);
				setMouseCallback(winName, callBackFunc2);
				canvas.copyTo(displayed(Rect(0, 0, canvas.cols, canvas.rows)));
				imshow(winName, displayed);
				while (!pauseContinue)
					waitKey(1);
			}
			pauseContinue = false;
			if (pauseExit)
				break;
		}
		//	imshow("Test", whole);
		cv::waitKey(1);
	}return 0;
}
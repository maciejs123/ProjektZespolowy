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

///Konstruktor pojedynczej przeszkody
///Ustala jej poloznenie i grafike
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

		checksum = ((checksum / 3) % 6)+3;
		cornerx = 600 - (width / 2);
	}
	limbocornery = 0;
	cornery = limbocornery - 200;
}

///Aktualizacja pozycji pojedynczej sciany
///Nie pobiera argumentow
///Nie zwraca niczego
///Dziala na wewnetrznych zmiennych obiektu Obstacle
void Obstacle::update_position()
{
	limbocornery += 20;
	cornery += 20;
}
///Konstruktor zbioru scian
///Wczytuje tekstury
///Nie pobiera argumentow
ObstacleArray::ObstacleArray()
{
	teks[0] = imread("Assets/Obstacles/BlueFlag.png", IMREAD_UNCHANGED);
	teks[1] = imread("Assets/Obstacles/RedFlag.png", IMREAD_UNCHANGED);
	teks[2] = imread("Assets/Obstacles/Balcony.png", IMREAD_UNCHANGED);
	teks[3] = imread("Assets/Obstacles/BlackCat.png", IMREAD_UNCHANGED);
	teks[4] = imread("Assets/Obstacles/GingerCat.png", IMREAD_UNCHANGED);
	teks[5] = imread("Assets/Obstacles/GrayCat.png", IMREAD_UNCHANGED);
	teks[6] = imread("Assets/Obstacles/WhiteCat.png", IMREAD_UNCHANGED);
	teks[7] = imread("Assets/Obstacles/BlackFlowerPot.png", IMREAD_UNCHANGED);
	teks[8] = imread("Assets/Obstacles/BrownFlowerPot.png", IMREAD_UNCHANGED);

	


	Size size_one(200, 200);
	for (int i = 0; i < 3; i++)
		resize(teks[i], teks[i], size_one);
	Size size_two(100, 100);
	for (int i = 3; i < 9; i++)
		resize(teks[i], teks[i], size_two);

	for (int i = 0; i < 9; i++)
	{
		vector<Mat> rgbLayer;

		split(teks[i], rgbLayer);
		Mat cs[3] = { rgbLayer[0],rgbLayer[1],rgbLayer[2] };
		merge(cs, 3, teks[i]);
		mask[i] = rgbLayer[3];
	}
}
///Sprawdza czy zbior przeszkod jest pusty
///Nie pobiera argumentow
///Nie zwraca arguementow
///Zmienia wewnetrzna zmienna bool - empty
void ObstacleArray::check_empty()
{
	if (things.size() == 0)
		empty = true;
	else
		empty = false;

}

///Zwraca zmienna bool okreslajaca czy zbior przeszkod jest pusty
bool ObstacleArray::get_empty()
{
	return empty;
}

///Odlicza moment do wywolania konstruktora nowej przeszkody i go wywoluje
///Nie pobiera argumentow
///Nie zwraca niczego
///Dziala na wewnetrznych zmiennych w ObstacleArray
void ObstacleArray::obstacle_countdown()
{
	if (counter == 0)
	{
		things.push_back(Obstacle());
		counter = (70 - (int)difficultymeter) + (rand() % (80 - (int)difficultymeter));
		if (difficultymeter < 60.0)
			difficultymeter += 1;
	}
	else
		counter--;

}

///Sprawdza czy przeszkode mozna usunac
///Nie pobiera argumentow
///Nie zwraca niczego
///Dziala na wewnetrznych zmiennych w ObstacleArray

void ObstacleArray::check_cornery()
{
	if (things[0].cornery > 1200)
	{
		things.pop_front();
	}
}

///Sprawdza czy przeszkode mozna usunac
///Nie pobiera argumentow
///Nie zwraca niczego
///Wywoluje funkcje update_position dla wszystkich pojedynczych przeszkod
void ObstacleArray::update_position()
{
	for (int i = 0; i<things.size(); i++)
	{
		things[i].update_position();
	}
}
///Znajduje wskaznik na poczatek zbioru pojedynczych przeszkod, ktory jest deque bedacym elementem klasy ObstacleArray 
///Nie pobiera argumentow
///Zwraca deque<Obstacle>::iterator
deque<Obstacle>::iterator ObstacleArray::begin()
{
	return this->things.begin();
}
///Znajduje wskaznik na koniec zbioru pojedynczych przeszkod, ktory jest deque bedacym elementem klasy ObstacleArray 
///Nie pobiera argumentow
///Zwraca deque<Obstacle>::iterator
deque<Obstacle>::iterator ObstacleArray::end()
{
	return this->things.end();
}
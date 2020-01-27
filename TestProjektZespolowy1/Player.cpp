#include "stdafx.h"
#include "Player.h"

#define IMAGE_SIZE_MULTIPLAYER 0.13

//podwyzsza pozycje wertykalna postaci
void player::climbUp(int amount, int ceiling) {
	if (posY > ceiling)
		if (isInAir == false)
			posY -= amount;
}

//obniza pozycje wertykalna postaci
void player::climbDown(int amount, int floor) {
	if (posY < floor)
		if (isInAir == false)
			posY += amount;
}

//ustawia isInAir na true jesli postac nie jest w powietrzu oraz sprawdza mozliwosc podwojnego skoku
void player::jump() {
	if (isInAir == false)
	{
		isInAir = true;
		switchSide();
	}
	else if (doubleJump == true) {
		switchSide();
		doubleJump = false;
	}
}

//oblicza parabole lotu postaci, ograniczajac pozycje postaci przez ceiling, floor, lWall i rWall
void player::flight(double a, double p, double q, int speed, int frameNo, int ceiling, int floor, int y, int& x, int lWall, int rWall) {
	if (frameNo >= 7) {
		if (frameNo == 7) {
			if (sideLeft == false)
				x = x - abs(x - lWall);
			else
				x = x + abs(x - rWall);
		}
		if (isInAir == true && sideLeft == true) {
			x = x - speed;
			posX = posX - speed;
			y += 8;
			posY = int(y + a*(x - p)*(x - q));
		}
		else if (isInAir == true && sideLeft == false) {
			x = x + speed;
			posX = posX + speed;
			y += 8;
			posY = int(y + a*(x - p)*(x - q));
		}
		if (posY < ceiling)
			posY = ceiling;
		if (posY > floor)
			posY = floor;
		if (posX < lWall)
			posX = lWall;
		if (posX > rWall)
			posX = rWall;
	}
}

//sprawdza czy uzytkownik wyladowal na scianie, jesli tak to przestaje skakac
void player::land(int leftWall, int rightWall) {
	if (sideLeft == true && posX <= leftWall) {
		isInAir = false;
		doubleJump = true;
	}
	else if (sideLeft == false && posX >= rightWall) {
		isInAir = false;
		doubleJump = true;
	}
}

//rysuje wybranego sprite'a na Matryce Image w zaleznosci od wyborS
void player::drawSprites(Mat image, int wyborS) {

	if (wyborS == 0)
		climb1.copyTo(image(Rect(posX, posY, climb1.cols, climb2.rows)), mask1);
	else if (wyborS == 1)
		climb2.copyTo(image(Rect(posX, posY, climb2.cols, climb2.rows)), mask2);
	else if (wyborS == 2)
		climb3.copyTo(image(Rect(posX, posY, climb3.cols, climb3.rows)), mask3);
	else if (wyborS == 3)
		climb2.copyTo(image(Rect(posX, posY, climb2.cols, climb2.rows)), mask2);
	else if (wyborS == 4)
		jump1.copyTo(image(Rect(posX, posY, jump1.cols, jump1.rows)), maskj1);
	else if (wyborS == 5)
		jump2.copyTo(image(Rect(posX, posY, jump2.cols, jump2.rows)), maskj2);
	else if (wyborS == 6)
		fly1.copyTo(image(Rect(posX, posY, fly1.cols, fly1.rows)), maskf1);
	else if (wyborS == 7)
		fly2.copyTo(image(Rect(posX, posY, fly2.cols, fly2.rows)), maskf2);
	else if (wyborS == 8)
		death.copyTo(image(Rect(posX, posY, death.cols, death.rows)), maskd);
}

//dopasowuje wybor sprite w zaleznosci na ktorej klatce w petli jestesmy: 0 1 2 3 - sprite'y wchodzenia na sciane 4 5 6 7 - sprite'y skoku
int player::selectSprite(int frameNo, int wyborS) {
	if (wyborS == 8)
		return wyborS;
	if (isInAir == true && frameNo == 0)
		return 4;
	else if (frameNo % 7 == 0)
	{
		int spriteNo = frameNo / 7;

		if (isInAir == false) {
			return spriteNo % 4;		// 0 1 2 3 - sprite'y wchodzenia na  sciane
		}
		else
		{/*
			if (frameNo == 0)
				return 4;
			else if (frameNo == 7)
				return 5;
			else*/
			return (spriteNo % 2) + 6;	// 4 5 6 7 - sprite'y skoku
		}
	}
	else if (isInAir == true && frameNo == 3)
		return 5;
	else 
		return wyborS;
}

//odwraca wartosc boola sideLeft oraz uzywa metody flipSprites() do obrocenia wszystkich obrazkow
void player::switchSide() {
	if (sideLeft == true)
		sideLeft = false;
	else
		sideLeft = true;

	flipSprites();
}

//obraca wszystkie sprite'y postaci horyzontalnie
void player::flipSprites() {
	flip(climb1, climb1, 1);
	flip(climb2, climb2, 1);
	flip(climb3, climb3, 1);
	flip(jump1, jump1, 1);
	flip(jump2, jump2, 1);
	flip(fly1, fly1, 1);
	flip(fly2, fly2, 1);
	flip(death, death, 1);

	flip(mask1, mask1, 1);
	flip(mask2, mask2, 1);
	flip(mask3, mask3, 1);
	flip(maskj1, maskj1, 1);
	flip(maskj2, maskj2, 1);
	flip(maskf1, maskf1, 1);
	flip(maskf2, maskf2, 1);
	flip(maskd, maskd, 1);
}

player::player() { 
	posX = 75; 
	posY = 600; 
	isInAir = false; 
	sideLeft = true;
	doubleJump = true;
	isDead = false;
	climb1 = imread("Assets/Postac/ClimbLeft.png", IMREAD_UNCHANGED);
	climb2 = imread("Assets/Postac/ClimbMiddle.png", IMREAD_UNCHANGED);
	climb3 = imread("Assets/Postac/ClimbRight.png", IMREAD_UNCHANGED);
	jump1 = imread("Assets/Postac/JumpStart.png", IMREAD_UNCHANGED);
	jump2 = imread("Assets/Postac/JumpStart2.png", IMREAD_UNCHANGED);
	fly1 = imread("Assets/Postac/JumpFlight1.png", IMREAD_UNCHANGED);
	fly2 = imread("Assets/Postac/JumpFlight2.png", IMREAD_UNCHANGED);
	death = imread("Assets/Postac/Death.png", IMREAD_UNCHANGED);
	cv::resize(climb1, climb1, cv::Size(), IMAGE_SIZE_MULTIPLAYER, IMAGE_SIZE_MULTIPLAYER);
	cv::resize(climb2, climb2, cv::Size(), IMAGE_SIZE_MULTIPLAYER, IMAGE_SIZE_MULTIPLAYER);
	cv::resize(climb3, climb3, cv::Size(), IMAGE_SIZE_MULTIPLAYER, IMAGE_SIZE_MULTIPLAYER);
	cv::resize(jump1, jump1, cv::Size(), IMAGE_SIZE_MULTIPLAYER, IMAGE_SIZE_MULTIPLAYER);
	cv::resize(jump2, jump2, cv::Size(), IMAGE_SIZE_MULTIPLAYER, IMAGE_SIZE_MULTIPLAYER);
	cv::resize(fly1, fly1, cv::Size(), IMAGE_SIZE_MULTIPLAYER, IMAGE_SIZE_MULTIPLAYER);
	cv::resize(fly2, fly2, cv::Size(), IMAGE_SIZE_MULTIPLAYER, IMAGE_SIZE_MULTIPLAYER);
	cv::resize(death, death, cv::Size(), IMAGE_SIZE_MULTIPLAYER, IMAGE_SIZE_MULTIPLAYER);

	flip(jump1, jump1, 1);
	flip(jump2, jump2, 1);
	flip(fly1, fly1, 1);
	flip(fly2, fly2, 1);
	flip(death, death, 1);

	//tworzenie masek z warstwy alfa
	vector<Mat> rgbLayer1, rgbLayer2, rgbLayer3, rgbLayerJ1, rgbLayerJ2, rgbLayerF1, rgbLayerF2, rgbLayerD;

	split(climb1, rgbLayer1);
	Mat cs[3] = { rgbLayer1[0],rgbLayer1[1],rgbLayer1[2] };
	merge(cs, 3, climb1);
	mask1 = rgbLayer1[3];

	split(climb2, rgbLayer2);
	cs[0] = rgbLayer2[0];
	cs[1] = rgbLayer2[1];
	cs[2] = rgbLayer2[2];
	merge(cs, 3, climb2);
	mask2 = rgbLayer2[3];

	split(climb3, rgbLayer3);
	cs[0] = rgbLayer3[0];
	cs[1] = rgbLayer3[1];
	cs[2] = rgbLayer3[2];
	merge(cs, 3, climb3);
	mask3 = rgbLayer3[3];

	split(jump1, rgbLayerJ1);
	cs[0] = rgbLayerJ1[0];
	cs[1] = rgbLayerJ1[1];
	cs[2] = rgbLayerJ1[2];
	merge(cs, 3, jump1);
	maskj1 = rgbLayerJ1[3];

	split(jump2, rgbLayerJ2);
	cs[0] = rgbLayerJ2[0];
	cs[1] = rgbLayerJ2[1];
	cs[2] = rgbLayerJ2[2];
	merge(cs, 3, jump2);
	maskj2 = rgbLayerJ2[3];

	split(fly1, rgbLayerF1);
	cs[0] = rgbLayerF1[0];
	cs[1] = rgbLayerF1[1];
	cs[2] = rgbLayerF1[2];
	merge(cs, 3, fly1);
	maskf1 = rgbLayerF1[3];

	split(fly2, rgbLayerF2);
	cs[0] = rgbLayerF2[0];
	cs[1] = rgbLayerF2[1];
	cs[2] = rgbLayerF2[2];
	merge(cs, 3, fly2);
	maskf2 = rgbLayerF2[3];

	split(death, rgbLayerD);
	cs[0] = rgbLayerD[0];
	cs[1] = rgbLayerD[1];
	cs[2] = rgbLayerD[2];
	merge(cs, 3, death);
	maskd = rgbLayerD[3];
}

player::player(string c1, string c2, string c3, string j1, string j2, string f1, string f2, string d, int x, int y, bool air, bool side, bool djump) {
	posX = x;
	posY = y;
	isInAir = air;
	sideLeft = side;
	doubleJump = djump;
	climb1 = imread(c1, IMREAD_UNCHANGED);
	climb2 = imread(c2, IMREAD_UNCHANGED);
	climb3 = imread(c3, IMREAD_UNCHANGED);
	jump1 = imread(j1, IMREAD_UNCHANGED);
	jump2 = imread(j2, IMREAD_UNCHANGED);
	fly1 = imread(f1, IMREAD_UNCHANGED);
	fly2 = imread(f2, IMREAD_UNCHANGED);
	death = imread(d, IMREAD_UNCHANGED);
	cv::resize(climb1, climb1, cv::Size(), IMAGE_SIZE_MULTIPLAYER, IMAGE_SIZE_MULTIPLAYER);
	cv::resize(climb2, climb2, cv::Size(), IMAGE_SIZE_MULTIPLAYER, IMAGE_SIZE_MULTIPLAYER);
	cv::resize(climb3, climb3, cv::Size(), IMAGE_SIZE_MULTIPLAYER, IMAGE_SIZE_MULTIPLAYER);
	cv::resize(jump1, jump1, cv::Size(), IMAGE_SIZE_MULTIPLAYER, IMAGE_SIZE_MULTIPLAYER);
	cv::resize(jump2, jump2, cv::Size(), IMAGE_SIZE_MULTIPLAYER, IMAGE_SIZE_MULTIPLAYER);
	cv::resize(fly1, fly1, cv::Size(), IMAGE_SIZE_MULTIPLAYER, IMAGE_SIZE_MULTIPLAYER);
	cv::resize(fly2, fly2, cv::Size(), IMAGE_SIZE_MULTIPLAYER, IMAGE_SIZE_MULTIPLAYER);
	cv::resize(death, death, cv::Size(), IMAGE_SIZE_MULTIPLAYER, IMAGE_SIZE_MULTIPLAYER);

	flip(jump1, jump1, 1);
	flip(jump2, jump2, 1);
	flip(fly1, fly1, 1);
	flip(fly2, fly2, 1);
	flip(death, death, 1);

	//tworzenie masek z warstwy alfa
	vector<Mat> rgbLayer1, rgbLayer2, rgbLayer3, rgbLayerJ1, rgbLayerJ2, rgbLayerF1, rgbLayerF2, rgbLayerD;

	split(climb1, rgbLayer1);
	Mat cs[3] = { rgbLayer1[0],rgbLayer1[1],rgbLayer1[2] };
	merge(cs, 3, climb1);
	mask1 = rgbLayer1[3];

	split(climb2, rgbLayer2);
	cs[0] = rgbLayer2[0];
	cs[1] = rgbLayer2[1];
	cs[2] = rgbLayer2[2];
	merge(cs, 3, climb2);
	mask2 = rgbLayer2[3];

	split(climb3, rgbLayer3);
	cs[0] = rgbLayer3[0];
	cs[1] = rgbLayer3[1];
	cs[2] = rgbLayer3[2];
	merge(cs, 3, climb3);
	mask3 = rgbLayer3[3];

	split(jump1, rgbLayerJ1);
	cs[0] = rgbLayerJ1[0];
	cs[1] = rgbLayerJ1[1];
	cs[2] = rgbLayerJ1[2];
	merge(cs, 3, jump1);
	maskj1 = rgbLayerJ1[3];

	split(jump2, rgbLayerJ2);
	cs[0] = rgbLayerJ2[0];
	cs[1] = rgbLayerJ2[1];
	cs[2] = rgbLayerJ2[2];
	merge(cs, 3, jump2);
	maskj2 = rgbLayerJ2[3];

	split(fly1, rgbLayerF1);
	cs[0] = rgbLayerF1[0];
	cs[1] = rgbLayerF1[1];
	cs[2] = rgbLayerF1[2];
	merge(cs, 3, fly1);
	maskf1 = rgbLayerF1[3];

	split(fly2, rgbLayerF2);
	cs[0] = rgbLayerF2[0];
	cs[1] = rgbLayerF2[1];
	cs[2] = rgbLayerF2[2];
	merge(cs, 3, fly2);
	maskf2 = rgbLayerF2[3];


	split(death, rgbLayerD);
	cs[0] = rgbLayerD[0];
	cs[1] = rgbLayerD[1];
	cs[2] = rgbLayerD[2];
	merge(cs, 3, death);
	maskd = rgbLayerD[3];
}

int player::getPosX() { return posX; };
int player::getPosY() { return posY; };
bool player::getAirVal() { return isInAir; };
bool player::getDJumpVal() { return doubleJump; };

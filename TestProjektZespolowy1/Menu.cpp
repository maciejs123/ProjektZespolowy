#include "stdafx.h"
#include "Menu.h"

string menu::intToStr(int n)
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


Mat menu::glowneMenu(Mat limbo) {
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
};

void menu::wyniki() {
	cout << "Wczytanie najwyzszych wynikow" << endl;
	rectangle(canvas(button2), button, Scalar(0, 0, 255), 2);
	Mat img = Mat::zeros(600, 550, CV_8UC3);
	imshow("Highscores", img);

	int wyniki[10];
	ifstream plik;
	plik.open("highscores.txt");
	if (!plik.good()) {
		cout << "Otwarcie pliku nie powiodlo siê";
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

void menu::callBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (button.contains(Point(x, y)))
			;
		else if (button2.contains(Point(x, y)))
			wyniki();
		else if (button3.contains(Point(x, y)))
		{
			cout << "Koniec gry" << endl;
			rectangle(canvas(button3), button, Scalar(0, 0, 255), 2);
			exit(0);
		}
	}
	imshow(winName, canvas);
	waitKey(0);
}

void menu::callBackFunc2(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (button4.contains(Point(x, y)))
		{
			cout << "Powrót do menu" << endl;
			rectangle(canvas(button), button, Scalar(0, 0, 255), 2);
			exit(0);
		}
	}
	imshow(winName, canvas);
}

menu::menu() {
	buttonText = "Play";
	buttonText2 = "Highscores";
	buttonText3 = "Exit";
	buttonText4 = "Return";
	buttonText5 = "Exit";
	winName = "Wallrunner";
}
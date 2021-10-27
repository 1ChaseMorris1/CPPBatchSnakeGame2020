#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>
#include <vector>
#include <conio.h>
#include <dos.h>
#include <direct.h>
#include <shlobj.h>
#include <fstream>
#include <algorithm>
using namespace std;
enum size {BOARDSIZE = 22, SNAKESIZE = 100, FRUIT = 3};
struct Snake {
	char board[BOARDSIZE][BOARDSIZE]; 
	int snakeHeadX; 
	int snakeHeadY;
	vector<int> tailx; 
	vector<int> taily; 
	bool up; 
	bool down; 
	bool right; 
	bool left; 
	int downAgain; 
	int upAgain; 
	int leftAgain; 
	int rightAgain;
	int fruitx[FRUIT]; 
	int fruity[FRUIT]; 
	int fruitAmnt;
	int speed; 
	int score;
	int cont; 
	string directory;
	vector<string> name;
	vector<int> scores;
	char snakeHead; 
	char snakeBody;
};
string getDirectory(); 
void boardDefine(Snake*); 
void writeFile(Snake*);
void topFive(Snake*);
void boardDisplay(Snake*);
void defineSnake(Snake*); 
void loseDis();
void boardAdjust(Snake*);
void sorted(Snake*);
void mainMenu(); 
int getMainSelection(); 
void move(Snake*); 
void controls(Snake*);
void fruitPlace(Snake*); 
void lose(Snake*); 
void tailMove(Snake*);
void eatFruit(Snake*); 
void win(Snake*);
void difficulty(Snake*);
void leaderboard(Snake*);
void populate(Snake*); 
void customSnake(Snake*); 
int main() {
	Snake items;
	int x, selection;

	items.directory = getDirectory(); 

	populate(&items);

	defineSnake(&items);

	mainMenu(); 
	items.speed = 200;
	cout << "\n";
	selection = getMainSelection(); 

	switch (selection) {
	case 2: difficulty(&items); break;
	case 3: leaderboard(&items); break; 
	case 4: customSnake(&items); break; 
	case 5: return 0; break;
	}


	boardDefine(&items);  
	fruitPlace(&items);

	system("cls");
	boardDisplay(&items);
	cout << "Press any key to start..."; 
	cin.get(); 
	cin.ignore();

	do {

		system("CLS");

		controls(&items);

		tailMove(&items);

		move(&items);

		eatFruit(&items);

		boardAdjust(&items);

		boardDisplay(&items);

		if(items.cont == 0)
		win(&items);

		lose(&items);

		Sleep(items.speed);

	} while (1);

}
void writeFile(Snake* items) {
	fstream file; 

	file.open(items->directory + '\\' + "Scores.txt", ios::out | ios::app); 

	file << items->score << "\n";

	file.close(); 
}
void loseDis() {
	int num;
	cout << "\n\nYou Lose!\n";

	cout << "1. Play again\n2. Exit\nEnter your selection : ";

	while ((!(cin >> num) || cin.fail() || (num > 2) || (num < 1))) {
		cout << "Enter your selection : ";
		cin.clear();
		cin.sync();
		cin.ignore(100, '\n');
	}

	switch (num) {
	case 1: main(); break;
	case 2: exit(0); break;
	}
}
void fruitPlace(Snake* items) {
	int wrong = 0;
	for (int i = 0; i < FRUIT; i++) {
		if (items->fruitx[i] == 0 && items->fruity[i] == 0) {
			do {
				items->fruitx[i] = (rand() % (19 - 2 + 1)) + 2;
				items->fruity[i] = (rand() % (19 - 2 + 1)) + 2;
				if (items->board[items->fruity[i]][items->fruitx[i]] == ' ') { wrong++; }
			} while (wrong == 0);
			wrong = 0;
		}
	}
}
void controls(Snake* items) {
	if (_kbhit()) {
		switch(_getch()){
		case 'a': 
			if (items->right == false) {
				items->left = true;
				items->down = false;
				items->up = false;
				items->right = false;
			}
			break; 
		case 'd': 
			if (items->left == false) {
				items->right = true;
				items->down = false;
				items->up = false;
				items->left = false;
			}
			break; 
		case 'w': 
			if (items->down == false) {
				items->up = true;
				items->down = false;
				items->left = false;
				items->right = false;
			}
			break; 
		case 's':
			if (items->up == false) {
				items->down = true;
				items->up = false;
				items->right = false;
				items->left = false;
			}
			break;
		}
	}
}
void eatFruit(Snake* items) {
	for (int i = 0; i < FRUIT; i++) {
		if (items->snakeHeadX == items->fruitx[i] && items->snakeHeadY == items->fruity[i]) {
			items->tailx.push_back(items->snakeHeadX);
			items->taily.push_back(items->snakeHeadY);
			items->fruitx[i] = 0; 
			items->fruity[i] = 0;
			items->score += 10;
			fruitPlace(items);

		}
	}
}
void win(Snake*items){
	int num;
	if (items->score == 1000) {
		cout << "\n\nYou win!\n";

		cout << "1. Play again\n2. Continue\n3. Exit\nEnter your selection : ";

		while ((!(cin >> num) || cin.fail() || (num > 3) || (num < 1))) {
			cin.clear(); 
			cin.sync(); 
			cin.ignore(100, '\n');
		}

		switch (num) {
		case 1: main(); break; 
		case 2: items->cont++; break;
		case 3:
			writeFile(items);
			sorted(items);

			exit(0);
			break;
		}
	}
}
void difficulty(Snake*items){
	int num;
	system("cls"); 
	cout << "1. Easy\n2. Medium\n3. Hard\n4. Insane\n5. Impossible\nEnter your selection : "; 

	while ((!(cin >> num) || cin.fail() || (num > 5) || (num < 1))) {
		cout << "Enter your selection : "; 
		cin.clear(); 
		cin.sync(); 
		cin.ignore(100, '\n');
	}

	switch (num) {
	case 1: items->speed = 400; break;
	case 2: items->speed = 200; break;
	case 3: items->speed = 100; break;
	case 4: items->speed = 50; break;
	case 5: items->speed = 2; break;
	}
}
void leaderboard(Snake*items){
	fstream file;
	vector<int> score;
	vector<string> name; 
	int x, w = 4;
	string n;

	system("cls");

	file.open(items->directory + '\\' + "Scores.txt", ios::beg | ios::in); 
	while (file >> x) { score.push_back(x); }
	file.close(); 
	
	sort(score.begin(), score.end());

	file.open(items->directory + '\\' + "Names.txt", ios::beg | ios::in);
	while (getline(file, n)) { name.push_back(n); }

	cout << right  << " " << score[score.size() - 1] << "  " << name[0] << setw(15) << "FIRST" << "\n\n";
	cout << right  << " " << score[score.size() - 2] << "  " << name[1] << setw(15) << "SECOND" << "\n\n";
	cout << right  << " " << score[score.size() - 3] << "  " << name[2] << setw(15) << "THIRD" << "\n\n";

	for (int i = 7; i > 0; i--) {
		cout << " " << score[i] << setw(18) << w << "\n\n";
		w++;
	}

	cin.get();
	cin.ignore(); 

	main();
}
void populate(Snake*items){
	fstream file; 
	string x; 

	file.open(items->directory + '\\' + "lorium ipsum.txt", ios::out | ios::app);
	file.close();

	file.open(items->directory + '\\' + "lorium ipsum.txt", ios::beg | ios::in); 
		file >> x; 
	file.close(); 

	if (x != "X") {
		file.open(items->directory + '\\' + "Names.txt", ios::out); 
				file << "MG" << "\n";
				file << "CM" << "\n"; 
				file << "SK" << "\n"; 
		file.close(); 

		file.open(items->directory + '\\' + "Scores.txt", ios::out);
			for (int i = 0; i < 10; i++) {
				file << 1 << "\n"; 
			}
		file.close(); 

		file.open(items->directory + '\\' + "lorium ipsum.txt", ios::out);
			file << "X"; 
		file.close();
	}
}
void customSnake(Snake*items){
	system("CLS"); 
	cout << "What do you want the snakes head to be : "; 
	cin >> items->snakeHead; 
	cout << "What do you want the snakes body to be : "; 
	cin >> items->snakeBody; 
}
void lose(Snake* items) {
	int num;
	for (int j = 0; j < BOARDSIZE; j++) {
		for (int i = 0; i < BOARDSIZE; i++) {
			if (items->snakeHeadX == 0 || items->snakeHeadX == 21 || items->snakeHeadY == 0 || items->snakeHeadY == 21) {
				writeFile(items);
				sorted(items);

				loseDis();
			}
		}
	}

	if (items->left == true) {
		items->downAgain = 0; items->upAgain = 0; items->rightAgain = 0;
		for (int i = 0; i < items->tailx.size(); i++) {
			if (items->leftAgain == 0) {
				if (items->snakeHeadX - 1 == items->tailx[i] && items->snakeHeadY == items->taily[i]) { // -1 
					items->leftAgain++;
					break;
				}
			}
			else if(items->leftAgain == 1) {
				writeFile(items);
				sorted(items);

				loseDis();
			}
		}
	}

	if (items->right == true) {
		 items->downAgain = 0; items->upAgain = 0; items->leftAgain = 0;
		for (int i = 0; i < items->tailx.size(); i++) {
			if (items->rightAgain == 0) {
				if (items->snakeHeadX + 1 == items->tailx[i] && items->snakeHeadY == items->taily[i]) { // + 1
					items->rightAgain++;
					break;
				}
			}
			else if (items->rightAgain == 1) {
				writeFile(items);
				sorted(items);

				loseDis();
			}
		}
	}

	if (items->down == true) {
		 items->leftAgain = 0; items->upAgain = 0; items->rightAgain = 0;
		for (int i = 0; i < items->tailx.size(); i++) {
			if (items->downAgain == 0) {
				if (items->snakeHeadX == items->tailx[i] && items->snakeHeadY + 1 == items->taily[i]) {
					items->downAgain++;
					break;
				}
			}
			else if (items->downAgain == 1) {
				writeFile(items);
				sorted(items);

				loseDis();
			}
		}
	}

	if (items->up == true) {
		items->downAgain = 0; items->leftAgain = 0; items->rightAgain = 0;
		for (int i = 0; i < items->tailx.size(); i++) {
			if (items->upAgain == 0) {
				if (items->snakeHeadX == items->tailx[i] && items->snakeHeadY - 1 == items->taily[i]) {
					items->upAgain++;
					break;
				}
			}
			else if (items->upAgain == 1){
				writeFile(items);
				sorted(items);

				loseDis();
			}
		}
	}
}
void tailMove(Snake*items){
	if (!items->tailx.empty()) {
		items->tailx.pop_back();
		items->taily.pop_back();

		items->tailx.insert(items->tailx.begin(), items->snakeHeadX);
		items->taily.insert(items->taily.begin(), items->snakeHeadY);
	}
}
void move(Snake* items) {
	if (items->up == true) { items->snakeHeadY--; }
	if (items->down == true) { items->snakeHeadY++; }
	if (items->right == true) { items->snakeHeadX++; }
	if (items->left == true) { items->snakeHeadX--; }
}
void boardAdjust(Snake* items) {
	int x = 0; 

	boardDefine(items);

	for (int j = 1; j < BOARDSIZE - 1; j++) {
		for (int i = 1; i < BOARDSIZE - 1; i++) {
			if (items->snakeHeadY == j && items->snakeHeadX == i) { items->board[j][i] = items->snakeHead; }
		}
	}

	for (int i = 0; i < FRUIT; i++) {
		items->board[items->fruity[i]][items->fruitx[i]] = '%';
	}

	for (int i = 0; i < items->tailx.size(); i++) {
		items->board[items->taily[i]][items->tailx[i]] = items->snakeBody;
	}

}
void sorted(Snake*items){
	fstream file; 
	string m = "";
	int x, l, pass = 0; 

	file.open(items->directory + '\\' + "Scores.txt", ios::in | ios::beg); 

	while (file >> x) {
		items->scores.push_back(x);
	}
	file.close(); 

	sort(items->scores.begin(), items->scores.end());

	for (int i = 1; i < 4; i++) {
		if (items->score > items->scores[items->scores.size() - i]) {

			file.open(items->directory + '\\' + "Names.txt", ios::in | ios::beg); 
				while (getline(file, m)) {
					items->name.push_back(m);
				}
				file.close();

			cout << "You are number " << i - 1 << " on the leaderboard!\n"; 
				cout << "Enter your name : ";
				getline(cin, m); 
				cin.clear(); 
				cin.sync(); 
			items->name[i - 2] = m;
		
			file.open(items->directory + '\\' + "Names.txt", ios::out);
			for (int i = 0; i < items->name.size(); i++) {
				file << items->name[i] << "\n";
			}
			file.close(); 
			break; 
		}
	}
}
void mainMenu(){
	system("CLS");
	cout << "                                                     kkkkkkkk                           \n";
	cout << "                                                     k::::::k                           \n";
	cout << "                                                     k::::::k                           \n";
	cout << "                                                     k::::::k                           \n";
	cout << "    ssssssssss   nnnn  nnnnnnnn      aaaaaaaaaaaaa    k:::::k    kkkkkkk eeeeeeeeeeee    \n";
	cout << "  ss::::::::::s  n:::nn::::::::nn    a::::::::::::a   k:::::k   k:::::kee::::::::::::ee  \n";
	cout << "ss:::::::::::::s n::::::::::::::nn   aaaaaaaaa:::::a  k:::::k  k:::::ke::::::eeeee:::::ee\n";
	cout << "s::::::ssss:::::snn:::::::::::::::n           a::::a  k:::::k k:::::ke::::::e     e:::::e\n";
	cout << " s:::::s  ssssss   n:::::nnnn:::::n    aaaaaaa:::::a  k::::::k:::::k e:::::::eeeee::::::e\n";
	cout << "   s::::::s        n::::n    n::::n  aa::::::::::::a  k:::::::::::k  e:::::::::::::::::e \n";
	cout << "      s::::::s     n::::n    n::::n a::::aaaa::::::a  k:::::::::::k  e::::::eeeeeeeeeee  \n";
	cout << "ssssss   s:::::s   n::::n    n::::na::::a    a:::::a  k::::::k:::::k e:::::::e           \n";
	cout << "s:::::ssss::::::s  n::::n    n::::na::::a    a:::::a k::::::k k:::::ke::::::::e         \n";
	cout << "s::::::::::::::s   n::::n    n::::na:::::aaaa::::::a k::::::k  k:::::ke::::::::eeeeeeee \n";
	cout << " s:::::::::::ss    n::::n    n::::n a::::::::::aa:::ak::::::k   k:::::kee:::::::::::::e \n";
	cout << "  sssssssssss      nnnnnn    nnnnnn  aaaaaaaaaa  aaaakkkkkkkk    kkkkkkk eeeeeeeeeeeeee \n";
}
int getMainSelection(){
	int num; 
	cout << setw(48) << "1. Play" << endl; 
	cout << setw(54) << "2. Difficulty" << endl;
	cout << setw(55) << "3. Leaderboard" << endl;
	cout << setw(59) << "4. Customize Snake" << endl; 
	cout << setw(48) << "5. Exit" << endl;
	cout << "\n\n";
	cout << setw(48) << "Enter your selection : ";
	
	while ((!(cin >> num) || cin.fail() || (num > 5) || (num < 1))) {
		cout << setw(48) << "Enter your selection : ";
		cin.clear(); 
		cin.sync(); 
		cin.ignore(100, '\n');
	}
	
	return num;
}
void boardDisplay(Snake* items) {
	for (int j = 0; j < BOARDSIZE; j++) {
		for (int i = 0; i < BOARDSIZE; i++) {
			cout << items->board[j][i]; 
		}
		if (j == 1) { cout << setw(10) << "Score : " << items->score; }
		cout << "\n"; 
	}
}
void defineSnake(Snake *items){
	items->snakeHeadX = 10; 
	items->snakeHeadY = 10; 
	items->up = false; 
	items->down = false; 
	items->right = false; 
	items->left = true; 
	items->fruitAmnt = 0;
	for (int i = 0; i < FRUIT; i++) {
		items->fruitx[i] = 0;
		items->fruity[i] = 0;
	}
	items->score = 0;
	items->cont = 0;
	items->downAgain = 0;
	items->upAgain = 0; 
	items->leftAgain = 0; 
	items->rightAgain = 0;
	items->snakeBody = '0'; 
	items->snakeHead = '@';
}
string getDirectory(){
	char path[MAX_PATH];
	if (SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, path) != S_OK)
	{
		mainMenu();
	}
	else
	{
		string newpath(path);       
		newpath.append("\\Documents\\Scores");   
		int x = _mkdir(newpath.c_str());  								
		return string(newpath);
	}
}
void boardDefine(Snake* items) {
	for (int j = 0; j < BOARDSIZE; j++) {
		for (int i = 0; i < BOARDSIZE; i++) {
			if (j == 0 || j == 21) { items->board[j][i] = '#'; }
			else {
				if (i == 0 || i == 21) { items->board[j][i] = '#'; }
				else { items->board[j][i] = ' '; }
			}
		}
	}
}
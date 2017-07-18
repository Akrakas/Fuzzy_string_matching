#include <iostream>
#include <sys/time.h>
#include <ctime>
#include <time.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//Enregistrement
//#include "opencv2/videoio.hpp"
#include "fsm.h"

using namespace cv;

void load_dico(string filename, vector<string>* dico) {
	ifstream dico_file(filename, ios::in);
	string current_line;
	dico_file >> current_line;
	while(!dico_file.eof()) {
		dico->push_back(current_line);
		dico_file >> current_line;
	}
	dico_file.close();
}

vector<string> closest_words_index(string str, vector<string>* dico, vector<int>* score, int wordnumber)
{
	vector<string> closestwords;
	closestwords.resize(wordnumber);
	score->resize(wordnumber);
	for(int i=0 ; i<score->size() ; i++) {
		score->at(i) = 1000;
	}

	for(int i=0 ; i<dico->size() ; i++)
	{
		int LevenshteinScore = FastLevenshteinDistance(str, dico->at(i));
		for(int index=0 ; index<wordnumber ; index++) {
			if(LevenshteinScore < score->at(index)) {
				for(int index_shift=wordnumber-1 ; index_shift>index ; index_shift--) {
					closestwords.at(index_shift) = closestwords.at(index_shift-1);
					score->at(index_shift) = score->at(index_shift-1);
				}
				closestwords.at(index) = dico->at(i);
				score->at(index) = LevenshteinScore;
				break;
			}
		}
	}
	return closestwords;
}

int main() {
	//Initialisation
	string window_name = "FSM_Window";
	int WINDOW_HEIGHT = 250;
	int WINDOW_WIDTH = 500;
	int bd_of_guesses = 5;
	//VideoWriter outputvid("../DEMO/test.avi", VideoWriter::fourcc('D', 'I', 'V', 'X'), 30, Size(WINDOW_WIDTH, WINDOW_HEIGHT), true);

	//Chargement des ressources
	namedWindow(window_name, 1);
	Mat window(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3);
	Mat background = imread("../Assets/Background.png");
	addWeighted(window, 0.0, background, 1.0, 0.0, window);
	putText(window, String("Guess"), Point(10, 70), FONT_HERSHEY_PLAIN, 2, Scalar(0, 255, 0), 1, 8, false);
	putText(window, String("Score"), Point(300, 70), FONT_HERSHEY_PLAIN, 2, Scalar(0, 255, 0), 1, 8, false);
	imshow(window_name, window);

	//Dictionary loading
	vector<string> dico;
	load_dico("../Assets/English_10000_words.txt", &dico);
	vector<string> sentence;
	sentence.push_back("");

	while(1) {
		//Interface clavier
		int key = waitKey(33);
		if(key != -1) {
			if (key >= 'a' && key <= 'z') { //a-z
				sentence.back().push_back(key);
			} else if (key == 8) { //retour arriere
				if(sentence.back().length() > 0) {
					sentence.back().pop_back();
				} else if(sentence.size() > 1) {
					sentence.pop_back();
				}
			} else if (key == ' ') { //espace
				sentence.push_back("");
			}
		}

		//Formation de la phrase a afficher
		string full_sentence;
		for(int i=0 ; i<sentence.size() ; i++) {
			full_sentence.append(sentence.at(i));
			if(i+1 != sentence.size()) full_sentence.append(" ");
			else full_sentence.append("|");
		}

		//Pre-Affichage
		Mat temp = window.clone();
		putText(temp, String(full_sentence), Point(10, 30), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0), 2, 8, false);

		//Calcul propositions
		vector<int> Scores;
		vector<string> ClosestWords = closest_words_index(sentence.back(), &dico, &Scores, bd_of_guesses);
		for(int i=0 ; i<bd_of_guesses ; i++) {
			putText(temp, String(ClosestWords.at(i)), Point(10, (i*30)+100), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, (Scores.at(i)*70)), 2, 8, false);
			putText(temp, String(to_string(Scores.at(i))), Point(300, (i*30)+100), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, (Scores.at(i)*70)), 2, 8, false);
		}

		//Affichage
		imshow(window_name, temp);

		//Enregistrement
		//for(int i=0 ; i<1 ; i++) {
		//	outputvid << temp;
		//}
	}
	return 0;
}

/**
@file main.cpp
*/


#include <opencv\cv.h>                 
#include <opencv\highgui.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "arg_producer.h"
#include "edgel_dictionary.h"
#include "edgel.h"

using namespace std;
using namespace cv;

#define TURN_ON_EXHAUSTIVE true;

int main( int argc, char** argv )
{
	// OFF-LINE
	cout << "========= Off Line Stage =========" << endl;
	
	//create a range of contour names
	cout << "Phase : Processing cmd arguments" << endl;
	/*
	vector<char*> ucm_files;
	produceArgArray(argv, ucm_files);
	vector<char*> img_files;
	produceArgArray2(argv, img_files);
	*/

	vector<string> ucm_files;
	vector<string> img_files;
	char sdir[256], filter[256];
	strcpy(sdir, "D:\\Visual Studio Projects\\retrieval_engine\\Release\\rename");
	strcpy(filter, "*.ucm.jpg");
	fileInFolder(sdir, filter, ucm_files);
	strcpy(filter, "*.img.jpg");
	fileInFolder(sdir, filter, img_files);


	cout << endl;
	cout << "Phase : creatEdgelDic()" << endl;
	clock_t clk_beg = clock();
	
	map<int, Content_Vec> edgel_dic;
	vector<int> edgel_counter(ucm_files.size());
	vector<Mat> ctr_base(ucm_files.size());
	//createEdgelDic(ucm_files, edgel_dic, edgel_counter); 
	createEdgelDic(ucm_files, edgel_dic, edgel_counter, ctr_base); 
	
	clock_t clk_end = clock();
	cout << "Elapsed Time: " << double(clk_end - clk_beg) / CLOCKS_PER_SEC << " s."<< endl;
    
	//cout << endl;
	//cout << "Phase : analyzeEdgelDic()" << endl;
	//analyzeEdgelDic(edgel_dic); //TURN_ON_EXHAUSTIVE);

	cout << endl;
	cout << "Edgel dictionary based on " << ucm_files.size() << " contour images has been successfully built!" << endl;
	cout << endl;

	cout << "Writing edgel_dic into file" << endl;
	makeEdgelDicFile(edgel_dic);

	// ON-LINE
	cout << "========= On Line Stage =========" << endl;
	while(true)
	{
		cout << endl << endl;
		cout << "Waiting for sketch : ";
		string skch_name;//("skch.png");
		cin >> skch_name;
		skch_name.insert(0, "D:\\Visual Studio Projects\\retrieval_engine\\Release\\");
		Mat skch = imread(skch_name, 0);
		assert(skch.cols == 200 && skch.rows == 200);

		clk_beg = clock();
		cout << "Start computing scores" << endl;
		vector<double> score(ucm_files.size());
		computeDocScore(skch, edgel_dic, score);//, rtvl_result);

		//normalize score
		cout << endl;
		for(int i = 0; i < score.size(); i++)
		{
			score[i] /= edgel_counter[i];
			//cout << "score of img " << i << " = " << score[i] << endl;
		}
		clk_end = clock();
		cout << "Elapsed Time: " << double(clk_end - clk_beg) / CLOCKS_PER_SEC << " s." << endl;

		//retrieve at most 100 docID with the highest score
		cout << "Start retrieving the 100 highest scores" << endl;
		clk_beg = clock();
		vector<unsigned int> rtvl_result(min((int)score.size(), 100));
		vector<double> sorted_score(score);
	
		cout << endl << "See retrieval result:" << endl;
		sort(sorted_score.begin(), sorted_score.end());
		int bound = min((int)score.size()-1, 99);
		int rep = 0;
		for(int i = 0; i < bound ; i++)
		{
			if(i == 0)
			{
				rtvl_result[i] = distance(score.begin(), find(score.begin(), score.end(), sorted_score[sorted_score.size()-1-i]));
				cout << "rtvl_result[" << i << "] = " << rtvl_result[i] << " with score = " << sorted_score[sorted_score.size()-1-i] << endl;
			}
			else
			{
				//if this score is the same as the previous one
				if(sorted_score[sorted_score.size()-i] == sorted_score[sorted_score.size()-1-i])
					rep++;
				else
					rep = 0;

				vector<double>::iterator find_iter = score.begin();
				for(int count = 0; count <= rep ; count++)
				{
					find_iter = find(find_iter, score.end(), sorted_score[sorted_score.size()-1-i]);
					rtvl_result[i] = distance(score.begin(), find_iter);
					
					find_iter++;
				}
				cout << "rtvl_result[" << i << "] = " << rtvl_result[i] << " with score = " << sorted_score[sorted_score.size()-1-i] << endl;
			}
		}
		clk_end = clock();
		cout << "Elapsed Time: " << double(clk_end - clk_beg) / CLOCKS_PER_SEC << " s." << endl;


		cout << endl <<"Phase : output retrieved images by docID" << endl;
		// show retrieval results
		int rtvlNum = 0;
		char cc[20];
		char dd[20];
		imshow("Your sketch", skch);
		while(rtvlNum < 5)
		{
			sprintf(cc, "retrieval # %d", rtvlNum);
			sprintf(dd, "%s's contour", cc); 
			imshow(cc, imread(img_files[rtvl_result[rtvlNum]]));
			imshow(dd, ctr_base[rtvl_result[rtvlNum]]);
			rtvlNum++;
			waitKey();
		}
	}
	
	return 0;
}
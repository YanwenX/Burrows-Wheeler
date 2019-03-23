#include<iostream>
#include<vector>
#include<list>
#include"BurrowsWheelerTrans.h"
#include"InvBurrowsWheelerTrans.h"
using namespace std;

int main()
{
	/*int N = 10;
	list<vector<int>> myMatrix;
	for (int i = 0; i < N; ++i) {
		vector<int> rowVector;
		for (int j = 0; j < N; ++j) {
			rowVector.push_back(0);
		}
		myMatrix.push_back(rowVector);
	}

	for (list<vector<int>>::iterator i = myMatrix.begin(); i != myMatrix.end(); ++i) {
		for (int j = 0; j < N; ++j) {
			cout << (*i)[j] << '\t';
		}
		cout << endl;
	}*/

	const int N_1 = 20, N_2 = 30;
	string origFile_txt = "text.dat",
		encodedFile_txt20 = "text_bwt20.dat", encodedFile_txt30 = "text_bwt30.dat";
	string decodedFile_txt20 = "text_ibwt20.dat", decodedFile_txt30 = "text_ibwt30.dat";

	BurrowsWheelerTrans BWT_txt20(N_1), BWT_txt30(N_2);
	InvBurrowsWheelerTrans IBWT_txt20(N_1), IBWT_txt30(N_2);

	BWT_txt20.bwt(origFile_txt);
	BWT_txt20.showBWTstring();
	BWT_txt20.outputEncodedFile(encodedFile_txt20);
	/*IBWT_txt20.recoverBlocks(encodedFile_txt20);
	IBWT_txt20.ibwt(decodedFile_txt20);
	
	BWT_txt30.bwt(origFile_txt);
	BWT_txt30.outputEncodedFile(encodedFile_txt30);
	IBWT_txt30.recoverBlocks(encodedFile_txt30);
	IBWT_txt30.ibwt(decodedFile_txt30);*/

	return 0;
}
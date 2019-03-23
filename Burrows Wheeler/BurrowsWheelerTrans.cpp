#include"BurrowsWheelerTrans.h"
#include<vector>
#include<algorithm>
#include<iostream>
using std::ios;
using std::vector;
using std::noskipws;
using std::sort;
using std::cout;
using std::endl;

bool lexiCompareAscend(const vector<unsigned char> &a, const vector<unsigned char> &b)
{
	int L = a.size();
	bool isAscend = false;
	for (int i = 0; i < L; ++i) {
		if (a[i] != b[i]) {
			if (int(a[i]) < int(b[i])) {
				isAscend = true;
				break;
			}
			else {
				break;
			}
		}
		else
			continue;
	}
	return isAscend;
}

bool isEqual(vector<unsigned char> a, vector<unsigned char> b)
{
	int L = a.size();
	bool equal = true;
	for (int i = 0; i < L; ++i) {
		if (a[i] != b[i]) {
			equal = false;
			break;
		}
	}
	return equal;
}

bool checkMSB(unsigned char ch)
{
	return (int(ch) >= 128);
}

void BurrowsWheelerTrans::bwt(string fileName)
{
	inFile.open(fileName, ios::binary);
	unsigned char readNext;
	vector<vector<unsigned char>> bwMatrix;

	while (inFile >> noskipws >> readNext) {
		rawData.push_back(readNext);
	}

	inFile.close();
	
	/* initialize the matrix */
	for (int i = 0; i < blockSize; ++i) {
		vector<unsigned char> rowVector;
		for (int j = 0; j < blockSize; ++j) {
			rowVector.push_back('\0');
		}
		bwMatrix.push_back(rowVector);
	}

	int countRead = 0;
	while (!rawData.empty()) {
		if (countRead < blockSize) {
			// int j = 0;
			for (int i = 0; i < bwMatrix.size(); ++i) {
				bwMatrix[i][(i + countRead) % blockSize] = rawData.front();
			}
			rawData.pop_front();
			++countRead;
		}
		else {
			countRead = 0;
			vector<unsigned char> tempBlock = bwMatrix.front();
			/* sort the matrix by rows */
			sort(bwMatrix.begin(), bwMatrix.end(), lexiCompareAscend);
			/* find the "first" row in sorted matrix */
			int rowIndex;
			for (int i = 0; i < bwMatrix.size(); ++i) {
				if (isEqual(bwMatrix[i], tempBlock)) {
					rowIndex = i;
					break;
				}
			}
			/* push the transformed string with the index into the BWTblock */
			BWTblock.push_back(new Block(rowIndex));
			for (int i = 0; i < bwMatrix.size(); ++i) {
				BWTblock.back()->blockVector.push_back(bwMatrix[i][blockSize - 1]);
			}
		}
	}

	/* last few characters */
	vector<vector<unsigned char>> lastbwMatrix;
	vector<unsigned char> lastChars;
	for (int i = 0; i < countRead; ++i) {
		lastChars.push_back(bwMatrix[0][i]);
		
		vector<unsigned char> rowVector;
		for (int j = 0; j < countRead; ++j) {
			rowVector.push_back('\0');
		}
		lastbwMatrix.push_back(rowVector);
	}

	for (int i = 0; i < countRead; ++i) {
		for (int j = 0; j < countRead; ++j) {
			lastbwMatrix[i][j] = lastChars[(j - i + countRead) % countRead];
		}
	}

	vector<unsigned char> tempBlock = lastbwMatrix.front();
	sort(lastbwMatrix.begin(), lastbwMatrix.end(), lexiCompareAscend);
	int rowIndex;
	for (int i = 0; i < countRead; ++i) {
		if (isEqual(lastbwMatrix[i], tempBlock)) {
			rowIndex = i;
			break;
		}
	}
	BWTblock.push_back(new Block(rowIndex));
	for (int i = 0; i < countRead; ++i) {
		BWTblock.back()->blockVector.push_back(lastbwMatrix[i][countRead - 1]);
	}
}

void BurrowsWheelerTrans::showBWTstring()
{
	for (list<Blk>::iterator i = BWTblock.begin(); i != BWTblock.end(); ++i) {
		cout << (*i)->index << '\t';
		for (int j = 0; j < (*i)->blockVector.size(); ++j) {
			cout << (*i)->blockVector[j];
		}
		cout << endl;
	}
}

void BurrowsWheelerTrans::outputEncodedFile(string fileName)
{
	outFile.open(fileName, ios::binary);
	for (list<Blk>::iterator i = BWTblock.begin(); i != BWTblock.end(); ++i) {
		/* output index */
		outFile << unsigned char((*i)->index);
		
		/* load repeatSymbolList */
		for (int j = 0; j < (*i)->blockVector.size(); ++j) {
			if (repeatSymbolList.back()->value != (*i)->blockVector[j] ||
				repeatSymbolList.back()->isHead) {
				repeatSymbolList.push_back(new RepeatSymbol(1, (*i)->blockVector[j], false));
			}
			else
				++repeatSymbolList.back()->countRepeat;
		}

		/* output repeatSymbolList */
		for (list<RS>::iterator j = repeatSymbolList.begin(); j != repeatSymbolList.end(); ++j) {
			if (!(*j)->isHead) {
				if ((*j)->countRepeat == 1) {
					if (!checkMSB((*j)->value))
						outFile << (*j)->value;
					else {
						int specialCountOne = 129;
						outFile << unsigned char(specialCountOne) << (*j)->value;
					}
				}
				else if ((*j)->countRepeat > 127) {
					for (long k = 1; k < (*j)->countRepeat; ++k) {
						if (k % 127 == 0 && k != (*j)->countRepeat) {
							int specialCount127 = 255;
							outFile << unsigned char(specialCount127) << (*j)->value;
						}
						else if (k % 127 != 0 && k == (*j)->countRepeat)
							outFile << unsigned char(128 + k % 127) << (*j)->value;
					}
				}
				else
					outFile << unsigned char(128 + (*j)->countRepeat) << (*j)->value;
			}
		}

		repeatSymbolList.clear();
		repeatSymbolList.push_back(new RepeatSymbol(0, '\0', true));
	}

	outFile.close();
}
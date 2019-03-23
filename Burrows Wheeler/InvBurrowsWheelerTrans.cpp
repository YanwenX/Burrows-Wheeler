#include"InvBurrowsWheelerTrans.h"
#include<algorithm>
using std::sort;
using std::ios;
using std::noskipws;

bool charCompare(const BWCE &a, const BWCE &b)
{
	return (int(a->value) < int(b->value));
}

void InvBurrowsWheelerTrans::recoverBlocks(string fileName)
{
	inFile.open(fileName, ios::binary);
	unsigned char readNext;
	list<unsigned char> symbolList;
	while (inFile >> noskipws >> readNext) {
		symbolList.push_back(readNext);
	}

	int countRead = 0;
	while (!symbolList.empty()) {
		if (countRead == 0) {
			BWTblock.push_back(new Block(int(symbolList.front())));
			++countRead;
			symbolList.pop_front();
		}
		else if (countRead < blockSize + 1) {
			unsigned char  tempSymbol = symbolList.front();
			if (int(tempSymbol) > 128) {
				repeatSymbolList.push_back(new RepeatSymbol(int(tempSymbol) - 128, '\0', false));
				countRead += int(tempSymbol) - 128;
				symbolList.pop_front();
				repeatSymbolList.back()->value = symbolList.front();
				symbolList.pop_front();
			}
			else {
				repeatSymbolList.push_back(new RepeatSymbol(1, tempSymbol, false));
				++countRead;
				symbolList.pop_front();
			}
		}
		else {
			countRead = 0;
			/* update BWTblock */
			for (list<RS>::iterator i = repeatSymbolList.begin(); i != repeatSymbolList.end(); ++i) {
				for (int j = 0; j < (*i)->countRepeat; ++j) {
					BWTblock.back()->blockVector.push_back((*i)->value);
				}
			}

			/* reload repeatSymbol */
			repeatSymbolList.clear();
			repeatSymbolList.push_back(new RepeatSymbol(0, '\0', true));
		}
	}
	for (list<RS>::iterator i = repeatSymbolList.begin(); i != repeatSymbolList.end(); ++i) {
		for (int j = 0; j < (*i)->countRepeat; ++j) {
			BWTblock.back()->blockVector.push_back((*i)->value);
		}
	}

	inFile.close();
}

void InvBurrowsWheelerTrans::ibwt(string fileName)
{
	outFile.open(fileName, ios::binary);
	for (list<Blk>::iterator i = BWTblock.begin(); i != BWTblock.end(); ++i) {
		vector<BWCE> lastCol, firstCol;
		/* load the last column */
		for (int j = 0; j < (*i)->blockVector.size(); ++j) {
			lastCol.push_back(new bwColEntry((*i)->blockVector[j], 0, 0, false));
		}
		/* sort the last column to get the first column */
		firstCol = lastCol;
		sort(firstCol.begin(), firstCol.end(), charCompare);
		
		/* match the last and the first column */
		for (int j = 0; j < lastCol.size(); ++j) {
			/* last 2 first */
			lastCol[j]->last2first = firstCol[j];
			/* first to last */
			for (int k = 0; k < firstCol.size(); ++k) {
				if (firstCol[j]->value == lastCol[k]->value &&
					lastCol[k]->isMatched == false) {
					firstCol[j]->first2last = lastCol[k];
					firstCol[j]->isMatched = true;
					lastCol[k]->isMatched = true;
					break;
				}
			}
		}

		/* ibwt and output */
		BWCE tempEntry = firstCol[(*i)->index];
		while (tempEntry->first2last != lastCol[(*i)->index]) {
			outFile << tempEntry->value;
			tempEntry = tempEntry->first2last->last2first;
		}
		outFile << tempEntry->value;
	}
	outFile.close();
}


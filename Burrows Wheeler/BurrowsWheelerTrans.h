#ifndef BURROWSWHEELERTRANS_H
#define BURROWSWHEELERTRANS_H

#include<fstream>
#include<list>
#include<string>
#include<vector>
using std::ifstream;
using std::ofstream;
using std::list;
using std::string;
using std::vector;

typedef struct RepeatSymbol {
	int countRepeat;
	unsigned char value;
	bool isHead;
	RepeatSymbol(int c, unsigned char v, bool h) : countRepeat(c), value(v), isHead(h) {}
}*RS;

typedef struct Block {
	int index;
	vector<unsigned char> blockVector;
	Block(int idx) : index(idx) {}
}*Blk;

class BurrowsWheelerTrans
{
protected:
	ifstream inFile;
	int blockSize;
	list<unsigned char> rawData;
	list<Blk> BWTblock;
	list<RS> repeatSymbolList;
	ofstream outFile;
public:
	BurrowsWheelerTrans(int N) {
		blockSize = N;
		repeatSymbolList.push_back(new RepeatSymbol(0, '\0', true));
	}
	void bwt(string);
	void showBWTstring();
	void outputEncodedFile(string);
};

#endif	// BURROWSWHEELERTRANS
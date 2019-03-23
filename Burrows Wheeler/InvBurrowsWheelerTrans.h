#ifndef INVBURROWSWHEELERTRANS_H
#define INVBURROWSWHEELERTRANS_H

#include"BurrowsWheelerTrans.h"

typedef struct bwColEntry {
	unsigned char value;
	bwColEntry* last2first;		// pointer from the last column to the first
	bwColEntry* first2last;		// pointer from the first column to the last
	bool isMatched;				/* whether the element is matched with one 
								in another column*/
	bwColEntry(unsigned char v, bwColEntry* l2f, bwColEntry* f2l, bool m) :
		value(v), last2first(l2f), first2last(f2l), isMatched(m) {}
}*BWCE;

class InvBurrowsWheelerTrans : public BurrowsWheelerTrans
{
private:
	
public:
	InvBurrowsWheelerTrans(int N) : BurrowsWheelerTrans(N) {}
	void recoverBlocks(string);
	void ibwt(string);
};

#endif	// INVBURROWSWHEELERTRANS_H

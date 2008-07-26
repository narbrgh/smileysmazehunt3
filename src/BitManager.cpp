#include "BitManager.h"
#include "Math.h"
#include "hge.h"

extern HGE *hge;

BitManager::BitManager() {
	counter=0; //Start at position 0
	byte=0; //Fill the byte with 0's
	

}

BitManager::~BitManager() {

}

/*
 * This function adds a bit on at the current counter position. If the unsigned char is
 * full, return true.
 */

bool BitManager::addBit(bool bit) {
	setBit(7-counter,bit);
	counter++;

	if (counter > 7) return true;

	return false;
}

unsigned char BitManager::getCurrentChar() {
	unsigned char returnByte = byte;
    
	//clear out the byte and reset the counter
	byte=0;
	counter=0;

	return returnByte;
}

// Methods used in loading ///////////////////
void BitManager::setChar(unsigned char charToAdd) {
	byte = charToAdd;
	counter = 0;
}

twoBools BitManager::getNextBit() {
	twoBools returnData;
	
	returnData.nextBit = getBit(7-counter);
	
	returnData.isCharFullyRead = false;
	counter++;
	if (counter > 7) {
		counter=0;
		returnData.isCharFullyRead = true;
	}

	return returnData;

}

// Private ////////////////////////////////////////////////////

void BitManager::setBit(int position, bool bit) {
	if (bit) {	
		setTrue(position);
	} 
}

void BitManager::setTrue(int position) {
	byte = byte | (unsigned char)pow(2,position); // | is the bitwise OR
}

int BitManager::getBit(int position) {
	//pow(2,8-position) makes a byte of: 10000000 for position 0, 01000000 for position 1, etc.
	//& is the bitwise "and".
	//Our byte    (ex 01100111) 
	// & our pow  (ex 00000100) will return non-zero if the bit at the position in our byte was 1
	//              = 00000100
	return byte & (unsigned char)pow(2,position);
}
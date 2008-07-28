#include "ChangeManager.h"
#include "Smiley.h"
#include "hge.h"

using namespace std;

extern HGE *hge;

/**
 * Constructor
 */
ChangeManager::ChangeManager () {

}

/**
 * Destructor
 */
ChangeManager::~ChangeManager() {
	reset();
}

/**
 * Changes the change state of a grid tile. If it was already changed it
 * will be removed from the change list, otherwise it will be added.
 */
void ChangeManager::change(int area, int x, int y) {
	if (!removeChange(area,x,y)) addChange(area,x,y);    
}

/**
 * Returns whether or not there is a change at (x,y) in the specified
 * area.
 */
bool ChangeManager::isChanged(int area, int x, int y) {
	std::list<Change>::iterator i;

	for (i = theChanges.begin(); i != theChanges.end(); i++) {
		if (i->area == area && i->x == x && i->y == y) {
			return true;
		}
	}
	return false;
}

///////////// Private Methods ///////////////////

/**
 * Removes a change from the list.
 */
bool ChangeManager::removeChange(int area, int x, int y) {
	std::list<Change>::iterator i;

	for (i = theChanges.begin(); i != theChanges.end(); i++) {
		if (i->area == area && i->x == x && i->y == y) {
			i = theChanges.erase(i);
			return true;
		}
	}
	return false;
}

/**
 * Adds a new change to the list.
 */
void ChangeManager::addChange(int area, int x, int y) {
	
	hge->System_Log("Adding change at (%d, %d) in area %d", x, y, area);
	
	Change newChange;
	newChange.area = area;
	newChange.x = x;
	newChange.y = y;

	theChanges.push_back(newChange);
}

/**
 * Returns a string representation of the changes stored in the
 * Change Manager.
 */ 
char* ChangeManager::toString() {

	string returnString = "";
	
	//The string starts with the number of changes (3 digits)
	returnString += intToString(theChanges.size(), 3);

	//For each change
	std::list<Change>::iterator i;
	for (i = theChanges.begin(); i != theChanges.end(); i++) {
		
		//2 digits for the area
		returnString += intToString(i->area, 2);

		//3 digits for x coordinate
		returnString += intToString(i->x, 3);

		//3 digits for y coordinate
		returnString += intToString(i->y, 3);

	}

	char* dickens = (char*)malloc(returnString.size() + 3);
	strcpy(dickens,returnString.c_str());
	return dickens;

}

/**
 * Clears the change list.
 */
void ChangeManager::reset() {
	theChanges.clear();
}

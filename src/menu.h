#ifndef _MENU_H_
#define _MENU_H_

class MenuScreen;

//Menu States
#define NUM_MENU_SCREENS 4
#define TITLE_SCREEN 0
#define LOAD_SCREEN 1
#define CONTROLS_SCREEN 2
#define DEATH_SCREEN 3

class Menu {

public:
	Menu();
	~Menu();

	//methods
	void draw(float dt);
	bool update(float dt);
	void open(int _state);
	void setScreen(int screen);

	//Variables
	float mouseX, mouseY;
	MenuScreen *menuScreen;
	int currentScreen;

};

#endif
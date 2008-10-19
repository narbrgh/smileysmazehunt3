#ifndef _MENU_H_
#define _MENU_H_

#include <list>

class hgeSprite;
class OptionsWindow;
class Button;

//Menu States
#define NUM_MENU_SCREENS 4
#define TITLE_SCREEN 0
#define LOAD_SCREEN 1
#define OPTIONS_SCREEN 2
#define DEATH_SCREEN 3
#define LOADING_SCREEN 4
#define CREDITS_SCREEN 5

//------------------------------------------------------
//------------------MENU SCREEN-------------------------
//------------------------------------------------------
class MenuScreen {

public:

	virtual void draw(float dt) { };
	virtual bool update(float dt, float mouseX, float mouseY) { return false; };

};

//----------------------------------------------------
//------------------MENU------------------------------
//----------------------------------------------------
class MainMenu {

public:
	MainMenu();
	~MainMenu();

	//methods
	void draw(float dt);
	bool update(float dt);
	void open(int _state);
	void setScreen(int screen);
	void openLoadScreen(int file, bool fromLoadScreen);

	//Variables
	MenuScreen *menuScreen;
	int currentScreen;

};

//-------------------------------------------------------
//------------------CREDITS SCREEN-----------------------
//-------------------------------------------------------
struct CreditsItem {
	float x, float y;
	std::string name;
	hgeSprite *graphic;
};

class CreditsScreen : public MenuScreen {

public:

	CreditsScreen();
	~CreditsScreen();

	//Draw methods
	void draw(float dt);
	bool update(float dt, float mouseX, float mouseY);

private:

	void init();

	std::list<CreditsItem> enemyList;
	std::list<CreditsItem> bossList;
	std::list<CreditsItem> npcList;
	float offset;
	float bossStart;
	float npcStart;
	float timeScreenOpened;
	float endY;
	bool resourcesCachedYet;

};


//-----------------------------------------------------
//------------------DEATH SCREEN-----------------------
//-----------------------------------------------------
#define DS_NUM_BUTTONS 2
#define DS_QUIT_BUTTON 0
#define DS_CONTINUE_BUTTON 1

class DeathScreen : public MenuScreen {

public:

	DeathScreen();
	~DeathScreen();

	//Draw methods
	void draw(float dt);
	bool update(float dt, float mouseX, float mouseY);

	Button *buttons[DS_NUM_BUTTONS];
	float alpha;
	

};

//-------------------------------------------------------
//------------------LOADING SCREEN-----------------------
//-------------------------------------------------------
class LoadingScreen : public MenuScreen {

public:

	LoadingScreen(int fileNumber, bool fromLoadScreen);
	~LoadingScreen();

	//Draw methods
	void draw(float dt);
	bool update(float dt, float mouseX, float mouseY);

	//Variables
	float timeEnteredScreen;
	int fileNumber;
	bool startedLoadYet;
	bool fromLoadScreen;

};

//-------------------------------------------------------
//------------------OPTIONS SCREEN-----------------------
//-------------------------------------------------------
class OptionsScreen : public MenuScreen {

public:

	OptionsScreen();
	~OptionsScreen();

	//Draw methods
	void draw(float dt);
	bool update(float dt, float mouseX, float mouseY);

	OptionsWindow *optionsWindow;
	
};

//-----------------------------------------------------------
//------------------SELECT FILE SCREEN-----------------------
//-----------------------------------------------------------
#define SFS_NUM_BUTTONS 3
#define SFS_BACK_BUTTON 0
#define SFS_DELETE_BUTTON 1
#define SFS_START_BUTTON 2

#define ON_NOTHING -1
#define ON_DELETE_YES 7
#define ON_DELETE_NO 8

struct SaveBox {
	hgeRect *collisionBox;
	float x, y;
	int saveFile;
};

class SelectFileScreen : public MenuScreen {

public:

	SelectFileScreen();
	~SelectFileScreen();

	//Draw methods
	void draw(float dt);
	bool update(float dt, float mouseX, float mouseY);
	void enterScreen();
	void exitScreen();
	const char *getTimeString(int seconds);

	Button *buttons[SFS_NUM_BUTTONS];
	SaveBox saveBoxes[4];

	//Variables
	bool deletePrompt;
	int selectedFile;
	int x, y;
	int mouseOn;
	

};

//-----------------------------------------------------
//------------------TITLE SCREEN-----------------------
//-----------------------------------------------------
#define TS_NUM_BUTTONS 4
#define TS_PLAY_BUTTON 0
#define TS_OPTIONS_BUTTON 1
#define TS_EXIT_BUTTON 2
#define TS_CREDITS_BUTTON 3

class TitleScreen : public MenuScreen {

public:

	TitleScreen();
	~TitleScreen();

	//Draw methods
	void draw(float dt);
	bool update(float dt, float mouseX, float mouseY);

	Button *buttons[TS_NUM_BUTTONS];
	

};

#endif
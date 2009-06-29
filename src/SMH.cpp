#include "SmileyEngine.h"
#include "MainMenu.h"
#include "hge.h"
#include "hgeresource.h"
#include "Environment.h"
#include "NPCManager.h"
#include "Player.h"
#include "WindowFramework.h"
#include "EnemyFramework.h"
#include "LootManager.h"
#include "FenwarManager.h"
#include "ProjectileManager.h"
#include "Boss.h"
#include "ExplosionManager.h"

SMH::SMH(HGE *_hge) { 
	hge = _hge;
	debugMode = false;
	debugText = "";
	screenColorAlpha = 0.0;

	//Game time and frame counter are only set once and carry over when "re-entering" game mode. 
	gameTime = 0.0;
	frameCounter = 0;
}

SMH::~SMH() { }

/**
 * Creates the game objects. Note that order is important!
 */
void SMH::init() {

	log("-------------------------------------");
	log("---Initializing Smiley's Maze Hunt---");
	log("-------------------------------------");

	log("Creating ResourceManager");
	resources = new hgeResourceManager("Data/ResourceScript");
	hge->Resource_AttachPack("Data/Sounds.zip");
	hge->Resource_AttachPack("Data/Fonts.zip");
	hge->Resource_AttachPack("Data/GameData.zip");

	log("Creating Console");
	console = new Console();

	log("Creating PopupMessageManager");
	popupMessageManager = new PopupMessageManager();

	log("Creating SaveManager");
	saveManager = new SaveManager();

	log("Creating Player");
	player = new Player();

	log("Creating Enemy Manager");
	enemyManager = new EnemyManager();

	log("Creating Input");
	input = new SmileyInput();

	log("Creating GameData");
	gameData = new GameData();

	log("Creating NPCManager");
	npcManager = new NPCManager();

	log("Creating SoundManager");
	soundManager = new SoundManager();

	log("Creating MainMenu");
	menu = new MainMenu();

	log("Creating WindowManager");
	windowManager = new WindowManager();

	log("Creating EnemyGroupManager");
	enemyGroupManager = new EnemyGroupManager();

	log("Creating AreaChanger");
	areaChanger = new AreaChanger();
		
	log("Creating LootManager");
	lootManager = new LootManager();
		
	log("Creating ProjectileManager");
	projectileManager = new ProjectileManager();		

	log("Creating BossManager");
	bossManager = new BossManager();

	log("Creating FenwarManager");
	fenwarManager = new FenwarManager();

	log("Creating ScreenEffectManager");
	screenEffectsManager = new ScreenEffectsManager();

	log("Creating ExplosionManager");
	explosionManager = new ExplosionManager();

	log("Creating DeathEffectManager");
	deathEffectManager = new DeathEffectManager();

	//Create Environment last
	log("Creating Environment");
	environment = new Environment();

	log("-------Initialization Complete-------");
}

/**
 * This is called each frame to update the game. Returns true if the
 * game is finished and the program should exit.
 */
bool SMH::updateGame() {

	float dt = min(0.1, hge->Timer_GetDelta());

	timeInState += dt;
	frameCounter++;
	input->UpdateInput();
	
	//Input for taking screenshots
	if (hge->Input_KeyDown(HGEK_F9)) {
		hge->System_Snapshot();
	}
	
	if (gameState == MENU) {

		if (menu->update(dt)) return true;

		if (hge->Input_KeyDown(HGEK_G))
		{
			menu->open(MenuScreens::CLOSING_CINEMATIC_SCREEN);
		}

	} else if (gameState == GAME) {

		//Update the console
		if (smh->hge->Input_KeyDown(HGEK_GRAVE)) console->toggle();
		console->update(dt);

		//Toggle options/exit
		if (!deathEffectManager->isActive() && !windowManager->isOpenWindow() && hge->Input_KeyDown(HGEK_ESCAPE)) {
			windowManager->openWindow(new MiniMenu(MINIMENU_EXIT));
		}

		//Close game menu
		bool menuClosedThisFrame = false;
		if (input->keyPressed(INPUT_PAUSE) && windowManager->isGameMenuOpen()) {
			windowManager->closeWindow();
			menuClosedThisFrame = true;
		}

		windowManager->update(dt);
		areaChanger->update(dt);
		enemyGroupManager->update(dt);
		fenwarManager->update(dt);
		environment->updateAdviceMan(dt);
		player->updateGUI(dt);
		deathEffectManager->update(dt);
		popupMessageManager->update(dt);

		if (!windowManager->isOpenWindow() && !areaChanger->isChangingArea() && !fenwarManager->isEncounterActive() && 
			!environment->isAdviceManActive() && !deathEffectManager->isActive())
		{

			//Open game menu
			if (input->keyPressed(INPUT_PAUSE) && !windowManager->isOpenWindow() && !menuClosedThisFrame) {
				windowManager->openGameMenu();
			}

			player->update(dt);
			explosionManager->update(dt);
			environment->update(dt);
			bossManager->update(dt);
			enemyManager->update(dt);
			lootManager->update(dt);
			projectileManager->update(dt);
			npcManager->update(dt);
			screenEffectsManager->update(dt);
		}

		smh->resources->GetAnimation("fenwar")->Update(dt);

		//Keep track of the time that no windows are open.
		if (!windowManager->isOpenWindow()) gameTime += dt;
	
	}

	return false;

}

/**
 * This is called each frame to perform all rendering for the current frame.
 */
void SMH::drawGame() {

	float dt = hge->Timer_GetDelta();

	screenEffectsManager->applyEffect();
	hge->Gfx_BeginScene();

	if (getGameState() == MENU) {
		menu->draw(dt);
	} else {
		environment->draw(dt);
		lootManager->draw(dt);
		enemyManager->draw(dt);
		npcManager->draw(dt);
		bossManager->drawBeforeSmiley(dt);
		if (!deathEffectManager->isActive()) player->draw(dt);
		environment->drawAfterSmiley(dt);
		bossManager->drawAfterSmiley(dt);
		explosionManager->draw(dt);
		fenwarManager->draw(dt);
		projectileManager->draw(dt);
		player->drawJesusBeam();
		environment->drawSwitchTimers(dt);
		if (screenColorAlpha > 0.0) drawScreenColor(screenColor, screenColorAlpha);
		areaChanger->draw(dt);
		player->drawGUI(dt);
		windowManager->draw(dt);
		deathEffectManager->draw(dt);
		console->draw(dt);
		popupMessageManager->draw(dt);
	}

	if (isDebugOn()) {
		//Grid co-ords and fps
		resources->GetFont("consoleFnt")->printf(1000,5,HGETEXT_RIGHT,"(%d,%d)  FPS: %d", 
			player->gridX, player->gridY, hge->Timer_GetFPS());

		//Debug text
		resources->GetFont("consoleFnt")->printf(10,700,HGETEXT_LEFT,debugText.c_str());
	}

	hge->Gfx_EndScene();
}
	
/**
 * Switches the game state
 */
void SMH::enterGameState(int newState) {

	//If leaving the menu
	if (gameState == MENU) {	
		menu->closeScreen();
		resources->Purge(RES_MENU);
	}

	//If leaving game state
	if (gameState == GAME) {
		saveManager->saveTimePlayed();
		screenEffectsManager->stopEffect();
	}

	gameState = newState;
	timeInState = 0.0;

	//If entering game state
	if (gameState == GAME) {
		soundManager->playAreaMusic(saveManager->currentArea);
	}
}

/////////////////////////////////////////////
///////////// ACCESSORS / MUTATORS //////////
/////////////////////////////////////////////

int SMH::getGameState() {
	return gameState;
}

float SMH::getGameTime() {
	return gameTime;
}

void SMH::setGameTime(float _gameTime) {
	gameTime = _gameTime;
}

float SMH::getRealTime() {
	return hge->Timer_GetTime();
}

bool SMH::isDebugOn() {
	return debugMode;
}

void SMH::toggleDebugMode() {
	debugMode = !debugMode;
}

int SMH::getCurrentFrame() {
	return frameCounter;
}

void SMH::setScreenColor(int color, float alpha) {
	screenColor = color;
	screenColorAlpha = alpha;
	if (screenColorAlpha < 0.0) screenColorAlpha = 0.0;
}

float SMH::getScreenColorAlpha() {
	return screenColorAlpha;
}

/////////////////////////////
///// UTILITY FUNCTIONS /////
/////////////////////////////

/**
 * Draw a collision box. The color options are Colors::RED, Colors::GREEN, or Colors::BLUE. If any other value is
 * is specified it will default to black;
 */
void SMH::drawCollisionBox(hgeRect *box, int color) {

	int r = (color == Colors::RED) ? 255 : 0;
	int g = (color == Colors::GREEN) ? 255 : 0;
	int b = (color == Colors::BLUE) ? 255 : 0;

	int x1 = smh->getScreenX(box->x1);
	int x2 = smh->getScreenX(box->x2);
	int y1 = smh->getScreenY(box->y1);
	int y2 = smh->getScreenY(box->y2);

	hge->Gfx_RenderLine(x1, y1, x2, y1, ARGB(255,r,g,b));
	hge->Gfx_RenderLine(x2, y1, x2, y2, ARGB(255,r,g,b));
	hge->Gfx_RenderLine(x2, y2, x1, y2, ARGB(255,r,g,b));
	hge->Gfx_RenderLine(x1, y2, x1, y1, ARGB(255,r,g,b));

}

/**
 * Draws a sprite at a global position.
 */
void SMH::drawGlobalSprite(const char* sprite, float x, float y) {
	resources->GetSprite(sprite)->Render(smh->getScreenX(x), smh->getScreenY(y));
}

/**
 * Draws a sprite at an absolute position on the screen.
 */
void SMH::drawSprite(const char* sprite, float x, float y) {
	resources->GetSprite(sprite)->Render(x, y);
}

/**
 * Draws a sprite at an absolute position stretched to a height and width.
 */
void SMH::drawSprite(const char* sprite, float x, float y, float width, float height) {
	//resources->GetSprite(sprite)->Render(x,y);
	resources->GetSprite(sprite)->RenderStretch(x, y, x + width, y + height);
}

/**
 * Returns the screen x position given the global x position
 */
int SMH::getScreenX(int x) {
	return x - smh->environment->xGridOffset*64.0 - smh->environment->xOffset;
}

/**
 * Returns the screen y position given the global y position
 */
int SMH::getScreenY(int y) {
	return y - smh->environment->yGridOffset*64.0 - smh->environment->yOffset;										  
}

/**
 * Writes a message to the game log.
 */
void SMH::log(const char* text) {
	hge->System_Log(text);
}

/**
 * This text will be displayed on the debug screen
 */
void SMH::setDebugText(std::string text) {
	debugText = text;
}


/**
 * Generates a random integer in the specified range.
 */
int SMH::randomInt(int min, int max) {
	return hge->Random_Int(min, max);
}

/**
 * Generates a random float in the specified range.
 */
float SMH::randomFloat(float min, float max) {
	return hge->Random_Float(min, max);
}

/**
 * Returns the amount of time that has passed since time
 */
float SMH::timePassedSince(float time) {
	return gameTime - time;
}

float SMH::getFlashingAlpha(float n) {
	float flashingAlpha = 255.0;
	float x = smh->getRealTime();
	while (x > n) x -= n;
	if (x < n/2.0) {
		flashingAlpha = 255 * (x/(n/2.0));
	} else {
		flashingAlpha = 255.0 - 255.0 * ((x - n/2.0)/(n/2.0));
	}
	return flashingAlpha;
}

void SMH::drawScreenColor(int color, float alpha) 
{
	if (alpha == 0.0) return;
	
	std::string sprite;

	if (color == Colors::RED) {
		sprite = "redSquare";
	} else if (color == Colors::BLUE) {
		sprite = "blueSquare";
	} else if (color == Colors::YELLOW) {
		sprite = "yellowSquare";
	} else if (color == Colors::BLACK) {
		sprite = "blackSquare";
	} else if (color == Colors::WHITE) {
		sprite = "whiteSquare";
	}

	resources->GetSprite(sprite.c_str())->SetColor(ARGB(alpha,255,255,255));
	for (int i = 0; i < 16; i++) 
	{
		for (int j = 0; j < 12; j++) 
		{
			resources->GetSprite(sprite.c_str())->Render(i*64,j*64);
		}
	}
}
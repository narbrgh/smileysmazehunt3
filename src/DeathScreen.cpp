#include "SMH.h"
#include "DeathScreen.h"
#include "hgeresource.h"
#include "menu.h"
#include "environment.h"
#include "player.h"
#include "Button.h"
#include "Smiley.h"

extern SMH *smh;
extern Menu *theMenu;
extern hgeResourceManager *resources;
extern Environment *theEnvironment;

extern float darkness;

/**
 * Constructor
 */
DeathScreen::DeathScreen() {
	buttons[QUIT_BUTTON] = new Button(250.0, 650.0, "Quit");
	buttons[CONTINUE_BUTTON] = new Button(1024.0-250.0-250.0, 650.0, "Continue");
	alpha = 0;
	darkness = 0.0;
}

/**
 * Destructor
 */
DeathScreen::~DeathScreen() {
	for (int i = 0; i < NUM_BUTTONS; i++) delete buttons[i];
}

/**
 * Updates the death screen
 */
bool DeathScreen::update(float dt, float mouseX, float mouseY) {

	//Update buttons
	for (int i = 0; i < 2; i++) {
		buttons[i]->update(mouseX, mouseY);
	}

	//Click Quit
	if (buttons[QUIT_BUTTON]->isClicked()) {
		theMenu->setScreen(TITLE_SCREEN);
		return false;
	}

	//Click Continue
	if (buttons[CONTINUE_BUTTON]->isClicked()) {
		theMenu->openLoadScreen(smh->saveManager->currentSave, false);
		return false;
	}

	return false;
}

/**
 * Draws the death screen
 */
void DeathScreen::draw(float dt) {

	//Keep drawing the environment and player while the screen is fading in
	if (alpha < 255.0) {
		theEnvironment->draw(dt);
		smh->player->draw(dt);
	}

	//Death screen background fades in
	if (alpha < 255.0f) alpha += 200.0f*dt;
	if (alpha > 255.0f) alpha = 255.0f;
	shadeScreen(alpha);

	resources->GetFont("textBoxNameFnt")->SetColor(ARGB(alpha,255,255,255));
	resources->GetFont("textBoxNameFnt")->printf(512,300,HGETEXT_CENTER, "Game Over");
	resources->GetFont("textBoxNameFnt")->SetColor(ARGB(255,0,0,0));

	//Draw buttons
	for (int i = 0; i < 2; i++) {
		buttons[i]->draw(dt);
	}

}




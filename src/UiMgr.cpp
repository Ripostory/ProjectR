/*
 * UiMgr.cpp
 *
 *  Created on: Apr 12, 2017
 *      Author: chad
 */

#include <UiMgr.h>
#include <Engine.h>
#include <GfxMgr.h>
#include <InputMgr.h>
#include <EntityMgr.h>
#include <Types381.h>
#include <OgreTextAreaOverlayElement.h>

UiMgr::UiMgr(Engine* eng): Mgr(eng){
	// Initialize the OverlaySystem (changed for Ogre 1.9)
	    mOverlaySystem = new Ogre::OverlaySystem();
	    engine->gfxMgr->mSceneMgr->addRenderQueueListener(mOverlaySystem);
	    isOpen = false;
	    //Ogre::WindowEventUtilities::addWindowEventListener(engine->gfxMgr->ogreRenderWindow, this);
}

UiMgr::~UiMgr(){ // before gfxMgr destructor

}

void UiMgr::Init(){
	//init sdktrays

    mInputContext.mKeyboard = engine->inputMgr->mKeyboard;
    mInputContext.mMouse = engine->inputMgr->mMouse;
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", engine->gfxMgr->mWindow, mInputContext, this);
    fillingBox = false;

    //
    mTextBox = NULL;
    mNext = NULL;

    //mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    //mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    //mTrayMgr->hideCursor();
}

void UiMgr::stop(){

}

void UiMgr::openTextBox(Ogre::String n, Ogre::String text){
	dialogue.push(text);
	name.push(n);
}

void UiMgr::fillTextBox(Ogre::String text){
	Ogre::String stringTemp;

	if(current < text.size())
	{
		stringTemp = text[current];
		mTextBox->appendText(stringTemp);
		current++;
	}
	else
	{
		fillingBox = false;
	}
}

void UiMgr::LoadLevel(){
	Interrupt = true;
	mTrayMgr->showBackdrop("ECSLENT/UI");

	mTrayMgr->createLabel(OgreBites::TL_CENTER,"MenuLabel","PROJECT R(ARRR)",250);

	mTrayMgr->createButton(OgreBites::TL_CENTER, "MainMenu1", "Start New Game", 250);
	mTrayMgr->createButton(OgreBites::TL_CENTER, "MainMenu2", "Load Save Game", 250);
	mTrayMgr->createButton(OgreBites::TL_CENTER, "MainMenu3", "Quit Game", 250);
	mTrayMgr->createLabel(OgreBites::TL_BOTTOMLEFT,"DARE","Winners Don't do Drugs",250);
}

void UiMgr::Tick(float dt){
	mTrayMgr->refreshCursor();

	//check if there are queued messages
	if (!dialogue.empty())
	{
		//check if another dialogue box is open
		Interrupt = true;
		if (!fillingBox && !isOpen)
		{
	    	fillingBox = true;
	    	mTextBox->clearText();
	    	mTextBox->setCaption(name.front());
	    	text = dialogue.front();
	    	current = 0;
	    	dialogue.pop();
	    	name.pop();
			isOpen = true;
			mTextBox->show();
			mNext->show();
		}
	}

	if(fillingBox)
		fillTextBox(text);
}

void UiMgr::windowResized(Ogre::RenderWindow* rw){
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = engine->inputMgr->mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void UiMgr::windowClosed(Ogre::RenderWindow* rw){

}

bool UiMgr::keyPressed(const OIS::KeyEvent &arg) {
	std::cout << "Key Pressed: " << arg.key << std::endl;
	return true;
}
bool UiMgr::keyReleased(const OIS::KeyEvent &arg){
	return true;
}
bool UiMgr::mouseMoved(const OIS::MouseEvent &arg){
    if (mTrayMgr->injectMouseMove(arg)) return true;
	return false;
}
bool UiMgr::mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID mid) {
	std::cout << "mouse clicked" << std::endl;
	if (mTrayMgr->injectMouseDown(me, mid)) return true;
	return false;
}
bool UiMgr::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
    /* normal mouse processing here... */
	return false;
}

void UiMgr::buttonHit(OgreBites::Button *b){
    if(b->getName()=="MainMenu1")
    {
    	mTrayMgr->hideBackdrop();
    	//mTrayMgr->hideAll();
    	mTrayMgr->destroyAllWidgets();
    	Interrupt = false;

    	//load textbox
    	mTextBox = mTrayMgr->createTextBox(OgreBites::TL_BOTTOM, "Textbox", "GAME", 800, 150);
    	mTextBox->setText("   ");
    	mTextBox->setTextAlignment(Ogre::TextAreaOverlayElement::Left);
    	mTextBox->setPadding((Ogre::Real)40);
    	mTextBox->show();
    	mNext = mTrayMgr->createButton(OgreBites::TL_BOTTOMRIGHT, "Next", ">>", 150);
    	mNext->show();

        //starting dialogue
    	openTextBox("Game", "Welcome to Project R(Arrr)!");
    	openTextBox("User", "In this game you will be playing a prisoner aboard a space pirate vessel.");
    	openTextBox("Game", "It is your goal to try and escape your captors. There's only one problem...");
    	openTextBox("Game", "You're tied up and can't move!");
    	openTextBox("User", "Fortunately you have telekinetic abilities and can change your own personal gravity to any direction you want.");
    	openTextBox("Game", "Use the WASD keys to use your powers. \nYou can use this empty cell to aquaint yourself with the controls.");
    	openTextBox("Game", "After that you're ON YOUR OWN.");
    	openTextBox("Game", "If you can sneak through the ship to the escape pods you'll be home free!");
    	openTextBox("Game", "Be careful though there are pirates walking about and a security system that's less than friendly.");
    	openTextBox("Game", "Good Luck!");


    }
    else if(b->getName()=="Next"){
    	if(dialogue.empty()){
    		mNext = b;
    		mTextBox->hide();
    		b->hide();
    		Interrupt = false;
    		isOpen = false;
    		fillingBox = false;
    	}
    	else
    	{
    		Interrupt = true;
        	fillingBox = true;
        	isOpen = true;
        	mTextBox->clearText();
        	mTextBox->setCaption(name.front());
        	text = dialogue.front();
        	current = 0;
        	dialogue.pop();
        	name.pop();
    	}
    }

}

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

    //starting dialogue
	dialogue.push_back("Welcome to Project R(Arrr)!");
	dialogue.push_back("In this game you will be playing a prisoner aboard a space pirate vessel.");
	dialogue.push_back("It is your goal to try and escape your captors. There's only one problem...");
	dialogue.push_back("You're tied up and can't move!");
	dialogue.push_back("Fortunately you have telekinetic abilities and can change your own personal gravity to any direction you want.");
	dialogue.push_back("Use the WASD keys to use your powers. \nYou can use this empty cell to aquaint yourself with the controls.");
	dialogue.push_back("After that you're ON YOUR OWN.");
	dialogue.push_back("If you can sneak through the ship to the escape pods you'll be home free!");
	dialogue.push_back("Be careful though there are pirates walking about and a security system that's less than friendly.");
	dialogue.push_back("Good Luck!");
    //mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    //mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    //mTrayMgr->hideCursor();
}

void UiMgr::stop(){

}

void UiMgr::openTextBox(Ogre::String name, Ogre::String text){
	if(mTextBox == NULL)
	{
		mTextBox = mTrayMgr->createTextBox(OgreBites::TL_BOTTOM, "Textbox", "GAME", 800, 150);
		mTextBox->setTextAlignment(Ogre::TextAreaOverlayElement::Left);
		mTextBox->setPadding((Ogre::Real)40);
	}
	else
	{
		mTextBox->show();
		mTextBox->setCaption(name);
	}

	std::cout << "Test 2" << std::endl;
	std::cout << "Test 3" << std::endl;
	mTextBox->clearText();
	fillingBox = true;
	Interrupt = true;
	current = 0;

	if(mNext == NULL)
	{
		mTrayMgr->createButton(OgreBites::TL_BOTTOMRIGHT, "Next", ">>", 150);
	}
	else
	{
		mNext->show();
	}
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
    	openTextBox("Bob    ", "Hifjiweogjiodsjfoijoewijoeifdjmwgiorejiorjfoifjoijroieoijfeiofjeoigjoreij");

    }
    else if(b->getName()=="Next"){
    	if(dialogue.empty()){
    		mNext = b;
    		mTextBox->hide();
    		b->hide();
    		Interrupt = false;
    		return;
    	}
    	fillingBox = true;
    	mTextBox->clearText();
    	text = dialogue.front();
    	current = 0;
    	dialogue.erase(dialogue.begin());
    }

}

void UiMgr::itemSelected(OgreBites::SelectMenu *m){
    Ogre::Vector3 pos;
    pos.x = 0;
    pos.y = 0;
    pos.z = 100;
    switch(m->getSelectionIndex()){
    case 1:
    	engine->entityMgr->CreateEntityOfTypeAtPosition(SpeedBoatType,pos);
    	break;
    case 2:
    	engine->entityMgr->CreateEntityOfTypeAtPosition(DDG51Type,pos);
    	break;
    case 3:
    	engine->entityMgr->CreateEntityOfTypeAtPosition(CarrierType,pos);
    	mLabel->setCaption("Carrier has Arrived!");
    	break;
    default:
    	break;
    }

}

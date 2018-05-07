/*
 * UiMgr.h
 *
 *  Created on: Apr 12, 2017
 *      Author: chad
 */

#ifndef INC_UIMGR_H_
#define INC_UIMGR_H_

#include <OgreWindowEventUtilities.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <Mgr.h>
#include <vector>

#include <SdkTrays.h>

using namespace std;

class UiMgr : public Mgr, public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener, public Ogre::WindowEventListener, public OgreBites::SdkTrayListener
{
private:


protected:
	virtual void windowResized(Ogre::RenderWindow *rw);
	virtual void windowClosed(Ogre::RenderWindow *rw);

    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    void buttonHit(OgreBites::Button *b);

public:

	UiMgr(Engine *engine);
	~UiMgr();
	virtual void Init();
	virtual void Tick(float dt);
	virtual void LoadLevel();
	virtual void stop();
	void openTextBox(Ogre::String name, Ogre::String text);
	void fillTextBox(Ogre::String text);
	void growTextBox(Ogre::Real desiredWidth, Ogre::Real desiredHeight);
	void changeTextBox(Ogre::Real width, Ogre::Real height);

	OgreBites::InputContext mInputContext;
	OgreBites::SdkTrayManager* mTrayMgr;
	OgreBites::TextBox* mTextBox;
	OgreBites::Button* mNext;
	Ogre::OverlaySystem* mOverlaySystem;
	OgreBites::Label *mLabel;
	Ogre::String text;

	std::queue<Ogre::String> dialogue;
	std::queue<Ogre::String> name;
	bool Interrupt, fillingBox, isOpen;
	int current = 0;
};

#endif /* INC_UIMGR_H_ */

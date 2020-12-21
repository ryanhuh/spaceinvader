/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "InvaderReader.h"
#include "WeaponReader.h"

USING_NS_CC;

using namespace cocos2d;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    _gameState = GameState::title;


    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    CSLoader* instance = CSLoader::getInstance();
    if (instance) {
        instance->registReaderObject("InvaderReader", (ObjectFactory::Instance)InvaderReader::getInstance);
        instance->registReaderObject("WeaponReader", (ObjectFactory::Instance)WeaponReader::getInstance);
    }


    _rootNode = CSLoader::createNode("MainScene.csb");
    addChild(_rootNode);
    
    _enermyNode = _rootNode->getChildByName("enemyNode");
    for (int n = 0; n < 3; n++) {
        
        Invader* invader = nullptr;
        if (n == 0) {
            invader = (Invader*)CSLoader::createNode("InvaderA.csb");
        }
        else if (n == 1) {
            invader = (Invader*)CSLoader::createNode("InvaderB.csb");
        }
        else if (n == 2) {
            invader = (Invader*)CSLoader::createNode("InvaderC.csb");
        }
        else
        {
            invader = (Invader*)CSLoader::createNode("InvaderC.csb");
        }
        Sprite* shape1 = (Sprite*)invader->getChildByName("shape1");
        Sprite* shape2 = (Sprite*)invader->getChildByName("shape2");
        Sprite* shape3 = (Sprite*)invader->getChildByName("explosion");

        invader->setShape(shape1, shape2, shape3);
        invader->setShape1Visible(true);
        invader->setShape2Visible(false);
        invader->setExplosionVisible(false);
        invader->setDestroyed(false);
        invader->setPosition(n * 240.0f, 0);
        _enermyNode->addChild(invader);
        invaders.pushBack(invader);
    }

    //Main loop
    //this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::mainLoop));
    _spaceShip = (Sprite*)_rootNode->getChildByName("spaceShip");
    assert(_spaceShip != nullptr);
    return true;
}

void HelloWorld::onEnter() {
    Layer::onEnter();
    this->mainLoop(0);

}

void HelloWorld::mainLoop(float dt)
{
    auto touchListener = EventListenerTouchOneByOne::create();

    touchListener->onTouchBegan = [&](Touch* touch, Event* event) {

        switch (this->_gameState) {
        case GameState::title:
            triggerPlaying();
            break;
        case GameState::playing:
            this->playerLaunchWeapon(this->_spaceShip->getPositionX(),
                this->_spaceShip->getPositionY());
            break;
        case GameState::gameover:
            break;
        }
        return true;
    };

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        touchListener, this);
}


void HelloWorld::triggerGameover() {
    this->_gameState = GameState::gameover;

    this->unschedule(CC_SCHEDULE_SELECTOR(HelloWorld::spaceShipCheck));
    this->unschedule(CC_SCHEDULE_SELECTOR(HelloWorld::invadersCheck));
    this->unschedule(CC_SCHEDULE_SELECTOR(HelloWorld::shieldsCheck));

    this->unschedule(CC_SCHEDULE_SELECTOR(HelloWorld::weaponMove));
}

void HelloWorld::resetGame() {

}

void HelloWorld::triggerPlaying()
{
    this->resetGame();
    this->_gameState = GameState::playing;

    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::weaponMove), 0.1f);

    //	check whether is attacked
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::spaceShipCheck), 0.1f);
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::invadersCheck), 0.1f);
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::shieldsCheck), 0.1f);
}


void HelloWorld::spaceShipCheck(float dt) {

}

void HelloWorld::invadersCheck(float dt) {

}

void HelloWorld::shieldsCheck(float dt) {

}

void HelloWorld::weaponMove(float dt) {

    for each (Weapon *p in this->_weapons)
    {
        if (p->getPositionY() > 700.0f ||
            p->getPositionY() < 60.0f) {
            this->_rootNode->removeChild(p);
            this->_weapons.eraseObject(p);
            break;
        }
        cocos2d::MoveBy* moveAction = nullptr;
        if (p->getWeaponType() == WeaponType::player) {
            moveAction = cocos2d::MoveBy::create(0.25f, Vec2(0.0f, 80.0f));            
        }
        else {
            moveAction = cocos2d::MoveBy::create(0.25f, Vec2(0.0f, -80.0f));
        }
        p->runAction(moveAction);
    }
}

void HelloWorld::playerLaunchWeapon(float x, float y) {

    Weapon* weapon = (Weapon*)CSLoader::createNode("PlayerMissle.csb");
    if (weapon) {
        Sprite* weaponShape = weapon->getChildByName<Sprite*>("playerMissle");
        if (weaponShape) {
            weapon->setPosition(x, y);
            weapon->setWeaponType(WeaponType::player);
            weapon->setWeaponShape(weaponShape);
            this->_rootNode->addChild(weapon);
            this->_weapons.pushBack(weapon);
        }
    }
}




//
//void HelloWorld::menuCloseCallback(Ref* pSender)
//{
//    //Close the cocos2d-x game scene and quit the application
//    Director::getInstance()->end();
//
//    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/
//
//    //EventCustom customEndEvent("game_scene_close_event");
//    //_eventDispatcher->dispatchEvent(&customEndEvent);
//
//
//}



bool HelloWorld::onTouchBegan(Touch* touch, Event* unused_event)
{
    /*DrawNode* drawNode = (DrawNode*)this->getChildByName("Draw");
    if (drawNode) {
        drawNode->clear();
    }*/

    Point touchLoc = touch->getLocation();
    Sprite* sprite = nullptr;
    CCLOG("mouse X:%f Y:%f ", touch->getLocation().x, touch->getLocation().y);


    Sprite* start = (Sprite*)_rootNode->getChildByName("startLabel");
    if (start && start->isVisible()) {
        start->setVisible(false);
    }


    return true;
}
void HelloWorld::onTouchMoved(Touch* touch, Event* unused_event)
{
    if (_spaceShip && _spaceShip->isVisible()) {
        auto currentPos = _spaceShip->getPosition();
        Vec2 delta = Vec2(currentPos.x + touch->getDelta().x, currentPos.y);
        

        _spaceShip->setPosition(delta);
    }

}

void HelloWorld::onTouchEnded(Touch* touch, Event* unused_event)
{
    CCLOG("Touch End");
}
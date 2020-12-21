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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Invader.h"
#include "Constants.h"
#include "Weapon.h"


using namespace cocos2d;

class HelloWorld : public Layer
{
public:
    static Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    bool onTouchBegan(Touch* touch, Event* unused_event);
    /** Callback function for touch moved.
    *
    * @param touch Touch information.
    * @param unused_event Event information.
    * @js NA
    */
    void onTouchMoved(Touch* touch, Event* unused_event);
    /** Callback function for touch ended.
    *
    * @param touch Touch information.
    * @param unused_event Event information.
    * @js NA
    */
    void onTouchEnded(Touch* touch, Event* unused_event);

    // a selector callback
//    void menuCloseCallback(Ref* pSender);
    void onEnter() override;
    void mainLoop(float dt);

    void triggerPlaying();
    void resetGame();
    void triggerGameover();


    void weaponMove(float dt);
    void spaceShipCheck(float dt); //	check whether spaceShip is shot
    void invadersCheck(float dt);  //	check whether invaders are attacked
    void shieldsCheck(float dt);   //	check whether shields are attacked

    void playerLaunchWeapon(float x, float y);

public:
    Sprite* _spaceShip;
    Node* _rootNode;
    Node* _enermyNode;
    cocos2d::Vector<Invader*> invaders;

private:
    GameState _gameState;
    cocos2d::Vector<Weapon*> _weapons;
};

#endif // __HELLOWORLD_SCENE_H__

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <SimpleAudioEngine.h>
using namespace cocos2d;
using namespace CocosDenshion;

class HelloWorld : public cocos2d::Layer
{
private:
    Sprite* s;
    Sprite* bullet;
    Sprite* enemy;
    Label* boardl;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
        
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    virtual void bulletshoot(float directime);
    virtual void updatebullet(float directime);
    virtual void updateplane(float directime);
    
// add touch function, inherit by layer
public:
    virtual bool onTouchBegan(Touch *t, Event *e);
    virtual void onTouchMoved(Touch *t, Event *e);
    virtual void onTouchEnded(Touch *t, Event *e);
};

#endif // __HELLOWORLD_SCENE_H__

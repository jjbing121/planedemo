#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <SimpleAudioEngine.h>
#include "ResultScene.h"


using namespace cocos2d;
using namespace CocosDenshion;

class HelloWorld : public cocos2d::Layer
{
private:
    int j;
    
    Size globalsize;
    Sprite* bg;
    Sprite* boom;
    Sprite* UserPlane;
    Label* boomcount;
    
    Vector<Sprite*> prelog_sprite_list;  // 预加载的资源内容
    Vector<Sprite*> loadlog_sprite_list; // 正式的加载资源内容
    
public:
    static HelloWorld* create(Vector<Sprite*>);
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(Vector<Sprite*>);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
        
    // implement the "static create()" method manually
//    CREATE_FUNC(HelloWorld);
    
    virtual void bulletshoot(float directime);
    virtual void updatebullet(float directime);
    virtual void updateplane(float directime);
    
// add touch function, inherit by layer
public:
    virtual bool onTouchBegan(Touch *t, Event *e);
    virtual void onTouchMoved(Touch *t, Event *e);
    virtual void onTouchEnded(Touch *t, Event *e);
    
    virtual bool BoomtouchBegan(Touch *t, Event *e);

// add enemy from here.
public:
    virtual void load_enemy(float addtime);
};

#endif // __HELLOWORLD_SCENE_H__

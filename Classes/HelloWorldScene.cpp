#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
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
    

    Size globalsize = Director::getInstance()->getWinSize();
    
    // 背景音乐
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("diqiuyi.mp3");
    
    // 背景图
    Sprite* bg = Sprite::create("paperbg.png");
    bg->setPosition(globalsize.width/2, globalsize.height/2);
    bg->setAnchorPoint(Vec2(0.5,0.5));
    this->addChild(bg);
    
    // 纸飞机
    s = Sprite::create("PaperPlane.png");
    s->setPosition(globalsize.width/2, globalsize.height/2);
    s->setAnchorPoint(Vec2(0.5,0.5));
    this->addChild(s);
    
    // 敌机
    enemy = Sprite::create("LXPlane.png");
    enemy->setPosition(globalsize.width/2, globalsize.height+200);
    enemy->setAnchorPoint(Vec2(0.5,0.5));
    Sequence* enemy_action = Sequence::create(RotateBy::create(0.1, 180), MoveTo::create(3, Vec2(globalsize.width/2, globalsize.height-1200)), NULL);
    enemy->runAction(enemy_action);
    this->addChild(enemy);
    
    // 子弹
    bullet = Sprite::create("bullet1.png");
    bullet->setPosition(s->getPosition());
    bullet->setAnchorPoint(Vec2(0.5,0.5));
    this->addChild(bullet);
    
    // 记分板
    boardl = Label::createWithSystemFont("0", "Courier", 45);
    boardl->setPosition(globalsize.width-100, globalsize.height-50);
    boardl->setAnchorPoint(Vec2(0.5,0.5));
    boardl->setSystemFontSize(45);
    boardl->setTextColor(Color4B(70, 70, 70, 80));
    this->addChild(boardl);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    schedule(schedule_selector(HelloWorld::bulletshoot), 0.5f);
    schedule(schedule_selector(HelloWorld::updatebullet));
    schedule(schedule_selector(HelloWorld::updateplane), 0.5f);
    
    return true;
}

/*virtual*/ bool HelloWorld::onTouchBegan(Touch *t, Event *e)
{
    if (s->getBoundingBox().containsPoint(t->getLocation())) {
        return true;
    }
    return false;
};

/*virtual*/ void HelloWorld::onTouchMoved(Touch *t, Event *e)
{
    if (s!=NULL) {
        s->setPosition(Vec2(t->getLocation().x, t->getLocation().y));
    }
};

/*virtual*/ void HelloWorld::onTouchEnded(Touch *t, Event *e)
{
    CCLOG("touch ended.");
};

/*virtual*/ void HelloWorld::bulletshoot(float directime)
{
    if (bullet!=NULL && s!=NULL) {
        bullet->setPosition(s->getPosition());
        MoveTo* bullet_mv = MoveTo::create(0.5, bullet->getPosition()+Vec2(0,200));
        bullet->runAction(bullet_mv);
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("bulletvoice.wav");
    }
}
/*virtual*/ void HelloWorld::updatebullet(float directime)
{
    if (bullet!=NULL && enemy!=NULL && bullet->getBoundingBox().intersectsRect(enemy->getBoundingBox())) {
        CCLOG("bounding box ok.");
        FadeOut* enemy_out = FadeOut::create(1);
        enemy->runAction(enemy_out);
        enemy->stopAllActions();
        enemy->removeFromParentAndCleanup(true);
        enemy=NULL;
        int tmpmath = atoi(boardl->getString().c_str());
        char gmath[6];
        memset(gmath, 0, 6);
        sprintf(gmath, "%d", tmpmath+100);
        boardl->setString(gmath);
    }
};

/*virtual*/ void HelloWorld::updateplane(float directime)
{
    if (s!=NULL && enemy!=NULL && s->getBoundingBox().intersectsRect(enemy->getBoundingBox())) {
        CCLOG("plane touched.");
        Hide* plane_action = Hide::create();
        s->runAction(plane_action);

        s->stopAllActions();
        s->removeFromParentAndCleanup(true);
        s=NULL;
        bullet->stopAllActions();
        bullet->removeFromParentAndCleanup(true);
        bullet=NULL;
    }
}
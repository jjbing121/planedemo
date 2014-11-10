#include "HelloWorldScene.h"

USING_NS_CC;

cocos2d::Scene* HelloWorld::createScene(Vector<Sprite*> getTransportArray)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create(getTransportArray);

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
    
    globalsize = Director::getInstance()->getWinSize();
    
    // notification center callback
    
    // 背景音乐
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("diqiuyi.mp3");
    
    // 背景图
    bg = Sprite::create("paperbg.png");
    bg->setPosition(globalsize.width/2, globalsize.height/2);
    bg->setAnchorPoint(Vec2(0.5,0.5));
    this->addChild(bg);
    
    // 纸飞机
    this->addChild(prelog_sprite_list.at(0));
    this->UserPlane = prelog_sprite_list.at(0);
    // 子弹
    this->addChild(prelog_sprite_list.at(1));
    // 记分板
    this->addChild(prelog_sprite_list.at(2));
    // 敌机
    j=3;
    schedule(schedule_selector(HelloWorld::load_enemy), 2);
    // 炸弹初始化
    boom = Sprite::create("boom.png");
    boom->setPosition(globalsize.width*0.8, 50);
    boom->setAnchorPoint(Vec2(0.5, 0.5));
    boom->setScale(2);
    this->addChild(boom);
    // 炸弹个数初始化
    boomcount = Label::createWithSystemFont("1", "Courier", 45);
    boomcount->setPosition(globalsize.width*0.88, 50);
    boomcount->setAnchorPoint(Vec2(0.5,0.5));
    boomcount->setSystemFontSize(38);
    boomcount->setTextColor(Color4B(70, 70, 70, 80));
    this->addChild(boomcount);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

    auto boom_listener = EventListenerTouchOneByOne::create();
    boom_listener->setSwallowTouches(true);
    boom_listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::BoomtouchBegan, this);
    
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, prelog_sprite_list.at(0));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(boom_listener, boom);
    
    schedule(schedule_selector(HelloWorld::bulletshoot), 0.5f);  // 设置子弹射速
    schedule(schedule_selector(HelloWorld::updatebullet));       // 设置子弹碰撞
    schedule(schedule_selector(HelloWorld::updateplane), 0.5f);  // 设置飞机碰撞
    
    return true;
}

/*virtual*/ bool HelloWorld::onTouchBegan(Touch *t, Event *e)
{
    if (prelog_sprite_list.at(0)->getBoundingBox().containsPoint(t->getLocation())) {
        return true;
    }
    return false;
};

/*virtual*/ void HelloWorld::onTouchMoved(Touch *t, Event *e)
{
    if (prelog_sprite_list.at(0)!=NULL) {
        prelog_sprite_list.at(0)->setPosition(Vec2(t->getLocation().x, t->getLocation().y));
    }
};

/*virtual*/ void HelloWorld::onTouchEnded(Touch *t, Event *e)
{
    CCLOG("touch ended.");
};

/*virtual*/ void HelloWorld::bulletshoot(float directime)
{
    if (prelog_sprite_list.at(1) && prelog_sprite_list.at(0)) {
        prelog_sprite_list.at(1)->setPosition(prelog_sprite_list.at(0)->getPosition());
        MoveTo* bullet_mv = MoveTo::create(0.5, prelog_sprite_list.at(1)->getPosition()+Vec2(0,200));
        prelog_sprite_list.at(1)->runAction(bullet_mv);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("bulletmagic.wav");
    }
}

/*virtual*/ bool HelloWorld::BoomtouchBegan(Touch *t, Event *e)
{
    if (boom != NULL) {
        if (boom->getBoundingBox().containsPoint(t->getLocation())) {
            // 最先决条件 有boom可以使用
            if (atoi(boomcount->getString().c_str()) > 0) {
                // 扣除一次boom
                char leave_boom_count[6];
                memset(leave_boom_count, 0, 6);
                sprintf(leave_boom_count, "%d", atoi(boomcount->getString().c_str())-1);
                boomcount->setString(leave_boom_count);
            }else{
                return false; // return false 不执行boom操作
            }
            
            // 炸弹正式生效，并伴随一个闪烁的效果
            boom->setTextureRect(Rect(0, 0, globalsize.width, globalsize.height));
            Blink* blinkact = Blink::create(1, 8);
            bg->runAction(blinkact);
            
            // 所有对应在屏幕内的飞机需要被摧毁
            for (int i=3; i<prelog_sprite_list.size(); i++) {
                if (prelog_sprite_list.at(i)) {
                    if (boom->getBoundingBox().intersectsRect(prelog_sprite_list.at(i)->getBoundingBox())) {
                        FadeOut* enemy_out = FadeOut::create(1);
                        prelog_sprite_list.at(i)->runAction(enemy_out);
                        prelog_sprite_list.at(i)->stopAllActions();
                        prelog_sprite_list.at(i)->removeFromParentAndCleanup(true);
                        prelog_sprite_list.erase(i);
                        
                        // 对应的增加分数
                        Label* getscoreboard = (Label*)(prelog_sprite_list.at(2));
                        int tmpmath = atoi(getscoreboard->getString().c_str());
                        char gmath[6];
                        memset(gmath, 0, 6);
                        sprintf(gmath, "%d", tmpmath+100);
                        getscoreboard->setString(gmath);
                    }
                }
            }
            // 取消boom的碰撞体积
            boom->setTextureRect(Rect(0, 0, 32, 32));
            return true;
        }
    }
    return false;
}

/*virtual*/ void HelloWorld::updatebullet(float directime)
{
    for (int i=3; i<prelog_sprite_list.size(); i++) {
        
        if (prelog_sprite_list.at(i)) {
            
            if (prelog_sprite_list.at(1) && prelog_sprite_list.at(i)!=NULL && prelog_sprite_list.at(1)->getBoundingBox().intersectsRect(prelog_sprite_list.at(i)->getBoundingBox())) {
                FadeOut* enemy_out = FadeOut::create(1);
                prelog_sprite_list.at(i)->runAction(enemy_out);
                prelog_sprite_list.at(i)->stopAllActions();
                prelog_sprite_list.at(i)->removeFromParentAndCleanup(true);
                prelog_sprite_list.erase(i);
                
                Label* getscoreboard = (Label*)(prelog_sprite_list.at(2));
                int tmpmath = atoi(getscoreboard->getString().c_str());
                char gmath[6];
                memset(gmath, 0, 6);
                sprintf(gmath, "%d", tmpmath+100);
                getscoreboard->setString(gmath);
            }
        }
    }
};

/*virtual*/ void HelloWorld::updateplane(float directime)
{
    for (int i=3; i<prelog_sprite_list.size(); i++) {
        
        if (prelog_sprite_list.at(i)) {
            
            if (prelog_sprite_list.at(0) && prelog_sprite_list.at(i)!=NULL && prelog_sprite_list.at(0)->getBoundingBox().intersectsRect(prelog_sprite_list.at(i)->getBoundingBox())) {
                CCLOG("plane touched.");
                this->removeChild(prelog_sprite_list.at(2));
                Director::getInstance()->replaceScene(ResultScene::createScene((Label*)prelog_sprite_list.at(2)));
            }
        }
    }
}

HelloWorld* HelloWorld::create(Vector<Sprite*> getSpriteArray)
{
    HelloWorld* pRet = new HelloWorld();
    pRet->prelog_sprite_list = getSpriteArray;
    pRet->loadlog_sprite_list.operator=(getSpriteArray);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

void HelloWorld::load_enemy(float addtime)
{
    if (prelog_sprite_list.at(0) == this->UserPlane) {
        if (j<loadlog_sprite_list.size()) {
            this->addChild(loadlog_sprite_list.at(j));
            j++;
        }else{
            CCLOG("global size => %d", (int)loadlog_sprite_list.size());
            CCLOG("at last size => %d", (int)prelog_sprite_list.size());
            this->removeChild(prelog_sprite_list.at(2));
            Director::getInstance()->replaceScene(ResultScene::createScene((Label*)prelog_sprite_list.at(2)));
        }
    }
}
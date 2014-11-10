//
//  MenuScene.cpp
//  PlaneDemo
//
//  Created by Computer on 14/11/6.
//
//

#include "MenuScene.h"

USING_NS_CC;

Scene* MenuScene::MenuSceneCreate()
{
    auto scene = Scene::create();
    
    auto layer = MenuScene::create();
    
    scene->addChild(layer);
    
    return scene;
};

/*virtual*/ bool MenuScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    Size menu_global_size = Director::getInstance()->getVisibleSize();
    
    // menu背景图
    Sprite* menubg = Sprite::create("paperbg.png");
    menubg->setPosition(menu_global_size.width/2, menu_global_size.height/2);
    menubg->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(menubg);

    // menu背景音乐
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("menumusic.mp3");

    // menu开始游戏键
    Label* startbutton = Label::createWithSystemFont("开始游戏", "Courier", 38);
    startbutton->setPosition(menu_global_size.width/2, menu_global_size.height/4);
    startbutton->setAnchorPoint(Vec2(0.5, 0.5));
    startbutton->setTextColor(Color4B(0, 0, 0, 0));
    this->addChild(startbutton, 10);
    
    // menubutton_before
    menubutton_start = Sprite::create("startbutton_before.png");
    menubutton_start->setPosition(menu_global_size.width/2, menu_global_size.height/4);
    menubutton_start->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(menubutton_start, 1);
    
    // menubutton_after
    menubutton_after = Sprite::create("startbutton_after.png");
    menubutton_after->setPosition(menu_global_size.width/2, menu_global_size.height/4);
    menubutton_after->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(menubutton_after,0);
    
    // menu touch action listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan=CC_CALLBACK_2(MenuScene::onMenuButtonClick,this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, menubutton_start);
    
    return true;
};

/*virtual*/ bool MenuScene::onMenuButtonClick(Touch *touch, Event *unused_event)
{
    if (menubutton_start && menubutton_start->getBoundingBox().containsPoint(touch->getLocation())) {
        CCLOG("touch button.");
        Show* show_action = Show::create();
        Hide* hide_action = Hide::create();
        menubutton_start->runAction(hide_action);
        menubutton_after->runAction(show_action);
        
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonclick.wav");
    
        Director::getInstance()->replaceScene(LoadingScene::LodingSceneCreate());
        return true;
    }
    return false;
};

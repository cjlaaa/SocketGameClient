#include "HelloWorldScene.h"
#include "Client.h"
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
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    auto Client = Client::CreateClient(this);
    CCASSERT(Client!=NULL, "Client Create Error!");
    addChild(Client,enZorderFront,enTagClient);
    
    if(Client->ConnectToServer("127.0.0.1", 7000)==false) log("Connect Server Failed...");
    else schedule(schedule_selector(HelloWorld::update), 0.1f);
    
    Client->OnSendChangeNick("hENRYcHANG");
    
    auto pTextField = TextFieldTTF::textFieldWithPlaceHolder("<click here for input>",
                                                             "Arial",
                                                             20);
    pTextField->setDelegate(this);
    pTextField->setPosition(visibleSize.width/2,visibleSize.height/2);
    addChild(pTextField,enZorderFront,enTagTextField);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    auto pTextField = dynamic_cast<TextFieldTTF*>(getChildByTag(enTagTextField));
    CCASSERT(pTextField!=NULL, "TextField Get Error!");
    auto Client = dynamic_cast<class Client*>(getChildByTag(enTagClient));
    CCASSERT(pTextField!=NULL, "TextField Get Error!");
    
    Client->OnSendChatMsg("test Msg");
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
//    return;
//#endif
//
//    Director::getInstance()->end();
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
}

void HelloWorld::update(float fT)
{
    auto Client = dynamic_cast<class Client*>(getChildByTag(enTagClient));
    CCASSERT(Client!=NULL, "Client Get Error!");
    
    Client->update(fT);
}

void HelloWorld::GetServerMsg(std::string MsgFromServer)
{
    
}



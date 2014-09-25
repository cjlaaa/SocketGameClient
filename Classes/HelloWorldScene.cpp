#include "HelloWorldScene.h"

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

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, enZorderFront+1);

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
    
    if(Client->ConnectToServer("192.168.6.108", 7000)==false) log("Connect Server Failed...");
    else schedule(schedule_selector(HelloWorld::update),0.1f);
    Client->OnSendChangeNick("test");
    
    auto glview = Director::getInstance()->getOpenGLView();
    auto editBoxSize = Size(visibleSize.width - 100, 60);
    auto visibleOrigin = glview->getVisibleOrigin();
    auto editName = EditBox::create(editBoxSize, Scale9Sprite::create("green_edit.png"));
    editName->setPosition(Vec2(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y+visibleSize.height*1/10));
    editName->setFontName("Paint Boy");
    editName->setFontSize(25);
    editName->setFontColor(Color3B::RED);
    editName->setPlaceHolder("Type Here...");
    editName->setPlaceholderFontColor(Color3B::GRAY);
    editName->setMaxLength(80);
    editName->setReturnType(EditBox::KeyboardReturnType::DONE);
    editName->setDelegate((cocos2d::extension::EditBoxDelegate*)this);
    addChild(editName,enZorderFront,enTagTextField);
    
    closeItem->setPosition(Vec2(editName->getPositionX() + editName->getContentSize().width/2 + closeItem->getContentSize().width/2 ,
                                editName->getPositionY()));
    
    //LayerColor* pTalkList = LayerColor::create(Color4B::RED);
    Layer* pTalkList = Layer::create();
    CCASSERT(pTalkList!=NULL, "pTalkList Create Error!");
    addChild(pTalkList,enZorderMiddle,enTagTalkList);
    m_nTalkListPos = editName->getContentSize().height;
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
//    auto pTextField = dynamic_cast<TextFieldTTF*>(getChildByTag(enTagTextField));
//    CCASSERT(pTextField!=NULL, "TextField Get Error!");
    auto Client = dynamic_cast<class Client*>(getChildByTag(enTagClient));
    CCASSERT(Client!=NULL, "Client Get Error!");
    auto pEditBox = dynamic_cast<EditBox*>(getChildByTag(enTagTextField));
    CCASSERT(pEditBox!=NULL, "TextField Get Error!");
    
    Client->OnSendChatMsg(pEditBox->getText());
    
    pEditBox->setText("");
    
    
}

void HelloWorld::update(float fT)
{
//    auto pTextField = dynamic_cast<class TextFieldTTF*>(getChildByTag(enTagTextField));
//    CCASSERT(pTextField!=NULL, "TextFieldTTF Get Error!");
    auto Client = dynamic_cast<class Client*>(getChildByTag(enTagClient));
    CCASSERT(Client!=NULL, "Client Get Error!");
    
    Client->update(fT);
    //Client->OnSendAliveCheck();
}

void HelloWorld::GetServerMsg(std::string MsgFromServer)
{
    auto Client = dynamic_cast<class Client*>(getChildByTag(enTagClient));
    CCASSERT(Client!=NULL, "Client Get Error!");
    
    Client->ClientLog("%s",MsgFromServer.c_str());
    
    createNewLabel(MsgFromServer);
}

void HelloWorld::editBoxReturn(EditBox* editBox)
{
    auto Client = dynamic_cast<class Client*>(getChildByTag(enTagClient));
    CCASSERT(Client!=NULL, "Client Get Error!");
    Client->OnSendChatMsg(editBox->getText());
    
    editBox->setText("");
}

void HelloWorld::createNewLabel(std::string strText)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto glview = Director::getInstance()->getOpenGLView();
    auto visibleOrigin = glview->getVisibleOrigin();
    
    auto pEditBox = dynamic_cast<EditBox*>(getChildByTag(enTagTextField));
    CCASSERT(pEditBox!=NULL, "TextField Get Error!");
    auto pTalkList = dynamic_cast<Layer*>(getChildByTag(enTagTalkList));
    CCASSERT(pTalkList!=NULL,"TalkList Get Error!");
    
    Scale9Sprite *pBg = Scale9Sprite::create("green_edit.png");
    CCASSERT(pBg!=NULL, "pBg create error!");
    auto pLabel = LabelTTF::create(strText.c_str(), "Arial", 24);
    CCASSERT(pLabel!=NULL, "pLabel create error!");
    pLabel->setPosition(pLabel->getPosition() + pLabel->getContentSize()/1.9);
    pLabel->setDimensions(pEditBox->getContentSize());
    pBg->addChild(pLabel);
    pBg->setPosition(visibleSize.width/2,m_nTalkListPos);
    pBg->setContentSize(pEditBox->getContentSize());
    pTalkList->addChild(pBg);
    
    
    MoveBy* pMoveBy = MoveBy::create(0.2, Vec2(0,pEditBox->getContentSize().height * 1.5));
    pTalkList->runAction(pMoveBy);
    m_nTalkListPos -= pEditBox->getContentSize().height * 1.5;
}


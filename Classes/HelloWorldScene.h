#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Client.h"
#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;

enum
{
    enTagTextField = 0,
    enTagClient = 1,
    enTagTalkList = 2,
};

enum
{
    enZOrderBack = 0,
    enZorderMiddle = 100,
    enZorderFront = 200,
};

class HelloWorld : public cocos2d::Layer,public cocos2d::extension::EditBoxDelegate,public ClientDelegate
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    void update(float);
    
    void GetServerMsg(std::string);
    
    void editBoxReturn(EditBox* editBox);
    
    void createNewLabel(std::string);
    
    int m_nTalkListPos;
};

#endif // __HELLOWORLD_SCENE_H__

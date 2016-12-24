#include "HelloWorldScene.h"
#include"GameScene.h"
#include"OnlineGameScene.h"
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
	if (!Layer::init())
	{
		return false;
	}
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto bg = Sprite::create("background.png");
	bg->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	addChild(bg);
	auto labelGameName = Label::createWithSystemFont("Tic Tac Toe", "Arial", 70);
	labelGameName->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 100));
	addChild(labelGameName);
	auto singleGameLabel = Label::createWithSystemFont("Single Game", "Arial", 50);
	auto menuSingleItem = MenuItemLabel::create(singleGameLabel, CC_CALLBACK_1(HelloWorld::SingleGame, this));
	auto doubleGameLabel = Label::createWithSystemFont("Double Game", "Arial", 50);
	auto menuDoubleItem = MenuItemLabel::create(doubleGameLabel, CC_CALLBACK_1(HelloWorld::DoubleGame, this));
	auto labelExit = Label::createWithSystemFont("Exit", "Arial", 50);
	auto menuExit = MenuItemLabel::create(labelExit, [](Ref* pSender) {
		Director::getInstance()->end();
	});
	auto menu = Menu::create(menuSingleItem, menuDoubleItem,menuExit, NULL);
	menu->alignItemsVertically();
	addChild(menu);

	return true;
}

void HelloWorld::SingleGame(Ref* pSender)
{
	auto sc = GameScene::createScene();
	auto scene = TransitionFade::create(1.0f, sc);
	Director::getInstance()->replaceScene(scene);
}
void HelloWorld::DoubleGame(Ref* pSender)
{
	/*auto bg = Sprite::create("background");
	bg->setScale(0.5);
	bg->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2);
	addChild(bg);*/
	auto sc = OnlineGameScene::createScene();
	auto scene = TransitionFade::create(1.0f, sc);
	Director::getInstance()->replaceScene(scene);
	/*auto labelMakeServer = Label::create("Make Server", "Arial", 40);
	auto menuMakeServer = MenuItemLabel::create(labelMakeServer, CC_CALLBACK_1(HelloWorld::makeServer, this));
	auto labelMakeClient = Label::create("makeClient", "Arial", 40);
	auto menuMakeClient = MenuItemLabel::create(labelMakeClient, CC_CALLBACK_1(HelloWorld::makeClient, this));
	auto labelAccept = Label::create("Accept", "Arial", 40);
	auto menuAccept = MenuItemLabel::create(labelAccept, CC_CALLBACK_1(HelloWorld::Accept, this));
	auto menu = Menu::create(menuMakeServer, menuMakeClient, menuAccept, NULL);
	menu->alignItemsVertically();
	addChild(menu);*/
}

////����������  
//void HelloWorld::makeServer(Ref* pSender)
//{
//	this->_server = new SockServer(9888);
//
//	if (!this->_server->isValidSocket())
//	{
//		log("server ERR");
//	}
//	else
//	{
//		log("server OK");
//	}
//}
//
//
////�����ͷ���  
//void HelloWorld::makeClient(Ref* spSender)
//{
//	this->_client = new SockClient(9888, "127.0.0.1");
//
//	if (!this->_client->isValidSocket())
//	{
//		log("Client ERR");
//	}
//	else
//	{
//		log("Client OK");
//	}
//}
//
//
////��������  
//void HelloWorld ::Accept(Ref* pSender)
//{
//	this->_client = this->_server->accept();
//
//	if (!this->_client->isValidSocket())
//	{
//		log("Accept ERR");
//	}
//	else
//	{
//		log("Accept OK");
//	}
//}
//
//
////����  
//void HelloWorld::Send(Ref* pSender)
//{
//	//������Ϣ  
//	int ret = this->_client->send("Hello", 6);
//	log("send: ret is %d", ret);
//}
//
//
////����  
//void HelloWorld::Recv(Ref* pSender)
//{
//	char buf[128];
//
//	//������Ϣ  
//	int ret = this->_client->recv(buf, sizeof(buf));
//	log("recv buf is %s", buf);
//}

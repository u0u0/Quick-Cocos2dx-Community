#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "cocos2d.h"
#include "lua_module_register.h"
#include "xxtea/xxtea.h"

// extra lua module
#include "cocos2dx_extra.h"
#include "lua_extensions/lua_extensions_more.h"
#include "luabinding/cocos2dx_extra_luabinding.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "luabinding/cocos2dx_extra_ios_iap_luabinding.h"
#endif


USING_NS_CC;
using namespace std;

static void quick_module_register(lua_State *L)
{
    luaopen_lua_extensions_more(L);

    lua_getglobal(L, "_G");
    if (lua_istable(L, -1))//stack:...,_G,
    {
        register_all_quick_manual(L);
        // extra
        luaopen_cocos2dx_extra_luabinding(L);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        luaopen_cocos2dx_extra_ios_iap_luabinding(L);
#endif
    }
    lua_pop(L, 1);
}

//
AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

    GLView::setGLContextAttrs(glContextAttrs);
}

static void decoder(Data &data)
{
    unsigned char sign[] = "Xt";
    unsigned char key[] = "aaa";
    
    // decrypt XXTEA
    if (!data.isNull()) {
        bool isEncoder = false;
        unsigned char *buf = data.getBytes();
        ssize_t size = data.getSize();
        ssize_t len = strlen((char *)sign);
        if (size <= len) {
            return;
        }
        
        for (int i = 0; i < len; ++i) {
            isEncoder = buf[i] == sign[i];
            if (!isEncoder) {
                break;
            }
        }
        
        if (isEncoder) {
            xxtea_long newLen = 0;
            unsigned char* buffer = xxtea_decrypt(buf + len,
                                                  (xxtea_long)(size - len),
                                                  (unsigned char*)key,
                                                  (xxtea_long)strlen((char *)key),
                                                  &newLen);
            data.clear();
            data.fastSet(buffer, newLen);
        }
    }
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();    
    if(!glview) {
        string title = "__PROJECT_COCOS_NAME__";
        glview = cocos2d::GLViewImpl::create(title.c_str());
        director->setOpenGLView(glview);
        director->startAnimation();
    }
   
    auto engine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);
    lua_State* L = engine->getLuaStack()->getLuaState();
    lua_module_register(L);

    // use Quick-Cocos2d-X
    quick_module_register(L);
    
    // resource decode, include game.zip
    //FileUtils::getInstance()->setFileDataDecoder(decoder);
#if 0
    LuaStack* stack = engine->getLuaStack();
    // use luajit bytecode package
#ifdef CC_TARGET_OS_IPHONE
    if (sizeof(long) == 4) {
        stack->loadChunksFromZIP("res/game32.zip");
    } else {
        stack->loadChunksFromZIP("res/game64.zip");
    }
#else
    // android, mac, win32, etc
    stack->loadChunksFromZIP("res/game32.zip");
#endif
    stack->executeString("require 'main'");
#else // #if 0
    // use discrete files
    engine->executeScriptFile("src/main.lua");
#endif

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
    Director::getInstance()->pause();

    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("APP_ENTER_BACKGROUND_EVENT");
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->resume();
    Director::getInstance()->startAnimation();

    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("APP_ENTER_FOREGROUND_EVENT");
}

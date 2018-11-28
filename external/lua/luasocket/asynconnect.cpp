// Created by u0u0 on 2018/10/19.

#include <string.h>
#include <thread>

// for cpp call c
#ifdef __cplusplus
extern "C" {
#endif

#include "lua.h"
#include "auxiliar.h"
#include "inet.h"
#include "tcp.h"
    
#ifdef __cplusplus
}
#endif

// first connect to server may block main thread, new a thread to void UI blocked
static void connectThread(p_tcp tcp, const char *address, const char *port)
{
    struct addrinfo connecthints;
    const char *err;
    memset(&connecthints, 0, sizeof(connecthints));
    connecthints.ai_socktype = SOCK_STREAM;
    /* make sure we try to connect only to the same family */
    connecthints.ai_family = tcp->family;
    timeout_markstart(&tcp->tm);
    err = inet_tryconnect(&tcp->sock, &tcp->family, address, port,
                          &tcp->tm, &connecthints);
    tcp->asynConnStat = 2;// DONE first connect
}

extern "C" int asyn_connect(lua_State *L); // for c call cpp
int asyn_connect(lua_State *L)
{
    const char *address = luaL_checkstring(L, 2);
    const char *port = luaL_checkstring(L, 3);
    p_tcp tcp = (p_tcp) auxiliar_checkgroup(L, "tcp{any}", 1);
    if (tcp->asynConnStat == 0) {
        // start thread
        tcp->asynConnStat = 1;// first connecting
        auxiliar_setclass(L, "tcp{client}", 1);
        std::thread th(connectThread, tcp, address, port);
        th.detach();//exit from main thread, auto exit
    }
    if (tcp->asynConnStat < 2) {
        lua_pushnil(L);
        lua_pushstring(L, "Operation already in progress");
        return 2;
    }
    
    // sync connect for check status
    struct addrinfo connecthints;
    const char *err;
    memset(&connecthints, 0, sizeof(connecthints));
    connecthints.ai_socktype = SOCK_STREAM;
    /* make sure we try to connect only to the same family */
    connecthints.ai_family = tcp->family;
    timeout_markstart(&tcp->tm);
    err = inet_tryconnect(&tcp->sock, &tcp->family, address, port,
                          &tcp->tm, &connecthints);
    tcp->asynConnStat = 2;// DONE first connect
    /* have to set the class even if it failed due to non-blocking connects */
    auxiliar_setclass(L, "tcp{client}", 1);
    if (err) {
        lua_pushnil(L);
        lua_pushstring(L, err);
        return 2;
    }
    lua_pushnumber(L, 1);
    return 1;
}

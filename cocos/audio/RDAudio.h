// Copyright 2016 KeNan Liu
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __RDAudio_H__
#define __RDAudio_H__

#include <queue>
#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>

#include "platform/CCPlatformConfig.h"
#include "base/CCRef.h"

#include "alext.h"

typedef void (*AudioCallback)(int funcID, ALuint bufferID);

class CC_DLL RDAudio : public cocos2d::Ref
{
public:
    RDAudio();
    virtual ~RDAudio();
    
    static RDAudio *getInstance();
    static void destroyInstance();
    
    void pause();
    void resume();
    void loadFileAsyn(const char *filename, int funcID, AudioCallback cb);
private:
    struct AsyncStruct {
    public:
        AsyncStruct(const char *fn, int fid, AudioCallback func)
        : filename(fn)
        , cb(func)
        , funcID(fid)
        , pcmData(NULL)
        , channels(0)
        , rate(0)
        , size(0)
        {
        }
        ~AsyncStruct()
        {
            if (pcmData) free(pcmData);
        };
        std::string filename;
        AudioCallback cb;
        int funcID;
        unsigned char *pcmData;
        int channels;
        int rate;
        int size;
    };
    
    void init();
    void threadLoop();
    void scheduleLoop(float);
    void waitForQuit();
 
    static RDAudio *s_instant;
    // OpenAL
    ALCdevice *_device = NULL;
    ALCcontext *_context = NULL;
    // thread relative
    bool _needQuit;
    int _asyncRefCount;
    std::thread *_thread = NULL;
    std::queue<AsyncStruct *> _inQueue;
    std::queue<AsyncStruct *> _outQueue;
    std::mutex _inMutex;
    std::mutex _outMutex;
    std::condition_variable _inCondition;
};

#endif // __RDAudio_H__

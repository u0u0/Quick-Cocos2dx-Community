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

#ifndef __RDAudioOgg_H__
#define __RDAudioOgg_H__

#include "platform/CCFileUtils.h"
#include "cocos/audio/RDAudio.h"

#ifdef __cplusplus
extern "C" {
#endif

int decodeOgg(cocos2d::Data *data,
              unsigned char **pcmData,
              int32_t *pcmChannels,
              int32_t *pcmRate,
              int *pcmSize);

#ifdef __cplusplus
}
#endif

#endif // __RDAudioOgg_H__

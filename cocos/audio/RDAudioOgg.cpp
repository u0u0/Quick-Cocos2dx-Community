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

#include <stdlib.h>
#include <memory.h>

#include "RDAudioOgg.h"
#include "vorbis/vorbisfile.h"

typedef struct ogg_buffer
{
    unsigned char *curPtr;
    unsigned char *filePtr;
    size_t fileSize;
} ogg_buffer;

static size_t readOgg(void *dst, size_t size1, size_t size2, void *fh)
{
    ogg_buffer *of = (ogg_buffer *)fh;
    size_t len = size1 * size2;
    if (of->curPtr + len > of->filePtr + of->fileSize) {
        len = of->filePtr + of->fileSize - of->curPtr;
    }
    memcpy(dst, of->curPtr, len);
    of->curPtr += len;
    return len;
}

static int seekOgg(void *fh, ogg_int64_t to, int type)
{
    ogg_buffer *of = (ogg_buffer *)fh;
    
    switch(type) {
        case SEEK_CUR:
            of->curPtr += to;
            break;
        case SEEK_END:
            of->curPtr = of->filePtr + of->fileSize - to;
            break;
        case SEEK_SET:
            of->curPtr = of->filePtr + to;
            break;
        default:
            return -1;
    }
    if (of->curPtr < of->filePtr) {
        of->curPtr = of->filePtr;
        return -1;
    }
    if (of->curPtr > of->filePtr + of->fileSize) {
        of->curPtr = of->filePtr + of->fileSize;
        return -1;
    }
    return 0;
}

static int closeOgg(void *fh)
{
    return 0;
}

static long tellOgg(void *fh)
{
    ogg_buffer *of = (ogg_buffer *)fh;
    return (of->curPtr - of->filePtr);
}

int decodeOgg(unsigned char *oggData,
              int oggSize,
              unsigned char **pcmData,
              int *pcmChannels,
              int *pcmRate,
              int *pcmSize)
{
    ov_callbacks callbacks;
    ogg_buffer buffer;
    buffer.curPtr = buffer.filePtr = oggData;
    buffer.fileSize = oggSize;
    
    OggVorbis_File ov;
    memset(&ov, 0, sizeof(OggVorbis_File));
    
    callbacks.read_func = readOgg;
    callbacks.seek_func = seekOgg;
    callbacks.close_func = closeOgg;
    callbacks.tell_func = tellOgg;
    
    int ret = ov_open_callbacks((void *)&buffer, &ov, NULL, -1, callbacks);
    if (ret != 0) {
        return -1;
    }
    vorbis_info *vi = ov_info(&ov, -1);
    if (!vi) {
        ov_clear(&ov);
        return -1;
    }
    
    *pcmSize = (int)(ov_pcm_total(&ov, -1) * vi->channels * 2);
    *pcmData = (unsigned char *)malloc(*pcmSize);
    *pcmChannels = vi->channels;
    *pcmRate = (int)(vi->rate);
    
    int size = 0;
    int section = 0;
    long status = 0;
    while (size < *pcmSize) {
        status = ov_read(&ov, (char *)(*pcmData + size), *pcmSize - size, 0, 2, 1, &section);
        if (status > 0) {
            size += status;
        } else if (status < 0) {
            ov_clear(&ov);
            return -1;
        } else {
            break;
        }
    }
    if (size == 0) {
        ov_clear(&ov);
        return -1;
    }
    ov_clear(&ov);
    return 0;
}

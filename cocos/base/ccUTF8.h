/****************************************************************************
 Copyright (c) 2014 cocos2d-x.org
 Copyright (c) 2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __cocos2dx__ccUTF8__
#define __cocos2dx__ccUTF8__

#include "platform/CCPlatformMacros.h"
#include <vector>
#include <string>
#include <sstream>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include "platform/android/jni/JniHelper.h"
#endif

NS_CC_BEGIN

namespace StringUtils {
    
template<typename T>
std::string toString(T arg)
{
	std::stringstream ss;
	ss << arg;
	return ss.str();
}

std::string CC_DLL format(const char* format, ...) CC_FORMAT_PRINTF(1, 2);

/**
 *  @brief Converts utf8 string to utf16 string
 *  @param utf8 The utf8 string to be converted
 *  @param outUtf16 The output utf16 string
 *  @return true if succeed, otherwise false
 *  @note Please check the return value before using \p outUtf16
 *  e.g.
 *  @code
 *    std::u16string utf16;
 *    bool ret = StringUtils::UTF8ToUTF16("你好hello", utf16);
 *    if (ret) {
 *        do_some_thing_with_utf16(utf16);
 *    }
 *  @endcode
 */
CC_DLL bool UTF8ToUTF16(const std::string& utf8, std::u16string& outUtf16);

/**
 *  @brief Converts utf16 string to utf8 string
 *  @param utf16 The utf16 string to be converted
 *  @param outUtf8 The output utf8 string
 *  @return true if succeed, otherwise false
 *  @note Please check the return value before using \p outUtf8
 *  e.g.
 *  @code
 *    std::string utf8;
 *    bool ret = StringUtils::UTF16ToUTF8(u"\u4f60\u597d", utf16);
 *    if (ret) {
 *        do_some_thing_with_utf8(utf8);
 *    }
 *  @endcode
 */
CC_DLL bool UTF16ToUTF8(const std::u16string& utf16, std::string& outUtf8);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
/**
*  @brief convert jstring to utf8 std::string,  same function with env->getStringUTFChars. 
*         because getStringUTFChars can not pass special emoticon
*  @param env   The JNI Env
*  @param srcjStr The jstring which want to convert
*  @param ret   True if the conversion succeeds and the ret pointer isn't null
*  @returns the result of utf8 string
*/
CC_DLL std::string getStringUTFCharsJNI(JNIEnv* env, jstring srcjStr, bool* ret = nullptr);

/**
*  @brief create a jstring with utf8 std::string, same function with env->newStringUTF
*         because newStringUTF can not convert special emoticon
*  @param env   The JNI Env
*  @param srcjStr The std::string which want to convert
*  @param ret     True if the conversion succeeds and the ret pointer isn't null
*  @returns the result of jstring,the jstring need to DeleteLocalRef(jstring);
*/
CC_DLL jstring newStringUTFJNI(JNIEnv* env, const std::string& utf8Str, bool* ret = nullptr);
#endif

/**
 *  @brief Trims the unicode spaces at the end of char16_t vector
 */
CC_DLL void trimUTF16Vector(std::vector<char16_t>& str);

/**
 *  @brief Whether the character is a whitespace character.
 *
 *  @param ch    the unicode character
 *  @returns     whether the character is a white space character.
 *
 *  @see http://en.wikipedia.org/wiki/Whitespace_character#Unicode
 *
 */
CC_DLL bool isUnicodeSpace(char16_t ch);

/**
 *  @brief Whether the character is a Chinese/Japanese/Korean character.
 *
 *  @param ch    the unicode character
 *  @returns     whether the character is a Chinese character.
 *
 *  @see http://www.searchtb.com/2012/04/chinese_encode.html
 *  @see http://tieba.baidu.com/p/748765987
 *
 */
CC_DLL bool isCJKUnicode(char16_t ch);

/**
 *  @brief Returns the length of the string in characters.
 *
 *  @param utf8 an UTF-8 encoded string.
 *  @returns the length of the string in characters
 */
CC_DLL long getCharacterCountInUTF8String(const std::string& utf8);

/**
 *  @brief Gets the index of the last character that is not equal to the character given.
 *
 *  @param str   the string to be searched.
 *  @param c     the character to be searched for.
 *
 *  @returns the index of the last character that is not \p c.
 *
 */
CC_DLL unsigned int getIndexOfLastNotChar16(const std::vector<char16_t>& str, char16_t c);

/**
 *  @brief Gets char16_t vector from a given utf16 string
 */
CC_DLL std::vector<char16_t> getChar16VectorFromUTF16String(const std::u16string& utf16);

} // namespace StringUtils {

NS_CC_END

#endif /* defined(__cocos2dx__ccUTF8__) */

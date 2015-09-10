#include "cocos2d.h"
#include "xxtea.h"

NS_CC_BEGIN

static std::string xxteaKey = "2dxLua";
static ssize_t xxteaKeyLen = xxteaKey.length();
static std::string xxteaSign = "XXTEA";
static ssize_t xxteaSignLen = xxteaSign.length();

extern Data xxtea_file_get_data(const std::string& filename)
{
	Data data = FileUtils::getInstance()->getDataFromFile(filename);

    if (!data.isNull())
    {
        bool isXXTEA = true;
		unsigned char *buf = data.getBytes();
		ssize_t size = data.getSize();
        for (int i = 0; isXXTEA && i<xxteaSignLen && i<size; ++i)
        {
            isXXTEA = buf[i] == xxteaSign[i];
        }

        if (isXXTEA)
        { // decrypt XXTEA
            xxtea_long len = 0;
			unsigned char* buffer = xxtea_decrypt(
                                   buf + xxteaSignLen,
                                   (xxtea_long)size - (xxtea_long)xxteaSignLen,
                                   (unsigned char*)xxteaKey.c_str(),
                                   (xxtea_long)xxteaKeyLen,
                                   &len);
			data.clear();
			data.fastSet(buffer, len);
        }
    }

	return data;
}

NS_CC_END
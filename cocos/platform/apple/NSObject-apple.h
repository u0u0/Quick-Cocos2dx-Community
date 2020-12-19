#import <Foundation/Foundation.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL isObjHasSelector(id obj, NSString *selStr);
id invokeObjSelector(id obj, NSString *selStr, NSArray *objects);

#ifdef __cplusplus
}
#endif

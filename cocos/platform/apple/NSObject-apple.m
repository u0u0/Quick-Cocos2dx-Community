#import "NSObject-apple.h"
#import <objc/runtime.h>

@implementation NSObject (SelectorExtension)

BOOL isObjHasSelector(id obj, NSString *selStr) {
    unsigned int methodCount = 0;
    Method *methodList = class_copyMethodList([obj class],&methodCount);
    
    BOOL result = NO;
    for (int i = 0; i < methodCount; i++) {
        Method temp = methodList[i];
        const char *selectorName = sel_getName(method_getName(temp));
        NSString *tempSelectorString = [NSString stringWithUTF8String:selectorName];
        if ([tempSelectorString isEqualToString:selStr]) {
            result = YES;
            break;
        }
    }
    free(methodList);
    return result;
}

id invokeObjSelector(id obj, NSString *selStr, NSArray *objects) {
    SEL sel = NSSelectorFromString(selStr);
    NSMethodSignature *sign = [obj methodSignatureForSelector:sel];
    NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:sign];
    [invocation setTarget:obj];
    [invocation setSelector:sel];
    
    NSUInteger i = 1;
    if (objects && objects.count) {
        for (id object in objects) {
            id tempObject = object;
            [invocation setArgument:&tempObject atIndex:++i];
        }
    }
    [invocation invoke];
    
    if (sign.methodReturnLength) {
        id value;
        [invocation getReturnValue:&value];
        return value;
    }
    return nil;
}

@end

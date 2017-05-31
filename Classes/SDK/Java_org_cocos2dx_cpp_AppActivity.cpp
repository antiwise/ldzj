#include <stdlib.h>
#include "SDKManager.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>

extern "C" {

	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_payResultJnI(JNIEnv*  env, jobject thiz, jint paySuccess, jint type)
	{
		bool paySuccess2 = (paySuccess == 1 ? true : false);
		SDKManager().GetBuyResult(paySuccess2,type);
	}
}
#endif // ANDROID

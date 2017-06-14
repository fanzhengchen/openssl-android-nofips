//
// Created by fanzhengchen on 6/14/17.
//

#include "WebSocketManager.h"


extern "C"
JNIEXPORT void JNICALL
Java_com_example_WebSocket_connect(JNIEnv *env, jobject instance, jstring uri_) {
    const char *uri = env->GetStringUTFChars(uri_, 0);

    // TODO

    env->ReleaseStringUTFChars(uri_, uri);
}
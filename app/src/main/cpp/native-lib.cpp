#include <jni.h>
#include <string>
#include "uWS/uWS.h"
#include <unistd.h>
#include <chrono>
#include <android/log.h>
#include <assert.h>

extern "C"
{
#include <pthread.h>
}

#define  LOG_TAG "fuck"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

const int THREADS = 5;
uWS::Hub hub;

static JavaVM *jvm;
static pthread_t pthread;


void *connect_ws(void *) {
    LOGI("try to connect");
    hub.connect("ws://192.168.3.21:20000/webSocket", (void *) 1);
    hub.run();
    LOGI("after run");
    return NULL;
}

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = -1;

    hub.onConnection([](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req) {
        LOGI("connected");
    });

    hub.onError([](void *user) {
        LOGE("%s", "on error");
    });

    hub.onDisconnection(
            [](uWS::WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
                LOGE("disconnection");
            });

    hub.getDefaultGroup<uWS::CLIENT>().startAutoPing(2000, "");

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }

    result = JNI_VERSION_1_4;

    pthread_create(&pthread, NULL, connect_ws, NULL);

    return result;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_mark_com_websocket_1openssl_1libuv_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    return env->NewStringUTF(hello.c_str());
}

#include <jni.h>
#include <string>
#include "uWS/uWS.h"
#include <android/log.h>

char CLOSE_MESSAGE[] = "socket close";

#define  LOG_TAG __FILE__
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

uWS::Hub hub;

static JavaVM *jvm;
static uv_thread_t uv_thread;
const char *ws_uri;
static uv_loop_t *uvLoop;
static uv_work_t ws_conn_t;
static uv_idle_t idle_t;
static uWS::WebSocket<uWS::CLIENT> *webSocket;

void connect_ws(uv_idle_t *handler) {
    LOGI("%d try to connect %d", __LINE__, uv_thread);
    hub.connect(ws_uri, (void *) 1);
    hub.getDefaultGroup<uWS::CLIENT>().startAutoPing(2000, "");
    hub.run();
    LOGI("after run");
}

void uv_init(void *) {

    LOGE("%d uv_init", __LINE__);
    uvLoop = uv_loop_new();

    uv_idle_init(uvLoop, &idle_t);

    uv_idle_start(&idle_t, connect_ws);

    uv_run(uvLoop, UV_RUN_DEFAULT);
}

extern "C"
JNIEXPORT void JNICALL
Java_mark_com_websocket_1openssl_1libuv_WebSocketClient_send__Ljava_lang_String_2(JNIEnv *env,
                                                                                  jobject instance,
                                                                                  jstring message_) {
    const char *message = env->GetStringUTFChars(message_, 0);

    if (webSocket != nullptr) {
//        webSocket->se
        webSocket->send(message, uWS::OpCode::TEXT);
    }

    env->ReleaseStringUTFChars(message_, message);
}

JNIEXPORT void JNICALL
Java_mark_com_websocket_1openssl_1libuv_WebSocketClient_send__Lbyte_3_093_2(JNIEnv *env,
                                                                            jobject instance,
                                                                            jbyteArray bytes_) {
    jbyte *bytes = env->GetByteArrayElements(bytes_, NULL);

    env->ReleaseByteArrayElements(bytes_, bytes, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_mark_com_websocket_1openssl_1libuv_WebSocketClient_disconnect(JNIEnv *env, jobject instance) {

    // TODO
    hub.getDefaultGroup<uWS::CLIENT>().close(0, CLOSE_MESSAGE, sizeof(CLOSE_MESSAGE));
}

extern "C"
JNIEXPORT void JNICALL
Java_mark_com_websocket_1openssl_1libuv_WebSocketClient_connect(JNIEnv *env, jobject instance,
                                                                jstring uri_) {
    ws_uri = env->GetStringUTFChars(uri_, 0);

    LOGE("%d connection init", __LINE__);

    size_t len = env->GetStringLength(uri_);


    LOGI("%d %s %zu %d", __LINE__, ws_uri, len, uv_thread);

    uv_thread_create(&uv_thread, uv_init, NULL);

    env->ReleaseStringUTFChars(uri_, ws_uri);
}

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = JNI_VERSION_1_4;


    hub.onConnection([](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req) {
        LOGI("%d connected", __LINE__);
        webSocket = ws;
    });

    hub.onError([](void *user) {
        LOGE("%d on error", __LINE__);
        hub.getDefaultGroup<uWS::CLIENT>().close(0, CLOSE_MESSAGE, sizeof(CLOSE_MESSAGE));
    });

    hub.onDisconnection(
            [](uWS::WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
                LOGE("%d disconnection", __LINE__);
                hub.getDefaultGroup<uWS::CLIENT>().close(0, CLOSE_MESSAGE, sizeof(CLOSE_MESSAGE));
            });


    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }


    LOGI("jni on load");
    hub.onMessage(
            [](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode) {
                LOGI("%d %s %d", __LINE__, message, opCode);
            });


    return result;
}


extern "C"
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
}


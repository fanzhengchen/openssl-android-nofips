#include <jni.h>
#include <string>
#include "uWS/uWS.h"
#include <android/log.h>

char CLOSE_MESSAGE[] = "socket close";

#define  LOG_TAG __FILE__
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

uWS::Hub hub;

static JavaVM *javaVM;
static JNIEnv *jniEnv;
static jobject jobj;
static uv_thread_t uv_thread;
const char *ws_uri;
static uv_loop_t *uvLoop;
static uv_work_t ws_conn_t;
static uv_idle_t idle_t;
static uWS::WebSocket<uWS::CLIENT> *webSocket;
static uv_async_t uv_async;
uv_work_t uv_work;


void do_work(uv_work_t *handle) {
    hub.run();
}

void after_work(uv_work_t *handle, int status) {
    LOGI("%d after work", __LINE__);
}

void run_hun_in_queue(void *) {
    uvLoop = uv_loop_new();
    uv_queue_work(uvLoop, &uv_work, do_work, after_work);
    uv_run(uvLoop, UV_RUN_ONCE);
}


void cb_to_jvm() {
    JNIEnv *env;
//    javaVM->GetEnv((void **)&env, JNI_VERSION_1_4);

    javaVM->AttachCurrentThread(&env, NULL);

    jclass clazz = env->GetObjectClass(jobj);

    jmethodID methodID = env->GetMethodID(clazz, "onTextMessage", "(Ljava/lang/String;)V");

}

extern "C"
JNIEXPORT void JNICALL
Java_mark_com_websocket_1openssl_1libuv_WebSocketClient_sendBytes(JNIEnv *env, jobject instance,
                                                                  jbyteArray bytes_) {
    jbyte *bytes = env->GetByteArrayElements(bytes_, NULL);

    // TODO
    const char *data = (char *) bytes;

    if (webSocket != nullptr) {
        LOGI("%d send binary data: %s", __LINE__, data);

        webSocket->send(data, uWS::OpCode::BINARY);
    }


    env->ReleaseByteArrayElements(bytes_, bytes, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_mark_com_websocket_1openssl_1libuv_WebSocketClient_send__Ljava_lang_String_2(JNIEnv *env,
                                                                                  jobject instance,
                                                                                  jstring message_) {
    const char *message = env->GetStringUTFChars(message_, 0);

    LOGI("%d send message: webSocket null? %d", __LINE__, webSocket == nullptr);
    if (webSocket != nullptr) {
        webSocket->send(message, uWS::OpCode::TEXT);
    }

    env->ReleaseStringUTFChars(message_, message);
}

extern "C"
JNIEXPORT void JNICALL
Java_mark_com_websocket_1openssl_1libuv_WebSocketClient_disconnect(JNIEnv *env, jobject instance) {

    hub.getDefaultGroup<uWS::CLIENT>().terminate();
}

extern "C"
JNIEXPORT void JNICALL
Java_mark_com_websocket_1openssl_1libuv_WebSocketClient_connect(JNIEnv *env, jobject instance,
                                                                jstring uri_) {

    env->GetJavaVM(&javaVM);
    jobj = env->NewGlobalRef(instance);

    LOGE("%d connection init", __LINE__);

    size_t len = env->GetStringLength(uri_);

    LOGI("%d %s %zu %ld", __LINE__, ws_uri, len, uv_thread);

    if (webSocket == nullptr) {
        delete uvLoop;
        hub.connect(ws_uri, (void *) 1);
        uv_thread_create(&uv_thread, run_hun_in_queue, NULL);
    }

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
    });

    hub.onDisconnection(
            [](uWS::WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
                LOGE("%d disconnection", __LINE__);
                hub.getDefaultGroup<uWS::CLIENT>().close();
                webSocket = nullptr;

                //(0, CLOSE_MESSAGE, sizeof(CLOSE_MESSAGE));
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
    delete uvLoop;
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return;
    }

    if (env != NULL) {
        env->DeleteGlobalRef(jobj);


    }
}


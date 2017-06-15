//
// Created by fanzhengchen on 6/14/17.
//

#include "WebSocketManager.h"
#include "uv.h"

WebSocketManager *webSocketManager = nullptr;

uv_loop_t *uv_loop;
uv_work_t uv_work;
uv_thread_t uv_thread;

void uv_work_end(uv_work_t *handler, int status) {
    LOGI("%d uv work end", __LINE__);
}

void uv_work_on(uv_work_t *handler) {
    LOGI("%d uv work on", __LINE__);
    webSocketManager->getHub()->run();
}

void uv_init(void *) {
    LOGI("%d uv init", __LINE__);
    uv_run(uv_loop, UV_RUN_DEFAULT);

}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_WebSocket_connect(JNIEnv *env, jobject instance, jstring uri_) {
    const char *uri = env->GetStringUTFChars(uri_, 0);

    if (webSocketManager != nullptr && !webSocketManager->isConnected()) {
        LOGI("%d connect %s", __LINE__, uri);
        webSocketManager->getHub()->connect(uri);
        uv_thread_create(&uv_thread, uv_init, NULL);
        uv_queue_work(uv_loop, &uv_work, uv_work_on, uv_work_end);
    }
    env->ReleaseStringUTFChars(uri_, uri);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_WebSocket_close(JNIEnv *env, jobject instance) {

    if (webSocketManager != nullptr) {
        char message[] = "close socket";
        webSocketManager->getHub()->getDefaultGroup<uWS::CLIENT>().close(1000, message,
                                                                         sizeof(message));
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_WebSocket_sendPlainText(JNIEnv *env, jobject instance, jstring text_) {
    const char *text = env->GetStringUTFChars(text_, 0);

    if (webSocketManager != nullptr) {
        webSocketManager->sendPlainText(text);
    }

    env->ReleaseStringUTFChars(text_, text);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_WebSocket_sendBytes(JNIEnv *env, jobject instance, jbyteArray bytes_) {
    jbyte *bytes = env->GetByteArrayElements(bytes_, NULL);

    if (webSocketManager != nullptr) {
        const char *data = (const char *) bytes;
        webSocketManager->sendBytes(data);
    }

    env->ReleaseByteArrayElements(bytes_, bytes, 0);
}


extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {


    LOGI("jni on load");
    webSocketManager = new WebSocketManager();

    LOGI("%d websocketManager", __LINE__);
    webSocketManager->getHub()->onConnection(
            [](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest request) {
                LOGI("%d connection", __LINE__);
                webSocketManager->setWebSocket(ws);
            });


    webSocketManager->getHub()->onError([](void *user) {
        LOGI("%d on error", __LINE__);
    });

    webSocketManager->getHub()->onDisconnection(
            [](uWS::WebSocket<uWS::CLIENT> *ws, int code, char *message, size_t length) {
                LOGI("%d on disconnection", __LINE__);
                webSocketManager->setWebSocket(nullptr);
            });

    webSocketManager->getHub()->onMessage(
            [](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode code) {
                LOGI("%d on message: %s", __LINE__, message);
            });

    webSocketManager->getHub()->onPing(
            [](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length) {

            });

    uv_loop = uv_loop_new();
//
    uv_thread_create(&uv_thread, uv_init, NULL);

    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
    delete (webSocketManager);
}

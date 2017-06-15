//
// Created by fanzhengchen on 6/14/17.
//

#include "WebSocketManager.h"
#include "uv.h"

WebSocketManager *webSocketManager = nullptr;

uv_thread_t uv_thread;

void uv_init(void *) {
    LOGI("%d uv init", __LINE__);


    LOGI("%d websocketManager", __LINE__);

    webSocketManager->getHub()->run();

}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_WebSocket_connect(JNIEnv *env, jobject instance, jstring uri_) {
    const char *uri = env->GetStringUTFChars(uri_, 0);


    if (webSocketManager != nullptr && !webSocketManager->isConnected()) {
        webSocketManager->getHub()->connect(uri);
        webSocketManager->getHub()->getDefaultGroup<uWS::CLIENT>().startAutoPing(2000, "");
        uv_thread_create(&uv_thread, uv_init, NULL);
    }


    env->ReleaseStringUTFChars(uri_, uri);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_WebSocket_close(JNIEnv *env, jobject instance) {

    if (webSocketManager != nullptr) {
//        webSocketManager->getHub()->getDefaultGroup<uWS::CLIENT>().close();
        webSocketManager->close();
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
                webSocketManager->getHub()->getDefaultGroup<uWS::CLIENT>().close(1000, message,
                                                                                 sizeof(message));
            });

    webSocketManager->getHub()->onMessage(
            [](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode code) {
                LOGI("%d on message: %s", __LINE__, message);
            });

    webSocketManager->getHub()->onPing(
            [](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length) {
                LOGI("%d on ping pong ", __LINE__);
            });
//    uv_thread_create(&uv_thread, uv_init, NULL);

    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
    if (webSocketManager != nullptr) {
        webSocketManager->getHub()->getDefaultGroup<uWS::CLIENT>().close();
        delete (webSocketManager);
    }

}

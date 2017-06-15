//
// Created by mark on 17-6-12.
//

#ifndef OPENSSL_ANDROID_NOFIPS_WEBSOCKETMANAGER_H
#define OPENSSL_ANDROID_NOFIPS_WEBSOCKETMANAGER_H

#include "jni.h"
#include "uWS/Hub.h"
#include "uWS/uWS.h"
#include <android/log.h>

#define  LOG_TAG __FILE__
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

class WebSocketManager {
private:
    uWS::Hub *pHub;
    uWS::WebSocket<uWS::CLIENT> *webSocket;
    uv_work_t uv_work;
    uv_thread_t uv_thread;
    uv_loop_t uv_loop;

public:
    ~WebSocketManager();

    WebSocketManager();

    uWS::Hub *getHub();

    void setWebSocket(uWS::WebSocket<uWS::CLIENT> *ws);

    void sendPlainText(const char *);

    void sendBytes(const char *);

    bool isConnected();

    void close();
};


#endif //OPENSSL_ANDROID_NOFIPS_WEBSOCKETMANAGER_H

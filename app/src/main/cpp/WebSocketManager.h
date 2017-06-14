//
// Created by mark on 17-6-12.
//

#ifndef OPENSSL_ANDROID_NOFIPS_WEBSOCKETMANAGER_H
#define OPENSSL_ANDROID_NOFIPS_WEBSOCKETMANAGER_H

#include "uWS.h"
#include "jni.h"
#include "uWS/Hub.h"


class WebSocketManager {
private:
    uWS::Hub *hub;
    uWS::WebSocket<uWS::CLIENT> *webSocket;

    std::function<void(uWS::WebSocket<uWS::CLIENT> *, uWS::HttpRequest)> connectionHandler;

public:
    ~WebSocketManager();

    WebSocketManager();

    uWS::Hub *getHub();

    void setWebSocket(uWS::WebSocket<uWS::CLIENT> *ws);

    void sendPlainText(const char *);

    void sendBytes(const char *);
};


#endif //OPENSSL_ANDROID_NOFIPS_WEBSOCKETMANAGER_H

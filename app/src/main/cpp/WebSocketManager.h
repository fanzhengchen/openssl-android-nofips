//
// Created by mark on 17-6-12.
//

#ifndef OPENSSL_ANDROID_NOFIPS_WEBSOCKETMANAGER_H
#define OPENSSL_ANDROID_NOFIPS_WEBSOCKETMANAGER_H

#include "uWS.h"
#include "jni.h"
#include "uWS/Hub.h"

template<bool isServer>
class WebSocketManager {
private:
    uWS::Hub *hub;
    uWS::WebSocket<isServer> *webSocket;
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

    void onConnection(
            std::function<void(uWS::WebSocket<isServer> *, uWS::HttpRequest)> connectionHandler);

    void
    onDisconnection(std::function<void(uWS::WebSocket<isServer> *, int, char *, size_t)> handler);

    void onError(std::function<void(typename uWS::Group::errorType)> handler);

    void onPing(std::function<void(uWS::WebSocket<isServer> *, char *, size_t)> hanlder);

    void
    onMessage(std::function<void(uWS::WebSocket<isServer> *, char *, size_t, uWS::OpCode)> handler);

    void onTransfer(std::function<void(uWS::WebSocket<isServer> *)> handler);

    void connect(std::string uri, void *user, std::map<std::string, std::string> extraHeaders,
                 int timeoutMs, uWS::Group<uWS::CLIENT> *eh);

    void close(int code, char *message, size_t length);

    void run();

    void uv_run_loop();
};


#endif //OPENSSL_ANDROID_NOFIPS_WEBSOCKETMANAGER_H

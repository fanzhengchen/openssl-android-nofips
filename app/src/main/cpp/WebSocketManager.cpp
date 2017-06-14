//
// Created by mark on 17-6-12.
//

#include "WebSocketManager.h"

WebSocketManager::~WebSocketManager() {

    if (hub != nullptr) {
        hub->getDefaultGroup<uWS::CLIENT>().close();
    }
    delete (webSocket);
    delete (hub);
}

WebSocketManager::WebSocketManager() {
    hub = new uWS::Hub();
}

void WebSocketManager::setWebSocket(uWS::WebSocket<uWS::CLIENT> *ws) {
    webSocket = ws;
}

uWS::Hub *WebSocketManager::getHub() {
    return hub;
}

void WebSocketManager::connect(std::string uri, void *user,
                               std::map<std::string, std::string> extraHeaders, int timeoutMs,
                               uWS::Group<uWS::CLIENT> *eh) {
    if (hub) {
        hub->connect(uri, user, extraHeaders, timeoutMs, eh);
    }
}

void WebSocketManager::sendBytes(const char *data) {
    if (webSocket) {
        webSocket->send(data, uWS::BINARY);
    }
}

void WebSocketManager::sendPlainText(const char *data) {
    if (webSocket) {
        webSocket->send(data, uWS::TEXT);
    }
}

template<bool isServer>
void WebSocketManager::onConnection(
        std::function<void(uWS::WebSocket<isServer> *ws, uWS::HttpRequest req)> connectionHandler) {
    if (hub) {
        hub->onConnection(connectionHandler);
    }
}

template<bool isServer>
void WebSocketManager::onDisconnection(
        std::function<void(uWS::WebSocket<isServer> *ws, int, char *, size_t)> handler) {
    if (hub) {
        hub->onDisconnection(handler);
    }
}

void WebSocketManager::onError(std::function<void(typename uWS::Group::errorType)> handler) {
    if (hub) {
        hub->onError(handler);
    }
}

template<bool isServer>
void WebSocketManager::onPing(
        std::function<void(uWS::WebSocket<isServer> *ws, char *, size_t)> hanlder) {
    if (hub) {
        hub->onPing(hanlder);
    }
}

template<bool isServer>
void WebSocketManager::onMessage(
        std::function<void(uWS::WebSocket<isServer> *, char *, size_t, uWS::OpCode)> handler) {
    if (hub) {
        hub->onMessage(handler);
    }
}

template<bool isServer>
void WebSocketManager::onTransfer(std::function<void(uWS::WebSocket<isServer> *)> handler) {
    if (hub) {
        hub->onTransfer(handler);
    }
}

template<bool isServer>
void WebSocketManager::close(int code, char *message, size_t length) {
    if (hub) {
        hub->getDefaultGroup<isServer>().close(code, message, length);
    }
}

void WebSocketManager::run() {
    uv_thread_create(&uv_thread, uv_run_loop, NULL);
}

void WebSocketManager::uv_run_loop() {
    if (hub) {

    }
}
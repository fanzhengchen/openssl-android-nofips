//
// Created by mark on 17-6-12.
//

#include "WebSocketManager.h"

WebSocketManager::~WebSocketManager() {

    if (pHub != nullptr) {
        pHub->getDefaultGroup<uWS::CLIENT>().close();
    }
    delete (webSocket);
    delete (pHub);

//    uv_loop_close(&uv_loop);
    uv_loop_delete(&uv_loop);
}

WebSocketManager::WebSocketManager() {
    pHub = new uWS::Hub;
    webSocket = nullptr;
}

void WebSocketManager::setWebSocket(uWS::WebSocket<uWS::CLIENT> *ws) {
    webSocket = ws;
}

uWS::Hub *WebSocketManager::getHub() {
    return pHub;
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

bool WebSocketManager::isConnected() {
    return webSocket != nullptr;
}

void WebSocketManager::close() {
    if (webSocket != nullptr) {
        webSocket->close();
    }
}
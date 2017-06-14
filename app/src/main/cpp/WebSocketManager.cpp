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


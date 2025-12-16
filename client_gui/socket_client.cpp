// MIT License
// Copyright (c) 2025 Chat System Project

#include "socket_client.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

SocketClient::SocketClient(QObject* parent)
    : QObject(parent), socket_fd_(-1), connected_(false), should_stop_(false) {
}

SocketClient::~SocketClient() {
    disconnect();
}

bool SocketClient::connect_to_server(const std::string& host, int port) {
    if (connected_) {
        emit error_occurred("متصل بالفعل!");
        return false;
    }
    
    // إنشاء الـ socket
    socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd_ < 0) {
        emit error_occurred("فشل إنشاء الـ socket");
        return false;
    }
    
    // إعداد العنوان
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0) {
        emit error_occurred("عنوان الـ Server غير صحيح");
        close(socket_fd_);
        socket_fd_ = -1;
        return false;
    }
    
    // الاتصال بالـ Server
    if (::connect(socket_fd_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        emit error_occurred("فشل الاتصال بالـ Server");
        close(socket_fd_);
        socket_fd_ = -1;
        return false;
    }
    
    connected_ = true;
    should_stop_ = false;
    
    // بدء thread لاستقبال الرسائل (مهم: في الـ background)
    receive_thread_ = std::thread(&SocketClient::receive_loop, this);
    
    return true;
}

void SocketClient::disconnect() {
    if (!connected_) return;
    
    connected_ = false;
    should_stop_ = true;
    
    if (socket_fd_ >= 0) {
        // **مهم جداً:** shutdown() قبل close() عشان recv() يوقف
        shutdown(socket_fd_, SHUT_RDWR);
        close(socket_fd_);
        socket_fd_ = -1;
    }
    
    // **عدم استخدام join() هنا عشان الـ GUI ميفريزش**
    if (receive_thread_.joinable()) {
        receive_thread_.detach();
    }
    
    emit disconnected();
}

bool SocketClient::send_message(const std::string& text) {
    if (!connected_ || text.empty()) return false;
    
    Message msg(0, text);  // الـ Server هيحدد الـ ID
    std::string data = msg.serialize();
    
    ssize_t sent = send(socket_fd_, data.c_str(), data.size(), MSG_NOSIGNAL);
    
    if (sent < 0 || static_cast<size_t>(sent) != data.size()) {
        emit error_occurred("فشل إرسال الرسالة");
        disconnect();
        return false;
    }
    
    return true;
}

void SocketClient::receive_loop() {
    char buffer[4096];
    
    while (!should_stop_ && connected_) {
        ssize_t bytes_read = recv(socket_fd_, buffer, sizeof(buffer), 0);
        
        if (bytes_read <= 0) {
            if (bytes_read == 0 && !should_stop_) {
                emit error_occurred("الـ Server قطع الاتصال");
            }
            break;
        }
        
        Message msg;
        if (msg.deserialize(buffer, bytes_read)) {
            // إرسال signal للـ GUI (thread-safe)
            emit message_received(msg.client_id, QString::fromStdString(msg.text));
        }
    }
    
    if (connected_) {
        disconnect();
    }
}

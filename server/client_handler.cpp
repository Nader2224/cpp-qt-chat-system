// MIT License
// Copyright (c) 2025 Chat System Project

#include "client_handler.h"
#include "chat_server.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <cstring>

// تطبيق الـ broadcast function
void broadcast_message(const Message& msg, int exclude_client_id, ChatServer* server) {
    auto clients = server->get_clients();  // نسخة thread-safe
    
    for (auto& client : clients) {
        // عدم إرسال الرسالة للـ sender نفسه
        if (client->get_id() != exclude_client_id && client->is_connected()) {
            client->send_message(msg);
        }
    }
}

ClientHandler::ClientHandler(int socket_fd, int client_id, ChatServer* server)
    : socket_fd_(socket_fd), client_id_(client_id), server_(server), connected_(true) {
}

ClientHandler::~ClientHandler() {
    disconnect();
}

void ClientHandler::start() {
    // بدء thread لقراءة الرسائل
    receive_thread_ = std::thread(&ClientHandler::message_loop, this);
    
    // إرسال رسالة ترحيب
    Message welcome(0, "مرحباً! أنت Client #" + std::to_string(client_id_));
    send_message(welcome);
}

void ClientHandler::disconnect() {
    if (!connected_.exchange(false)) {
        return;  // Already disconnected
    }
    
    if (socket_fd_ >= 0) {
        shutdown(socket_fd_, SHUT_RDWR);  // مهم: بيوقف recv() من الانتظار
        close(socket_fd_);
        socket_fd_ = -1;
    }
    
    if (receive_thread_.joinable()) {
        receive_thread_.detach();  // عدم استخدام join() عشان مانوقفش الـ Server
    }
    
    server_->remove_client(client_id_);
}

bool ClientHandler::send_message(const Message& msg) {
    if (!connected_) return false;
    
    std::string data = msg.serialize();
    ssize_t sent = send(socket_fd_, data.c_str(), data.size(), MSG_NOSIGNAL);
    
    if (sent < 0 || static_cast<size_t>(sent) != data.size()) {
        std::cerr << "⚠️ فشل إرسال رسالة للـ Client #" << client_id_ << std::endl;
        disconnect();
        return false;
    }
    
    return true;
}

void ClientHandler::message_loop() {
    char buffer[4096];
    
    while (connected_) {
        ssize_t bytes_read = recv(socket_fd_, buffer, sizeof(buffer), 0);
        
        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                std::cout << "📤 Client #" << client_id_ << " قطع الاتصال بشكل طبيعي" << std::endl;
            } else {
                std::cerr << "⚠️ خطأ في قراءة بيانات Client #" << client_id_ << std::endl;
            }
            disconnect();
            break;
        }
        
        // معالجة الرسالة
        Message msg;
        if (msg.deserialize(buffer, bytes_read)) {
            msg.client_id = client_id_;  // التأكد من الـ ID الصحيح
            
            std::cout << "📩 Client #" << client_id_ << ": " << msg.text << std::endl;
            
            // **مهم جداً:** إرسال الرسالة لكل الـ Clients ماعدا المرسل
            broadcast_message(msg, client_id_, server_);
        } else {
            std::cerr << "⚠️ رسالة غير صالحة من Client #" << client_id_ << std::endl;
        }
    }
}

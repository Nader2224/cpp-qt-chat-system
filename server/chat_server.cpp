// MIT License
// Copyright (c) 2025 Chat System Project

#include "chat_server.h"
#include "client_handler.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <algorithm>

ChatServer::ChatServer(int port) 
    : port_(port), server_socket_(-1), running_(false), next_client_id_(1) {
}

ChatServer::~ChatServer() {
    stop();
}

bool ChatServer::start() {
    // إنشاء الـ socket
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ < 0) {
        std::cerr << "❌ فشل إنشاء الـ socket" << std::endl;
        return false;
    }
    
    // السماح بإعادة استخدام الـ port
    int opt = 1;
    if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "⚠️ تحذير: setsockopt فشل" << std::endl;
    }
    
    // ربط الـ socket بالـ port
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);
    
    if (bind(server_socket_, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "❌ فشل ربط الـ socket بالـ port " << port_ << std::endl;
        close(server_socket_);
        return false;
    }
    
    // الاستماع للـ connections
    if (listen(server_socket_, 10) < 0) {
        std::cerr << "❌ فشل الاستماع على الـ port" << std::endl;
        close(server_socket_);
        return false;
    }
    
    running_ = true;
    std::cout << "✅ الـ Server شغال على الـ port " << port_ << std::endl;
    
    // بدء الـ accept loop في thread منفصل
    std::thread accept_thread(&ChatServer::accept_loop, this);
    accept_thread.detach();
    
    return true;
}

void ChatServer::stop() {
    if (!running_) return;
    
    running_ = false;
    
    // إغلاق الـ server socket
    if (server_socket_ >= 0) {
        shutdown(server_socket_, SHUT_RDWR);
        close(server_socket_);
        server_socket_ = -1;
    }
    
    // قطع اتصال كل الـ Clients
    std::lock_guard<std::mutex> lock(clients_mutex_);
    for (auto& client : clients_) {
        client->disconnect();
    }
    clients_.clear();
    
    std::cout << "🛑 الـ Server اتوقف" << std::endl;
}

void ChatServer::accept_loop() {
    while (running_) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        int client_socket = accept(server_socket_, (struct sockaddr*)&client_addr, &client_len);
        
        if (client_socket < 0) {
            if (running_) {
                std::cerr << "⚠️ خطأ في قبول اتصال جديد" << std::endl;
            }
            continue;
        }
        
        if (!running_) {
            close(client_socket);
            break;
        }
        
        // إنشاء handler للـ Client الجديد
        int client_id = next_client_id_++;
        auto client = std::make_shared<ClientHandler>(client_socket, client_id, this);
        
        {
            std::lock_guard<std::mutex> lock(clients_mutex_);
            clients_.push_back(client);
        }
        
        client->start();
        std::cout << "➕ Client #" << client_id << " اتصل (عدد الـ Clients: " 
                  << clients_.size() << ")" << std::endl;
    }
}

void ChatServer::remove_client(int client_id) {
    std::lock_guard<std::mutex> lock(clients_mutex_);
    
    auto it = std::remove_if(clients_.begin(), clients_.end(),
        [client_id](const std::shared_ptr<ClientHandler>& client) {
            return client->get_id() == client_id;
        });
    
    if (it != clients_.end()) {
        clients_.erase(it, clients_.end());
        std::cout << "➖ Client #" << client_id << " قطع الاتصال (متبقي: " 
                  << clients_.size() << ")" << std::endl;
    }
}

std::vector<std::shared_ptr<ClientHandler>> ChatServer::get_clients() {
    std::lock_guard<std::mutex> lock(clients_mutex_);
    return clients_;  // نسخة thread-safe
}

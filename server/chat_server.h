// MIT License
// Copyright (c) 2025 Chat System Project

#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <vector>
#include <memory>
#include <mutex>
#include <atomic>

class ClientHandler;

/**
 * @brief الـ Chat Server الرئيسي
 * 
 * بيستقبل الـ Clients الجداد ويدير الاتصالات
 * Thread-safe وبيتعامل مع multiple clients بأمان
 */
class ChatServer {
public:
    ChatServer(int port = 8080);
    ~ChatServer();
    
    /**
     * @brief بدء الـ Server والاستماع للـ Clients
     * @return true لو الـ Server اشتغل بنجاح
     */
    bool start();
    
    /**
     * @brief إيقاف الـ Server بشكل آمن
     */
    void stop();
    
    /**
     * @brief إزالة client من القائمة (بيتنادي لما Client يقطع الاتصال)
     * @param client_id معرّف الـ Client
     */
    void remove_client(int client_id);
    
    /**
     * @brief الحصول على قائمة الـ Clients المتصلين
     * @return نسخة thread-safe من القائمة
     */
    std::vector<std::shared_ptr<ClientHandler>> get_clients();

private:
    void accept_loop();  // الـ Loop اللي بيستقبل connections جديدة
    
    int port_;
    int server_socket_;
    std::atomic<bool> running_;
    std::atomic<int> next_client_id_;
    
    std::vector<std::shared_ptr<ClientHandler>> clients_;
    std::mutex clients_mutex_;  // حماية الـ clients list
};

#endif // CHAT_SERVER_H

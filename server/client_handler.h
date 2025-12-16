// MIT License
// Copyright (c) 2025 Chat System Project

#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "message.h"
#include <atomic>
#include <thread>

class ChatServer;

/**
 * @brief معالج لكل Client متصل
 * 
 * كل Client عنده thread خاص بيه بيقرأ الرسائل
 * Thread-safe ومتزامن مع باقي الـ Clients
 */
class ClientHandler {
public:
    ClientHandler(int socket_fd, int client_id, ChatServer* server);
    ~ClientHandler();
    
    /**
     * @brief بدء معالجة الرسائل من الـ Client
     */
    void start();
    
    /**
     * @brief قطع اتصال الـ Client بشكل آمن
     */
    void disconnect();
    
    /**
     * @brief إرسال رسالة للـ Client
     * @param msg الرسالة المراد إرسالها
     * @return true لو الإرسال نجح
     */
    bool send_message(const Message& msg);
    
    /**
     * @brief الحصول على معرّف الـ Client
     */
    int get_id() const { return client_id_; }
    
    /**
     * @brief هل الـ Client لسه متصل؟
     */
    bool is_connected() const { return connected_; }

private:
    void message_loop();  // الـ Loop اللي بيقرأ الرسائل
    
    int socket_fd_;
    int client_id_;
    ChatServer* server_;
    std::atomic<bool> connected_;
    std::thread receive_thread_;
};

// Forward declaration للـ broadcast function
extern void broadcast_message(const Message& msg, int exclude_client_id, ChatServer* server);

#endif // CLIENT_HANDLER_H

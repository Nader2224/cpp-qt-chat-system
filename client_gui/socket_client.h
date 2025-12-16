// MIT License
// Copyright (c) 2025 Chat System Project

#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include <QObject>
#include <atomic>
#include <thread>
#include "message.h"

/**
 * @brief الـ Client اللي بيتصل بالـ Server
 * 
 * Thread-safe ومتكامل مع Qt signals/slots
 * بيشتغل في background thread عشان الـ GUI ميفريزش
 */
class SocketClient : public QObject {
    Q_OBJECT

public:
    explicit SocketClient(QObject* parent = nullptr);
    ~SocketClient();
    
    /**
     * @brief الاتصال بالـ Server
     * @param host عنوان الـ Server (مثلاً "127.0.0.1")
     * @param port رقم الـ Port (مثلاً 8080)
     * @return true لو الاتصال نجح
     */
    bool connect_to_server(const std::string& host, int port);
    
    /**
     * @brief قطع الاتصال بشكل آمن (بدون freeze)
     */
    void disconnect();
    
    /**
     * @brief إرسال رسالة للـ Server
     * @param text محتوى الرسالة
     * @return true لو الإرسال نجح
     */
    bool send_message(const std::string& text);
    
    /**
     * @brief هل متصل حالياً؟
     */
    bool is_connected() const { return connected_; }

signals:
    /**
     * @brief إشارة لما رسالة جديدة توصل
     * @param client_id معرّف المرسل
     * @param text محتوى الرسالة
     */
    void message_received(int client_id, QString text);
    
    /**
     * @brief إشارة لما الاتصال يقطع
     */
    void disconnected();
    
    /**
     * @brief إشارة لما يحصل خطأ
     * @param error_msg رسالة الخطأ
     */
    void error_occurred(QString error_msg);

private:
    void receive_loop();  // الـ Loop اللي بيقرأ الرسائل في background
    
    int socket_fd_;
    std::atomic<bool> connected_;
    std::atomic<bool> should_stop_;
    std::thread receive_thread_;
};

#endif // SOCKET_CLIENT_H

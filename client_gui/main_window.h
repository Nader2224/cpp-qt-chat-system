// MIT License
// Copyright (c) 2025 Chat System Project

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <memory>

class SocketClient;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief واجهة الـ Chat الرسومية
 * 
 * بسيطة وسهلة الاستخدام، متجاوبة ومافيهاش freeze
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void on_sendButton_clicked();
    void on_messageInput_returnPressed();
    
    // Slots للـ Socket signals
    void on_message_received(int client_id, QString text);
    void on_disconnected();
    void on_error_occurred(QString error_msg);

private:
    void update_ui_state(bool connected);
    void append_message(const QString& message, const QString& color = "#000000");
    
    Ui::MainWindow *ui;
    std::unique_ptr<SocketClient> client_;
};

#endif // MAIN_WINDOW_H

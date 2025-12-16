// MIT License
// Copyright (c) 2025 Chat System Project

#include "main_window.h"
#include "ui_main_window.h"
#include "socket_client.h"
#include <QMessageBox>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client_(std::make_unique<SocketClient>(this))
{
    ui->setupUi(this);
    
    // ربط الـ signals بالـ slots
    connect(client_.get(), &SocketClient::message_received,
            this, &MainWindow::on_message_received);
    connect(client_.get(), &SocketClient::disconnected,
            this, &MainWindow::on_disconnected);
    connect(client_.get(), &SocketClient::error_occurred,
            this, &MainWindow::on_error_occurred);
    
    update_ui_state(false);
    
    // تعيين الـ focus على حقل الإدخال
    ui->messageInput->setFocus();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_connectButton_clicked() {
    QString host = ui->serverInput->text().trimmed();
    if (host.isEmpty()) {
        host = "127.0.0.1";
    }
    
    if (client_->connect_to_server(host.toStdString(), 8080)) {
        update_ui_state(true);
        append_message("✅ اتصلت بالـ Server بنجاح!", "#008000");
    } else {
        QMessageBox::warning(this, "خطأ", "فشل الاتصال بالـ Server");
    }
}

void MainWindow::on_disconnectButton_clicked() {
    client_->disconnect();
    // الـ UI هيتحدث في on_disconnected()
}

void MainWindow::on_sendButton_clicked() {
    QString text = ui->messageInput->text().trimmed();
    if (text.isEmpty()) return;
    
    if (client_->send_message(text.toStdString())) {
        append_message("أنا: " + text, "#0000FF");
        ui->messageInput->clear();
    } else {
        append_message("❌ فشل إرسال الرسالة", "#FF0000");
    }
}

void MainWindow::on_messageInput_returnPressed() {
    on_sendButton_clicked();
}

void MainWindow::on_message_received(int client_id, QString text) {
    QString sender = (client_id == 0) ? "Server" : "Client #" + QString::number(client_id);
    append_message(sender + ": " + text, "#000080");
}

void MainWindow::on_disconnected() {
    update_ui_state(false);
    append_message("⚠️ الاتصال قُطع", "#FF0000");
}

void MainWindow::on_error_occurred(QString error_msg) {
    append_message("❌ خطأ: " + error_msg, "#FF0000");
}

void MainWindow::update_ui_state(bool connected) {
    ui->connectButton->setEnabled(!connected);
    ui->disconnectButton->setEnabled(connected);
    ui->sendButton->setEnabled(connected);
    ui->messageInput->setEnabled(connected);
    ui->serverInput->setEnabled(!connected);
}

void MainWindow::append_message(const QString& message, const QString& color) {
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString html = QString("<span style='color:%1'>[%2] %3</span>")
                   .arg(color, timestamp, message);
    ui->chatDisplay->append(html);
}

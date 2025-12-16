// MIT License
// Copyright (c) 2025 Chat System Project

#include "chat_server.h"
#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>

std::atomic<bool> keep_running(true);

void signal_handler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cout << "\n🛑 إيقاف الـ Server..." << std::endl;
        keep_running = false;
    }
}

int main() {
    // التعامل مع Ctrl+C
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    
    std::cout << "🚀 بدء Chat Server..." << std::endl;
    
    ChatServer server(8080);
    
    if (!server.start()) {
        std::cerr << "❌ فشل بدء الـ Server" << std::endl;
        return 1;
    }
    
    // الانتظار حتى يتم الإيقاف
    while (keep_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    server.stop();
    
    return 0;
}

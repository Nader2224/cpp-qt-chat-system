// MIT License
// Copyright (c) 2025 Chat System Project
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <cstdint>

/**
 * @brief رسالة الـ Chat بين الـ Server والـ Clients
 * 
 * الـ Protocol بسيط:
 * [4 bytes: message_length][4 bytes: client_id][N bytes: message_text]
 */
struct Message {
    int32_t client_id;      // معرّف الـ Client (0 = server message)
    std::string text;       // محتوى الرسالة
    
    Message() : client_id(0) {}
    Message(int32_t id, const std::string& msg) : client_id(id), text(msg) {}
    
    /**
     * @brief تحويل الرسالة لـ bytes عشان نبعتها على الـ Socket
     * @return البيانات الخام (raw bytes)
     */
    std::string serialize() const;
    
    /**
     * @brief استخراج رسالة من الـ bytes اللي جاية
     * @param data البيانات الخام
     * @param bytes_read عدد الـ bytes اللي اتقرأت
     * @return true لو الرسالة كاملة واتقرأت صح
     */
    bool deserialize(const char* data, size_t bytes_read);
    
    /**
     * @brief حساب الحجم الكلي للرسالة (header + text)
     * @return الحجم بالـ bytes
     */
    size_t total_size() const;
};

#endif // MESSAGE_H

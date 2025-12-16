// MIT License
// Copyright (c) 2025 Chat System Project

#include "message.h"
#include <cstring>
#include <arpa/inet.h>  // for htonl/ntohl (network byte order)

std::string Message::serialize() const {
    // الـ Format: [length][client_id][text]
    int32_t text_len = static_cast<int32_t>(text.size());
    int32_t net_len = htonl(text_len);      // تحويل لـ network byte order
    int32_t net_id = htonl(client_id);
    
    std::string result;
    result.reserve(8 + text.size());
    
    // إضافة الـ length
    result.append(reinterpret_cast<const char*>(&net_len), sizeof(net_len));
    // إضافة الـ client_id
    result.append(reinterpret_cast<const char*>(&net_id), sizeof(net_id));
    // إضافة الـ text
    result.append(text);
    
    return result;
}

bool Message::deserialize(const char* data, size_t bytes_read) {
    // محتاجين على الأقل 8 bytes للـ header
    if (bytes_read < 8) {
        return false;
    }
    
    // قراءة الـ length
    int32_t net_len;
    std::memcpy(&net_len, data, sizeof(net_len));
    int32_t text_len = ntohl(net_len);
    
    // قراءة الـ client_id
    int32_t net_id;
    std::memcpy(&net_id, data + 4, sizeof(net_id));
    client_id = ntohl(net_id);
    
    // التأكد إن البيانات كاملة
    if (bytes_read < static_cast<size_t>(8 + text_len)) {
        return false;
    }
    
    // قراءة الـ text
    text.assign(data + 8, text_len);
    
    return true;
}

size_t Message::total_size() const {
    return 8 + text.size();  // 4 bytes length + 4 bytes id + text
}

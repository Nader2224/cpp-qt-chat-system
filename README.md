# 🚀 C++ Qt Chat System

نظام chat متقدم مبني بـ C++ و Qt، مع دعم كامل للـ threading و thread-safety.

## ✨ المميزات

- ✅ **Multi-Client Support**: يدعم عدد غير محدود من الـ Clients
- ✅ **Thread-Safe**: استخدام `std::mutex` و `std::atomic` بشكل صحيح
- ✅ **No Freeze**: الـ GUI متجاوبة بدون أي تعليق
- ✅ **Real-time Broadcasting**: الرسائل توصل فوراً لكل الـ Clients
- ✅ **Clean Architecture**: كود منظم وسهل القراءة
- ✅ **Production Ready**: معالجة الأخطاء بشكل احترافي

## 📁 هيكل المشروع

```
cpp-qt-chat-system/
├── protocol/          # Message protocol (shared between server & client)
├── server/            # Chat server implementation
├── client_gui/        # Qt-based GUI client
└── CMakeLists.txt     # Main build configuration
```

## 🛠️ المتطلبات

- **CMake** >= 3.16
- **C++17** compiler (GCC/Clang)
- **Qt5** (Widgets module)
- **POSIX Threads** (pthread)

### تثبيت المتطلبات على Ubuntu/Debian:

```bash
sudo apt update
sudo apt install build-essential cmake qt5-default libqt5widgets5
```

## 🚀 البناء والتشغيل

### 1. Build المشروع

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### 2. تشغيل الـ Server

```bash
./server/chat_server
```

الـ Server هيشتغل على Port **8080**

### 3. تشغيل الـ Client

في terminal جديد:

```bash
./client_gui/chat_client
```

ممكن تفتح أكثر من client في نفس الوقت للتجربة!

## 📝 كيفية الاستخدام

1. **شغّل الـ Server** أولاً
2. **شغّل Client** (أو أكثر)
3. اكتب عنوان الـ Server (`127.0.0.1` للتجربة المحلية)
4. اضغط **"اتصل"**
5. ابدأ المحادثة! 💬

## 🏗️ معلومات تقنية

### Thread Safety

- استخدام `std::mutex` لحماية الـ client list
- استخدام `std::atomic<bool>` للـ flags
- RAII lock guards (`std::lock_guard`)

### Disconnect Without Freeze

- استخدام `shutdown()` قبل `close()` عشان `recv()` يوقف
- استخدام `detach()` بدل `join()` في الـ GUI
- Thread-safe signal/slot connections

### Message Protocol

البروتوكول بسيط وفعّال:

```
[4 bytes: message_length][4 bytes: client_id][N bytes: message_text]
```

## 🧪 الاختبار

### اختبار Multiple Clients

1. شغّل الـ Server
2. افتح 3-4 clients
3. اتصل من كل client
4. ابعت رسالة من أي client
5. تأكد إنها ظهرت في كل الـ clients التانية

### اختبار Disconnect

1. اضغط زر "قطع الاتصال" في أي client
2. تأكد إن الـ GUI مافريزتش
3. تأكد إن الـ Server سجّل الانفصال

## 📜 الترخيص

MIT License - استخدم المشروع براحتك!

## 🤝 المساهمة

المساهمات مرحب بيها! افتح Issue أو Pull Request.

## 📧 التواصل

لو عندك أي سؤال أو اقتراح، افتح Issue على GitHub.

---

**Made with ❤️ using C++ and Qt**

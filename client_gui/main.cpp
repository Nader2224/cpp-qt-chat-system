// MIT License
// Copyright (c) 2025 Chat System Project

#include "main_window.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    MainWindow window;
    window.show();
    
    return app.exec();
}

#pragma once
#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H
#include "QObject"


class MyException : public std::exception
{
public:
    MyException(const QString& message) : m_message(message) {}

    const char* what() const noexcept override {
        return qPrintable(m_message);
    }

private:
    QString m_message;
};
#endif // MYEXCEPTION_H

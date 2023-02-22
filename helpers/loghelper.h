#ifndef LOGHELPER_H
#define LOGHELPER_H

#include <QString>

void logOutput(QtMsgType type, const QMessageLogContext& contex, const QString& msg);
void setupLog();
QString getLogPath();

#endif // LOGHELPER_H

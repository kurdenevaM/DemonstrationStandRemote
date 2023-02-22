#ifndef APPLICATIONCONSTANTS_H
#define APPLICATIONCONSTANTS_H

#include <QDir>

const int kLogLevel = QtDebugMsg;
const QString kLogPath = QString("%1/%2").arg(QDir::homePath()).arg("BIN_V_024_M_Remote/Logs/");

#endif // APPLICATIONCONSTANTS_H

#include "threaddependobject.h"

ThreadDependObject::ThreadDependObject(QObject* parent) : QObject(parent) {}

void ThreadDependObject::onStartProcess(QStringList args) { emit ready(startProcess(args)); }

void ThreadDependObject::onStartProcessWithoutArgs() { emit ready(startProcess(QStringList())); }

void ThreadDependObject::onQuitProcess() { emit ready(quitProcess()); }

QString ThreadDependObject::getLastErrorString() const { return _lastErrorString; }

void ThreadDependObject::clearLastErrorString() { _lastErrorString.clear(); }

#ifndef NONLOCKWAITER_H
#define NONLOCKWAITER_H

#include "architect/threaddependobject.h"
#include <functional>

class NonLockWaiter : public QObject
{
    Q_OBJECT
public:
    explicit NonLockWaiter(QObject* parent = nullptr);
    /**
     * @brief wait - ждём msec миллисекунд не блокируя поток (на QEventLoop) Лучше вызывать не меньше чем на 2 мс.
     * @param msecs - количество миллисекунд ожидания
     */
    static void wait(int msecs);

    /**
     * @brief waitReadyObject - неблокирующее поток ожидание выполнения слота в другом потоке
     * @param object - объект, который будет выполнять ту работу, завершение которой мы ожидаем
     * @param signalEmitFunc - функция, вызывающая сигнал, приявязаный к тому слоту, выполнение которого мы будем ждать
     * @param maxWaitingTime - максимальное время, которое мы ждём выполнения слота. После - бьём тревогу.
     * @param isReady - флаг, показывающий, действительно ли работа была правильно завершена (или вышли по таймеру
     * @return - статус, возвращённый объектом при вызове ready
     *
     * Типичное использование:
     * ThreadDependObject* workObject;
     * bool isOk;
     * connect(this, &ThisClass::startWork, workObject, &WorkObjectClass::workSlot);
     * NonLockWaiter::waitReadyObject(workObject, [&]() { emit startWork(); }, 1000, isOk);
     */
    static int waitReadyObject(ThreadDependObject* object, std::function<void()> signalEmitFunc, int maxWaitingTime,
                               bool& isReady, int waitingQuantum = 2);
};

#endif // NONLOCKWAITER_H

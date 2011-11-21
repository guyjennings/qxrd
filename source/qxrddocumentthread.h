#ifndef QXRDDOCUMENTTHREAD_H
#define QXRDDOCUMENTTHREAD_H

#include <QThread>
#include <QSharedPointer>

class QxrdDocumentThread : public QThread
{
    Q_OBJECT
public:
    explicit QxrdDocumentThread(QObject *parent = 0);

signals:

public slots:

};

typedef QSharedPointer<QxrdDocumentThread> QxrdDocumentThreadPtr;

#endif // QXRDDOCUMENTTHREAD_H

#ifndef QXRDIMAGECALCULATOR_H
#define QXRDIMAGECALCULATOR_H

#include <QtGui/QDockWidget>

namespace Ui {
    class QxrdImageCalculator;
}

#include "qxrddataprocessor.h"

class QxrdImageCalculator : public QDockWidget {
    Q_OBJECT
public:
    QxrdImageCalculator(QxrdDataProcessorPtr processor, QWidget *parent = 0);
    ~QxrdImageCalculator();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QxrdImageCalculator *m_ui;

    QxrdDataProcessorPtr  m_Processor;

    HEADER_IDENT("$Id: qxrdimagecalculator.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
};

#endif // QXRDIMAGECALCULATOR_H

/******************************************************************
*
*  $Log: qxrdimagecalculator.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.4  2010/06/07 20:17:42  jennings
*  Added more cvs log and ident stuff - removed qxrdspecserver.cpp & qxrdspecserver.h
*
*
*
*******************************************************************/

#ifndef QXRDPOWDERFITWIDGET_H
#define QXRDPOWDERFITWIDGET_H

#include <QDialog>
#include <QVector>
#include "ui_qxrdpowderfitwidget.h"
#include "qwt_double_rect.h"
#include "qwt_symbol.h"
#include "qxrddataprocessor-ptr.h"

class QxrdPowderFitWidget : public QDialog, public Ui::QxrdPowderFitWidget {
  Q_OBJECT
public:
  QxrdPowderFitWidget(QxrdDataProcessorWPtr proc, QWidget *parent = 0);
  ~QxrdPowderFitWidget();

protected:
  void changeEvent(QEvent *e);

public slots:

  void performFit();
  void cancelFit();
  void undoFit();

  void appendOutputString(QString msg);
  void removeGraphMarkers();
  void appendGraphMarker(int n, QwtDoublePoint pt);

protected:

protected:
  QxrdDataProcessorWPtr      m_Processor;
  QVector<QwtSymbol::Style>  m_SymbolStyles;
};

#endif // QXRDPOWDERFITWIDGET_H

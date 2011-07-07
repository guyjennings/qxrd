#ifndef QXRDPOWDERFITRESULTSPAGE_H
#define QXRDPOWDERFITRESULTSPAGE_H

#include "qxrdpowderfitrefinepage.h"

class QxrdPowderFitResultsPage : public QxrdPowderFitRefinePage
{
  Q_OBJECT
public:
  QxrdPowderFitResultsPage(QxrdDataProcessor *proc, QWidget *parent);

public slots:
  void selectResultsPage();
};

#endif // QXRDPOWDERFITRESULTSPAGE_H

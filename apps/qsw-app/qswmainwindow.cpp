#include "qswmainwindow.h"
#include "qcepplotwidgetsettings.h"

QswMainWindow::QswMainWindow(QString name) :
  inherited(name),
  m_Settings(),
  m_PlotCurves()
{

//  connect(m_ActionSave, &QAction::triggered, this, &QswMainWindow::doSave);
//  connect(m_ActionQuit, &QAction::triggered, this, &QswMainWindow::possiblyClose);
}

QswMainWindow::~QswMainWindow()
{
}

void QswMainWindow::initialize(QcepObjectWPtr parent)
{
  setupUi(this);

  inherited::initialize(parent);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  m_Settings = QcepPlotWidgetSettings::findPlotWidgetSettings(parent);

  m_Plot -> initialize(m_Settings);
  m_Plot -> addPlotCommandSpacer();
  m_Plot -> enableZooming();
}

void QswMainWindow::onDataAvailable(QVector<float> d)
{
  int nPts = d.count() - 4;

  QVector<double> x,y;

  for (int i=0; i<nPts; i++) {
    x.append(i);
    y.append(d.value(i));
  }

  QwtPlotCurve *pc = new QwtPlotCurve("data");

  pc->setSamples(x, y);

  pc->attach(m_Plot->plot());

  m_PlotCurves.append(pc);

  fadeoutPlotCurves();

  m_Plot->plot()->replot();
}

void QswMainWindow::fadeoutPlotCurves()
{
  const int MAX_PLOT_CURVES = 10;

  while (m_PlotCurves.length() > MAX_PLOT_CURVES) {
    QwtPlotCurve *pc = m_PlotCurves.takeFirst();

    if (pc) {
      pc->detach();

      delete pc;
    }
  }

  int nCurves = m_PlotCurves.length();
  int step    = 255/(MAX_PLOT_CURVES+1);

  for (int i=0; i<nCurves; i++) {
    QwtPlotCurve *pc = m_PlotCurves.value(i);

    if (pc) {
      int f = (nCurves-i)*step;
      pc->setPen(QColor(255, f, f));
    }
  }
}

void QswMainWindow::doSave()
{
}

void QswMainWindow::possiblyClose()
{
}

void QswMainWindow::saveExperiment()
{
}

void QswMainWindow::saveExperimentCopy()
{
}

void QswMainWindow::saveExperimentAs()
{
}

void QswMainWindow::doEditPreferences()
{
}

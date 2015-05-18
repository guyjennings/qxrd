#include "qxrdcalibrantdialog.h"
#include "ui_qxrdcalibrantdialog.h"
#include "qxrdcalibrantlibrarymodel.h"
#include "qxrdcalibrantdspacingsmodel.h"
#include "qxrdcalibrantlibrary.h"
#include "qxrdcalibrant.h"
#include "qxrdcenterfinder.h"

QxrdCalibrantDialog::QxrdCalibrantDialog(QxrdCalibrantLibraryPtr cal, QxrdCenterFinderWPtr cf, QWidget *parent) :
  QDockWidget(parent),
  m_CalibrantLibrary(cal),
  m_CenterFinder(cf)
{
  setupUi(this);

  if (m_CalibrantLibrary) {
    m_CalibrantLibraryModel = QxrdCalibrantLibraryModelPtr(
          new QxrdCalibrantLibraryModel(m_CalibrantLibrary));

    m_CalibrantDSpacingsModel = QxrdCalibrantDSpacingsModelPtr(
          new QxrdCalibrantDSpacingsModel(m_CalibrantLibrary, &m_CalibrantDSpacingsVector));

    m_CalibrantTableView -> setModel(m_CalibrantLibraryModel.data());
    m_CalibrantDSpacingsView -> setModel(m_CalibrantDSpacingsModel.data());

    m_CalibrantTableView->horizontalHeader()->setStretchLastSection(true);
    m_CalibrantDSpacingsView->horizontalHeader()->setStretchLastSection(true);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  m_CalibrantTableView->horizontalHeader()->sectionResizeMode(QHeaderView::ResizeToContents);
  m_CalibrantTableView->verticalHeader()->sectionResizeMode(QHeaderView::ResizeToContents);
  m_CalibrantDSpacingsView->horizontalHeader()->sectionResizeMode(QHeaderView::ResizeToContents);
  m_CalibrantDSpacingsView->verticalHeader()->sectionResizeMode(QHeaderView::ResizeToContents);
#else
  m_CalibrantTableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  m_CalibrantTableView->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  m_CalibrantDSpacingsView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  m_CalibrantDSpacingsView->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif
  }

  connect(m_CalibrantTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(onLibrarySelectionChanged(QItemSelection,QItemSelection)));
}

QxrdCalibrantDialog::~QxrdCalibrantDialog()
{
}

void QxrdCalibrantDialog::onLibrarySelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
  int row = selected.first().top();

  if (m_CalibrantLibrary) {
//    m_CalibrantLibrary->printMessage(tr("Row %1 selected").arg(row));

    QxrdCalibrantPtr cal = m_CalibrantLibrary->calibrant(row);

    if (cal) {
      QxrdCenterFinderPtr cf(m_CenterFinder);

      if (cf) {
        m_CalibrantDSpacingsVector = cal->dSpacings(cf->get_Energy());
      } else {
        m_CalibrantDSpacingsVector = cal->dSpacings(15000);
      }

      m_CalibrantDSpacingsModel -> everythingChanged(m_CalibrantDSpacingsVector.count());

//      m_CalibrantLibrary->printMessage(tr("%1 diffraction peaks").arg(m_CalibrantDSpacingsVector.count()));
    }
  }
}

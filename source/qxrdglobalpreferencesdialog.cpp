#include "qxrdglobalpreferencesdialog.h"
#include "ui_qxrdglobalpreferencesdialog.h"
#include "qxrdapplication.h"
#include "qxrdallocator.h"

QxrdGlobalPreferencesDialog::QxrdGlobalPreferencesDialog(QxrdApplication *app, QWidget *parent) :
  QDialog(parent),
  m_Application(app)
{
  setupUi(this);

  m_OpenDirectly -> setChecked(m_Application->get_OpenDirectly());
  m_RecentExperimentsSize -> setValue(m_Application->get_RecentExperimentsSize());

  int debugLevel = m_Application -> get_Debug();

  QxrdAllocatorPtr alloc(m_Application->allocator());

  if (alloc) {
    m_ReservedMemory32 -> setRange(500, 3000);
    m_ReservedMemory32 -> setValue(alloc->get_TotalBufferSizeMB32());
    m_ReservedMemory64 -> setRange(500, 60000);
    m_ReservedMemory64 -> setValue(alloc->get_TotalBufferSizeMB64());
    m_ExtraReservedMemory -> setRange(0, 500);
    m_ExtraReservedMemory -> setValue(alloc->get_Reserve());
  }

  m_FileBrowserLimit -> setValue(m_Application->get_FileBrowserLimit());

  m_MessageWindowLines -> setValue(m_Application->get_MessageWindowLines());
  m_UpdateIntervalMsec -> setValue(m_Application->get_UpdateIntervalMsec());

  setupDebugWidgets(debugLevel);
}

QxrdGlobalPreferencesDialog::~QxrdGlobalPreferencesDialog()
{
}

void QxrdGlobalPreferencesDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdGlobalPreferencesDialog::accept()
{
  int debugLevel = readDebugWidgets();

  m_Application -> set_OpenDirectly(m_OpenDirectly->isChecked());
  m_Application -> set_RecentExperimentsSize(m_RecentExperimentsSize->value());

  int bufferSize32 = m_ReservedMemory32 -> value();
  int bufferSize64 = m_ReservedMemory64 -> value();
  int extraReserve = m_ExtraReservedMemory -> value();


  m_Application -> set_Debug(debugLevel);

  QxrdAllocatorPtr alloc(m_Application->allocator());

  if (alloc) {
    alloc         -> set_TotalBufferSizeMB32(bufferSize32);
    alloc         -> set_TotalBufferSizeMB64(bufferSize64);
    alloc         -> set_Reserve(extraReserve);
  }

  m_Application -> set_FileBrowserLimit(m_FileBrowserLimit->value());
  m_Application -> set_MessageWindowLines(m_MessageWindowLines -> value());
  m_Application -> set_UpdateIntervalMsec(m_UpdateIntervalMsec -> value());

  QDialog::accept();
}

void QxrdGlobalPreferencesDialog::setupDebugWidgets(int dbg)
{
  QGridLayout *grid = new QGridLayout(m_DebugWidgets);

  int mask = 1;

  for (int i=0; gDebugStrings[i]; i++) {
    QCheckBox *cb = new QCheckBox(gDebugStrings[i]);
    cb->setChecked(dbg & mask);

    mask <<= 1;

    m_DebugWidgetList.append(cb);
  }

  int ndebug = m_DebugWidgetList.count();
  int ncol   = ndebug - ndebug/2;

  for (int i=0; gDebugStrings[i]; i++) {
    QCheckBox *cb = m_DebugWidgetList[i];

    if (i < ncol) {
      grid->addWidget(cb, i, 0);
    } else {
      grid->addWidget(cb, i - ncol, 1);
    }
  }
}

int QxrdGlobalPreferencesDialog::readDebugWidgets()
{
  int mask = 1;
  int newDbg = 0;

  for (int i=0; gDebugStrings[i]; i++) {
    if (m_DebugWidgetList[i]->isChecked()) {
      newDbg |= mask;
    }

    mask <<= 1;
  }

  return newDbg;
}

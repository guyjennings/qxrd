/******************************************************************
*
*  $Id: qxrdfilebrowser.h,v 1.1 2009/10/22 19:22:28 jennings Exp $
*
*******************************************************************/

#ifndef QXRDFILEBROWSER_H
#define QXRDFILEBROWSER_H

class QxrdDataProcessor;
class QDirModel;
class QFileDialog;

#include <QWidget>
#include "ui_qxrdfilebrowser.h"

class QxrdFileBrowser : public QWidget, public Ui::QxrdFileBrowser
{
public:
  QxrdFileBrowser(QxrdDataProcessor *processor, QWidget *parent=0);

private:
  QxrdDataProcessor *m_Processor;
  QDirModel         *m_Model;
  QFileDialog       *m_FileDialog;
};

#endif // QXRDFILEBROWSER_H

/******************************************************************
*
*  $Log: qxrdfilebrowser.h,v $
*  Revision 1.1  2009/10/22 19:22:28  jennings
*  Initial file browser
*
*
*******************************************************************/

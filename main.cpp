#include <QMainWindow>
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QMainWindow *w=NULL;

  w = new QMainWindow();
  w -> show();

  return a.exec();
}


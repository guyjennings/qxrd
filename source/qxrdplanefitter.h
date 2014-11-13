#ifndef QXRDPLANEFITTER_H
#define QXRDPLANEFITTER_H

class QxrdPlaneFitter
{
public:
  QxrdPlaneFitter();

  void   addPoint(double x, double y, double z);
  double value(double x, double y);

private:
  double s;
  double sx;
  double sy;
  double sxx;
  double syy;
  double sxy;
  double sz;
  double sxz;
  double syz;
};

#endif // QXRDPLANEFITTER_H

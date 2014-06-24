#ifndef QXRDPOINT4D_H
#define QXRDPOINT4D_H

class QxrdPoint4D
{
public:
  QxrdPoint4D();
  QxrdPoint4D(double x, double y, double z, double t);
  QxrdPoint4D(const QxrdPoint4D &p);

  double x() const;
  double y() const;
  double z() const;
  double t() const;

  double &rx();
  double &ry();
  double &rz();
  double &rt();

private:
  double m_X;
  double m_Y;
  double m_Z;
  double m_T;
};

inline QxrdPoint4D::QxrdPoint4D()
  : m_X(0),
    m_Y(0),
    m_Z(0),
    m_T(0)
{
}

inline QxrdPoint4D::QxrdPoint4D(double x, double y, double z, double t)
  : m_X(x),
    m_Y(y),
    m_Z(z),
    m_T(t)
{
}

inline QxrdPoint4D::QxrdPoint4D(const QxrdPoint4D &p)
  : m_X(p.x()),
    m_Y(p.y()),
    m_Z(p.z()),
    m_T(p.t())
{
}

inline double QxrdPoint4D::x() const
{
  return m_X;
}

inline double QxrdPoint4D::y() const
{
  return m_Y;
}

inline double QxrdPoint4D::z() const
{
  return m_Z;
}

inline double QxrdPoint4D::t() const
{
  return m_T;
}

inline double &QxrdPoint4D::rx()
{
  return m_X;
}

inline double &QxrdPoint4D::ry()
{
  return m_Y;
}

inline double &QxrdPoint4D::rz()
{
  return m_Z;
}

inline double &QxrdPoint4D::rt()
{
  return m_T;
}

#endif // QXRDPOINT4D_H

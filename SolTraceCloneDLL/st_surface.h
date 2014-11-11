#ifndef ST_SURFACE_H
#define ST_SURFACE_H

#include <memory>

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QStringList>
#include <QVector>

#include <Geom_BSplineSurface.hxx>
#include <Standard.hxx>
#include <Bnd_Box2d.hxx>

class ST_element;
class ST_elements_model;
class ST_surface_zernike;

// ST_surface: usage may seem a little strange. Sorry.
// Example:
// QTextStream ts; // ... initialized to a QFile eg.
// // Usually not the whole line but for example:
// QStringList tokens = ts.getline().split("\t");
// ST_surface * aSurface = new ST_surface::readSurface(tokens);
// aSurface->refresh(); // If surface parameters are defined in a file, read them.
// Handle(Geom_BSplineSurface) aBSurface = aSurface->makeSurface(aBox);
class ST_surface
{
public:
  enum ST_surface_type { ZERNIKE, VSHOT, POLY, CUBIC_SPLINE, FED, PARABOLIC,
                         SPHERICAL, HYPERBOLOID, FLAT, CONICAL, CYLINDER,
                         TORUS };
  static ST_surface_type stringToType(QString tag);
  static QString typeToString(ST_surface_type type);

  // External classes, use this instead of constructor. May return a subtype of ST_surface.
  static std::shared_ptr<ST_surface> readSurface(QStringList &tokens);
  virtual void refresh(const QDir &parent);

  // Always makes a flat surface. Cannot change it!
  ST_surface();

protected:
  ST_surface(QStringList &tokens);

public:
  void write(QTextStream &out) const;
  virtual Handle(Geom_BSplineSurface) makeSurface(const Bnd_Box2d &box, bool isSingleAxis) const;
  virtual double z(const double &x, const double &y) const;

protected:
  ST_surface_type type;
  double data[8];
  QString filename;

  friend class ST_element;
  friend class ST_elements_model;
};

class ST_surface_zernike : public ST_surface
{
public:
  ST_surface_zernike(QStringList &in);
  virtual void refresh(const QDir &parent);
  virtual double z(const double &x, const double &y) const;
protected:
  int N;
  QVector<QVector<double>> B;
};

class ST_surface_vshot : public ST_surface
{
public:
  ST_surface_vshot(QStringList &in);
  virtual void refresh(const QDir &parent);
  virtual double z(const double &x, const double &y) const;
protected:
  int N;
  QVector<QVector<double>> B;
};

class ST_surface_poly : public ST_surface
{
public:
  ST_surface_poly(QStringList &in);
  virtual void refresh(const QDir &parent);
  virtual double z(const double &x, const double &y) const;
protected:
  int N;
  QVector<double> C;
};

class ST_surface_cubic : public ST_surface
{
public:
  ST_surface_cubic(QStringList &in);
  virtual void refresh(const QDir &parent);
  virtual double z(const double &x, const double &y) const;
protected:
  int N;
  QVector<double> radius_i;
  QVector<double> height_i;
  double slope_1;
  double slope_N;
};

QTextStream &operator<<(QTextStream &out, const ST_surface &surface);

#endif // ST_SURFACE_H

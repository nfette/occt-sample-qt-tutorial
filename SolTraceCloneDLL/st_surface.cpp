#include "st_surface.h"

#include <functional>

#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include <TColStd_Array2OfReal.hxx>
#include <GeomAPI_PointsToBSplineSurface.hxx>
#include <Geom2dAPI_PointsToBSpline.hxx>

ST_surface::ST_surface_type ST_surface::stringToType(QString tag)
{
  tag = tag.toLower();
  if (tag == "m")
    return ZERNIKE;
  else if (tag == "v")
    return VSHOT;
  else if (tag == "r")
    return POLY;
  else if (tag == "i")
    return CUBIC_SPLINE;
  else if (tag == "fed")
    return FED;
  else if (tag == "p")
    return PARABOLIC;
  else if (tag == "s")
    return SPHERICAL;
  else if (tag == "o")
    return HYPERBOLOID;
  else if (tag == "f")
    return FLAT;
  else if (tag == "c")
    return CONICAL;
  else if (tag == "t")
    return CYLINDER;
  else if (tag == "d")
    return TORUS;
  else {
    QString msg = QString("Unrecognized surface tag: \"%1\"").arg(tag);
    throw std::invalid_argument(msg.toLatin1().data());
  }
}

QString ST_surface::typeToString(ST_surface_type aType)
{
  switch (aType) {
  case ZERNIKE:
    return "m";
  case VSHOT:
    return "v";
  case POLY:
    return "r";
  case CUBIC_SPLINE:
    return "i";
  case FED:
    return "fed";
  case PARABOLIC:
    return "p";
  case SPHERICAL:
    return "s";
  case HYPERBOLOID:
    return "o";
  case FLAT:
    return "f";
  case CONICAL:
    return "c";
  case CYLINDER:
    return "t";
  case TORUS:
    return "d";
  default:
    return "f";
  }
}

std::shared_ptr<ST_surface> ST_surface::readSurface(QStringList &tokens)
{
  switch (stringToType(tokens[0])) {
  case ZERNIKE:
    return std::make_shared<ST_surface_zernike>(tokens);
  case VSHOT:
    //return new ST_surface_vshot(tokens);
    throw std::invalid_argument("VSHOT surface conversion not defined.");
  case POLY:
    return std::make_shared<ST_surface_poly>(tokens);
  case CUBIC_SPLINE:
    return std::make_shared<ST_surface_cubic>(tokens);
  case FED:
    throw std::invalid_argument("Error: FED surface is future functionality!");
  case PARABOLIC:
  case SPHERICAL:
  case HYPERBOLOID:
  case FLAT:
  case CONICAL:
  case CYLINDER:
  case TORUS:
    return std::shared_ptr<ST_surface>(new ST_surface (tokens));
  default:
    throw std::invalid_argument("Error: No such surface tag!");
  }
}

ST_surface::ST_surface()
  : data()
{
  type = FLAT;
}

ST_surface::ST_surface(QStringList &tokens)
{
  QString aTag = tokens[0];
  type = stringToType(aTag);
  for (int i = 0; i < 8; i++) {
    data[i] = tokens[1 + i].toDouble();
  }
  filename = tokens[9];
  // Do we need to read the file? No, not until we are called to refresh().
}

void ST_surface::write(QTextStream &out) const
{
  QString sep("\t");
  out << typeToString(type) << sep;
  for (int i = 0; i < 8; i++) {
    out << data[i] << sep;
  }
  out << filename;
}

Handle(Geom_BSplineSurface) ST_surface::makeSurface(const Bnd_Box2d &box, bool isSingleAxis) const
{
  // TODO: make member variables and accessors for nu, nv.
  int nu = 50, nv = 50;
  TColStd_Array2OfReal ZPoints(1,nu+1, 1,nv+1);
  double X, Y;

  // Determine the bounds, slightly oversize.
  // TODO: WARNING: this may not be possible.
  double oversize = 1.2;
  double XminB, YminB, XmaxB, YmaxB;
  box.Get(XminB, YminB, XmaxB, YmaxB);
  double Xc = 0.5 * (XminB + XmaxB),
      Xsize = 0.5 * (XmaxB - XminB),
      Yc = 0.5 * (YminB + YmaxB),
      Ysize = 0.5 * (YmaxB - YminB);
  double Xmin = Xc - oversize * Xsize,
      Xmax = Xc + oversize * Xsize,
      Ymin = Yc - oversize * Ysize,
      Ymax = Yc + oversize * Ysize;
  double dX = (Xmax - Xmin) / nu;
  double dY = (Ymax - Ymin) / nv;

  int i, j;
  for (i = 0; i <= nu; i++) {
    X = Xmin + dX * i;
    for (j = 0; j <= nv; j++) {
      if (isSingleAxis)
        Y = 0;
      else
        Y = Ymin + dY * j;
      ZPoints(1+i, 1+j) = z(X,Y);
    }
  }

  // Bowl : make the NURBS surface
  Handle(Geom_BSplineSurface) aSurface = GeomAPI_PointsToBSplineSurface(ZPoints, Xmin, dX, Ymin, dY);
  return aSurface;
}

double ST_surface::z(const double &x, const double &y) const
{
  static double r2;
  r2 = x*x + y*y;
  switch (type) {
  case SPHERICAL:
  {
    static double c2;
    c2 = data[0] * data[0];
    if (r2 * c2 > 1)
      return data[0];
    else
      return data[0] * r2 / (1.0 + Sqrt(1.0 - data[0] * data[0] * r2));
  }
  case FLAT:
    return 0;
  case HYPERBOLOID:
  case PARABOLIC:
  {
    static double cx = data[0], cy = data[1];
    return 0.5 * (cx * x * x + cy * y * y);
  }
  case CONICAL:
  case CYLINDER:
  case TORUS:
    throw std::invalid_argument("Surface definition not yet implemented.");
  default:
    throw std::invalid_argument("Don't call ST_surface constructor directly.");
  }
}

// Null operation, to be implemented by descendants.
void ST_surface::refresh(const QDir &parent) { ; }

ST_surface_zernike::ST_surface_zernike(QStringList &in)
  : ST_surface(in)
{ }

void ST_surface_zernike::refresh(const QDir &parent)
{
  // Look first in the folder of the current file.
  // Then look in the "working folder".
  QString fullfile = parent.filePath(filename);

  QFile *file = new QFile(fullfile);
  if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    throw std::invalid_argument("Error with file I/O.");

  {
    QTextStream in(file);

    in >> N;
    B.resize(N+1);
    for (int i = 0; i <= N; i++)
    {
      B[i].resize(i+1);
      for (int j = 0; j <= i; j++)
      {
        in >> B[i][j];
      }
    }
  }

  file->close();
}

double ST_surface_zernike::z(const double &x, const double &y) const
{
  double z = 0;
  for (int i = 0; i <= N; i++)
  {
    for (int j = 0; j <= i; j++)
    {
      z += B[i][j] * pow(x, j) * pow(y, i-j);
    }
  }
  return z;
}

ST_surface_poly::ST_surface_poly(QStringList &in)
  : ST_surface(in)
{ }

void ST_surface_poly::refresh(const QDir &parent)
{
  // Look first in the folder of the current file.
  // Then look in the "working folder".
  QString fullfile = parent.filePath(filename);

  QFile *file = new QFile(fullfile);
  if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    throw std::invalid_argument("Error with file I/O.");

  {
    QTextStream in(file);

    in >> N;
    C.resize(N+1);
    for (int i = 0; i <= N; i++)
    {
      in >> C[i];
    }
  }
  file->close();
}

double ST_surface_poly::z(const double &x, const double &y) const
{
  double z = 0;
  double r2 = x * x + y * y;
  double r = std::sqrt(r2);
  for (int i = 0; i <= N; i++)
  {
    z += C[i] * pow(r, i);
  }
  return z;
}


ST_surface_cubic::ST_surface_cubic(QStringList &in)
  : ST_surface(in)
{ }

void ST_surface_cubic::refresh(const QDir &parent)
{
  // Look first in the folder of the current file.
  // Then look in the "working folder".
  QString fullfile = parent.filePath(filename);

  QFile *file = new QFile(fullfile);
  if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    throw std::invalid_argument("Error with file I/O.");

  {
    QTextStream in(file);

    in >> N;
    radius_i.resize(N+1);
    height_i.resize(N+1);
    for (int i = 0; i <= N; i++)
    {
      in >> radius_i[i] >> height_i[i];
    }
  }
  file->close();
}

double ST_surface_cubic::z(const double &x, const double &y) const
{
  double z = 0;
  for (int i = 0; i <= N; i++)
  {
    z += 0;
  }
  return z;
}

QTextStream &operator<<(QTextStream &out, const ST_surface &surface)
{
  surface.write(out);
  return out;
}

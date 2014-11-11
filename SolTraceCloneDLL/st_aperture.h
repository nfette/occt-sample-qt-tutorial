#ifndef ST_APERTURE_H
#define ST_APERTURE_H

#include <QStringList>
#include <QString>
#include <QTextStream>
#include <QVector>

#include <Bnd_Box2d.hxx>
#include <Geom2d_Curve.hxx>
#include <TopoDS_Wire.hxx>

class ST_element;
class ST_elements_model;

class ST_aperture
{
public:
  enum ST_aperture_type {CIRCULAR, HEXAGONAL, TRIANGULAR, RECTANGULAR,
                         SINGLE_AXIS_CURVATURE_SECTION, ANNULAR,
                         IRREGULAR_TRIANGLE, IRREGULAR_QUAD};
  static ST_aperture_type stringToType(QString tag);
  static QString typeToString(ST_aperture_type);

  ST_aperture();
  ST_aperture(const QStringList &tokens);
  ST_aperture(ST_aperture_type type, double (&data)[8]);

  void write(QTextStream &out) const;

  const ST_aperture_type & type() const;
  void makeBounds(QVector<Handle(Geom2d_Curve)> &wire, Bnd_Box2d &aCBox) const;

protected:

  ST_aperture_type myType;
  double data[8];

  friend class ST_element;
  friend class ST_elements_model;
};

QTextStream &operator<<(QTextStream &out, const ST_aperture &aperture);

#endif // ST_APERTURE_H

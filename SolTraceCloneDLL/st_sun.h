#ifndef ST_SUN_H
#define ST_SUN_H

#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QPair>

class ST_sun_model;

class ST_sun
{
public:
  enum ST_sun_shape {
    GAUSSIAN, PILLBOX, USER_DEFINED
  };
  const static ST_sun_shape allowedShapes[3];

  static QString ST_sun_shape_tag(const ST_sun_shape &aShape, bool longName = false);

  static ST_sun_shape readSunShape(QString s);

  ST_sun();
  ST_sun(QTextStream &in);
  void write(QTextStream &out);

protected:
  bool ptsrc;
  ST_sun_shape shape;
  double sigma;
  double halfwidth;
  double xyz[3];
  bool useLatDayHour;
  double LatDayHour[3];
  QVector<QPair<double,double>> userShapeData;

  friend class ST_sun_model;
};

#endif // ST_SUN_H

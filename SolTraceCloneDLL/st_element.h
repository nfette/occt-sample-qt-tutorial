#ifndef ST_STAGE_ROW_H
#define ST_STAGE_ROW_H

#include "st_aperture.h"
#include "st_surface.h"

#include <memory>

#include <QTextStream>
#include <QFile>

#include <TopoDS_Shape.hxx>

class ST_element
{
public:
  enum intentType { REFRACTION = 1, REFLECTION = 2};
  static QString typeToString(const intentType &it);
  static intentType stringToType(QString s);

  ST_element();
  ST_element(QTextStream &in);
  ~ST_element();

  gp_Trsf getTransform() const;
  TopoDS_Shape toShape(const QDir &parent) const;

  bool isEnabled;
  double xyz[3];
  double aim[3];
  double zrot;
  ST_aperture aperture;
  std::shared_ptr<ST_surface> surface;
  QString opticsName;
  intentType myIntent;
  QString myName;

  void write(int version, QTextStream &out);
};

#endif // ST_STAGE_ROW_H

#ifndef ST_STAGE_H
#define ST_STAGE_H

#include "st_element.h"

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QVector>

#include <gp_Trsf.hxx>
#include <TopoDS_Shape.hxx>

class ST_elements_model;

class ST_stage
{
public:
  ST_stage();
  ST_stage(QTextStream &in);
  void write(QTextStream &out, int version = 0);
  bool writeFile(QString &filename, int version = 0);

  gp_Trsf getTransform() const;
  TopoDS_Shape toShape(const QDir &parent, int whichRow) const;
  TopoDS_Shape toShape(const QDir &parent) const;

  double xyz[3];
  double aim[3];
  double zrot;
  bool isVirtual;
  bool isMultiHit;
  bool isTraceThrough;
  QString myName;
  QVector<ST_element> rows;

  friend class ST_elements_model;
};

#endif // ST_STAGE_H

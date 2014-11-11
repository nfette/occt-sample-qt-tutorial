#ifndef ST_DOCUMENT_H
#define ST_DOCUMENT_H

#include <QVector>
#include <QTextStream>
#include <QFile>

#include "st_sun.h"
#include "st_optical.h"
#include "st_stage.h"

class ST_document
{
public:
  ST_document();
  ST_document(QFile *file, QTextStream &in);
  void write(QTextStream &out);

  ST_sun & sun();
  const ST_sun & sun() const;
  const QVector<ST_optical_pair> & opticalPairs() const;
  QVector<ST_stage> & stages();
  const QVector<ST_stage> & stages() const;
  QFile * file() const;
  const QDir path() const;

protected:
  ST_sun mySun;
  QVector<ST_optical_pair> myOpticalPairs;
  QVector<ST_stage> myStages;
  QFile * myFile;

  friend class ST_document_model;
  friend class ST_elements_model;
  friend class ST_stages_model;
};

#endif // ST_DOCUMENT_H

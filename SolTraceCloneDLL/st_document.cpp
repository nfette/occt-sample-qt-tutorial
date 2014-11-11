#include "st_document.h"

#include "st_sun.h"
#include "st_optical.h"
#include "st_stage.h"

#include <QString>
#include <QStringList>
#include <QFileInfo>

ST_document::ST_document()
{
  myOpticalPairs.append(ST_optical_pair());
  myStages.append(ST_stage());
}

ST_document::ST_document(QFile *file, QTextStream &in)
{
  myFile = file;

  qint64 pos = in.pos();
  QString line;
  QStringList tokens;
  line = in.readLine();
  if ("#" != line.at(0))
    in.seek(pos);
  ST_sun aSun(in);
  mySun = aSun;

  line = in.readLine();
  tokens = line.split(QRegExp("\t"));
  int n = tokens.at(1).toInt();
  for (int i = 0; i < n; i++) {
    ST_optical_pair optics(in);
    myOpticalPairs.append(optics);
  }

  line = in.readLine();
  tokens = line.split(QRegExp("\t"));
  n = tokens[1].toInt();
  for (int i = 0; i < n; i++) {
    ST_stage stage(in);
    myStages.append(stage);
  }

}

ST_sun &ST_document::sun()
{
  return mySun;
}

const ST_sun &ST_document::sun() const
{
  return mySun;
}

const QVector<ST_optical_pair> &ST_document::opticalPairs() const
{
  return myOpticalPairs;
}

QVector<ST_stage> & ST_document::stages()
{
  return myStages;
}

const QVector<ST_stage> & ST_document::stages() const
{
  return myStages;
}

QFile * ST_document::file() const
{
  return myFile;
}

const QDir ST_document::path() const
{
  QFileInfo info1(*myFile);
  return info1.dir();
}

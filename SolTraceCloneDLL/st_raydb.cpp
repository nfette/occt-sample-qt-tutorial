#include "st_raydb.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>

#include "st_rayintersection.h"

ST_raydb::ST_raydb()
{
}

bool ST_raydb::readCSV(QString csvFilename)
{
  QFile *file = new QFile(csvFilename);
  if (file->open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(file);
    QString line;
    double x,y,z,cosx,cosy,cosz;
    int elemId,stageId,rayId;
    int elemIndex, stageIndex, rayIndex;
    ST_rayintersection::HIT_TYPE hitType;

    in.readLine();
    while (!in.atEnd()) {
      line = in.readLine();
      QStringList tokens = line.split(",");
      x = tokens[0].toDouble();
      y = tokens[1].toDouble();
      z = tokens[2].toDouble();
      cosx = tokens[3].toDouble();
      cosy = tokens[4].toDouble();
      cosz = tokens[5].toDouble();
      elemId = tokens[6].toInt();
      stageId = tokens[7].toInt();
      rayId = tokens[8].toInt();
      stageIndex = stageId - 1;
      rayIndex = rayId - 1;
      if (elemId > 0) {
        hitType = ST_rayintersection::REFLECT;
        elemIndex = elemId - 1;
      } else if (elemId < 0) {
        hitType = ST_rayintersection::ABSORB;
        elemIndex = (-elemId) - 1;
      } else {
        hitType = ST_rayintersection::MISS;
        elemIndex = -1;
      }
      ST_rayintersection ray(x,y,z,cosx,cosy,cosz,elemIndex,stageIndex,rayIndex,hitType);
      myIntersections.push_back(ray);
    }
    file->close();
    return true;
  } else {
    qDebug() << "Unable to open CSV ray database: " << csvFilename;
    return false;
  }
}

void ST_raydb::countHits(int stageIndex, int elementIndex,
                         int &incidentHits, int &absorbHits) const
{
  incidentHits = 0;
  absorbHits = 0;
  for (int i = 0; i < myIntersections.size(); i++) {
    ST_rayintersection ray = myIntersections[i];
    if (ray.StageIndex == stageIndex && ray.ElementIndex == elementIndex) {
      switch (ray.myHitType) {
      case ST_rayintersection::ABSORB:
        incidentHits++;
        absorbHits++;
        break;
      case ST_rayintersection::REFLECT:
        incidentHits++;
        break;
      default:
        // miss
        ;
      }
    }
  }
}

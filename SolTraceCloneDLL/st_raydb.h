#ifndef ST_RAYDB_H
#define ST_RAYDB_H

#include <vector>

#include <QString>

#include "st_rayintersection.h"

class ST_raydb
{
public:
  ST_raydb();
  bool readCSV(QString csvFile);
  void countHits(int stageIndex, int elementIndex,
                 int &incidentHits, int &absorbHits) const;

  std::vector<ST_rayintersection> myIntersections;
  double powerPerRay;
};

#endif // ST_RAYDB_H

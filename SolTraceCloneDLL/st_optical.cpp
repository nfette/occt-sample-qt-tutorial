#include "st_optical.h"

#include <QString>
#include <QStringList>

ST_optical::ST_optical()
{
  myErrorType = GAUSSIAN;
  apertureStopType = 3;
  opticalSurfaceNumber = 1;
  diffractionOrder = 4;
  reflectivity = 0.96;
  transmissivity = 1;
  slopeError_mrad = 0.95;
  specularityError_mrad = 0.20;
  refractionIndex.real(1.1);
  refractionIndex.imag(1.2);
  useAngularReflectivityFunction = false;
  gratingSpacingCoefficients[0] = 1.1;
  gratingSpacingCoefficients[0] = 1.2;
  gratingSpacingCoefficients[0] = 1.3;
  gratingSpacingCoefficients[0] = 1.4;
}

ST_optical::ST_optical(QTextStream &in)
{
  QString line;
  QStringList tokens;

  line = in.readLine();
  tokens = line.split("\t");
  if ("p" == tokens[1])
    myErrorType = GAUSSIAN;
  else
    myErrorType = PILLBOX;
  apertureStopType = tokens[2].toInt();
  opticalSurfaceNumber = tokens[3].toInt();
  diffractionOrder = tokens[4].toInt();
  reflectivity = tokens[5].toDouble();
  transmissivity = tokens[6].toDouble();
  slopeError_mrad = tokens[7].toDouble();
  specularityError_mrad = tokens[8].toDouble();
  refractionIndex.real(tokens[9].toDouble());
  refractionIndex.imag(tokens[10].toDouble());
  for (int i = 0; i < 4; i++) {
    gratingSpacingCoefficients[i] = tokens[11 + i].toDouble();
  }
  if (tokens.count() > 15) { // OPTICAL.v2
    useAngularReflectivityFunction = tokens[15].toInt();
    int numPoints = tokens[16].toInt();
    if (useAngularReflectivityFunction) {
      double incidenceAngleMrad, reflectivity;
      for (int i = 0; i < numPoints; i++) {
        in >> incidenceAngleMrad;
        in >> reflectivity;
        QPair<double, double> pair(incidenceAngleMrad, reflectivity);
        angularReflectivityFunction.append(pair);
      }
    }
  } else {
    useAngularReflectivityFunction = false;
  }
}

ST_optical_pair::ST_optical_pair()
{
  myName = "New optics";
}

ST_optical_pair::ST_optical_pair(QTextStream &in)
{
  myName = in.readLine().split("\t")[1];

  {
    ST_optical read(in);
    front = read;
  }
  {
    ST_optical read(in);
    back = read;
  }
}

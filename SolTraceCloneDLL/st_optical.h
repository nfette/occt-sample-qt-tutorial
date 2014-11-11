#ifndef ST_OPTICAL_H
#define ST_OPTICAL_H

#include <complex>

#include <QTextStream>
#include <QVector>
#include <QPair>

class ST_optical
{
public:
  enum ERROR_TYPE {
    GAUSSIAN, PILLBOX
  };

  ST_optical();
  ST_optical(QTextStream &in);

  void write(QTextStream &out);

protected:
  ERROR_TYPE myErrorType;
  int apertureStopType;
  int opticalSurfaceNumber;
  int diffractionOrder;
  double reflectivity;
  double transmissivity;
  double slopeError_mrad;
  double specularityError_mrad;
  std::complex<double> refractionIndex;
  double gratingSpacingCoefficients[4];
  bool useAngularReflectivityFunction;
  QVector<QPair<double, double>> angularReflectivityFunction;

};

class ST_optical_pair
{
public:
  ST_optical_pair();
  ST_optical_pair(QTextStream &in);

  QString myName;
  ST_optical front;
  ST_optical back;
};

#endif // ST_OPTICAL_H

#include "st_aperture.h"
#include "st_geom.h"

#include <sstream>

#include <Standard_Macro.hxx>
#include <BndLib_Add2dCurve.hxx>
#include <Geom2dAdaptor_Curve.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <Precision.hxx>
#include <gp_Ax22d.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Circ2d.hxx>
#include <GCE2d_MakeSegment.hxx>
#include <GCE2d_MakeCircle.hxx>
#include <GCE2d_MakeArcOfCircle.hxx>

ST_aperture::ST_aperture_type ST_aperture::stringToType(QString tag)
{
  tag = tag.toLower();
  if (tag == "c") {
    return CIRCULAR;
  } else if (tag == "h") {
    return HEXAGONAL;
  } else if (tag == "t") {
    return TRIANGULAR;
  } else if (tag == "r") {
    return RECTANGULAR;
  } else if (tag == "l") {
    return SINGLE_AXIS_CURVATURE_SECTION;
  } else if (tag == "a") {
    return ANNULAR;
  } else if (tag == "i") {
    return IRREGULAR_TRIANGLE;
  } else if (tag == "q") {
    return IRREGULAR_QUAD;
  } else {
    QString msg = QString("Unrecognized aperture tag: \"%1\"").arg(tag);
    throw std::invalid_argument(msg.toLatin1().data());
  }
}

QString ST_aperture::typeToString(ST_aperture_type type)
{
  switch (type) {
  case CIRCULAR:
    return "c";
  case HEXAGONAL:
    return "h";
  case TRIANGULAR:
    return "t";
  case RECTANGULAR:
    return "r";
  case SINGLE_AXIS_CURVATURE_SECTION:
    return "l";
  case ANNULAR:
    return "a";
  case IRREGULAR_TRIANGLE:
    return "i";
  case IRREGULAR_QUAD:
    return "q";
  default:
    return "r";
  }
}

ST_aperture::ST_aperture()
  : data()
{
  myType = RECTANGULAR;
  data[0] = 1;
  data[1] = 1;
}

ST_aperture::ST_aperture(const QStringList &tokens)
{
  QString tag = tokens[0];
  myType = stringToType(tag);
  for (int i = 0; i < 8; i++) {
    data[i] = tokens[1 + i].toDouble();
  }
}

ST_aperture::ST_aperture(ST_aperture::ST_aperture_type type, double (&theData)[8])
  : myType(type)
{
  std::copy_n(theData, 8, data);
}

void ST_aperture::write(QTextStream &out) const
{
  QString sep("\t");
  out << typeToString(myType) << sep;
  for (int i = 0; i < 8; i++) {
    out << data[i];
    if (i < 7)
      out << sep;
  }
}

const ST_aperture::ST_aperture_type & ST_aperture::type() const
{
  return myType;
}

void ST_aperture::makeBounds(QVector<Handle(Geom2d_Curve)> &wire, Bnd_Box2d &aCBox) const
{
  wire.clear();
  aCBox.SetVoid();
  switch (myType) {
  case CIRCULAR:
  {
    gp_Pnt2d p0;
    double radius = 0.5 * data[0];
    wire.append(GCE2d_MakeCircle(p0, static_cast<Standard_Real>(radius)));
    break;
  }
  case HEXAGONAL:
  {
    double diam = data[0];
    for (int i = 0; i < 6; i++) {
      double theta0 = 2.0 * 3.14159265 * static_cast<double>(i) / 6.0,
          theta1 = 2.0 * 3.14159265 * static_cast<double>(1 + i) / 6.0;
      gp_Pnt2d p0(0.5 * diam * cos(theta0), 0.5 * diam * sin(theta0)),
          p1(0.5 * diam * cos(theta1), 0.5 * diam * sin(theta1));
      wire.append(GCE2d_MakeSegment(p0, p1));
    }
    break;
  }
  case TRIANGULAR:
  {
    double diam = data[0];
    for (int i = 0; i < 3; i++) {
      double theta0 = 2.0 * 3.14159265 * static_cast<double>(i) / 3.0,
          theta1 = 2.0 * 3.14159265 * static_cast<double>(1 + i) / 3.0;
      gp_Pnt2d p0(0.5 * diam * cos(theta0), 0.5 * diam * sin(theta0)),
          p1(0.5 * diam * cos(theta1), 0.5 * diam * sin(theta1));
      Handle(Geom2d_Curve) aSegment = GCE2d_MakeSegment(p0, p1);
      wire.append(aSegment);
      Geom2dAdaptor_Curve GACC(aSegment);
      BndLib_Add2dCurve::Add(GACC, Precision::Approximation(), aCBox);
    }
    break;
  }
  case RECTANGULAR:
  {
    const double W = data[0], H = data[1];
    gp_Pnt2d p0(0.5 * W, 0.5 * H),
        p1(-0.5 * W, 0.5 * H),
        p2(-0.5 * W, -0.5 * H),
        p3(0.5 * W, -0.5 * H);

    wire.append(GCE2d_MakeSegment(p0, p1));
    wire.append(GCE2d_MakeSegment(p1, p2));
    wire.append(GCE2d_MakeSegment(p2, p3));
    wire.append(GCE2d_MakeSegment(p3, p0));
    break;
  }
  case SINGLE_AXIS_CURVATURE_SECTION:
  {
    double X1 = data[0], X2 = data[1], L = data[2];
    gp_Pnt2d p0(X2, 0.5 * L),
        p1(X1, 0.5 * L),
        p2(X1, -0.5 * L),
        p3(X2, -0.5 * L);

    wire.append(GCE2d_MakeSegment(p0, p1));
    wire.append(GCE2d_MakeSegment(p1, p2));
    wire.append(GCE2d_MakeSegment(p2, p3));
    wire.append(GCE2d_MakeSegment(p3, p0));
    break;
  }
  case ANNULAR:
  {
    double R1 = data[0], R2 = data[1], thetaDeg = data[2], theta1, theta2;
    theta1 = 0.5 * thetaDeg * ST_geom::radPerDeg;
    theta2 = -0.5 * thetaDeg * ST_geom::radPerDeg;
    gp_Circ2d circ1(gp_Ax22d(), R1),
        circ2(gp_Ax22d(), R2);
    gp_Pnt2d p1, p2, p3, p4;
    Handle(Geom2d_TrimmedCurve) curve2(GCE2d_MakeArcOfCircle(circ1, theta2, theta1, 0));
    Handle(Geom2d_TrimmedCurve) curve4(GCE2d_MakeArcOfCircle(circ2, theta2, theta1, 1));
    p1 = curve4->EndPoint();
    p2 = curve2->StartPoint();
    p3 = curve2->EndPoint();
    p4 = curve4->StartPoint();
    Handle(Geom2d_TrimmedCurve) curve1(GCE2d_MakeSegment(p1,p2));
    Handle(Geom2d_TrimmedCurve) curve3(GCE2d_MakeSegment(p3,p4));
    wire.append(curve1);
    wire.append(curve2);
    wire.append(curve3);
    wire.append(curve4);
    break;
  }
  case IRREGULAR_TRIANGLE:
  {
    gp_Pnt2d p1(data[0],data[1]), p2(data[2],data[3]), p3(data[4],data[5]);
    wire.append(GCE2d_MakeSegment(p1,p2));
    wire.append(GCE2d_MakeSegment(p2,p3));
    wire.append(GCE2d_MakeSegment(p3,p1));
    break;
  }
  case IRREGULAR_QUAD:
  {
    gp_Pnt2d p1(data[0],data[1]),
        p2(data[2],data[3]),
        p3(data[4],data[5]),
        p4(data[6],data[7]);
    wire.append(GCE2d_MakeSegment(p1,p2));
    wire.append(GCE2d_MakeSegment(p2,p3));
    wire.append(GCE2d_MakeSegment(p3,p4));
    wire.append(GCE2d_MakeSegment(p4,p1));
    break;
  }
  default:
  {
    double diam = 1;
    for (int i = 0; i < 6; i++) {
      double theta0 = 2.0 * 3.14159265 * static_cast<double>(i) / 6.0,
          theta1 = 2.0 * 3.14159265 * static_cast<double>(1 + i) / 6.0;
      gp_Pnt2d p0(0.5 * diam * cos(theta0), 0.5 * diam * sin(theta0)),
          p1(0.5 * diam * cos(theta1), 0.5 * diam * sin(theta1));
      Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(p0, p1);
      wire.append(aSegment);
    }
  }
  }

  foreach (Handle(Geom2d_Curve) aCurve, wire) {
    BndLib_Add2dCurve::Add(Geom2dAdaptor_Curve(aCurve), Precision::Approximation(), aCBox);
  }
}


QTextStream &operator<<(QTextStream &out, const ST_aperture &aperture)
{
  aperture.write(out);
  return out;
}

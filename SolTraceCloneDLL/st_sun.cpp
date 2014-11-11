#include "st_sun.h"

const ST_sun::ST_sun_shape ST_sun::allowedShapes[] = {ST_sun::GAUSSIAN, ST_sun::PILLBOX, ST_sun::USER_DEFINED};

QString ST_sun::ST_sun_shape_tag(const ST_sun::ST_sun_shape &aShape, bool longName)
{
  if (!longName) {
    switch (aShape) {
    case GAUSSIAN:
      return "g";
    case PILLBOX:
      return "p";
    case USER_DEFINED:
      return "d";
    }
  } else {
    switch (aShape) {
    case GAUSSIAN:
      return "Gaussian";
    case PILLBOX:
      return "Pillbox";
    case USER_DEFINED:
      return "User defined";
    }
  }
  throw std::invalid_argument("aShape is not of enum type ST_sun_shape");
}

ST_sun::ST_sun_shape ST_sun::readSunShape(QString s)
{
  s = s.toLower();
  if (s == ST_sun_shape_tag(GAUSSIAN)
      || s == ST_sun_shape_tag(GAUSSIAN, true).toLower()) {
    return GAUSSIAN;
  } else if (s == ST_sun_shape_tag(PILLBOX)
             || s == ST_sun_shape_tag(PILLBOX, true).toLower()) {
    return PILLBOX;
  } else if (s == ST_sun_shape_tag(USER_DEFINED)
             || s == ST_sun_shape_tag(USER_DEFINED, true).toLower()) {
    return USER_DEFINED;
  }
  throw std::invalid_argument(QString("No such sun shape: \"%1\"").arg(s).toLatin1().constData());
}

ST_sun::ST_sun()
{
  ptsrc = false;
  shape = PILLBOX;
  sigma = 2.67;
  halfwidth = 4.65;
  xyz[0] = 0;
  xyz[1] = 0;
  xyz[2] = 100;
  useLatDayHour = false;
  LatDayHour[0] = 39.5;
  LatDayHour[1] = 90;
  LatDayHour[2] = 10;
}

ST_sun::ST_sun(QTextStream &in) {
  QString line;
  QStringList tokens;

  line = in.readLine();
  tokens = line.split("\t");
  ptsrc = tokens[2].toInt();
  shape = readSunShape(tokens[4]);
  sigma = tokens[6].toDouble();
  halfwidth = tokens[8].toDouble();

  line = in.readLine();
  tokens = line.split("\t");
  xyz[0] = tokens[1].toDouble();
  xyz[1] = tokens[2].toDouble();
  xyz[2] = tokens[3].toDouble();
  useLatDayHour = tokens[5].toInt();
  LatDayHour[0] = tokens[7].toDouble();
  LatDayHour[1] = tokens[8].toDouble();
  LatDayHour[2] = tokens[9].toDouble();

  int numUserData;
  line = in.readLine();
  tokens = line.split("\t");
  numUserData = tokens[1].toInt();

  QRegExp re("\\s");
  for (int i = 0; i < numUserData; i++) {
    line = in.readLine();
    tokens = line.split(re);
    QPair<double,double> qp(tokens[0].toDouble(), tokens[1].toDouble());
    userShapeData.append(qp);
  }
}

#include "st_sun_model.h"

#include <QDebug>

ST_sun_model::ST_sun_model(ST_document * const aDoc, QObject *parent) :
  QAbstractTableModel(parent),
  myDoc(aDoc)
{
}

ST_sun &ST_sun_model::mySun()
{
  return myDoc->sun();
}

const ST_sun &ST_sun_model::mySun() const
{
  return myDoc->sun();
}

int ST_sun_model::rowCount(const QModelIndex &parent) const
{
  return 12 + 2 * mySun().userShapeData.size();
}

int ST_sun_model::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant ST_sun_model::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.row() < 0 || index.row() >= 12 + 2 * mySun().userShapeData.size())
    return QVariant();

  int section = index.row();
  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    switch(section) {
    case 0:
      return mySun().ptsrc;
    case 1:
      return ST_sun::ST_sun_shape_tag(mySun().shape, true);
    case 2:
      return QVariant(mySun().sigma).toString();
    case 3:
      return QVariant(mySun().halfwidth).toString();
    case 4:
      return QVariant(mySun().xyz[0]).toString();
    case 5:
      return QVariant(mySun().xyz[1]).toString();
    case 6:
      return QVariant(mySun().xyz[2]).toString();
    case 7:
      return mySun().useLatDayHour;
    case 8:
      return QVariant(mySun().LatDayHour[0]).toString();
    case 9:
      return QVariant(mySun().LatDayHour[1]).toString();
    case 10:
      return QVariant(mySun().LatDayHour[2]).toString();
    case 11:
      return mySun().userShapeData.size();
    }
    // Still here? Must be asking for user data.
    int i = section - 12;
    int j = i >> 1;
    int k = i - 2 * j;
    QPair<double, double> qp = mySun().userShapeData.at(j);
    return QVariant((!k)?qp.first:qp.second).toString();
  } else if (role == Qt::CheckStateRole) {
    switch (section) {
    case 0:
      return (mySun().ptsrc)?Qt::Checked:Qt::Unchecked;
    case 7:
      return (mySun().useLatDayHour)?Qt::Checked:Qt::Unchecked;
    default:
      return QVariant();
    }
  }
  return QVariant();
}

QVariant ST_sun_model::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole){
    if (orientation == Qt::Vertical) {
      switch(section) {
      case 0:
        return "Point source?";
      case 1:
        return "Sun shape";
      case 2:
        return "Sigma";
      case 3:
        return "Halfwidth";
      case 4:
        return "X";
      case 5:
        return "Y";
      case 6:
        return "Z";
      case 7:
        return "Use LatDayHour?";
      case 8:
        return "Lat";
      case 9:
        return "Day";
      case 10:
        return "Hour";
      case 11:
        return "User shape data";
      }
      // Still here? Must be asking for user data.
      int i = section - 12;
      int j = i >> 1;
      int k = i - 2 * j;
      return tr("%1[%2]").arg((!k)?"angle (mrad)":"intensity").arg(j);
    } else {
      return section;
    }
  }
  return QVariant();
}

Qt::ItemFlags ST_sun_model::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

//  if (index.row() == 0 || index.row() == 7)
//    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsUserCheckable;

  return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool ST_sun_model::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.isValid() && role == Qt::EditRole) {
    switch(index.row()) {
    case 0:
      mySun().ptsrc = value.toBool(); break;
    case 1:
      try {
        mySun().shape = ST_sun::readSunShape(value.toString());
        break;
      } catch (std::invalid_argument &e) {
        qDebug() << e.what();
        return false;
      }
    case 2:
      mySun().sigma = value.toDouble(); break;
    case 3:
      mySun().halfwidth = value.toDouble(); break;
    case 4:
      mySun().xyz[0] = value.toDouble(); break;
    case 5:
      mySun().xyz[1] = value.toDouble(); break;
    case 6:
      mySun().xyz[2] = value.toDouble(); break;
    case 7:
      mySun().useLatDayHour = value.toBool(); break;
    case 8:
      mySun().LatDayHour[0] = value.toDouble(); break;
    case 9:
      mySun().LatDayHour[1] = value.toDouble(); break;
    case 10:
      mySun().LatDayHour[2] = value.toDouble(); break;
    case 11:
      int datasize = value.toInt();
      if (datasize < 0)
          return false;
      int oldsize = mySun().userShapeData.size();
      if (oldsize < datasize) {
        beginInsertRows(QModelIndex(), 12 + 2 * oldsize, 11 + 2 * datasize);
        mySun().userShapeData.resize(datasize);
        endInsertRows();
      } else if (datasize < oldsize) {
        beginRemoveRows(QModelIndex(), 12 + 2 * datasize, 11 + 2 * oldsize);
        mySun().userShapeData.resize(datasize);
        endRemoveRows();
      } else {
        return false;
      }
      emit(dataChanged(index, ST_sun_model::index(11 + 2 * datasize, index.column())));
      return true;
    }
    if (index.row() >= 12 && index.row() <= 11 + 2 * mySun().userShapeData.size()) {
      // Still here? Must be asking for user data.
      int i = index.row() - 12;
      int j = i >> 1;
      int k = i - 2 * j;
      QPair<double, double> &qp = mySun().userShapeData[j];
      if (!k) {
        qp.first = value.toDouble();
      } else {
        qp.second = value.toDouble();
      }
    }
    emit(dataChanged(index,index));
    return true;
  }
  //if (role == Qt::CheckStateRole) {
  //  if (index.row() == 0)
  //    ;
  //}
  return false;
}

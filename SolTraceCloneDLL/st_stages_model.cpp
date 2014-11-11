#include "st_stages_model.h"

ST_stages_model::ST_stages_model(ST_document * aDoc, QObject *parent) :
  QAbstractTableModel(parent),
  myDoc(aDoc)
{
}

int ST_stages_model::rowCount(const QModelIndex &parent) const
{
  return myDoc->stages().size();
}

int ST_stages_model::columnCount(const QModelIndex &parent) const
{
  return 11;
}

QVariant ST_stages_model::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.row() < 0 || index.row() >= myDoc->stages().size())
    return QVariant();

  int i = index.row();
  int section = index.column();
  const ST_stage & aStage (myDoc->stages()[i]);

  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    switch(section) {
    case 0:
      return aStage.xyz[0];
    case 1:
      return aStage.xyz[1];
    case 2:
      return aStage.xyz[2];
    case 3:
      return aStage.aim[0];
    case 4:
      return aStage.aim[1];
    case 5:
      return aStage.aim[2];
    case 6:
      return aStage.zrot;
    case 7:
      return aStage.isVirtual;
    case 8:
      return aStage.isMultiHit;
    case 9:
      return aStage.isTraceThrough;
    case 10:
      return aStage.myName;
    }
  }
  return QVariant();
}

QVariant ST_stages_model::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Vertical)
      return section;
    else {
      switch (section) {
      case 0:
        return "x";
      case 1:
        return "y";
      case 2:
        return "z";
      case 3:
        return "ax";
      case 4:
        return "ay";
      case 5:
        return "az";
      case 6:
        return "zrot (deg)";
      case 7:
        return "virtual?";
      case 8:
        return "multi hit?";
      case 9:
        return "trace through?";
      case 10:
        return "name";
      }
    }
  }
  return QVariant();
}

Qt::ItemFlags ST_stages_model::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool ST_stages_model::setData(const QModelIndex &index, const QVariant &value, int role)
{
  ST_stage & aStage (myDoc->stages()[index.row()]);

  if (index.isValid() && role == Qt::EditRole) {
    switch(index.column()) {
    case 0:
      aStage.xyz[0] = value.toDouble(); break;
    case 1:
      aStage.xyz[1] = value.toDouble(); break;
    case 2:
      aStage.xyz[2] = value.toDouble(); break;
    case 3:
      aStage.aim[0] = value.toDouble(); break;
    case 4:
      aStage.aim[1] = value.toDouble(); break;
    case 5:
      aStage.aim[2] = value.toDouble(); break;
    case 6:
      aStage.zrot = value.toDouble(); break;
    case 7:
      aStage.isVirtual = value.toBool(); break;
    case 8:
      aStage.isMultiHit = value.toBool(); break;
    case 9:
      aStage.isTraceThrough = value.toBool(); break;
    case 10:
      aStage.myName = value.toString(); break;
    default:
      return false;
    }
    emit(dataChanged(index,index));
    return true;
  }
  return false;
}

#include "st_elements_model.h"

#include <QDebug>

ST_elements_model::ST_elements_model(ST_document * const aDoc, int nStage, QObject *parent) :
  QAbstractTableModel(parent),
  myDoc(aDoc),
  nStage(nStage)
{
}

const ST_stage &ST_elements_model::myStage() const
{
  return myDoc->stages().at(nStage);
}

ST_stage &ST_elements_model::myStage()
{
  return myDoc->myStages[nStage];
}

int ST_elements_model::rowCount(const QModelIndex &parent) const
{
  return myDoc->stages().at(nStage).rows.size();
}

int ST_elements_model::columnCount(const QModelIndex &parent) const
{
  return 29;
}

QVariant ST_elements_model::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.row() < 0 || index.row() >= myStage().rows.size())
    return QVariant();

  int i = index.row();
  int section = index.column();
  const ST_element &aRow(myStage().rows.at(i));

  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    switch(section) {
    case 0:
      return aRow.isEnabled;
    case 1:
      return aRow.xyz[0];
    case 2:
      return aRow.xyz[1];
    case 3:
      return aRow.xyz[2];
    case 4:
      return aRow.aim[0];
    case 5:
      return aRow.aim[1];
    case 6:
      return aRow.aim[2];
    case 7:
      return aRow.zrot;
    case 8:
      return ST_aperture::typeToString(aRow.aperture.type());
    case 9:
      return aRow.aperture.data[0];
    case 10:
      return aRow.aperture.data[1];
    case 11:
      return aRow.aperture.data[2];
    case 12:
      return aRow.aperture.data[3];
    case 13:
      return aRow.aperture.data[4];
    case 14:
      return aRow.aperture.data[5];
    case 15:
      return aRow.aperture.data[6];
    case 16:
      return aRow.aperture.data[7];
    case 17:
      return ST_surface::typeToString(aRow.surface->type);
    case 18:
      return aRow.surface->data[0];
    case 19:
      return aRow.surface->data[1];
    case 20:
      return aRow.surface->data[2];
    case 21:
      return aRow.surface->data[3];
    case 22:
      return aRow.surface->data[4];
    case 23:
      return aRow.surface->data[5];
    case 24:
      return aRow.surface->data[6];
    case 25:
      return aRow.surface->data[7];
    case 26:
      return aRow.surface->filename;
    case 27:
      return ST_element::typeToString(aRow.myIntent);
    case 28:
      return aRow.myName;
    default:
      return QVariant();
    }
  } else if (role == Qt::CheckStateRole) {
    switch (section) {
    case 0:
      return (aRow.isEnabled)?Qt::Checked:Qt::Unchecked;
    default:
      return QVariant();
    }
  }
  return QVariant();
}

QVariant ST_elements_model::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole){
    if (orientation == Qt::Horizontal) {
      switch(section) {
      case 0:
        return "Enabled?";
      case 1:
        return "X";
      case 2:
        return "Y";
      case 3:
        return "Z";
      case 4:
        return "aX";
      case 5:
        return "aY";
      case 6:
        return "aZ";
      case 7:
        return "Zrot (deg)";
      case 8:
        return "Aperture";
      case 9:
        return "1";
      case 10:
        return "2";
      case 11:
        return "3";
      case 12:
        return "4";
      case 13:
        return "5";
      case 14:
        return "6";
      case 15:
        return "7";
      case 16:
        return "8";
      case 17:
        return "Surface";
      case 18:
        return "1";
      case 19:
        return "2";
      case 20:
        return "3";
      case 21:
        return "4";
      case 22:
        return "5";
      case 23:
        return "6";
      case 24:
        return "7";
      case 25:
        return "8";
      case 26:
        return "file";
      case 27:
        return "Interaction";
      case 28:
        return "Comment";
      default:
        return QVariant();
      }
    } else {
      return section;
    }
  }
  return QVariant();
}

Qt::ItemFlags ST_elements_model::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool ST_elements_model::setData(const QModelIndex &index, const QVariant &value, int role)
{
  ST_element &aRow(myStage().rows[index.row()]);

  if (index.isValid() && role == Qt::EditRole) {
    switch(index.column()) {
    case 0:
      aRow.isEnabled = value.toBool(); break;
    case 1:
      aRow.xyz[0] = value.toDouble(); break;
    case 2:
      aRow.xyz[1] = value.toDouble(); break;
    case 3:
      aRow.xyz[2] = value.toDouble(); break;
    case 4:
      aRow.aim[0] = value.toDouble(); break;
    case 5:
      aRow.aim[1] = value.toDouble(); break;
    case 6:
      aRow.aim[2] = value.toDouble(); break;
    case 7:
      aRow.zrot = value.toDouble(); break;
    case 8:
      try {
        aRow.aperture.myType = ST_aperture::stringToType(value.toString());
        break;
      } catch (std::invalid_argument& e) {
        qDebug() << e.what();
        return false;
      }
    case 9:
      aRow.aperture.data[0] = value.toDouble(); break;
    case 10:
      aRow.aperture.data[1] = value.toDouble(); break;
    case 11:
      aRow.aperture.data[2] = value.toDouble(); break;
    case 12:
      aRow.aperture.data[3] = value.toDouble(); break;
    case 13:
      aRow.aperture.data[4] = value.toDouble(); break;
    case 14:
      aRow.aperture.data[5] = value.toDouble(); break;
    case 15:
      aRow.aperture.data[6] = value.toDouble(); break;
    case 16:
      aRow.aperture.data[7] = value.toDouble(); break;
    case 17:
    try {
      aRow.surface->type = ST_surface::stringToType(value.toString());
      break;
    } catch (std::invalid_argument& e) {
      qDebug() << e.what();
      return false;
    }
    case 18:
      aRow.surface->data[0] = value.toDouble(); break;
    case 19:
      aRow.surface->data[1] = value.toDouble(); break;
    case 20:
      aRow.surface->data[2] = value.toDouble(); break;
    case 21:
      aRow.surface->data[3] = value.toDouble(); break;
    case 22:
      aRow.surface->data[4] = value.toDouble(); break;
    case 23:
      aRow.surface->data[5] = value.toDouble(); break;
    case 24:
      aRow.surface->data[6] = value.toDouble(); break;
    case 25:
      aRow.surface->data[7] = value.toDouble(); break;
    case 26:
      aRow.surface->filename = value.toString(); break;
    case 27:
      try {
        aRow.myIntent = ST_element::stringToType(value.toString());
        break;
      } catch (std::invalid_argument& e) {
        qDebug() << e.what();
        return false;
      }
    case 28:
      aRow.myName = value.toString(); break;
    default:
      return false;
    }
    emit(dataChanged(index, index));
  }
  return false;
}

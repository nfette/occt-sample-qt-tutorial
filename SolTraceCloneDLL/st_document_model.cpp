#include "st_document_model.h"
#include <QDebug>

ST_document_model::ST_document_model(QObject *parent) :
  QAbstractItemModel(parent),
  myDoc(new ST_document)
{
  initPtrs();
}

ST_document_model::ST_document_model(ST_document *aDoc, QObject *parent) :
  QAbstractItemModel(parent),
  myDoc(aDoc)
{
  initPtrs();
}

ST_document_model::~ST_document_model() {
  for (int i = 0; i < 8; i++)
    delete ptrs[i];
}

void ST_document_model::initPtrs()
{
  for (int i = 0; i < 8; i++)
    ptrs[i] = new int(i);
}

void ST_document_model::setDoc(ST_document * const aDoc)
{
  myDoc = aDoc;
}

ST_document *ST_document_model::doc() {
  return myDoc;
}

///////////////////////////////////////
// QAbstractListModel implementation //
///////////////////////////////////////

int ST_document_model::rowCount(const QModelIndex &parent) const
{
  int * ptr = static_cast<int *>(parent.internalPointer());
  //qDebug() << "rowCount() " << ptr << "->" << ((ptr)?*ptr:-1) << "," << parent.row() << parent.column() << parent.isValid();

  if (ptr) {
    switch (*ptr) {
    case 0:
      return 3;
    case 1:
      return 0;
    case 2:
      return myDoc->opticalPairs().size();
    case 3:
      return myDoc->stages().size();
    default:
      return 0;
    }
  }
  return 3;
}

int ST_document_model::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant ST_document_model::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch(section) {
    case 0:
      return "Name";
    case 1:
      return "Two";
    case 2:
      return "Three";
    default:
      return QVariant();
    }
  }
  return QVariant();
}

QVariant ST_document_model::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || index.row() < 0)
    return QVariant();

  if (role != Qt::DisplayRole && role != Qt::EditRole)
    return QVariant();

  int * ptr = static_cast<int *>(index.internalPointer());

  if (ptr) {
    switch (*ptr) {
    case 1:
      return "Sun shape";
    case 2:
      return "Optical properties";
    case 3:
      return "System stages";
    case 4:
      return myDoc->myOpticalPairs.at(index.row()).myName;
    case 5:
      return myDoc->myStages.at(index.row()).myName;
    }
  }
  return QVariant();
}

QModelIndex ST_document_model::index(int row, int column, const QModelIndex &parent) const
{
  int * ptr = static_cast<int *>(parent.internalPointer());
  //qDebug() << "index() " << ptr << "->" << ((ptr)?*ptr:-1) << "," << parent.row() << parent.column() << parent.isValid()
  //         << " requesting " << row << column;

  if (parent.isValid() && parent.column() != 0)
    return QModelIndex();

  if (ptr) {
    switch (*ptr) {
    case 1:
      return QModelIndex();
    case 2:
      return createIndex(row,column,ptrs[4]);
    case 3:
      return createIndex(row,column,ptrs[5]);
    }
  }
  switch (row) {
  case 0:
    return createIndex(row,column,ptrs[1]);
  case 1:
    return createIndex(row,column,ptrs[2]);
  case 2:
    return createIndex(row,column,ptrs[3]);
  default:
    return QModelIndex();
  }
}

QModelIndex ST_document_model::parent(const QModelIndex &child) const
{
  int * ptr = static_cast<int *>(child.internalPointer());
  //qDebug() << "parent() " << ptr << "->" << ((ptr)?*ptr:-1) << "," << child.row() << child.column() << child.isValid();

  if (!child.isValid())
      return QModelIndex();

  switch (*ptr) {
  case 1:
    return QModelIndex();
  case 2:
    return QModelIndex();
  case 3:
    return QModelIndex();
  case 4:
    return createIndex(1,0,ptrs[2]);
  case 5:
    return createIndex(2,0,ptrs[3]);
  }
  return QModelIndex();
}

Qt::ItemFlags ST_document_model::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  int * ptr = static_cast<int *>(index.internalPointer());
  if (ptr) {
    switch (*ptr) {
    case 4:
    case 5:
      return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    }
  }
  return QAbstractItemModel::flags(index);
}

bool ST_document_model::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.isValid() && role == Qt::EditRole) {
    int * ptr = static_cast<int *>(index.internalPointer());

    if (!ptr)
      return false;

    switch (*ptr) {
    case 1:
    case 2:
    case 3:
      return false;
    case 4:
      myDoc->myOpticalPairs[index.row()].myName = value.toString();
      break;
    case 5:
      myDoc->myStages[index.row()].myName = value.toString();
      break;
    default:
      return false;
    }
    emit(dataChanged(index, index));

    return true;
  }

  return false;
}

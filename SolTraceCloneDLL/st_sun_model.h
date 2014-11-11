#ifndef ST_SUN_MODEL_H
#define ST_SUN_MODEL_H

#include "st_document.h"

#include <QAbstractItemModel>

class ST_sun_model : public QAbstractTableModel
{
  Q_OBJECT
public:
  explicit ST_sun_model(ST_document * const aSun, QObject *parent = 0);

  ST_sun & mySun();
  const ST_sun & mySun() const;

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role);
  
signals:
  
public slots:
  
protected:
  ST_document *myDoc;
};

#endif // ST_SUN_MODEL_H

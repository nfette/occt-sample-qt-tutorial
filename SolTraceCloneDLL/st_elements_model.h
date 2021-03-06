#ifndef ST_STAGE_MODEL_H
#define ST_STAGE_MODEL_H

#include "st_document.h"
#include "st_stage.h"
#include <QAbstractTableModel>

class ST_elements_model : public QAbstractTableModel
{
  Q_OBJECT
public:
  explicit ST_elements_model(ST_document * const aDoc, int nStage, QObject *parent = 0);

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role);

signals:
  
public slots:
  
protected:
  ST_document * myDoc;
  int nStage;
  ST_stage &myStage();
  const ST_stage &myStage() const;
};

#endif // ST_STAGE_MODEL_H

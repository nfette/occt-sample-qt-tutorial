#ifndef ST_DOCUMENT_MODEL_H
#define ST_DOCUMENT_MODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QStringList>

#include "st_document.h"

class Widget;

// Document model is shown with a tree view.
// When asked for an index, the internal pointer evaluates to int as shown (in parenthesis),
// and the row is in reference to the children of its parent [in square brackets].
// root = QModelIndex() [invalid index]
// + sun       (1) [0]
// + optics    (2) [1]
//   + optics1 (4) [0]
//   + optics2 (4) [1]
// + stages    (3) [0]
//   + stage1  (5) [0]
//   + stage2  (5) [1]
class ST_document_model : public QAbstractItemModel
{
  Q_OBJECT
public:
  explicit ST_document_model(QObject *parent = 0);
  explicit ST_document_model(ST_document *aDoc, QObject *parent = 0);
  ~ST_document_model();
  void initPtrs();

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  //bool hasChildren(const QModelIndex &parent) const;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role);

  void setDoc(ST_document * const aDoc);
  ST_document *doc();

signals:
  
public slots:

protected:
  ST_document *myDoc;
  int * ptrs[8];

  friend class Widget;
};

#endif // ST_DOCUMENT_MODEL_H

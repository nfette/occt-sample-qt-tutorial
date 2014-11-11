#ifndef WIDGET_H
#define WIDGET_H

#include "st_document_model.h"

#include <QWidget>
#include <QItemSelection>

#include <TopoDS_Shape.hxx>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
  Q_OBJECT
  
public:
  explicit Widget(QWidget *parent = 0);
  ~Widget();
  
public slots:
  void onOpenFileButton();
  void onDocSelectionChanged (const QItemSelection &, const QItemSelection &);
  void onAddShapeButton();
  void onUpdateStageButton();

signals:
  void addShape(TopoDS_Shape shape);

private:
  Ui::Widget *ui;
  ST_document_model *myDocModel;
};

#endif // WIDGET_H

#include "widget.h"
#include "ui_widget.h"
#include "st_document.h"
#include "st_sun_model.h"
#include "st_elements_model.h"
#include "st_stages_model.h"

#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QDebug>

#include <UnitsAPI.hxx>
#include <UnitsAPI_SystemUnits.hxx>
#include <Interface_Static.hxx>

Widget::Widget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Widget),
  myDocModel(new ST_document_model)
{
  ui->setupUi(this);
  ui->treeView->setModel(myDocModel);

  QItemSelectionModel *selectionModel= ui->treeView->selectionModel();
  connect(selectionModel,SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
          this, SLOT(onDocSelectionChanged (const QItemSelection &, const QItemSelection &)));

  // Figure out what assumptions we've already made.
  UnitsAPI_SystemUnits sysUnits = UnitsAPI::LocalSystem();
  switch (sysUnits) {
  case UnitsAPI_DEFAULT:
    qDebug() << "Units system: Default (same as SI) " << endl; break;
  case UnitsAPI_SI:
    qDebug() << "Units system: SI " << endl; break;
  case UnitsAPI_MDTV:
    qDebug() << "Units system: MDTV" << endl; break;
  default:
    qDebug() << "Units system: unknown (bad)" << endl;
  }
  //const char * lengthUnit = UnitsAPI::CurrentUnit("LENGTH");
  //qDebug() << lengthUnit << endl;
  Interface_Static::Standards();
  const char * stepLengthUnit = Interface_Static::CVal("xstep.cascade.unit");
  qDebug() << "xstep.cascade.unit = \"" << stepLengthUnit << "\"" << endl;
  Interface_Static::SetCVal("xstep.cascade.unit", "m");
  const char * stepLengthUnit2 = Interface_Static::CVal("xstep.cascade.unit");
  qDebug() << "xstep.cascade.unit = \"" << stepLengthUnit2 << "\"" << endl;
}

Widget::~Widget()
{
  delete ui;
}

void Widget::onOpenFileButton()
{
  QTextStream qout(stdout);
  QTextStream qin(stdin);

  QString filename;
  qout << "Open file button" << endl;
  qout << "Filename: " << flush;
  //filename = qin.readLine();
  filename = QFileDialog::getOpenFileName(this,
      tr("Open SolTrace input file"), "", tr("SolTrace files (*.stinput);;All files (*.*)"));
  qout << "The filename was " << filename << endl;

  if (QFile::exists(filename)) {
    qout << "The file exists." << endl;
    QFile *file = new QFile(filename);
    qout << "Is a link to: " << file->symLinkTarget() << endl;
    if (file->open(QIODevice::ReadOnly | QIODevice::Text)) {
      QTextStream ts(file);
      ST_document *doc = new ST_document(file, ts);
      file->close();
      myDocModel->setDoc(doc);
      ui->treeView->setModel(myDocModel);
      ui->treeView->reset();
      ui->tableView->setModel(new ST_sun_model(doc));
    } else {
      qout << "Can't do it." << endl;
    }
  } else {
    qout << "Open file failed: no such file: " << filename << endl;
  }
}

void Widget::onDocSelectionChanged (const QItemSelection & selected, const QItemSelection & deselected)
{
  if (!selected.isEmpty())
  {
    QModelIndex index = selected.indexes().first();
    int * ptr = static_cast<int *>(index.internalPointer());
    switch (*ptr) {
    case 1:
      ui->tableView->setModel(new ST_sun_model(myDocModel->doc()));
      for (int column = 0; column < ui->tableView->model()->columnCount(); ++column)
          ui->tableView->resizeColumnToContents(column);
      return;
    case 3:
      ui->tableView->setModel(new ST_stages_model(myDocModel->doc()));
      for (int column = 0; column < ui->tableView->model()->columnCount(); ++column)
          ui->tableView->resizeColumnToContents(column);
      return;
    case 5:
      ui->tableView->setModel(new ST_elements_model(myDocModel->doc(),index.row()));
      for (int column = 0; column < ui->tableView->model()->columnCount(); ++column)
          ui->tableView->resizeColumnToContents(column);
      return;
    default:
      ui->tableView->setModel(0);
    }
  }
}

void Widget::onAddShapeButton()
{
  QItemSelectionModel *treeSelect = ui->treeView->selectionModel(),
      *tableSelect = ui->tableView->selectionModel();

  if (!treeSelect || !tableSelect)
    return;

  QModelIndexList treeRows = treeSelect->selectedRows();
  QModelIndexList tableRows = tableSelect->selectedRows();

  if (!treeRows.isEmpty() && !tableRows.isEmpty()) {
    int whichStage = treeRows.first().row();
    foreach (QModelIndex rowIndex, tableRows) {
      int whichRow = rowIndex.row();
      qDebug() << "Which stage: " << whichStage << ", which row: " << whichRow;
      TopoDS_Shape shape = myDocModel->doc()->stages().at(whichStage).toShape(myDocModel->doc()->path(),whichRow);
      emit addShape(shape);
    }
  }
}

void Widget::onUpdateStageButton()
{
  QItemSelectionModel *treeSelect = ui->treeView->selectionModel();

  if (!treeSelect)
    return;

  QModelIndexList treeRows = treeSelect->selectedRows();

  foreach (QModelIndex qmi, treeRows) {
    qDebug() << "Which stage: " << qmi.row();
    TopoDS_Shape shape = myDocModel->doc()->stages().at(qmi.row()).toShape(myDocModel->doc()->path());
    emit addShape(shape);
  }
}

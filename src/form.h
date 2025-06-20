#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QKeyEvent>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class Form; }
QT_END_NAMESPACE

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();
    void keyPressEvent(QKeyEvent *event) override;  // 实现快捷方式

private:
    Ui::Form *ui;
    QStandardItemModel *model;
    
    void loadDataToTableView();
    void saveDataToFile();
    void findDataSLOT();
    void deleteDataSLOT();
    void addDataSLOT();
    
    bool isDuplicateOrInvalid(const QString &id, int ignoreRow = -1);
};

#endif // FORM_H

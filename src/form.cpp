/**
 * @file form.cpp
 * @brief Implementation of the Form class for managing student data in a Qt application.
 *
 * This file contains the implementation of the Form class, which provides a user interface
 * for displaying, adding, finding, deleting, and saving student information using a QTableView.
 * The data is loaded from and saved to a text file, with each row representing a student's record.
 * The class supports keyboard shortcuts for common operations and ensures data integrity by
 * checking for duplicate or invalid entries.
 *
 * Main functionalities:
 * - Load student data from a file into a QTableView.
 * - Save current data from the table to a file.
 * - Add, find, and delete student records.
 * - Keyboard shortcuts for save (Ctrl+S), find (Ctrl+F), add (Ctrl+N), and delete (Delete key).
 * - Navigation within the table using arrow keys and editing with Enter/Return.
 * - Validation to prevent duplicate or empty student IDs.
 *
 * @author Dolphin_Chan
 * @date 2025-06-10
 */
#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent)
    : QWidget(parent), ui(new Ui::Form)
{
    ui->setupUi(this);
    ui->tableView->setFocus();
    ui->tableView->setEditTriggers(
        QAbstractItemView::SelectedClicked |
        QAbstractItemView::EditKeyPressed |
        QAbstractItemView::DoubleClicked
);
    // 加载数据到 QTableView
    loadDataToTableView();

    connect(ui->saveButton, &QPushButton::clicked, this, &Form::saveDataToFile);
    connect(ui->addstuButton, &QPushButton::clicked, this, &Form::addDataSLOT);
    connect(ui->findButton, &QPushButton::clicked, this, &Form::findDataSLOT);
    connect(ui->delstuButton, &QPushButton::clicked, this, &Form::deleteDataSLOT);
}

Form::~Form()
{
    delete ui;
}

/**
 * @brief Loads student data from a text file into the table view.
 *
 * This function creates a QStandardItemModel, sets up the table headers,
 * and reads student data from "./data/data.txt". Each line in the file
 * should contain six fields separated by Chinese commas (，), corresponding
 * to student number, name, gender, age, province, and major. The data is
 * loaded into the model and displayed in the QTableView. If the file cannot
 * be opened, a warning message is shown. The table view columns are set to
 * automatically stretch to fit the available space.
 */
void Form::loadDataToTableView()
{
    // 创建模型
    model = new QStandardItemModel(this);

    // 设置表头
    model->setHorizontalHeaderLabels(
        {tr("学号"), tr("姓名"), tr("性别"), tr("年龄"), tr("省份"), tr("专业")});

    // 打开文件
    QFile file("./data/data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("错误"), tr("无法打开文件 data.txt"));
        return;
    }

    QTextStream in(&file);

    // 逐行读取文件内容
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList values = line.split("，"); // 使用中文逗号分隔

        if (values.size() == 6)
        { // 确保每行有 6 列数据
            QList<QStandardItem *> items;
            for (const QString &value : values)
            {
                items.append(new QStandardItem(value));
            }
            model->appendRow(items);
        }
    }
    file.close();

    // 将模型设置到 QTableView
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()
        ->setStretchLastSection(true); // 自动调整最后一列宽度
    ui->tableView->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch); // 自动调整列宽
    ui->tableView->setCurrentIndex(model->index(0, 0));
}

/**
 * @brief Saves the data from the model to a text file.
 *
 * This function first checks each row in the model for duplicate or invalid IDs
 * using the isDuplicateOrInvalid() function. If any row contains a duplicate or
 * invalid input, the save operation is aborted.
 *
 * If all rows are valid, the function attempts to open the file "./data/data.txt"
 * for writing. If the file cannot be opened, a warning message is displayed.
 * Otherwise, the function writes each row of the model to the file, with column
 * values separated by the Chinese comma character "，" and each row on a new line.
 *
 * Upon successful completion, an information message is displayed to notify the
 * user that the data has been saved.
 */
void Form::saveDataToFile()
{
    // 检查所有行是否有重复或非法输入
    for (int row = 0; row < model->rowCount(); ++row)
    {
        QString id = model->item(row, 0)->text();
        if (isDuplicateOrInvalid(id, row))
        {
            return; // 有问题则不保存
        }
    }

    QFile file("./data/data.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("错误"), tr("无法保存数据到文件 data.txt"));
        return;
    }

    QTextStream out(&file);
    for (int row = 0; row < model->rowCount(); ++row)
    {
        QStringList values;
        for (int col = 0; col < model->columnCount(); ++col)
        {
            values << model->item(row, col)->text();
        }
        out << values.join("，") << "\n";
    }
    QMessageBox::information(this, tr("保存成功"), tr("数据已成功保存到文件 data.txt"));
    file.close();
}

/**
 * @brief Slot function to search for data in the table model based on user input.
 *
 * This function retrieves the text entered by the user in the FindLineEdit widget,
 * trims any whitespace, and searches for matching entries in the table model.
 * - If the input is empty, a warning message is displayed.
 * - If the input length is greater than 5, the search is performed in the first column (column 0).
 * - Otherwise, the search is performed in the second column (column 1).
 * If a match is found, the corresponding row is selected and scrolled into view,
 * and an information message is shown. If no match is found, a warning is displayed,
 * and the input field is cleared and focused for further input.
 */
void Form::findDataSLOT()
{
    QString searchText = ui->FindLineEdit->text().trimmed();
    QString values;
    bool found = false;
    if (searchText.isEmpty())
    {
        QMessageBox::warning(this, tr("查找失败"), tr("请输入要查找的内容"));
        return;
    }
    if (searchText.length() > 5)
    {
        for (int row = 0; row < model->rowCount(); ++row)
        {
            values = model->item(row, 0)->text();
            if (values.contains(searchText, Qt::CaseInsensitive))
            {
                ui->tableView->setCurrentIndex(model->index(row, 0));
                ui->tableView->scrollTo(model->index(row, 0));
                QMessageBox::information(this, tr("查找结果"), tr("找到匹配的行: %1").arg(values));
                found = true;
                break;
            }
        }
    }
    else
    {
        for (int row = 0; row < model->rowCount(); ++row)
        {
            values = model->item(row, 1)->text();
            if (values.contains(searchText, Qt::CaseInsensitive))
            {
                ui->tableView->setCurrentIndex(model->index(row, 1));
                ui->tableView->scrollTo(model->index(row, 1));
                QMessageBox::information(this, tr("查找结果"), tr("找到匹配的行: %1").arg(values));
                found = true;
                break;
            }
        }
    }
    if (!found)
    {
        QMessageBox::warning(this, tr("查找结果"), tr("未找到该对象"));
        ui->FindLineEdit->clear();
        ui->FindLineEdit->setFocus();
    }
}

/**
 * @brief 删除表格中选中的数据行。
 *
 * 此槽函数用于删除QTableView中用户选中的一行或多行数据。
 * - 如果用户未选择任何行，则弹出警告提示。
 * - 如果用户多选了多行，则优先按行号降序依次删除，避免索引错乱。
 * - 如果只选中了一行，则删除当前选中行。
 * 删除操作完成后，若表格仍有数据，则自动选中最后一行；否则清空选中状态。
 */
void Form::deleteDataSLOT()
{
    QModelIndexList selectedLine = ui->tableView->selectionModel()->selectedRows();
    int select_row = ui->tableView->currentIndex().row();
    if (selectedLine.isEmpty() && select_row < 0)
    {
        QMessageBox::warning(this, tr("删除失败"), tr("请先选择要删除的行"));
        return;
    }
    // 优先删除多选行
    if (!selectedLine.isEmpty())
    {
        // 按行号降序删除，避免索引错乱
        std::sort(selectedLine.begin(), selectedLine.end(), [](const QModelIndex &a, const QModelIndex &b)
                  { return a.row() > b.row(); });
        for (const QModelIndex &index : selectedLine)
        {
            model->removeRow(index.row());
        }
    }
    else if (select_row >= 0 && select_row < model->rowCount())
    {
        model->removeRow(select_row);
    }
    // 删除后设置选中行
    int rowCount = model->rowCount();
    if (rowCount > 0)
    {
        ui->tableView->setCurrentIndex(model->index(rowCount - 1, 0));
    }
    else
    {
        ui->tableView->clearSelection();
    }
}

/**
 * @brief Adds a new empty row to the data model and updates the table view.
 *
 * This slot creates a new row with empty items for each column in the model,
 * appends it to the model, scrolls the table view to the bottom, and sets
 * the current selection to the newly added row.
 */
void Form::addDataSLOT()
{
    QList<QStandardItem *> items;
    for (int i = 0; i < model->columnCount(); ++i)
    {
        items.append(new QStandardItem(""));
    }
    model->appendRow(items);
    ui->tableView->scrollToBottom();
    ui->tableView->setCurrentIndex(model->index(model->rowCount() - 1, 0));
}

/**
 * @brief Handles key press events for the Form.
 *
 * This function overrides the default key press event handler to provide custom
 * keyboard shortcuts and navigation for the form. The following key events are handled:
 * - Ctrl+S: Triggers saving data to file.
 * - Ctrl+F: Focuses the find line edit or triggers the find operation if not empty.
 * - Ctrl+N: Adds a new data entry.
 * - Delete: Deletes the currently selected data.
 * - Arrow keys (Up, Down, Left, Right): Navigates between cells in the table view.
 * - Enter/Return:
 *    - If focus is on the find line edit, triggers the find operation.
 *    - If focus is on the table view, enters edit mode for the current cell.
 *
 * @param event Pointer to the QKeyEvent containing information about the key press.
 */
void Form::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S)
    {
        saveDataToFile();
    }
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_F)
    {
        if (ui->FindLineEdit->text().isEmpty())
        {
            ui->FindLineEdit->setFocus();
        }
        else
        {
            findDataSLOT();
        }
    }
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_N)
    {
        addDataSLOT();
    }
    if (event->key() == Qt::Key_Delete)
    {
        deleteDataSLOT();
    }

    QModelIndex current = ui->tableView->currentIndex();
    int row = current.row();
    int col = current.column();
    if (event->key() == Qt::Key_Up)
    {
        if (row > 0)
            ui->tableView->setCurrentIndex(model->index(row - 1, col));
        return;
    }
    if (event->key() == Qt::Key_Down)
    {
        if (row < model->rowCount() - 1)
            ui->tableView->setCurrentIndex(model->index(row + 1, col));
        return;
    }
    if (event->key() == Qt::Key_Left)
    {
        if (col > 0)
            ui->tableView->setCurrentIndex(model->index(row, col - 1));
        return;
    }
    if (event->key() == Qt::Key_Right)
    {
        if (col < model->columnCount() - 1)
            ui->tableView->setCurrentIndex(model->index(row, col + 1));
        return;
    }

    QWidget *focusWidget = this->focusWidget();

    // 回车键多功能实现
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        // 如果焦点在查找输入框，执行查找
        if (focusWidget == ui->FindLineEdit)
        {
            findDataSLOT();
            return;
        }
        // 如果焦点在表格，进入编辑或跳到下一行
        if (focusWidget == ui->tableView)
        {
            QModelIndex current = ui->tableView->currentIndex();
            if (current.isValid())
            {
                // 进入编辑模式
                ui->tableView->edit(current);
            }
            return;
        }
    }
}

/**
 * @brief Checks if the given ID is invalid (empty) or a duplicate in the model.
 *
 * This function verifies whether the provided student ID is either empty or already exists
 * in the data model, excluding the row specified by @p ignoreRow (useful during editing).
 * If the ID is invalid or duplicated, a warning message box is displayed and the function returns true.
 *
 * @param id The student ID to check.
 * @param ignoreRow The row index to ignore during the duplicate check (typically the row being edited).
 * @return true if the ID is empty or a duplicate; false otherwise.
 */
bool Form::isDuplicateOrInvalid(const QString &id, int ignoreRow)
{
    if (id.trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("非法输入"), tr("学号不能为空！"));
        return true;
    }
    for (int row = 0; row < model->rowCount(); ++row)
    {
        if (row == ignoreRow)
            continue; // 忽略自身（用于编辑时）
        QString curId = model->item(row, 0)->text();
        if (curId == id)
        {
            QMessageBox::warning(this, tr("重复数据"), tr("已存在相同的学号！"));
            ui->tableView->setCurrentIndex(model->index(row, 0));
            return true;
        }
    }
    return false;
}

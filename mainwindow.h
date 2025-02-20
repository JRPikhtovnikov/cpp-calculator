#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "calculator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDigitClicked();
    void onOperationClicked();
    void onEqualClicked();
    void onClearClicked();
    void onDotClicked();
    void onEraseClicked();
    void onSignChangeClicked();
    void onMemorySaveClicked();
    void onMemoryReadClicked();
    void onMemoryClearClicked();

private:
    void updateResultLabel();
    void updateFormulaLabel();
    void updateMemoryLabel();
    Ui::MainWindow *ui;
    Calculator calculator_;
    QString input_number_;
    QString formula_;
    double active_number_;
    Operation current_operation_;
    bool result_ready_;
    double memory_cell_;
    bool memory_saved_;
};

#endif // MAINWINDOW_H

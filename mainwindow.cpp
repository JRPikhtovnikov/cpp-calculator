#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->l_result->setText("0");
    ui->l_memory->setText("");
    ui->l_formula->setText("");

    connect(ui->btn_0, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btn_1, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btn_2, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btn_3, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btn_4, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btn_5, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btn_6, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btn_7, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btn_8, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btn_9, &QPushButton::clicked, this, &MainWindow::onDigitClicked);

    connect(ui->btn_plus, &QPushButton::clicked, this, &MainWindow::onOperationClicked);
    connect(ui->btn_min, &QPushButton::clicked, this, &MainWindow::onOperationClicked);
    connect(ui->btn_mul, &QPushButton::clicked, this, &MainWindow::onOperationClicked);
    connect(ui->btn_div, &QPushButton::clicked, this, &MainWindow::onOperationClicked);
    connect(ui->btn_pow, &QPushButton::clicked, this, &MainWindow::onOperationClicked);

    connect(ui->btn_result, &QPushButton::clicked, this, &MainWindow::onEqualClicked);
    connect(ui->btn_clear, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(ui->btn_dot, &QPushButton::clicked, this, &MainWindow::onDotClicked);
    connect(ui->btn_erase, &QPushButton::clicked, this, &MainWindow::onEraseClicked);
    connect(ui->btn_change, &QPushButton::clicked, this, &MainWindow::onSignChangeClicked);

    connect(ui->btn_ms, &QPushButton::clicked, this, &MainWindow::onMemorySaveClicked);
    connect(ui->btn_mr, &QPushButton::clicked, this, &MainWindow::onMemoryReadClicked);
    connect(ui->btn_mc, &QPushButton::clicked, this, &MainWindow::onMemoryClearClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

QString OperationToString(Operation op) {
    switch (op) {
    case Operation::ADDITION:
        return "+";
    case Operation::SUBTRACTION:
        return "−";
    case Operation::MULTIPLICATION:
        return "×";
    case Operation::DIVISION:
        return "÷";
    case Operation::POWER:
        return "^";
    case Operation::NO_OPERATION:
    default:
        return "";
    }
}

void MainWindow::updateResultLabel() {
    active_number_ = round(active_number_ * 1e1) / 1e1;
    ui->l_result->setText(QString::number(active_number_, 'g', 15));
}

void MainWindow::updateFormulaLabel() {
    ui->l_formula->setText(formula_);
}

void MainWindow::updateMemoryLabel() {
    ui->l_memory->setText(memory_saved_ ? "M" : "");
}

void MainWindow::onDigitClicked() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());

    if (result_ready_) {
        ui->l_formula->clear();
        formula_.clear();
        result_ready_ = false;
    }

    input_number_ += button->text();
    active_number_ = input_number_.toDouble();
    updateResultLabel();
}

void MainWindow::onOperationClicked() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());

    if (result_ready_) {
        formula_ = QString::number(active_number_) + " ";
        result_ready_ = false;
    }

    if (!input_number_.isEmpty()) {
        formula_ += input_number_ + " ";
        calculator_.Set(input_number_.toDouble());
        input_number_.clear();
    }

    if (button == ui->btn_plus) {
        current_operation_ = Operation::ADDITION;
    } else if (button == ui->btn_min) {
        current_operation_ = Operation::SUBTRACTION;
    } else if (button == ui->btn_mul) {
        current_operation_ = Operation::MULTIPLICATION;
    } else if (button == ui->btn_div) {
        current_operation_ = Operation::DIVISION;
    } else if (button == ui->btn_pow) {
        current_operation_ = Operation::POWER;
    }

    formula_ += OperationToString(current_operation_) + " ";

    updateFormulaLabel();
}

void MainWindow::onEqualClicked() {
    if (current_operation_ == Operation::NO_OPERATION) {
        return;
    }

    if (!input_number_.isEmpty()) {
        active_number_ = input_number_.toDouble();
        input_number_.clear();
    }

    switch (current_operation_) {
    case Operation::ADDITION:
        calculator_.Add(active_number_);
        break;
    case Operation::SUBTRACTION:
        calculator_.Sub(active_number_);
        break;
    case Operation::MULTIPLICATION:
        calculator_.Mul(active_number_);
        break;
    case Operation::DIVISION:
        calculator_.Div(active_number_);
        break;
    case Operation::POWER:
        calculator_.Pow(active_number_);
        break;
    default:
        break;
    }

    formula_ += QString::number(active_number_) + " =";
    updateFormulaLabel();

    active_number_ = calculator_.GetNumber();
    updateResultLabel();

    current_operation_ = Operation::NO_OPERATION;
    result_ready_ = true;
}

void MainWindow::onClearClicked() {
    input_number_.clear();
    active_number_ = 0;
    formula_.clear();
    current_operation_ = Operation::NO_OPERATION;
    calculator_.Set(0);
    updateResultLabel();
    ui->l_formula->setText("");
    result_ready_ = false;
}

void MainWindow::onDotClicked() {
    if (!input_number_.contains('.')) {
        input_number_ += ".";
        updateResultLabel();
    }
}

void MainWindow::onEraseClicked() {
    if (!input_number_.isEmpty()) {
        input_number_.chop(1);
        active_number_ = input_number_.isEmpty() ? 0 : input_number_.toDouble();
        updateResultLabel();
    }
}

void MainWindow::onSignChangeClicked() {
    active_number_ *= -1;
    input_number_ = QString::number(active_number_);
    updateResultLabel();
}

void MainWindow::onMemorySaveClicked() {
    memory_cell_ = active_number_;
    memory_saved_ = true;
    updateMemoryLabel();
}

void MainWindow::onMemoryReadClicked() {
    if (memory_saved_) {
        input_number_ = QString::number(memory_cell_);
        active_number_ = memory_cell_;
        updateResultLabel();
        result_ready_ = false;
    }
}


void MainWindow::onMemoryClearClicked() {
    memory_cell_ = 0;
    memory_saved_ = false;
    updateMemoryLabel();
}


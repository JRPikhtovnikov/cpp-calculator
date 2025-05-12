#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QPushButton>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    for (int digit = 0; digit <= 9; ++digit) {
        QString btnName = QString("btn_%1").arg(digit);
        QPushButton *digitButton = findChild<QPushButton *>(btnName);
        if (digitButton) {
            connect(digitButton, &QPushButton::clicked,
                    this, &MainWindow::OnDigitButtonClicked);
        }
    }

    connect(ui->btn_plus, &QPushButton::clicked, this, &MainWindow::OnOperationButtonClicked);
    connect(ui->btn_min, &QPushButton::clicked, this, &MainWindow::OnOperationButtonClicked);
    connect(ui->btn_mul, &QPushButton::clicked, this, &MainWindow::OnOperationButtonClicked);
    connect(ui->btn_div, &QPushButton::clicked, this, &MainWindow::OnOperationButtonClicked);
    connect(ui->btn_pow, &QPushButton::clicked, this, &MainWindow::OnOperationButtonClicked);
    connect(ui->btn_clear, &QPushButton::clicked, this, &MainWindow::OnControlButtonClicked);
    connect(ui->btn_result, &QPushButton::clicked, this, &MainWindow::OnControlButtonClicked);
    connect(ui->btn_change, &QPushButton::clicked, this, &MainWindow::OnControlButtonClicked);
    connect(ui->btn_erase, &QPushButton::clicked, this, &MainWindow::OnControlButtonClicked);
    connect(ui->btn_ms, &QPushButton::clicked, this, &MainWindow::OnControlButtonClicked);
    connect(ui->btn_mr, &QPushButton::clicked, this, &MainWindow::OnControlButtonClicked);
    connect(ui->btn_mc, &QPushButton::clicked, this, &MainWindow::OnControlButtonClicked);
    connect(ui->tb_extra, &QPushButton::clicked, this, &MainWindow::OnControlButtonClicked);
    ui->tb_extra->setVisible(false);
    connect(ui->cmb_controller, &QComboBox::currentIndexChanged, this, &MainWindow::OnControllerChanged);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SetInputText(const std::string &text) {
    ui->l_result->setText(QString::fromStdString(text));
    ui->l_result->setStyleSheet("");
}

void MainWindow::SetErrorText(const std::string &text) {
    ui->l_result->setText(QString::fromStdString(text));
    ui->l_result->setStyleSheet("color: red;");
}

void MainWindow::SetFormulaText(const std::string& text) {
    ui->l_formula->setText(QString::fromStdString(text));
}

void MainWindow::SetMemText(const std::string &text) {
    ui->l_memory->setText(QString::fromStdString(text));
}

void MainWindow::SetExtraKey(const std::optional<std::string> &key) {
    if (key) {
        ui->tb_extra->setText(QString::fromStdString(*key));
        ui->tb_extra->setVisible(true);
    } else {
        ui->tb_extra->setVisible(false);
    }
}

void MainWindow::SetDigitKeyCallback(std::function<void(int)> cb) {
    digit_cb_ = std::move(cb);
}

void MainWindow::SetProcessOperationKeyCallback(std::function<void(Operation)> cb) {
    operation_cb_ = std::move(cb);
}

void MainWindow::SetProcessControlKeyCallback(std::function<void(ControlKey)> cb) {
    control_cb_ = std::move(cb);
}

void MainWindow::SetControllerCallback(std::function<void(ControllerType)> cb) {
    controller_cb_ = std::move(cb);
}

void MainWindow::OnControllerChanged() {
    if (controller_cb_) {
        controller_cb_(static_cast<ControllerType>(ui->cmb_controller->currentIndex()));
    }
}

void MainWindow::OnDigitButtonClicked() {
    if (!digit_cb_) return;

    auto button = qobject_cast<QPushButton *>(sender());
    if (!button) return;

    bool ok = false;
    int digit = button->text().toInt(&ok);
    if (ok) {
        digit_cb_(digit);
    }
}

void MainWindow::OnOperationButtonClicked() {
    if (!operation_cb_) return;

    auto button = qobject_cast<QPushButton *>(sender());
    if (!button) return;

    QString txt = button->text();
    if (txt == "+") {
        operation_cb_(Operation::ADDITION);
    } else if (txt == "−") {
        operation_cb_(Operation::SUBTRACTION);
    } else if (txt == "×") {
        operation_cb_(Operation::MULTIPLICATION);
    } else if (txt == "÷") {
        operation_cb_(Operation::DIVISION);
    } else if (txt == "xʸ") {
        operation_cb_(Operation::POWER);
    }
}

void MainWindow::OnControlButtonClicked() {
    if (!control_cb_) return;

    auto button = qobject_cast<QPushButton *>(sender());
    if (!button) return;

    QString txt = button->text();
    if (txt == "C") {
        control_cb_(ControlKey::CLEAR);
    } else if (txt == "=") {
        control_cb_(ControlKey::EQUALS);
    } else if (txt == "MS") {
        control_cb_(ControlKey::MEM_SAVE);
    } else if (txt == "MR") {
        control_cb_(ControlKey::MEM_LOAD);
    } else if (txt == "MC") {
        control_cb_(ControlKey::MEM_CLEAR);
    } else if (txt == "±") {
        control_cb_(ControlKey::PLUS_MINUS);
    } else if (txt == "⌫") {
        control_cb_(ControlKey::BACKSPACE);
    } else {
        control_cb_(ControlKey::EXTRA_KEY);
    }
}

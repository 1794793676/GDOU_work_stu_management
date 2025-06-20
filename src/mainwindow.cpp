/*
 * @file mainwindow.cpp
 * @brief Implementation of the Widget class for a simple login interface.
 *
 * This file defines the Widget class, which provides a login form with account and password input fields.
 * The login credentials are checked against hardcoded values ("admin" and "12345678").
 * Upon successful login, the main window closes and a new Form window is displayed.
 *
 * Key Features:
 * - Connects input fields and login button to the login handler.
 * - Validates user input for account and password fields.
 * - Displays appropriate message boxes for input errors, login success, or login failure.
 * - Clears and focuses input fields as needed based on user actions.
 *
 * Dependencies:
 * - mainwindow.h
 * - form.h
 * - ui_mainwindow.h
 * - Qt Widgets (QWidget, QLineEdit, QPushButton, QMessageBox)
 *
 * Usage:
 * - Instantiate Widget as the main window.
 * - User enters account and password, then presses Enter or clicks the login button.
 * - On successful login, a new Form window is shown.
 */
#include "mainwindow.h"
#include "form.h"
#include "ui_mainwindow.h"

#define ACCOUNT "admin"
#define PWD 12345678

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->AccountInput, &QLineEdit::returnPressed, this, &Widget::on_AccountInput_returnPressed);
    connect(ui->PwdInput, &QLineEdit::returnPressed, this, &Widget::on_AccountInput_returnPressed);
    connect(ui->loginButton, &QPushButton::clicked, this, &Widget::on_AccountInput_returnPressed);
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief Slot triggered when the user presses Enter in the account input field.
 *
 * This function handles the login process by validating the account and password fields.
 * - If the account field is empty, it prompts the user to enter an account.
 * - If the password field is empty, it sets focus to the password input.
 * - If both fields are filled, it checks the credentials:
 *   - On success, displays a welcome message, closes the current widget, and opens the main form.
 *   - On failure, notifies the user of invalid credentials and clears the input fields.
 */
void Widget::on_AccountInput_returnPressed()
{
    QString account = ui->AccountInput->text();
    QString password = ui->PwdInput->text();
    if(account.isEmpty()) {
        ui->PwdInput->clear();
        ui->AccountInput->setFocus();
        QMessageBox::warning(this, tr("Input Error"), tr("Please enter your account."));
    }
    else if(password.isEmpty()) {
        ui->PwdInput->setFocus();
    } else {
        if(account == QString::fromLatin1(ACCOUNT) && password == QString::number(PWD)) {
            QMessageBox::information(this, tr("Login Successful"), tr("Welcome!"));
            this->close();
            Form *chart = new Form();
            chart->show();
            chart->setFocus();
        } else {
            QMessageBox::warning(this, tr("Login Failed"), tr("Invalid account or password."));
            ui->AccountInput->clear();
            ui->PwdInput->clear();
            ui->AccountInput->setFocus();
        }
    }
}

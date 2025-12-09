#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QFileDialog"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_BrowseBtn_clicked()
{
    filePath = QFileDialog::getOpenFileName(this, "Open Text File", "", "XML Files (*.txt);;All Files (*)");
    if (!filePath.isEmpty()) {
        // Load and display file content
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            ui->filePathEdit->setText(filePath);
            QTextStream in(&file);
            ui->fileContent->setPlainText(in.readAll());
            file.close();
        } else {
            QMessageBox::warning(this, "Error", "Unable to open the file.");
        }
    }
}


void MainWindow::on_clearButton_clicked()
{
    filePath = "";
    ui->filePathEdit->clear();
    ui->fileContent->clear();
    ui->imageLabel_2->clear();
    ui->imageLabel_2->setText("                      Image wil be displayed here                  ");
}


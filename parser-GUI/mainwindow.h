#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "include/ASTNode.hpp"
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_BrowseBtn_clicked();

    void on_clearButton_clicked();

    void on_parseBtn_clicked();

    void drawTree(ASTNode *root);

    void drawNode(ASTNode *node);

    void highlightErrorLine(int lineNumber);

    QString getNodeLabel(ASTNode *node);

private:
    Ui::MainWindow *ui;
    QString filePath;
    QGraphicsScene *scene;

};
#endif // MAINWINDOW_H

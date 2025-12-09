#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QFileDialog"
#include "QMessageBox"
#include "include/token.hpp"
#include "include/tokenReader.hpp"
#include "include/tokenStream.hpp"
#include "include/parser.hpp"
#include "include/TreeLayout.hpp"
#include <vector>
#include <QDebug>
#include <QFile>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPen>
#include <QSplitter>
#include <QTextStream>
#include <QVBoxLayout>
#include <QPainter>

static const int NODE_WIDTH = 110;
static const int NODE_HEIGHT = 60;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
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


void MainWindow::on_parseBtn_clicked()
{
    ui->imageLabel_2->clear();
    try {
        vector<Token> tokens = readTokensFromFile(filePath.toStdString());
        TokenStream ts(tokens);
        Parser parser(ts);
        ASTNode *root = parser.parseProgram();
        if (root){
            TreeLayout::calculateLayout(root);
            drawTree(root);
        }
    } catch (std::exception e) {
        QMessageBox::critical(this, "Error", e.what());
        ui->imageLabel_2->setText("                      Image wil be displayed here                  ");
    }
}

void MainWindow::drawTree(ASTNode *root)
{
    scene->clear();

    drawNode(root);

    QRectF bounds = scene->itemsBoundingRect();
    scene->setSceneRect(bounds);

    // Render scene to pixmap
    QImage image(bounds.size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    scene->render(&painter);

    // Now scale to fit QLabel
    QPixmap pix = QPixmap::fromImage(image);
    QSize targetSize = ui->imageLabel_2->size();
    QPixmap scaled = pix.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->imageLabel_2->setPixmap(scaled);
}

void MainWindow::drawNode(ASTNode *node) {
    if (!node)
        return;

    // Draw the node itself
    QRectF rect(node->x, node->y, NODE_WIDTH, NODE_HEIGHT);

    // Custom Shape Logic
    QAbstractGraphicsShapeItem *shapeItem = nullptr;
    bool isRect = true;

    switch (node->type) {
    case ASTNodeType::Number:
        isRect = false;
        break;
    case ASTNodeType::Identifier:
        isRect = false;
        break;
    case ASTNodeType::OP:
        isRect = false;
        break;
    default:
        isRect = true;
        break;
    }

    if (isRect) {
        shapeItem = scene->addRect(rect);
    } else {
        shapeItem = scene->addEllipse(rect);
    }

    if (shapeItem) {
        shapeItem->setBrush(Qt::lightGray);
        shapeItem->setPen(QPen(Qt::black));
    }

    // Text: Type \n (Value)
    QString typeStr = getNodeLabel(node);
    QString valStr = QString::fromStdString(node->value);
    QString label;

    if(typeStr != "Write" && typeStr != "If" && typeStr != "Repeat"){
        label = typeStr + "\n(" + valStr + ")";
    }else{
        label = typeStr;
    }

    QGraphicsTextItem *textItem = scene->addText(label);
    QFont font = textItem->font();
    font.setPointSize(14);
    textItem->setFont(font);

    // Center text
    // For ellipses, centering in the bounding rect is usually "good enough"
    // visually
    QRectF textRect = textItem->boundingRect();
    textItem->setPos(node->x + (NODE_WIDTH - textRect.width()) / 2,
                     node->y + (NODE_HEIGHT - textRect.height()) / 2);

    // Line Pen (Blue)
    QPen linePen(Qt::blue);
    linePen.setWidth(2); // slightly thicker for visibility

    // Draw Lines to Children
    for (ASTNode *child : node->children) {
        if (child) {
            QPointF refreshStart(node->x + NODE_WIDTH / 2, node->y + NODE_HEIGHT);
            QPointF refreshEnd(child->x + NODE_WIDTH / 2, child->y);
            scene->addLine(QLineF(refreshStart, refreshEnd), linePen);

            drawNode(child);
        }
    }

    // Draw Lines to Siblings
    ASTNode *prev = node;
    for (ASTNode *sib : node->sibling) {
        if (sib) {
            QPointF start(prev->x + NODE_WIDTH, prev->y + NODE_HEIGHT / 2);
            QPointF end(sib->x, sib->y + NODE_HEIGHT / 2);
            scene->addLine(QLineF(start, end), linePen);

            drawNode(sib);
            prev = sib;
        }
    }
}


QString MainWindow::getNodeLabel(ASTNode *node)
{
    switch (node->type) {
        case ASTNodeType::IfStmt:
            return "If";
        case ASTNodeType::ReadStmt:
            return "Read";
        case ASTNodeType::WriteStmt:
            return "Write";
        case ASTNodeType::AssignStmt:
            return "Assign";
        case ASTNodeType::RepeatStmt:
            return "Repeat";
        case ASTNodeType::OP:
            return "op";
        case ASTNodeType::Identifier:
            return "id";
        case ASTNodeType::Number:
            return "const";
    }
    return "?";
}


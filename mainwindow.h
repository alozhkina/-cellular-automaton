#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QWidget>
#include <QVector>
#include "cells.h"
#include <bitset>
#include <QMap>
#include <QString>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void createState(int row, int col);

private slots:
    void createGrid();  // Слот для создания сетки
    void changeStates();
    void randomFill();

private:
    QSpinBox *mSpinBox;  // Поле для ввода m
    QSpinBox *nSpinBox;  // Поле для ввода n
    QPushButton *generateButton;  // Кнопка для генерации сетки
    QPushButton *NewGenerateButton; //Кнопка нового состояния
    QPushButton *RandomButton; //Кнопка нового состояния
    QWidget *gridWidget;  // Центральный виджет для сетки
    QGridLayout *gridLayout;  // Сетка


    QVector<QVector<Cells*>> NeumannCells;
    void clearGrid();  // Метод для очистки сетки перед созданием новой

    QString checkNeighbors(int row, int col);
};
#endif // MAINWINDOW_H

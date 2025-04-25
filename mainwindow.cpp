#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cells.h"
#include <QDebug>
#include <iostream>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mSpinBox(new QSpinBox(this)), nSpinBox(new QSpinBox(this)), generateButton(new QPushButton("Generate Grid", this)),
      NewGenerateButton(new QPushButton("Generate new state", this)), RandomButton(new QPushButton("Generate random state"))

{// Настройка интерфейса для ввода m и n
    mSpinBox->setRange(2, 20);
    nSpinBox->setRange(2, 30);
//cначала кнопки выключены
    NewGenerateButton->setEnabled(false);
    RandomButton->setEnabled(false);

    QLabel *mLabel = new QLabel("m:", this);
    QLabel *nLabel = new QLabel("n:", this);

    // Layout для ввода m, n и кнопки
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(mLabel);
    inputLayout->addWidget(mSpinBox);
    inputLayout->addWidget(nLabel);
    inputLayout->addWidget(nSpinBox);
    inputLayout->addWidget(generateButton);
    QHBoxLayout *buttonLayoutRow1 = new QHBoxLayout;
    buttonLayoutRow1->addWidget(RandomButton);
    buttonLayoutRow1->addWidget(NewGenerateButton);

    inputLayout->setAlignment(Qt::AlignLeft);
    buttonLayoutRow1->setAlignment(Qt::AlignLeft);
    // Виджет для сетки
    gridWidget = new QWidget(this);
    gridLayout = new QGridLayout(gridWidget);

    // Основной layout для окна
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(buttonLayoutRow1);
    mainLayout->addWidget(gridWidget);

    setLayout(mainLayout);
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Подключаем кнопку к слоту создания сетки
    connect(generateButton, &QPushButton::clicked, this, &MainWindow::createGrid);
    connect(NewGenerateButton, &QPushButton::clicked, this, &MainWindow::changeStates);
    connect(RandomButton, &QPushButton::clicked, this, &MainWindow::randomFill);

}

MainWindow::~MainWindow()
{
}
void MainWindow::createGrid() {
    int m = mSpinBox->value();
    int n = nSpinBox->value();

    clearGrid();  // Удаляем старую сетку, если она существует
    NeumannCells.resize(m);
    // Заполняем сетку новыми клетками (QLabel)
    for (int i = 0; i < m; ++i) {
        NeumannCells[i].resize(n);
        for (int j = 0; j < n; ++j) {
            Cells *cell = new Cells(this);  // Используем Cells
            gridLayout->addWidget(cell, i, j);
            NeumannCells[i][j] = cell;
        }
    }
    // Включаем кнопки, когда сетка создана
        NewGenerateButton->setEnabled(true);
        RandomButton->setEnabled(true);
}
//QString MainWindow::checkNeighbors(int row, int col) {

//    QString neighbors;
//    neighbors.append(NeumannCells[row-1][col]->isSelected() ? 1 : 0);
//    neighbors.append(NeumannCells[row+1][col]->isSelected() ? 1 : 0);
//    neighbors.append(NeumannCells[row][col-1]->isSelected() ? 1 : 0);
//    neighbors.append(NeumannCells[row][col+1]->isSelected() ? 1 : 0);
//        // Проверка соседей сверху, снизу, слева и справа
//      //  if (row > 0) neighbors.append(QPoint(row -1, col));  // сверху
//      //  if (row < m - 1) neighbors.append(QPoint(row + 1, col));  // снизу
//      //  if (col > 0) neighbors.append(QPoint(row, col - 1));  // слева
//      //  if (col < n - 1) neighbors.append(QPoint(row, col + 1));  // справа

//        return neighbors;
//}


//void MainWindow::createState(int row, int col) {
//    int rule = 23795640;  // Правило, представленное числом
//    std::bitset<32> RULE (rule);
//    QMap<QString, bool> table;//таблица истинности
//    for (int r = 0; r<32;r++){
//        QString key;
//        std::bitset<5> bin = r;
//        for (int rj = 0; rj<5; rj++)
//            key.at(bin[rj]);
//        table[key] = RULE[r];
//    }

//    //создали вектор соседей и с0
//    for (int i = 0; i<row; i++){
//        for (int j = 0; j<col; j++){
//            QString neighbors = checkNeighbors(row, col);
//            neighbors.prepend(NeumannCells[row][col]->isSelected() ? 1 : 0);
//            //решаем что с новым состоянием
//            NeumannCells[row][col]->setSelected(table[neighbors]);
//        }
//    }


//}

void MainWindow::createState(int rows, int cols) {
    int rule = 23795640;  // Правило, представленное числом
    std::bitset<32> RULE(rule);
    std::string RULE1 = RULE.to_string();
    // Создаём временное хранилище для новых состояний ячеек
    QVector<QVector<bool>> newStates(rows, QVector<bool>(cols, false));

    // Пробегаемся по каждой ячейке и вычисляем её новое состояние
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            QString neighbors = checkNeighbors(i, j);

            // Преобразуем строку соседей в целое число (индекс)
            std::string binary = neighbors.toStdString();
            unsigned int decimal = std::bitset<5>(binary).to_ulong();


            // Используем бит из `RULE` для нового состояния ячейки

            int newState= std::bitset<1>(RULE1[decimal]).to_ulong();
            // Сохраняем новое состояние во временное хранилище
            newStates[i][j] = newState;

        }
    }

    // Применяем новые состояния к ячейкам
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            NeumannCells[i][j]->setSelected(newStates[i][j]);

        }
    }
}

QString MainWindow::checkNeighbors(int row, int col) {
    QString neighbors;
    int m = NeumannCells.size();
    int n = NeumannCells[0].size();
    // Центральная ячейка
    neighbors.append(NeumannCells[row][col]->isSelected() ? "1" : "0");

    // Сосед сверху
    if (row > 0) {
        neighbors.append(NeumannCells[row - 1][col]->isSelected() ? "1" : "0");
    } else {
        neighbors.append(NeumannCells[m-1][col]->isSelected() ? "1" : "0");;
    }

    // Сосед снизу
    if (row < NeumannCells.size() - 1) {
        neighbors.append(NeumannCells[row + 1][col]->isSelected() ? "1" : "0");
    } else {
        neighbors.append(NeumannCells[0][col]->isSelected() ? "1" : "0");;
    }

    // Сосед слева
    if (col > 0) {
        neighbors.append(NeumannCells[row][col - 1]->isSelected() ? "1" : "0");
    } else {
       neighbors.append(NeumannCells[row][n-1]->isSelected() ? "1" : "0");
    }

    // Сосед справа
    if (col < NeumannCells[0].size() - 1) {
        neighbors.append(NeumannCells[row][col + 1]->isSelected() ? "1" : "0");
    } else {
         neighbors.append(NeumannCells[row][0]->isSelected() ? "1" : "0");
    }

    return neighbors;  // Вернёт строку из 5 символов (битов), например "11010"
}


void MainWindow::changeStates() {
    int apr = 0;
    int m = NeumannCells.size();
    int n = NeumannCells[0].size();
    for (int i = 0; i<m; i++){
        for (int j = 0; j<n; j++){
            if (NeumannCells[i][j]->isSelected())
                apr++;
        }
    }
    std::ofstream file("C:\\Users\\Anastasiya\\Documents\\lab1aut\\Komi.txt", std::ios::app); // std::ios::app — режим дозаписи
        if (file.is_open()) {
            file << apr << " ";  // Записываем число живых клеток в файл
            file.close();
            qDebug() << "Данные успешно добавлены в файл.";
        } else {
            qDebug() << "Ошибка: файл не удалось открыть для дозаписи.";
        }
    if (m > 1 && n > 1) {

            createState(m, n);
        }


}


void MainWindow::randomFill(){
    srand(time(0));
    int m = NeumannCells.size();
    int n = NeumannCells[0].size();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
           int stat = rand() % 2;
            NeumannCells[i][j]->setSelected(stat);

        }
    }


}
//для будущей гистограммы
void MainWindow::clearGrid() {
    QLayoutItem *item;
      while ((item = gridLayout->takeAt(0)) != nullptr) {
          delete item->widget();
          delete item;
      }
      std::ofstream file("C:\\Users\\Anastasiya\\Documents\\lab1aut\\Komi.txt", std::ios::app); // std::ios::app — режим дозаписи
          if (file.is_open()) {
              file << "\n";  // Записываем число живых клеток в файл
              file.close();
              qDebug() << "Данные успешно добавлены в файл.";
          } else {
              qDebug() << "Ошибка: файл не удалось открыть для дозаписи.";
          }
      NeumannCells.clear();
}



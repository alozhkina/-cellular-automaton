#ifndef CELLWIDGET_H
#define CELLWIDGET_H

#include <QLabel>
#include <QMouseEvent>
#include <QColor>
#include <QString>
#include <iostream>
#include <QDebug>

class Cells : public QLabel
{
    Q_OBJECT

public:
    explicit Cells(QWidget *parent = nullptr);

    bool isSelected() const;  // Метод для проверки состояния выделения
    void setSelected(bool selected);  // Метод для установки состояния выделения

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    bool m_isSelected;  // Флаг выделения клетки
    void updateColor();  // Метод для обновления цвета клетки

};

#endif // CELLWIDGET_H

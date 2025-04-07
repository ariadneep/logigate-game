#ifndef GRIDSQUARE_H
#define GRIDSQUARE_H

#include <QObject>

class GridSquare : public QObject
{
    Q_OBJECT
public:
    explicit GridSquare(QObject *parent = nullptr);

signals:
};

#endif // GRIDSQUARE_H

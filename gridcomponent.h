#ifndef GRIDCOMPONENT_H
#define GRIDCOMPONENT_H

#include <QObject>

class GridComponent : public QObject {

    Q_OBJECT

    public:
        explicit GridComponent(QObject *parent = nullptr);

    signals:

};

#endif // GRIDCOMPONENT_H

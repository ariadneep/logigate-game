#ifndef LEVEL_H
#define LEVEL_H

#include <QObject>

enum class Component {
    WIRE_TOPRIGHT, WIRE_TOPBOTTOM, WIRE_TOPLEFT, WIRE_TOPEND,
    WIRE_RIGHTTOP, WIRE_RIGHTBOTTOM, WIRE_RIGHTLEFT, WIRE_RIGHTEND
};

class Level : public QObject {

    Q_OBJECT

    public:
        explicit Level(int width, QObject *parent = nullptr);
        ~Level();

    private:
        const int WIDTH;
        Component* grid;
        QString description;

    signals:

};

#endif // LEVEL_H

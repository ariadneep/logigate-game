#ifndef GATE_H
#define GATE_H

#include <QObject>
#include "gridcomponent.h"

class Gate : public GridComponent {

    public:
        explicit Gate(QObject *parent = nullptr);

};

#endif // GATE_H

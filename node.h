#ifndef NODE_H
#define NODE_H

#include <QObject>
#include "gridcomponent.h"

class Node : public GridComponent {

    public:
        explicit Node(QObject *parent = nullptr);

};

#endif // NODE_H

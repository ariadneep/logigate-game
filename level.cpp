#include "level.h"

enum class Component;

Level::Level(int width, QObject *parent)
    : QObject{parent}
    , WIDTH(width) {
    //levelNum = 0;
}

Level::~Level() {

}



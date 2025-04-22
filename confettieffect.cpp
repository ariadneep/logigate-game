/*
 * Sources Used:
 * https://stackoverflow.com/questions/4063549/box2d-rotate-an-object-how
 * https://doc.qt.io/qt-6/qrandomgenerator.html
 * https://stackoverflow.com/questions/4360084/creating-complex-shapes-with-box2d
 * https://www.qt.io/blog/2010/02/26/qt-box2d-is-easy#:~:text=This%20example%20shows%20how%20to%20create%20a%20body%2C,simply%20don%27t%20move.%20By%20default%2C%20bodies%20are%20static.
 * https://www.qt.io/blog/2010/02/26/qt-box2d-is-easy
 * https://doc.qt.io/qt-6/qgraphicsscene.html
 *
 * For rendering and display:
 * https://runebook.dev/en/articles/qt/qgraphicsscene
 * https://stackoverflow.com/questions/50253360/how-to-set-central-widget-fill-in-the-whole-main-window-in-qt
 */

#include "confettieffect.h"
#include <QGraphicsRectItem>
#include <QPainter>
#include <QRandomGenerator>

ConfettiEffect::ConfettiEffect(float xPos, float yPos, b2World* box2DWorld) : xPos(xPos), yPos(yPos) {
    confettiColor = QColor(
        QRandomGenerator::global()->bounded(256),
        QRandomGenerator::global()->bounded(256),
        QRandomGenerator::global()->bounded(256)
        );

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(xPos / 30.0f, (yPos / 30.0f) - 7.5);
    box2DBody = box2DWorld->CreateBody(&bodyDef);

    b2PolygonShape box2DShape;
    box2DShape.SetAsBox(0.1f, 0.05f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box2DShape;
    fixtureDef.density = 0.1f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.2f;
    box2DBody->CreateFixture(&fixtureDef);


}

Confetti::Confetti(QGraphicsScene* graphicsScene, b2World* box2DWorld) : graphicsScene(graphicsScene), box2DWorld(box2DWorld), existingConfetti(0) {
    for(int i = 0; i < 30; i++) {
        confettiParticles[i] = nullptr;
        rectItems[i] = nullptr;
    }
}

Confetti::~Confetti() {
    for(int i = 0; i < existingConfetti; i++) {
        delete confettiParticles[i];
        delete rectItems[i];
    }
}

void Confetti::spawnConfetti() {
    //Spawn confetti at the center of the screen.
    for(int i = 0; i < 30; i++) {
        ConfettiEffect* confettiParticle = new ConfettiEffect(0, 0, box2DWorld);
        QGraphicsRectItem* confettiRect = new QGraphicsRectItem(-5, -2.5, 10, 5);

        confettiRect->setPos(0, 0);
        confettiRect->setBrush(confettiParticle->confettiColor);
        graphicsScene->addItem(confettiRect);
        confettiParticles[i] = confettiParticle;
        rectItems[i] = confettiRect;
        existingConfetti++;
    }
}

void Confetti::updateConfetti() {
    for(int i = 0; i < 30; i++) {
        ConfettiEffect* confettiParticle = confettiParticles[i];
        QGraphicsRectItem* graphicRects = rectItems[i];
        if(confettiParticle && graphicRects) {
            confettiParticle->xPos = confettiParticle->box2DBody->GetPosition().x * 30.0f;
            confettiParticle->yPos = confettiParticle->box2DBody->GetPosition().y * 30.0f;
            graphicRects->setPos(confettiParticle->xPos, confettiParticle->yPos);
        }
    }
}

void Confetti::clearConfetti() {
    for(int i = 0; i < existingConfetti; i++) {
        if(rectItems[i]) {
            graphicsScene->removeItem(rectItems[i]);
            delete rectItems[i];
            rectItems[i] = nullptr;
        }
        if(confettiParticles[i]) {
            if(confettiParticles[i]->box2DBody) {
                box2DWorld->DestroyBody(confettiParticles[i]->box2DBody);
                confettiParticles[i]->box2DBody = nullptr;
            }
            delete confettiParticles[i];
            confettiParticles[i] = nullptr;
        }
    }
    existingConfetti = 0;
}

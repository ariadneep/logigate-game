/**
 * This class manages the confetti effect upon a level victory, in which confetti
 * particles will spawn on screen to indicate the player has won the level.
 *
 * @authors Max Donaldson, Jason Khong,
 * Ariadne Petroulakis, Evan Tucker, and Ian Weller
 *
 * @version 7 April 2025
 */

#ifndef CONFETTIEFFECT_H
#define CONFETTIEFFECT_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <Box2D/Box2D.h>

class ConfettiEffect {

public:
    /**
     * @brief ConfettiEffect - The default ConfettiEffect constructor
     * @param x - the x position of the confetti
     * @param y - the y position of the confetti
     * @param box2DWorld - The Box2D world.
     */
    ConfettiEffect(float x, float y, b2World* box2DWorld);

    /**
     * @brief xPos - The x position of the confetti particle.
     */
    float xPos;

    /**
     * @brief yPos - The y position of the confetti particle.
     */
    float yPos;

    /**
     * @brief confettiColor - Manages the color of the confetti particle.
     */
    QColor confettiColor;

    /**
     * @brief confettiOpacity - Manages the opacity of the confetti particle.
     */
    float confettiOpacity;

    /**
     * @brief box2DBody - The Box2D body
     */
    b2Body* box2DBody;

};

class Confetti {

public:
    /**
     * @brief Confetti - default constructor for internal class.
     * @param scene - Manages the scene
     * @param box2DWorld - Manages the Box2D world.
     */
    Confetti(QGraphicsScene* graphicsScene, b2World* box2DWorld);

    /**
     * Destructor for removing confetti.
     */
    ~Confetti();

    /**
     * @brief spawnConfetti - Manages spawning the confetti particles.
     */
    void spawnConfetti();

    /**
     * @brief updateConfetti - Manages updating the confetti particles.
     */
    void updateConfetti();

private:
    /**
     * @brief graphicsScene - Manages the scene where confetti particles spawn.
     */
    QGraphicsScene* graphicsScene;

    /**
     * @brief box2DWorld - The Box2D world.
     */
    b2World* box2DWorld;

    /**
     * @brief confettiParticles - Manages the total amount of confetti particles.
     */
    ConfettiEffect* confettiParticles[30];

    /**
     * @brief rectItems - Manages the confetti visualization.
     */
    QGraphicsRectItem* rectItems[30];

    /**
     * @brief confettiOpacities - Manages the confetti opacities.
     */
    float confettiOpacities[30];

    /**
     * @brief existingConfetti - Manages the confetti particles which still exist in scene.
     */
    int existingConfetti;
};

#endif // CONFETTIEFFECT_H

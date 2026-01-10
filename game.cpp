#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

int screenX = 1280;
int screenY = 720;

// scaling sprites so that both sprites have the same size
float targetHeight = 300;

// player 1
float player1Scale = targetHeight / 302;
int player1Height = 302 * player1Scale;
int player1Width = 132 * player1Scale;

// player 2
float player2Scale = targetHeight / 170;
int player2Height = 170 * player2Scale;
int player2Width = 102 * player2Scale;

float speed = 5.0f;
float gravity = 1.0f;
float velocity1 = 0;
float velocity2 = 0;
float offsetY1 = 0;
float offsetY2 = 0;
float jumpstrength = -20.0f;
float terminalVelocity = 20.0f;

// ground variable stores Y value of ground where player should be standing
int ground = 410;
int player1X = 50;
int player1Y = ground;
int player2X = 1100;
int player2Y = ground;

bool player1isOnGround = true;
bool player2isOnGround = true;

bool player1isFacingRight = true;
bool player2isFacingRight = false;

bool playerCollision(int x1, int x2, int y1, int y2, int h1, int h2, int w1, int w2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void movement(Sprite &player1Sprite, Sprite &player2Sprite)
{

    // player 1
    if (Keyboard::isKeyPressed(Keyboard::A) && player1X > 0)
    {
        player1isFacingRight = false;
        float nextX = player1X - speed;
        if (!playerCollision(nextX, player2X, player1Y, player2Y, player1Height, player2Height, player1Width, player2Width))
            player1X = nextX;
    }

    if (Keyboard::isKeyPressed(Keyboard::D) && player1X + player1Width < screenX)
    {
        player1isFacingRight = true;
        float nextX = player1X + speed;
        if (!playerCollision(nextX, player2X, player1Y, player2Y, player1Height, player2Height, player1Width, player2Width))

            player1X = nextX;
    }

    if (Keyboard::isKeyPressed(Keyboard::W) && player1isOnGround)
    {
        player1isOnGround = false;
        velocity1 = jumpstrength;
    }

    // player 2
    if (Keyboard::isKeyPressed(Keyboard::Left) && player2X > 0)
    {
        player2isFacingRight = false;
        float nextX = player2X - speed;
        if (!playerCollision(player1X, nextX, player1Y, player2Y, player1Height, player2Height, player1Width, player2Width))
            player2X = nextX;
    }

    if (Keyboard::isKeyPressed(Keyboard::Right) && player2X + player2Width < screenX)
    {
        player2isFacingRight = true;
        float nextX = player2X + speed;
        if (!playerCollision(player1X, nextX, player1Y, player2Y, player1Height, player2Height, player1Width, player2Width))
            player2X = nextX;
    }

    if (Keyboard::isKeyPressed(Keyboard::Up) && player2isOnGround)
    {
        player2isOnGround = false;
        velocity2 = jumpstrength;
    }

    // updating position
    if (player1isFacingRight)
    {
        player1Sprite.setScale(player1Scale, player1Scale);
        player1Sprite.setPosition(player1X, player1Y);
    }
    else
    {
        player1Sprite.setScale(-player1Scale, player1Scale);
        player1Sprite.setPosition(player1X + player1Width, player1Y);
    }

    if (player2isFacingRight)
    {
        player2Sprite.setScale(-player2Scale, player2Scale);
        player2Sprite.setPosition(player2X + player2Width, player2Y);
    }
    else
    {
        player2Sprite.setScale(player2Scale, player2Scale);
        player2Sprite.setPosition(player2X, player2Y);
    }
}

void playerGravity()
{
    // apply gravity to velocity if player is not on ground
    if (!player1isOnGround)
    {
        velocity1 += gravity;
        if (velocity1 > terminalVelocity)
            velocity1 = terminalVelocity;
    }
    else
        velocity1 = 0;

    if (!player2isOnGround)
    {
        velocity2 += gravity;
        if (velocity2 > terminalVelocity)
            velocity2 = terminalVelocity;
    }
    else
        velocity2 = 0;

    // calculating next Y position after jump
    offsetY1 = player1Y;
    offsetY1 += velocity1;

    offsetY2 = player2Y;
    offsetY2 += velocity2;

    if (offsetY1 > ground)
    {
        player1isOnGround = true;
        velocity1 = 0;
        player1Y = ground;
    }
    else
    {
        player1Y = offsetY1;
        player1isOnGround = false;
    }

    if (offsetY2 > ground)
    {
        player2isOnGround = true;
        velocity2 = 0;
        player2Y = ground;
    }
    else
    {
        player2Y = offsetY2;
        player2isOnGround = false;
    }
}

int main()
{
    // creating window
    RenderWindow window(VideoMode(screenX, screenY), "Game", Style::Default);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    // LOADING SPRITES

    // background sprite
    Sprite bgSprite;
    Texture bgTexture;
    bgTexture.loadFromFile("Assets/Sprites/bg.jpg");
    bgSprite.setTexture(bgTexture);

    // player1 sprite
    Sprite player1Sprite;
    Texture player1Texture;
    player1Texture.loadFromFile("Assets/Sprites/player 1.png");
    player1Sprite.setTexture(player1Texture);
    player1Sprite.setScale(player1Scale, player1Scale);
    player1Sprite.setPosition(player1X, player1Y);

    // player2 sprite
    Sprite player2Sprite;
    Texture player2Texture;
    player2Texture.loadFromFile("Assets/Sprites/player 2.png");
    player2Sprite.setTexture(player2Texture);
    player2Sprite.setScale(player2Scale, player2Scale);
    player2Sprite.setPosition(player2X, player2Y);

    // to check co-ordinates of a sprite
    // Vector2f position = player1Sprite.getPosition();
    // cout << position.x << " " << position.y << "\n";

    // main game loop
    Event Ev;
    while (window.isOpen())
    {
        window.clear();

        while (window.pollEvent(Ev))
        {
            if (Ev.type == Event::Closed) // when the close window icon is pressed it closes window
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();

        // function calling
        movement(player1Sprite, player2Sprite);
        playerGravity();

        window.draw(bgSprite);
        window.draw(player1Sprite);
        window.draw(player2Sprite);
        window.display();
    }
}
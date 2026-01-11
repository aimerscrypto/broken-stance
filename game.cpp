#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

int screenX = 1280;
int screenY = 720;

// scaling sprites to increase the size
float playerScale = 3;
int textureSize = 128;

// player 1
int player1Height = 82 * playerScale;
int player1Width = 32 * playerScale;

// player 2
int player2Height = 82 * playerScale;
int player2Width = 32 * playerScale;

// player state variables
float speed = 5.0f;
float gravity = 1.0f;
float velocity1 = 0;
float velocity2 = 0;
float offsetY1 = 0;
float offsetY2 = 0;
float jumpstrength = -20.0f;
float terminalVelocity = 20.0f;

bool player1isOnGround = true;
bool player2isOnGround = true;
bool player1isFacingRight = true;
bool player2isFacingRight = false;
bool player1isJumping = false;
bool player2isJumping = false;
bool player1isIdle = true;
bool player2isIdle = true;
bool player1isAttacking = false;
bool player2isAttacking = false;

// ground variable stores Y value of ground where player should be standing
int ground = 316;
int player1X = 50;
int player1Y = ground;
int player2X = 1000;
int player2Y = ground;

// animation state variables
int p1Frame = 0;
int p1Counter = 0;
int p2Frame = 0;
int p2Counter = 0;

void attack()
{
    // distance between player
    float distance = player2X - (player1X + player1Width);

    // player 1
    if (Keyboard::isKeyPressed(Keyboard::Q) && !player1isAttacking) // attack 1
    {
        player1isAttacking = true;
        if (distance < 25)
        {
            // hit player2 and descrease its health
        }
    }

    // player 2
    if (Keyboard::isKeyPressed(Keyboard::K) && !player2isAttacking) // attack 1
    {
        player2isAttacking = true;
        if (distance < 25)
        {
            // hit player1 and descrease its health
        }
    }
}

void animation(Sprite &player1Sprite, Sprite &player2Sprite, Texture &p1Idle, Texture &p1Jump, Texture &p1Run, Texture &p1Attack1,
               Texture &p2Idle, Texture &p2Jump, Texture &p2Run, Texture &p2Attack1)
{
    int animSpeed = 5;

    // player 1 animaiton
    if (player1isAttacking)
    {
        player1Sprite.setTexture(p1Attack1);
        int totalFrames = 4;

        p1Counter++;

        if (p1Counter >= animSpeed)
        {
            p1Counter = 0;
            p1Frame++;
            if (p1Frame >= totalFrames)
            {
                p1Frame = 0;
                player1isAttacking = false;
            }
        }
        player1Sprite.setTextureRect(IntRect(p1Frame * 128, 0, 128, 128));
    }
    else if (player1isJumping)
    {
        player1Sprite.setTexture(p1Jump);
        int totalFrames = 10;

        p1Counter++;
        if (p1Counter >= animSpeed)
        {
            p1Counter = 0;
            p1Frame++;
            if (p1Frame >= totalFrames)
                p1Frame = 0;
            player1Sprite.setTextureRect(IntRect(p1Frame * 128, 0, 128, 128));
        }
    }
    else if (!player1isIdle)
    {
        player1Sprite.setTexture(p1Run);
        int totalFrames = 8;

        p1Counter++;
        if (p1Counter >= animSpeed)
        {
            p1Counter = 0;
            p1Frame++;
            if (p1Frame >= totalFrames)
                p1Frame = 0;

            player1Sprite.setTextureRect(IntRect(p1Frame * 128, 0, 128, 128));
        }
    }
    else
    {
        player1Sprite.setTexture(p1Idle);
        int totalFrames = 6;

        p1Counter++;
        if (p1Counter >= animSpeed)
        {
            p1Counter = 0;
            p1Frame++;
            if (p1Frame >= totalFrames)
                p1Frame = 0;
            player1Sprite.setTextureRect(IntRect(p1Frame * 128, 0, 128, 128));
        }
    }

    // player 2 animation
    if (player2isAttacking)
    {
        player2Sprite.setTexture(p2Attack1);
        int totalFrames = 6;

        p2Counter++;
        if (p2Counter >= animSpeed)
        {
            p2Counter = 0;
            p2Frame++;
            if (p2Frame >= totalFrames)
            {
                p2Frame = 0;
                player2isAttacking = false;
            }
        }
        player2Sprite.setTextureRect(IntRect(p2Frame * 128, 0, 128, 128));
    }
    else if (player2isJumping)
    {
        player2Sprite.setTexture(p2Jump);
        int totalFrames = 12;

        p2Counter++;
        if (p2Counter >= animSpeed)
        {
            p2Counter = 0;
            p2Frame++;
            if (p2Frame >= totalFrames)
                p2Frame = 0;
            player2Sprite.setTextureRect(IntRect(p2Frame * 128, 0, 128, 128));
        }
    }
    else if (!player2isIdle)
    {
        player2Sprite.setTexture(p2Run);
        int totalFrames = 8;

        p2Counter++;
        if (p2Counter >= animSpeed)
        {
            p2Counter = 0;
            p2Frame++;
            if (p2Frame >= totalFrames)
                p2Frame = 0;
            player2Sprite.setTextureRect(IntRect(p2Frame * 128, 0, 128, 128));
        }
    }
    else
    {
        player2Sprite.setTexture(p2Idle);
        int totalFrames = 6;

        p2Counter++;
        if (p2Counter >= animSpeed)
        {
            p2Counter = 0;
            p2Frame++;
            if (p2Frame >= totalFrames)
                p2Frame = 0;
            player2Sprite.setTextureRect(IntRect(p2Frame * 128, 0, 128, 128));
        }
    }
}

bool playerCollision(int x1, int x2, int y1, int y2, int h1, int h2, int w1, int w2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void movement(Sprite &player1Sprite, Sprite &player2Sprite)
{
    player1isIdle = true;
    player2isIdle = true;
    // player 1
    if (Keyboard::isKeyPressed(Keyboard::A) && player1X > 0)
    {
        player1isIdle = false;
        player1isFacingRight = false;
        float nextX = player1X - speed;
        if (!playerCollision(nextX, player2X, player1Y, player2Y, player1Height, player2Height, player1Width, player2Width))
            player1X = nextX;
    }

    if (Keyboard::isKeyPressed(Keyboard::D) && player1X + player1Width < screenX)
    {
        player1isIdle = false;
        player1isFacingRight = true;
        float nextX = player1X + speed;
        if (!playerCollision(nextX, player2X, player1Y, player2Y, player1Height, player2Height, player1Width, player2Width))

            player1X = nextX;
    }

    if (Keyboard::isKeyPressed(Keyboard::W) && player1isOnGround)
    {
        player1isJumping = true;
        player1isOnGround = false;
        velocity1 = jumpstrength;
    }

    // player 2
    if (Keyboard::isKeyPressed(Keyboard::Left) && player2X > 0)
    {
        player2isIdle = false;
        player2isFacingRight = false;
        float nextX = player2X - speed;
        if (!playerCollision(player1X, nextX, player1Y, player2Y, player1Height, player2Height, player1Width, player2Width))
            player2X = nextX;
    }

    if (Keyboard::isKeyPressed(Keyboard::Right) && player2X + player2Width < screenX)
    {
        player2isIdle = false;
        player2isFacingRight = true;
        float nextX = player2X + speed;
        if (!playerCollision(player1X, nextX, player1Y, player2Y, player1Height, player2Height, player1Width, player2Width))
            player2X = nextX;
    }

    if (Keyboard::isKeyPressed(Keyboard::Up) && player2isOnGround)
    {
        player2isJumping = true;
        player2isOnGround = false;
        velocity2 = jumpstrength;
    }

    // updating position

    int visualSize = textureSize * playerScale; // 128*3=384
    // Calculating the "Empty Space" offsets
    // X: (384 - 96) / 2 = 144 pixels of empty space on the left
    int offsetX = (visualSize - player1Width) / 2;

    if (player1isFacingRight)
    {
        player1Sprite.setScale(playerScale, playerScale);
        player1Sprite.setPosition(player1X - offsetX, player1Y);
    }
    else
    {
        player1Sprite.setScale(-playerScale, playerScale);
        player1Sprite.setPosition(player1X + visualSize - offsetX, player1Y);
    }

    if (player2isFacingRight)
    {
        player2Sprite.setScale(playerScale, playerScale);
        player2Sprite.setPosition(player2X - offsetX, player2Y);
    }
    else
    {
        player2Sprite.setScale(-playerScale, playerScale);
        player2Sprite.setPosition(player2X + visualSize - offsetX, player2Y);
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
        player1isJumping = false;
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
        player2isJumping = false;
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
    RenderWindow window(VideoMode(screenX, screenY), "Broken Stance", Style::Default);
    window.setPosition(Vector2i(150, 50));
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    // LOADING TEXTURES

    // background
    Texture bgTexture;
    bgTexture.loadFromFile("Assets/Sprites/bg.png");

    // player
    Texture p1IdleTex, p1JumpTex, p1RunTex, p1Attack1;
    p1IdleTex.loadFromFile("Assets/Sprites/player1/idle.png");
    p1JumpTex.loadFromFile("Assets/Sprites/player1/Jump.png");
    p1RunTex.loadFromFile("Assets/Sprites/player1/Run.png");
    p1Attack1.loadFromFile("Assets/Sprites/player1/Attack_1.png");

    // player 2
    Texture p2IdleTex, p2JumpTex, p2RunTex, p2Attack1;
    p2IdleTex.loadFromFile("Assets/Sprites/player2/idle.png");
    p2JumpTex.loadFromFile("Assets/Sprites/player2/Jump.png");
    p2RunTex.loadFromFile("Assets/Sprites/player2/Run.png");
    p2Attack1.loadFromFile("Assets/Sprites/player2/Attack_1.png");

    // Creating Sprites

    // background
    Sprite bgSprite;
    bgSprite.setTexture(bgTexture);

    // player 1
    Sprite player1Sprite;
    player1Sprite.setTexture(p1IdleTex);
    player1Sprite.setTextureRect(IntRect(0, 0, textureSize, textureSize));
    player1Sprite.setScale(playerScale, playerScale);

    // player 2
    Sprite player2Sprite;
    player2Sprite.setTexture(p2IdleTex);
    player2Sprite.setTextureRect(IntRect(0, 0, textureSize, textureSize));
    player2Sprite.setScale(-playerScale, playerScale);

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
        attack();
        movement(player1Sprite, player2Sprite);
        playerGravity();
        animation(player1Sprite, player2Sprite, p1IdleTex, p1JumpTex, p1RunTex, p1Attack1, p2IdleTex, p2JumpTex, p2RunTex, p2Attack1);

        // drawing on screen
        window.draw(bgSprite);
        window.draw(player1Sprite);
        window.draw(player2Sprite);
        window.display();
    }
}
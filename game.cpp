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
int player1Health = 100;
int player2Health = 100;

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
bool isGameOver = false;
bool player1isHurt = false;
bool player2isHurt = false;

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

// game state variables
int gameState = 0;       // 0 - menu, 1 - instructions, 2 - game ,3 - game over
int selectedOption = 0;  // 0=Local, 1=Online, 2=AI, 3=Exit
string menuMessage = ""; // string for coming soon messages

void resetGame(RectangleShape &player1HealthBar, RectangleShape &player2HealthBar, Text &player1Text, Text &player2Text)
{
    player1Health = 100;
    player2Health = 100;
    player1X = 50;
    player1Y = ground;
    player2X = 1000;
    player2Y = ground;
    isGameOver = false;
    player1isHurt = false;
    player2isHurt = false;
    player1isAttacking = false;
    player2isAttacking = false;

    // reset animation
    p1Frame = 0;
    p1Counter = 0;
    p2Frame = 0;
    p2Counter = 0;

    // reset health bars and text
    player1HealthBar.setSize({300.f, 50.f});
    player2HealthBar.setSize({300.f, 50.f});
    player1Text.setString("PLAYER 1: 100%");
    player2Text.setString("PLAYER 2: 100%");
}

void attack(RectangleShape &player1HealthBar, RectangleShape &player2HealthBar, Text &player1Text, Text &player2Text)
{

    // distance between player
    float distance = (player2X - (player1X + player1Width));

    // player 1
    if (Keyboard::isKeyPressed(Keyboard::Q) && !player1isAttacking && !player1isHurt) // attack 1
    {
        player1isAttacking = true;
        p1Frame = 0;
        p1Counter = 0;

        if (distance < 60)
        {
            if (player2Health > 0)
            {

                player2isHurt = true;
                player2isAttacking = false;
                p2Frame = 0;
                p2Counter = 0;

                player2Health -= 10;
                player2HealthBar.setSize({player2Health * 3.f, 50.f});
                player2Text.setString("Player 2: " + to_string(player2Health) + "%");

                if (player2X + 20 < screenX - player2Width)
                    player2X += 20;

                if (player2Health <= 0)
                {
                    isGameOver = true;
                    player2Health = 0;
                }
            }
        }
    }

    // player 2
    if (Keyboard::isKeyPressed(Keyboard::K) && !player2isAttacking && !player2isHurt) // attack 1
    {
        player2isAttacking = true;
        p2Frame = 0;
        p2Counter = 0;

        if (distance < 60)
        {

            if (player1Health > 0)
            {

                player1isHurt = true;
                player1isAttacking = false;
                p1Frame = 0;
                p1Counter = 0;

                player1Health -= 10;
                player1HealthBar.setSize({player1Health * 3.f, 50.f});
                player1Text.setString("Player 1: " + to_string(player1Health) + "%");

                if (player1X - 20 > 0)
                    player1X -= 20;

                if (player1Health <= 0)
                {
                    player1Health = 0;
                    isGameOver = true;
                }
            }
        }
    }
}
void animation(Sprite &player1Sprite, Sprite &player2Sprite, Texture &p1Idle, Texture &p1Jump, Texture &p1Run, Texture &p1Attack1, Texture &p1Hurt, Texture &p2Idle, Texture &p2Jump, Texture &p2Run, Texture &p2Attack1, Texture &p2Hurt)
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
    else if (player1isHurt)
    {
        player1Sprite.setTexture(p1Hurt);
        int totalFrames = 3;

        p1Counter++;

        if (p1Counter >= animSpeed)
        {
            p1Counter = 0;
            p1Frame++;
            if (p1Frame >= totalFrames)
            {
                p1Frame = 0;
                player1isHurt = false;
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
    else if (player2isHurt)
    {
        player2Sprite.setTexture(p2Hurt);
        int totalFrames = 2;

        p2Counter++;
        if (p2Counter >= animSpeed)
        {
            p2Counter = 0;
            p2Frame++;
            if (p2Frame >= totalFrames)
            {
                p2Frame = 0;
                player2isHurt = false;
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
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    window.requestFocus(); // Request focus so keyboard input works immediately

    // LOADING TEXTURES

    // background
    Texture bgTexture;
    bgTexture.loadFromFile("Assets/Sprites/bg.png");

    // player
    Texture p1IdleTex, p1JumpTex, p1RunTex, p1Attack1, p1HurtTex;
    p1IdleTex.loadFromFile("Assets/Sprites/player1/idle.png");
    p1JumpTex.loadFromFile("Assets/Sprites/player1/Jump.png");
    p1RunTex.loadFromFile("Assets/Sprites/player1/Run.png");
    p1Attack1.loadFromFile("Assets/Sprites/player1/Attack_1.png");
    p1HurtTex.loadFromFile("Assets/Sprites/player1/Hurt.png");

    // player 2
    Texture p2IdleTex, p2JumpTex, p2RunTex, p2Attack1, p2HurtTex;
    p2IdleTex.loadFromFile("Assets/Sprites/player2/idle.png");
    p2JumpTex.loadFromFile("Assets/Sprites/player2/Jump.png");
    p2RunTex.loadFromFile("Assets/Sprites/player2/Run.png");
    p2Attack1.loadFromFile("Assets/Sprites/player2/Attack_1.png");
    p2HurtTex.loadFromFile("Assets/Sprites/player2/Hurt.png");

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

    // HEALTH BARS

    Font font;
    font.loadFromFile("Assets/Fonts/arial.ttf");

    // player1
    Text player1Text;
    player1Text.setFont(font);
    player1Text.setString("PLAYER 1: 100%");
    player1Text.setPosition(50, 10);
    player1Text.setStyle(Text::Bold);
    player1Text.setOutlineColor(Color::Black);
    player1Text.setOutlineThickness(1);

    // background bar static
    RectangleShape bgHealthBar1;
    bgHealthBar1.setSize({300.f, 50.f});
    bgHealthBar1.setPosition({50.0f, 50.f});
    bgHealthBar1.setFillColor(Color::Black);

    // player1 health bar
    RectangleShape player1HealthBar;
    player1HealthBar.setSize({300.f, 50.f});
    player1HealthBar.setPosition({50.0f, 50.0f});
    player1HealthBar.setFillColor(Color::Blue);

    // player2
    Text player2Text;
    player2Text.setFont(font);
    player2Text.setString("PLAYER 2: 100%");
    player2Text.setPosition(950, 10);
    player2Text.setStyle(Text::Bold);
    player2Text.setOutlineColor(Color::Black);
    player2Text.setOutlineThickness(1);

    // background bar static
    RectangleShape bgHealthBar2;
    bgHealthBar2.setSize({300.f, 50.f});
    bgHealthBar2.setPosition({950.0f, 50.0f});
    bgHealthBar2.setFillColor(Color::Black);

    // player2 health bar
    RectangleShape player2HealthBar;
    player2HealthBar.setSize({300.f, 50.f});
    player2HealthBar.setPosition({950.0f, 50.0f});
    player2HealthBar.setFillColor(Color::Red);

    // GAME OVER TEXT
    Text GameOverText;
    GameOverText.setFont(font);
    GameOverText.setString("PLAYER 1 WINS!");
    GameOverText.setCharacterSize(100);
    GameOverText.setFillColor(Color::Blue);
    GameOverText.setPosition(screenX / 2 - 300, screenY / 2 - 50);

    // buttons
    Text Title;
    Title.setFont(font);
    Title.setString("BROKEN STANCE");
    Title.setCharacterSize(100);
    Title.setFillColor(Color::White);
    Title.setPosition(screenX / 2 - 300, 50);

    Text button1;
    button1.setFont(font);
    button1.setString("LOCAL PvP");
    button1.setCharacterSize(50);
    button1.setFillColor(Color::White);
    button1.setPosition(screenX / 2 - 100, 200);

    Text button2;
    button2.setFont(font);
    button2.setString("ONLINE PvP");
    button2.setCharacterSize(50);
    button2.setFillColor(Color::White);
    button2.setPosition(screenX / 2 - 100, 300);

    Text button3;
    button3.setFont(font);
    button3.setString("vs AI");
    button3.setCharacterSize(50);
    button3.setFillColor(Color::White);
    button3.setPosition(screenX / 2 - 100, 400);

    Text button4;
    button4.setFont(font);
    button4.setString("EXIT");
    button4.setCharacterSize(50);
    button4.setFillColor(Color::White);
    button4.setPosition(screenX / 2 - 100, 500);

    Text messageText;
    messageText.setFont(font);
    messageText.setCharacterSize(30);
    messageText.setFillColor(Color::Yellow);
    messageText.setPosition(screenX / 2 - 150, 600);

    Text instructionsText;
    instructionsText.setFont(font);
    instructionsText.setCharacterSize(30);
    instructionsText.setFillColor(Color::White);
    instructionsText.setPosition(50, 100);
    instructionsText.setString("PLAYER 1 CONTROLS:\nA - Move Left\nD - Move Right\nW - Jump\nQ - Attack\n\nPLAYER 2 CONTROLS:\nLeft Arrow - Move Left\nRight Arrow - Move Right\nUp Arrow - Jump\nK - Attack\n\nPress ESC to return to Menu");

    // main game loop
    Event Ev;
    while (window.isOpen())
    {
        window.clear();

        while (window.pollEvent(Ev))
        {
            if (Ev.type == Event::Closed) // when the close window icon is pressed it closes window
                window.close();

            if (gameState == 0) // menu state
            {
                if (Ev.type == Event::KeyPressed)
                {
                    if (Ev.key.code == Keyboard::Escape)
                    {
                        window.close();
                    }
                    if (Ev.key.code == Keyboard::Up)
                    {
                        selectedOption--;
                        if (selectedOption < 0)
                            selectedOption = 3; // loop to last option
                    }
                    if (Ev.key.code == Keyboard::Down)
                    {
                        selectedOption++;
                        if (selectedOption > 3)
                            selectedOption = 0; // loop to first option
                    }
                    if (Ev.key.code == Keyboard::Enter)
                    {
                        if (selectedOption == 0) // Local PvP
                        {
                            gameState = 1; // open instructions and move to game state after that
                            resetGame(player1HealthBar, player2HealthBar, player1Text, player2Text);
                        }
                        else if (selectedOption == 1) // Online PvP
                            menuMessage = "ONLINE PvP COMING SOON!";
                        else if (selectedOption == 2) // vs AI
                            menuMessage = "AI MODE COMING SOON!";
                        else if (selectedOption == 3) // Exit
                            window.close();
                    }
                }
            }
            else if (gameState == 1)
            {
                if (Ev.type == Event::KeyPressed)
                {
                    if (Ev.key.code == Keyboard::Enter)
                    {
                        resetGame(player1HealthBar, player2HealthBar, player1Text, player2Text);   // Reset everything
                        gameState = 2; // START GAME!
                    }
                    if (Ev.key.code == Keyboard::Escape)
                    {
                        gameState = 0; // Return to menu
                        menuMessage = "";
                    }
                }
            }
            else if (gameState == 2) // game state
            {
                if (Ev.type == Event::KeyPressed && Ev.key.code == Keyboard::Escape)
                {
                    gameState = 0; // Return to menu
                    menuMessage = "";
                }
            }
            else if (gameState == 3) //game over state
            {
                if (Ev.type == Event::KeyPressed)
                {
                    if (Ev.key.code == Keyboard::R)
                    {
                        resetGame(player1HealthBar, player2HealthBar, player1Text, player2Text);
                        gameState = 2; // Restart
                    }
                    if (Ev.key.code == Keyboard::Escape)
                    {
                        gameState = 0; // Go to Menu
                        menuMessage = "";
                    }
                }
            }
        } // end of poll event loop

        window.clear();
        window.draw(bgSprite);

        if (gameState == 0) // menu
        {
            if (selectedOption == 0)
                button1.setFillColor(Color::Red);
            else
                button1.setFillColor(Color::White);
            if (selectedOption == 1)
                button2.setFillColor(Color::Red);
            else
                button2.setFillColor(Color::White);
            if (selectedOption == 2)
                button3.setFillColor(Color::Red);
            else
                button3.setFillColor(Color::White);
            if (selectedOption == 3)
                button4.setFillColor(Color::Red);
            else
                button4.setFillColor(Color::White);

            messageText.setString(menuMessage);
            window.draw(Title);
            window.draw(button1);
            window.draw(button2);
            window.draw(button3);
            window.draw(button4);
            window.draw(messageText);
        }

        else if (gameState == 1) // instructions
        {
            window.draw(Title);
            window.draw(instructionsText);
        }

        else if (gameState == 2) // main game
        {
            if (isGameOver)
                gameState = 3; // go to game over state
            else
            {
                attack(player1HealthBar, player2HealthBar, player1Text, player2Text);
                movement(player1Sprite, player2Sprite);
                playerGravity();
                animation(player1Sprite, player2Sprite, p1IdleTex, p1JumpTex, p1RunTex, p1Attack1, p1HurtTex, p2IdleTex, p2JumpTex, p2RunTex, p2Attack1, p2HurtTex);

                window.draw(player1Sprite);
                window.draw(player2Sprite);

                window.draw(bgHealthBar1);
                window.draw(bgHealthBar2);
                window.draw(player1HealthBar);
                window.draw(player2HealthBar);
                window.draw(player1Text);
                window.draw(player2Text);
            }
        }

        else if (gameState == 3) // game over
        {
            // Determine winner
            if (player1Health <= 0)
            {
                GameOverText.setString("PLAYER 2 WINS!");
                GameOverText.setFillColor(Color::Red);
            }
            else
            {
                GameOverText.setString("PLAYER 1 WINS!");
                GameOverText.setFillColor(Color::Blue);
            }
            Text restartText;
            restartText.setFont(font);
            restartText.setString("Press R to Restart or ESC to return to Menu");
            restartText.setCharacterSize(40);
            restartText.setFillColor(Color::White);
            restartText.setPosition(screenX / 2 - 300, screenY / 2 + 100);

            window.draw(GameOverText);
            window.draw(restartText);
        }
        window.display();
    }
}
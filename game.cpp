#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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

// Draws a button that glows Red when selected
void drawButton(RenderWindow &window, Font &font, string text, int yPosition, bool isSelected)
{
    int width = 400;
    int height = 60;
    // Calculate X to center the button
    int xPosition = (screenX - width) / 2;

    RectangleShape buttonBox({(float)width, (float)height});
    buttonBox.setPosition(xPosition, yPosition);

    if (isSelected)
    {
        buttonBox.setFillColor(Color(200, 0, 0, 200)); // Red tint
        buttonBox.setOutlineThickness(4);
        buttonBox.setOutlineColor(Color::White);
    }
    else
    {
        buttonBox.setFillColor(Color(0, 0, 0, 150)); // Dark tint
        buttonBox.setOutlineThickness(2);
        buttonBox.setOutlineColor(Color(100, 100, 100));
    }

    Text buttonText;
    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(30);
    buttonText.setFillColor(Color::White);

    // Center text exactly inside the box using local bounds
    FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    buttonText.setPosition(xPosition + width / 2.0f, yPosition + height / 2.0f);

    window.draw(buttonBox);
    window.draw(buttonText);
}
// Draws the Title with a Shadow effect
void drawTitle(RenderWindow &window, Font &font, string text)
{
    // Shadow (Black, slightly offset)
    Text titleShadow;
    titleShadow.setFont(font);
    titleShadow.setString(text);
    titleShadow.setCharacterSize(80);
    titleShadow.setFillColor(Color::Black);
    titleShadow.setStyle(Text::Bold);

    FloatRect shadowRect = titleShadow.getLocalBounds();
    titleShadow.setOrigin(shadowRect.left + shadowRect.width / 2.0f, shadowRect.top + shadowRect.height / 2.0f);
    titleShadow.setPosition(screenX / 2 + 4, 84); // Offset by 4 pixels

    // Main Text (White)
    Text titleMain;
    titleMain.setFont(font);
    titleMain.setString(text);
    titleMain.setCharacterSize(80);
    titleMain.setFillColor(Color::White);
    titleMain.setStyle(Text::Bold);

    FloatRect mainRect = titleMain.getLocalBounds();
    titleMain.setOrigin(mainRect.left + mainRect.width / 2.0f, mainRect.top + mainRect.height / 2.0f);
    titleMain.setPosition(screenX / 2, 80);

    window.draw(titleShadow);
    window.draw(titleMain);
}

// Draws a dark semi-transparent panel behind text
void drawPanel(RenderWindow &window, int x, int y, int w, int h)
{
    RectangleShape panel({(float)w, (float)h});
    panel.setPosition(x, y);
    panel.setFillColor(Color(0, 0, 0, 220)); // High opacity black
    panel.setOutlineColor(Color::White);
    panel.setOutlineThickness(2);
    window.draw(panel);
}

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

void attack(RectangleShape &player1HealthBar, RectangleShape &player2HealthBar, Text &player1Text, Text &player2Text,
            Sound &sP1Atk, Sound &sP1Hit, Sound &sP1Effort,
            Sound &sP2Atk, Sound &sP2Hit, Sound &sP2Effort)
{
    // distance between player
    float distance = (player2X - (player1X + player1Width));

    // player 1
    if (Keyboard::isKeyPressed(Keyboard::Q) && !player1isAttacking && !player1isHurt) // attack 1
    {
        player1isAttacking = true;
        p1Frame = 0;
        p1Counter = 0;

        // Play Sounds
        sP1Atk.play();    // Swoosh
        sP1Effort.play(); // Grunt

        if (distance < 60)
        {
            if (player2Health > 0)
            {

                player2isHurt = true;
                player2isAttacking = false;
                p2Frame = 0;
                p2Counter = 0;

                sP1Hit.play(); // Impact Sound

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

        // Play Sounds
        sP2Atk.play();    // sword Splash
        sP2Effort.play(); // // High pitch Grunt

        if (distance < 60)
        {

            if (player1Health > 0)
            {

                player1isHurt = true;
                player1isAttacking = false;
                p1Frame = 0;
                p1Counter = 0;

                sP2Hit.play(); // Impact Sound

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

    // loading audios
    Music bgMusic;
    SoundBuffer bMenu, bEffort, bP1Atk, bP1Hit, bP2Atk, bP2Hit;
    Sound sMenu, sP1Effort, sP2Effort, sP1Atk, sP1Hit, sP2Atk, sP2Hit;

    bgMusic.openFromFile("Assets/Audio/battle_theme.ogg");
    bgMusic.setLoop(true);
    bgMusic.setVolume(20); // Set volume to 20%
    bgMusic.play();

    bMenu.loadFromFile("Assets/Audio/menu_move.ogg");
    bEffort.loadFromFile("Assets/Audio/effort.ogg");
    bP1Atk.loadFromFile("Assets/Audio/p1_attack.ogg");
    bP1Hit.loadFromFile("Assets/Audio/p1_hit.ogg");
    bP2Atk.loadFromFile("Assets/Audio/p2_attack.ogg");
    bP2Hit.loadFromFile("Assets/Audio/p2_hit.ogg");

    // Assign Buffers to Sounds
    sMenu.setBuffer(bMenu);
    sP1Atk.setBuffer(bP1Atk);
    sP1Hit.setBuffer(bP1Hit);
    sP2Atk.setBuffer(bP2Atk);
    sP2Hit.setBuffer(bP2Hit);
    sP1Effort.setBuffer(bEffort);

    // Effort Sound Setup (Pitch Shifting for Girl)
    sP1Effort.setBuffer(bEffort); // Boy uses normal pitch
    sP2Effort.setBuffer(bEffort);
    sP2Effort.setPitch(1.3f); // Girl uses higher pitch (1.3x)

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

    Text messageText;
    messageText.setFont(font);
    messageText.setCharacterSize(30);
    messageText.setFillColor(Color::Yellow);
    messageText.setPosition(screenX / 2 - 150, 600);

    Text instructionsText;
    instructionsText.setFont(font);
    instructionsText.setCharacterSize(30);
    instructionsText.setFillColor(Color::White);
    instructionsText.setPosition(50,120);
    instructionsText.setString("PLAYER 1 CONTROLS:\nA - Move Left\nD - Move Right\nW - Jump\nQ - Attack\n\nPLAYER 2 CONTROLS:\nLeft Arrow - Move Left\nRight Arrow - Move Right\nUp Arrow - Jump\nK - Attack\n\nPress ENTER to start game\n\nPress ESC to return to Menu");

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
                        sMenu.play(); // play menu select sound

                        if (selectedOption < 0)
                            selectedOption = 3; // loop to last option
                    }
                    if (Ev.key.code == Keyboard::Down)
                    {
                        selectedOption++;
                        sMenu.play(); // play menu select sound

                        if (selectedOption > 3)
                            selectedOption = 0; // loop to first option
                    }
                    if (Ev.key.code == Keyboard::Enter)
                    {
                        sMenu.play(); // play menu select sound

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
                        resetGame(player1HealthBar, player2HealthBar, player1Text, player2Text); // Reset everything
                        gameState = 2;                                                           // START GAME!
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
            else if (gameState == 3) // game over state
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
            drawTitle(window, font, "BROKEN STANCE");

            // Draw buttons and tell them which one is selected
            // The last argument (true/false) tells the function to glow red or not
            drawButton(window, font, "LOCAL PvP", 250, selectedOption == 0);
            drawButton(window, font, "ONLINE PvP", 320, selectedOption == 1);
            drawButton(window, font, "PLAY AGAINST AI", 390, selectedOption == 2);
            drawButton(window, font, "EXIT", 460, selectedOption == 3);

            messageText.setString(menuMessage);

            // Center and draw Coming Soon message
            FloatRect msgRect = messageText.getLocalBounds();
            messageText.setOrigin(msgRect.left + msgRect.width / 2.0f, msgRect.top + msgRect.height / 2.0f);
            messageText.setPosition(screenX / 2, 600);
            window.draw(messageText);
        }

        else if (gameState == 1) // instructions
        {
            drawTitle(window, font, "HOW TO PLAY");
            // Draw a dark box behind the text
            drawPanel(window, 50, 120, 1180, 530);
            window.draw(instructionsText);
        }

        else if (gameState == 2) // main game
        {
            if (isGameOver)
                gameState = 3; // go to game over state
            else
            {
                attack(player1HealthBar, player2HealthBar, player1Text, player2Text,
                       sP1Atk, sP1Hit, sP1Effort,
                       sP2Atk, sP2Hit, sP2Effort);

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
            // Draw a box in the center
            drawPanel(window, 200, 200, 880, 320);

            // set winner text color
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

            // Center Winner Text
            FloatRect textRect = GameOverText.getLocalBounds();
            GameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            GameOverText.setPosition(screenX / 2.0f, screenY / 2.0f - 50);

            // Center Restart Text
            Text restartText;
            restartText.setFont(font);
            restartText.setString("Press R to Restart or ESC to return to Menu");
            restartText.setCharacterSize(30);
            restartText.setFillColor(Color::White);

            FloatRect rRect = restartText.getLocalBounds();
            restartText.setOrigin(rRect.left + rRect.width / 2.0f, rRect.top + rRect.height / 2.0f);
            restartText.setPosition(screenX / 2, screenY / 2 + 50);

            window.draw(GameOverText);
            window.draw(restartText);
        }
        window.display();
    }
}
#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;


bool isRolling = false;
Clock diceClock;
float rollDuration = 1.5f;
int diceFace = 1;
bool rollCompleted = false;
bool gotSnakeOrLadder =false;

enum GameState { MENU, GAME, GAME_OVER };


enum PlayerState { ACTIVE, WON };

int numPlayers = 0;


struct Player {
    int position;
    PlayerState state;
};

vector<Vector2f> getDotsForFace(int face, const FloatRect& diceBounds) {
    float dotRadius = 10.f;
    vector<Vector2f> dotPositions;


    float centerX = diceBounds.left + diceBounds.width/2.5f;
    float centerY = diceBounds.top + diceBounds.height/2.5f;
    float offset = diceBounds.width / 4;

    switch (face) {
        case 1:
            dotPositions.push_back(Vector2f(centerX, centerY));
            break;
        case 2:
            dotPositions.push_back(Vector2f(centerX - offset, centerY - offset));
            dotPositions.push_back(Vector2f(centerX + offset, centerY + offset));
            break;
        case 3:
            dotPositions.push_back(Vector2f(centerX, centerY));
            dotPositions.push_back(Vector2f(centerX - offset, centerY - offset));
            dotPositions.push_back(Vector2f(centerX + offset, centerY + offset));
            break;
        case 4:
            dotPositions.push_back(Vector2f(centerX - offset, centerY - offset));
            dotPositions.push_back(Vector2f(centerX + offset, centerY - offset));
            dotPositions.push_back(Vector2f(centerX - offset, centerY + offset));
            dotPositions.push_back(Vector2f(centerX + offset, centerY + offset));
            break;
        case 5:
            dotPositions.push_back(Vector2f(centerX, centerY));
            dotPositions.push_back(Vector2f(centerX - offset, centerY - offset));
            dotPositions.push_back(Vector2f(centerX + offset, centerY - offset));
            dotPositions.push_back(Vector2f(centerX - offset, centerY + offset));
            dotPositions.push_back(Vector2f(centerX + offset, centerY + offset));
            break;
        case 6:
            dotPositions.push_back(Vector2f(centerX - offset, centerY - offset));
            dotPositions.push_back(Vector2f(centerX + offset, centerY - offset));
            dotPositions.push_back(Vector2f(centerX - offset, centerY));
            dotPositions.push_back(Vector2f(centerX + offset, centerY));
            dotPositions.push_back(Vector2f(centerX - offset, centerY + offset));
            dotPositions.push_back(Vector2f(centerX + offset, centerY + offset));
            break;
    }

    return dotPositions;
}

int rollDie() {
    return rand() % 6 + 1;
}


void movePlayer(int& currPos, int roll, int& stepsRemaining, bool& reachedTarget, int& finalTarget, int snakesAndLadders[]) {

    if (stepsRemaining == 0) {
        finalTarget = currPos + roll;
        if (finalTarget > 100) {
            finalTarget = currPos;
            reachedTarget = true;
        }else
        stepsRemaining = roll;
    }

    if (stepsRemaining >0) {

            if(gotSnakeOrLadder){
                    currPos=finalTarget-1;
                    gotSnakeOrLadder= false;
            }
        currPos++;
        stepsRemaining--;


        if (stepsRemaining == 0) {

            if (snakesAndLadders[currPos] != 0) {
                finalTarget = currPos + snakesAndLadders[currPos];
                gotSnakeOrLadder = true;
                stepsRemaining = 1;

            } else {
                reachedTarget = true;
            }
        }

    }
}


void handleMenu(RenderWindow& window, Font& font, GameState& state, int& numPlayers) {
    window.clear();

    Texture backgroundTexture;
    Texture logoTexture;
    if (!backgroundTexture.loadFromFile("background.jpg")) {
        cout << "Error loading background image\n";
        return;
    }
    if (!logoTexture.loadFromFile("logo.png")) {
        cout << "Error loading logo image\n";
        return;
    }
    Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(2.5f, 2.5f);
    window.draw(backgroundSprite);

    Sprite logoSprite(logoTexture);
    logoSprite.setScale(0.5f, 0.5f);
    logoSprite.setPosition(900, 600);
    window.draw(logoSprite);



    Font titleFont;
    if (!titleFont.loadFromFile("./SuperBoys-vmW67.ttf")) {
        cout << "Error loading title font\n";
        return;
    }


    Text title("Snake and Ladders", titleFont, 130);
    title.setFillColor(Color::Yellow);
    title.setOutlineColor(Color::Black);
    title.setOutlineThickness(10);
    title.setPosition(60, 80);


    window.draw(title);


    Text text("Choose number of players", font, 30);
    text.setFillColor(Color::White);
    text.setOutlineColor(Color::Black);
    text.setOutlineThickness(4);
    text.setPosition(400, 300);
    window.draw(text);


    RectangleShape twoPlayersButton(Vector2f(200, 50));
    twoPlayersButton.setFillColor(Color(120, 255, 120));
    twoPlayersButton.setOutlineColor(Color::Black);
    twoPlayersButton.setOutlineThickness(6);
    twoPlayersButton.setPosition(500, 400);
    window.draw(twoPlayersButton);

    Text twoPlayersText("2 Players", font, 24);
    twoPlayersText.setFillColor(Color::Black);
    twoPlayersText.setPosition(550, 410);
    window.draw(twoPlayersText);

    RectangleShape threePlayersButton(Vector2f(200, 50));
    threePlayersButton.setFillColor(Color(120, 200, 255));
    threePlayersButton.setOutlineColor(Color::Black);
    threePlayersButton.setOutlineThickness(6);
    threePlayersButton.setPosition(500, 500);
    window.draw(threePlayersButton);

    Text threePlayersText("3 Players", font, 24);
    threePlayersText.setFillColor(Color::Black);
    threePlayersText.setPosition(550, 510);
    window.draw(threePlayersText);

    RectangleShape fourPlayersButton(Vector2f(200, 50));
    fourPlayersButton.setFillColor(Color(255, 120, 120));
    fourPlayersButton.setOutlineColor(Color::Black);
    fourPlayersButton.setOutlineThickness(6);
    fourPlayersButton.setPosition(500, 600);
    window.draw(fourPlayersButton);

    Text fourPlayersText("4 Players", font, 24);
    fourPlayersText.setFillColor(Color::Black);
    fourPlayersText.setPosition(550, 610);
    window.draw(fourPlayersText);

    window.display();


    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
        } else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            Vector2i mousePos = Mouse::getPosition(window);
            if (mousePos.x >= 500 && mousePos.x <= 700) {
                if (mousePos.y >= 400 && mousePos.y <= 450) {
                    numPlayers = 2;
                    state = GAME;
                } else if (mousePos.y >= 500 && mousePos.y <= 550) {
                    numPlayers = 3;
                    state = GAME;
                } else if (mousePos.y >= 600 && mousePos.y <= 650) {
                    numPlayers = 4;
                    state = GAME;
                }
            }
        }
    }
}

void handleGame(RenderWindow& window, const Font& font, GameState& state, vector<Player>& players, int& currPlayer, int& roll, bool& spacePressedLastFrame, int &numPlayers) {


    Texture backgroundTexture;
    Texture logoTexture;
    if (!backgroundTexture.loadFromFile("background2.jpg")) {
        cout << "Error loading background image\n";
        return;
    }

    Sprite backgroundSprite(backgroundTexture);
   backgroundSprite.setScale(2.5f, 2.5f);




    RectangleShape diceRect(Vector2f(80, 80));
    diceRect.setFillColor(Color::White);
    diceRect.setOutlineThickness(2);
    diceRect.setOutlineColor(Color::Black);
    diceRect.setPosition(1000,600);

    bool spacePressed = Keyboard::isKeyPressed(Keyboard::Space);


    RectangleShape backButton(Vector2f(60, 40));
    backButton.setFillColor(Color(0,90,205));
    backButton.setPosition(5, 750);
    backButton.setOutlineColor(Color::Black);
    backButton.setOutlineThickness(6);


    Text backButtonText("Back", font, 20);
    backButtonText.setFillColor(Color::White);
    backButtonText.setPosition(12, 757);
    backButtonText.setOutlineColor(Color::Black);
    backButtonText.setOutlineThickness(4);


    Vector2i mousePos = Mouse::getPosition(window);
    bool isMouseOverButton = mousePos.x >= 5 && mousePos.x <= 65 &&
                             mousePos.y >= 750&& mousePos.y <= 790;



    FloatRect logoBounds = diceRect.getGlobalBounds();
    bool isMouseOverLogo = logoBounds.contains(static_cast<Vector2f>(mousePos));

   if (Mouse::isButtonPressed(Mouse::Left) && isMouseOverLogo && !spacePressedLastFrame) {
        roll = rollDie();
        isRolling = true;
        diceClock.restart();
        rollCompleted = false;
    }


    if (isRolling) {
        if (diceClock.getElapsedTime().asSeconds() < rollDuration) {
            int elapsedMs = diceClock.getElapsedTime().asMilliseconds();
            diceFace = (elapsedMs / 100) % 6 + 1;
        } else {
            diceFace = roll;
            isRolling = false;
            rollCompleted = true;
        }
    }


    static int stepsRemaining = 0;
    static bool reachedTarget = false;
    static int finalTarget = 0;

    int snakesAndLadders[101] = {0};
    snakesAndLadders[2] = 58;
    snakesAndLadders[6] = 21;
    snakesAndLadders[22] = -17;
    snakesAndLadders[32] = -20;
    snakesAndLadders[34] = 41;
    snakesAndLadders[37] = 48;
    snakesAndLadders[84] = -42;
    snakesAndLadders[89] = -65;

    if (rollCompleted && !reachedTarget) {
        movePlayer(players[currPlayer].position, roll, stepsRemaining, reachedTarget, finalTarget, snakesAndLadders);
    }


    if (reachedTarget) {
        if (players[currPlayer].position == 100) {
            players[currPlayer].state = WON;
            state = GAME_OVER;
        }


        currPlayer = (currPlayer + 1) % players.size();
        rollCompleted = false;
        reachedTarget = false;
        stepsRemaining = 0;
    }

    if (Mouse::isButtonPressed(Mouse::Left) && isMouseOverButton) {
        state = MENU;
        players.clear();
    }

    spacePressedLastFrame = Mouse::isButtonPressed(Mouse::Left);


    window.clear();


    window.draw(backgroundSprite);
    Texture snake1Texture, snake2Texture, snake3Texture, snake4Texture;
    if (!snake1Texture.loadFromFile("./snake1.png") ||
        !snake2Texture.loadFromFile("./snake2.png") ||
        !snake3Texture.loadFromFile("./snake3.png") ||
        !snake4Texture.loadFromFile("./snake4.png")) {
        cout << "Error loading snake images\n";
        return;
    }

    Sprite snake1Sprite(snake1Texture);
    Sprite snake2Sprite(snake2Texture);
    Sprite snake3Sprite(snake3Texture);
    Sprite snake4Sprite(snake4Texture);
    snake1Sprite.setScale(0.5f, 0.5f);
    snake2Sprite.setScale(0.8f, 0.8f);
    snake3Sprite.setScale(0.7f, 0.7f);
    snake4Sprite.setScale(1.0f, 1.0f);
    snake1Sprite.setPosition(300, 70);
    snake2Sprite.setPosition(100, 480);
    snake3Sprite.setPosition(560, 440);
    snake4Sprite.setPosition(105, 100);


//
//    // Define outline color
//    sf::Color outlineColor(0, 0, 0);
//
//    // Define outline thickness
//    float outlineThickness = 2;
//
//    // Create outline effect for each sprite
//    Sprite snake1Outline(snake1Sprite);
//    Sprite snake2Outline(snake2Sprite);
//    Sprite snake3Outline(snake3Sprite);
//    Sprite snake4Outline(snake4Sprite);
//
//    // Set outline color for each sprite
//    snake1Outline.setColor(Color(10,200,10));
//    snake2Outline.setColor(Color(180,160,200));
//    snake3Outline.setColor(Color(200,200,5));
//    snake4Outline.setColor(Color(200,80,100));
//
//    // Draw slightly larger sprites behind each original sprite to simulate outline
//    snake1Outline.setScale(0.52,0.52);
//    snake2Outline.setScale(0.85,0.85);
//    snake3Outline.setScale(0.75,0.75);
//    snake4Outline.setScale(1.05,1.05);
//
//    snake1Outline.setPosition(290, 60);
//    snake2Outline.setPosition(90, 470);
//    snake3Outline.setPosition(550, 430);
//    snake4Outline.setPosition(95, 90);


    Texture ladder1Texture, ladder2Texture;
    if (!ladder1Texture.loadFromFile("./ladder1.png") || !ladder2Texture.loadFromFile("./ladder2.png")) {
        cout << "Error loading ladder images\n";
        return;
    }

    Sprite ladder1Sprite(ladder1Texture);
    Sprite ladder2Sprite(ladder2Texture);
    Sprite ladder3Sprite(ladder1Texture);
    Sprite ladder4Sprite(ladder2Texture);
    ladder1Sprite.setScale(0.4f, 0.9f);
    ladder2Sprite.setScale(0.2f, 0.7f);
    ladder3Sprite.setScale(0.4f, 0.4f);
    ladder4Sprite.setScale(0.21f, 0.5f);
    ladder1Sprite.setPosition(300, 100);
    ladder2Sprite.setPosition(120, 350);
    ladder3Sprite.setPosition(480, 600);
    ladder4Sprite.setPosition(515, 210);


    float rectWidth = 300;
    float rectHeight = 780;
    RectangleShape rect1(Vector2f(rectWidth, rectHeight));
    rect1.setFillColor(Color(200, 200, 104));
    rect1.setPosition(890, 10);
    rect1.setOutlineThickness(5);
    rect1.setOutlineColor(Color::Black);
    window.draw(rect1);

    RectangleShape rect2(Vector2f(280, 80));
    rect2.setFillColor(Color(184, 235, 164));
    rect2.setPosition(900, 100);
    rect2.setOutlineThickness(5);
    rect2.setOutlineColor(Color::Black);
    window.draw(rect2);

    RectangleShape rect3(Vector2f(280, 80));
    rect3.setFillColor(Color(164, 144, 225));
    rect3.setPosition(900, 200);
    rect3.setOutlineThickness(5);
    rect3.setOutlineColor(Color::Black);
    window.draw(rect3);

    RectangleShape rect4(Vector2f(280, 80));
    rect4.setFillColor(Color(255, 204, 184));
    rect4.setPosition(900, 300);
    rect4.setOutlineThickness(5);
    rect4.setOutlineColor(Color::Black);
    window.draw(rect4);


    if (numPlayers >= 3) {
        RectangleShape rect5(Vector2f(280, 80));
        rect5.setFillColor(Color(235, 200, 225));
        rect5.setPosition(900, 400);
        rect5.setOutlineThickness(5);
        rect5.setOutlineColor(Color::Black);
        window.draw(rect5);
    }
    if (numPlayers == 4) {
        RectangleShape rect6(Vector2f(280, 80));
        rect6.setFillColor(Color(205, 105, 125));
        rect6.setPosition(900, 500);
        rect6.setOutlineThickness(5);
        rect6.setOutlineColor(Color::Black);
        window.draw(rect6);
    }

    RectangleShape PlayerHolder(Vector2f(60, 350));
    PlayerHolder.setFillColor(Color(139, 69, 19));
    PlayerHolder.setPosition(5, 50);
    PlayerHolder.setOutlineThickness(5);
    PlayerHolder.setOutlineColor(Color::Black);
    window.draw(PlayerHolder);

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::Black);

    string currentPlayerText = "Player " + to_string(currPlayer + 1);
    string prevPlayerText = "Player " + to_string((currPlayer == 0 ? players.size() : currPlayer));
    scoreText.setString(currentPlayerText + ", please roll\n the dice.\n\n" +
                        prevPlayerText + " rolled a : " + to_string(roll) + "\n\n\n");

    for (int i = 0; i < players.size(); ++i) {
        scoreText.setString(scoreText.getString() + "Player " + to_string(i + 1) + " position: " +
                            to_string(players[i].position) + "\n\n\n");
    }

    scoreText.setPosition(910, 20);
    window.draw(scoreText);



    window.draw(diceRect);


    vector<Vector2f> dots = getDotsForFace(diceFace, diceRect.getGlobalBounds());


    for (const Vector2f& dotPos : dots) {
        CircleShape dot(10.f);
        dot.setFillColor(Color::Black);
        dot.setPosition(dotPos);
        window.draw(dot);
    }



    int gridSize = 10;
    float cellSize = window.getSize().y / gridSize;
    RectangleShape square(Vector2f(cellSize, cellSize));

for (int i = 0; i < gridSize; ++i) {
    for (int j = 0; j < gridSize; ++j) {
        int index = (9 - i) * gridSize + (i % 2 == 1 ? j : (9 - j)) + 1;
        square.setPosition((j + 1) * cellSize, i * cellSize);
        square.setFillColor((i + j) % 2 == 0 ? Color(238, 238, 210) : Color(118, 150, 86));
        square.setOutlineThickness(7);
        square.setOutlineColor(Color(101, 67, 15));
        window.draw(square);

        Text text;
        text.setFont(font);
        text.setString(to_string(index));
        text.setCharacterSize(20);
        text.setFillColor(Color::Black);

        text.setPosition((j + 1) * cellSize + cellSize / 3.0f,
                         i * cellSize + cellSize / 3.0f);
        window.draw(text);
    }
}



    window.draw(ladder1Sprite);
    window.draw(ladder2Sprite);
    window.draw(ladder3Sprite);
    window.draw(ladder4Sprite);
//     // Draw outline sprites
//        window.draw(snake1Outline);
//        window.draw(snake2Outline);
//        window.draw(snake3Outline);
//        window.draw(snake4Outline);

    window.draw(snake1Sprite);
    window.draw(snake2Sprite);
    window.draw(snake3Sprite);
    window.draw(snake4Sprite);


    vector<Color> playerColors = { Color(100,100,210), Color(205,80,0), Color(0,205,90), Color(220,220,0) };
for (int i = 0; i < players.size(); ++i) {
    CircleShape playerCircle(20);
    playerCircle.setFillColor(playerColors[i]);
    playerCircle.setOutlineThickness(5);
    playerCircle.setOutlineColor(Color::Black);

    if (players[i].position == 0) {
        playerCircle.setPosition(15, (i + 1) * 80);
    } else {
        int playerRow =9 - (players[i].position - 1) / 10;
        int playerCol;
        if (playerRow % 2 == 0) {
            playerCol = 9 -(players[i].position - 1) % 10;
        } else {
            playerCol =  (players[i].position - 1) % 10;
        }
        playerCircle.setPosition((playerCol + 1) * cellSize + cellSize / 4, playerRow * cellSize + cellSize / 4);
    }
    window.draw(playerCircle);


    Text playerNumberText;
    playerNumberText.setFont(font);
    playerNumberText.setString(to_string(i + 1));
    playerNumberText.setCharacterSize(20);
    playerNumberText.setFillColor(Color::Black);


    playerNumberText.setPosition(playerCircle.getPosition().x + playerCircle.getRadius()/1.5f,
                                  playerCircle.getPosition().y + playerCircle.getRadius()/3.f);
    window.draw(playerNumberText);
}
    window.draw(backButton);
    window.draw(backButtonText);

    window.display();
}


void handleGameOver(RenderWindow& window, Font& font, GameState& state, vector<Player>& players) {
    window.clear();


    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.jpg")) {
        cout << "Error loading background image\n";
        return;
    }
    Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(2.5f, 2.5f);
    window.draw(backgroundSprite);



    Texture logoTexture;
    if (!logoTexture.loadFromFile("logo.png")) {
        cout << "Error loading logo image\n";
        return;
    }
    Sprite logoSprite(logoTexture);
    logoSprite.setScale(0.5f, 0.5f);
    logoSprite.setPosition(900, 600);
    window.draw(logoSprite);


    Font titleFont;
    if (!titleFont.loadFromFile("./SuperBoys-vmW67.ttf")) {
        cout << "Error loading title font\n";
        return;
    }


    Text congratsText("CONGRATULATIONS!", titleFont, 120);
    congratsText.setFillColor(Color::Yellow);
     congratsText.setOutlineColor(Color::Black);
    congratsText.setOutlineThickness(10);
    congratsText.setPosition(100, 80);
    window.draw(congratsText);


    for (int i = 0; i < players.size(); ++i) {
        if (players[i].state == WON) {
            Text winnerText("PLAYER " + to_string(i + 1) + " WON!", font, 50);
            winnerText.setFillColor(Color::White);
             winnerText.setOutlineColor(Color::Black);
             winnerText.setOutlineThickness(6);
            winnerText.setPosition(400, 280);

            window.draw(winnerText);
            break;
        }
    }


    RectangleShape playAgainButton(Vector2f(200, 50));
    playAgainButton.setFillColor(Color(60,205,0));
    playAgainButton.setPosition(500, 400);
     playAgainButton.setOutlineColor(Color::Black);
     playAgainButton.setOutlineThickness(6);
    window.draw(playAgainButton);

    Text playAgainText("Play Again", font, 30);
    playAgainText.setFillColor(Color::Black);
    playAgainText.setPosition(530, 405);
    window.draw(playAgainText);

    RectangleShape exitButton(Vector2f(200, 50));
    exitButton.setFillColor(Color(205,60,0));
    exitButton.setPosition(500, 500);
    exitButton.setOutlineColor(Color::Black);
    exitButton.setOutlineThickness(6);

    window.draw(exitButton);

    Text exitText("Exit", font, 30);
    exitText.setFillColor(Color::Black);
    exitText.setPosition(560, 505);
    window.draw(exitText);

    window.display();


    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
        } else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            Vector2i mousePos = Mouse::getPosition(window);
            if (mousePos.x >= 500 && mousePos.x <= 700 && mousePos.y >= 400 && mousePos.y <= 450) {
                state = MENU;
                players.clear();
            } else if (mousePos.x >= 500 && mousePos.x <= 700 && mousePos.y >= 500 && mousePos.y <= 550) {
                window.close();
            }
        }
    }
}


int main() {
    srand(time(0));

    vector<Player> players;
    int currPlayer = 0;
    int roll = 0;
    bool spacePressedLastFrame = false;

    GameState state = MENU;

    RenderWindow window(VideoMode(1200, 800), "Snake and Ladder Game");

    Font font;
    if (!font.loadFromFile("./OpenSans-Regular.ttf")) {
        cout << "Error loading font\n";
        return -1;
    }

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        switch (state) {
            case MENU:
                handleMenu(window, font, state, numPlayers);
                if (state == GAME) {
                    players = vector<Player>(numPlayers, {0, ACTIVE});
                    currPlayer = 0;
                    roll = 0;
                    spacePressedLastFrame = false;
                }
                break;
            case GAME:
                handleGame(window, font, state, players, currPlayer, roll, spacePressedLastFrame,numPlayers);
                break;
            case GAME_OVER:
                handleGameOver(window, font, state, players);
                break;
        }
    }

    return 0;
}

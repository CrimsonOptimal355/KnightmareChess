#include <SFML/Graphics.hpp>
#include <vector>

struct CrossMark
{
    sf::VertexArray vertices;

    CrossMark(sf::Vector2f position) : vertices(sf::PrimitiveType::Lines, 4)
    {
        vertices[0].position = {position.x + 10.f, position.y + 10.f};
        vertices[1].position = {position.x + 140.f, position.y + 140.f};

        vertices[2].position = {position.x + 10.f, position.y + 140.f};
        vertices[3].position = {position.x + 140.f, position.y + 10.f};

        setToActiveColors();
    }

    void setToActiveColors()
    {
        vertices[0].color = sf::Color::Blue;
        vertices[1].color = sf::Color::Blue;
        vertices[2].color = sf::Color::Blue;
        vertices[3].color = sf::Color::Blue;
    }
};

int main()
{
    const float windowWidth = 600.f;
    const float windowHeight = 600.f;

    sf::RenderWindow window(
        sf::VideoMode({static_cast<unsigned int>(windowWidth), static_cast<unsigned int>(windowHeight)}),
        "Big 3x3 Grid");

    window.setFramerateLimit(60);

    const int numRows = 3;
    const int numCols = 3;

    const float cellSize = 150.f;

    const float totalGridWidth = numCols * cellSize;
    const float totalGridHeight = numRows * cellSize;

    const float offsetX = (windowWidth - totalGridWidth) / 2.f;
    const float offsetY = (windowHeight - totalGridHeight) / 2.f;

    std::vector<std::vector<char>> board(3, std::vector<char>(3, ' '));

    int gridX = 1;
    int gridY = 1;
    sf::RectangleShape rectangle(sf::Vector2f(140.f, 140.f));

    rectangle.setOutlineThickness(5.f);
    rectangle.setOutlineColor(sf::Color::Blue);
    rectangle.setFillColor(sf::Color::Transparent);

    sf::VertexArray lines(sf::PrimitiveType::Lines);

    for (int i = 1; i < numCols; ++i)
    {
        float x = offsetX + (i * cellSize);

        lines.append(sf::Vertex{sf::Vector2f(x, offsetY)});
        lines.append(sf::Vertex{sf::Vector2f(x, offsetY + totalGridHeight)});
    }

    for (int j = 1; j < numRows; ++j)
    {
        float y = offsetY + (j * cellSize);

        lines.append(sf::Vertex{sf::Vector2f(offsetX, y)});
        lines.append(sf::Vertex{sf::Vector2f(offsetX + totalGridWidth, y)});
    }

    std::vector<CrossMark> Saved;

    while (window.isOpen())
    {

        float rectX = offsetX + (gridX * cellSize) + 5.f;
        float rectY = offsetY + (gridY * cellSize) + 5.f;

        rectangle.setPosition({rectX, rectY});

        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {

                if (keyPressed->code == sf::Keyboard::Key::W)
                {
                    if (gridY > 0)
                    {
                        gridY--;
                    }
                }

                if (keyPressed->code == sf::Keyboard::Key::S)
                {
                    if (gridY < 2)
                    {
                        gridY++;
                    }
                }

                if (keyPressed->code == sf::Keyboard::Key::A)
                {
                    if (gridX > 0)
                    {
                        gridX--;
                    }
                }

                if (keyPressed->code == sf::Keyboard::Key::D)
                {
                    if (gridX < 2)
                    {
                        gridX++;
                    }
                }

                if (keyPressed->code == sf::Keyboard::Key::Space)
                {
                    if (board[gridY][gridX] == ' ')
                    {
                        board[gridY][gridX] = 'X';

                        sf::Vector2f markPosition = {offsetX + (gridX * cellSize), offsetY + (gridY * cellSize)};

                        CrossMark newMark(markPosition);

                        Saved.push_back(newMark);
                    }
                }

                if (keyPressed->code == sf::Keyboard::Key::R)
                {
                    board = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};

                    Saved.clear();

                    gridX = 1;
                    gridY = 1;
                }
            }
        }

        window.clear(sf::Color::Black);

        window.draw(lines);

        for (const auto &mark : Saved)
        {
            window.draw(mark.vertices);
        }

        window.draw(rectangle);

        window.display();
    }
}
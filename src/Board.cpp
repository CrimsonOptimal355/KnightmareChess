#include "Board.hpp"
#include "MoveStruct.hpp"

#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

char board[8][8] =
    {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}};
bool isWhite(char piece)
{
    return std::isupper(piece);
}
bool isBlack(char piece)
{
    return std::islower(piece);
}
sf::Vector2i findKing(bool whiteKing)
{
    char king =
        whiteKing ? 'K' : 'k';

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            if (board[row][col] == king)
            {
                return {row, col};
            }
        }
    }
    return {-1, -1};
}
void displayBoard()
{
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            std::cout << board[row][col] << " ";
        }

        std::cout << std::endl;
    }
}

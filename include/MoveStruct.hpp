#pragma once

struct Move
{
    int startRow;
    int startCol;

    int endRow;
    int endCol;

    char movedPiece;
    char capturedPiece;

    bool castling = false;
    bool promotion = false;
};

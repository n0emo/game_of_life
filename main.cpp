#include <cstdlib>
#include <ctime>
#include <raylib.h>
#include <vector>

float random_float()
{
    return (float)rand() / RAND_MAX;
}

enum CellState
{
    Alive,
    Dead
};

class Board
{
    using board_t = std::vector<std::vector<CellState>>;

  private:
    board_t m_board;
    size_t m_rows;
    size_t m_cols;

  public:
    constexpr Board(size_t rows, size_t cols)
    {
        m_rows = rows;
        m_cols = cols;
        m_board.resize(rows);
        for (auto &r : m_board)
        {
            r.resize(cols);
            for (auto &c : r)
            {
                c = Dead;
            }
        }
    }

    constexpr size_t rows() const
    {
        return m_rows;
    }

    constexpr size_t cols() const
    {
        return m_cols;
    }

    constexpr CellState &operator[](size_t r, size_t c)
    {
        return m_board[r % m_rows][c % m_cols];
    }

    void populate(float alive_probability)
    {
        srand(time(NULL));
        for (auto &r : m_board)
        {
            for (auto &c : r)
            {
                c = random_float() < alive_probability ? Alive : Dead;
            }
        }
    }

    size_t count_neightbours(size_t r, size_t c)
    {
        size_t result = 0;
        (*this)[r - 1, c - 1] == Alive &&result++;
        (*this)[r - 1, c] == Alive &&result++;
        (*this)[r - 1, c + 1] == Alive &&result++;
        (*this)[r, c - 1] == Alive &&result++;
        (*this)[r, c + 1] == Alive &&result++;
        (*this)[r + 1, c - 1] == Alive &&result++;
        (*this)[r + 1, c] == Alive &&result++;
        (*this)[r + 1, c + 1] == Alive &&result++;
        return result;
    }

    void step()
    {
        board_t next_board(m_rows);

        for (size_t r = 0; r < m_rows; r++)
        {
            next_board[r].resize(m_cols);
            for (size_t c = 0; c < m_cols; c++)
            {
                size_t neightbours = count_neightbours(r, c);
                CellState cell = m_board[r][c];
                if (cell == Alive and (neightbours < 2 or neightbours > 3))
                {
                    cell = Dead;
                }
                else if (cell == Dead and (neightbours == 3))
                {
                    cell = Alive;
                }
                next_board[r][c] = cell;
            }
        }

        m_board = next_board;
    }
};

const size_t screen_width = 1280;
const size_t screen_height = 720;
const size_t size = 10;

int main()
{
    size_t fps = 60;
    InitWindow(screen_width, screen_height, "Game of Life");
    SetTargetFPS(10);

    Board board(screen_height / size, screen_width / size);
    board.populate(0.5);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_UP))
        {
            fps += 10;
            SetTargetFPS(fps);
        }

        if (IsKeyPressed(KEY_DOWN))
        {
            if (fps <= 10)
                fps = 10;
            else
                fps -= 10;
            SetTargetFPS(fps);
        }
        BeginDrawing();

        ClearBackground(BLACK);

        for (size_t r = 0; r < board.rows(); r++)
        {
            for (size_t c = 0; c < board.cols(); c++)
            {
                Color color = BLACK;
                switch (board[r, c])
                {
                case Alive:
                    color = GREEN;
                    break;
                case Dead:
                    color = BLACK;
                    break;
                };

                DrawRectangle(c * size, r * size, size, size, color);
            }
        }

        EndDrawing();

        board.step();
    }

    CloseWindow();
    return 0;
}

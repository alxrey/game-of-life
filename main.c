/**
 * Game of life
 * @file main.c
 * @author alxrey
 * @date 24.01.2018
 *
 * @brief Main program body, Conway's game of life with result displayed on
 * stdout
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> // usleep function
#include <stdlib.h> // random functions

/* Define --------------------------------------------------------------------*/
#define WIDTH               42  ///< width of the cell array
#define HEIGHT              42  ///< height of the cell array

// Stable structure
#define BLOCK               1
#define TUB                 2
#define NAVIRE              3
#define CLIGNOTANT          4
#define GALAXIEDEKOK        5
#define PENTADECATHLON      6
#define PLANEUR             7

#define RANDOM              8

/* Function prototypes -------------------------------------------------------*/
/**
 * @brief determines whether a cell is alive or not
 * @param[in] cell_         cell array
 * @param[in] line_         vertical coordinate of the array
 * @param[in] column_       horizontal coordinate of the array
 * @return                  a boolean, true: cell is alive, false: cell is dead
 */
bool isCellAlive(const bool cell_[][WIDTH],
                 const int32_t line_,
                 const int32_t column_);

/**
 * @brief displays the game board
 * @param[in] cell_         cell array
 * @param[in] noIteration_  number of iterations performed
 * @param[in] cellAliveCpt_ total number of living cells at current iteration
 */
void lifeGameDisplay(const bool cell_[][WIDTH],
                     const int32_t noIteration_,
                     const int32_t cellAliveCpt_);

/**
 * @brief sets if a cell is dead or alive
 * @param[in]       cell_     cell array
 * @param[in]       line_     vertical coordinate of the array
 * @param[in]       column_   horizontal coordinate of the array
 * @param[in]       life_     bool indicating if the cell should live (1) or die
 *                            (0)
 * @param[in, out]  cellCpt_  total number of living cells (only ++ here)
 */
void setCell(bool cell_[][WIDTH],
             const int32_t line_,
             const int32_t column_,
             const bool life_,
             int32_t *cellCpt_);

/**
 * @brief performs an iteration on the game board
 * @param[in] cellsA_            cell array to analyse
 * @param[in] cellsB_            cell array created from the analysis of cellsA_
 * @param[in, out] cellAliveCpt_ number of living cells (only reset here)
 */
void lifeGameUpdate(const bool cellsA_[][WIDTH],
                    bool cellsB_[][WIDTH],
                    int32_t *cellAliveCpt_);

/**
 * @brief adds a stable structure to a position on the board
 * @param[in] cell_         cell array
 * @param[in] line_         vertical coordinate of the array
 * @param[in] column_       horizontal coordinate of the array
 * @param[in] choice_       element to add initially
 * @param[in] cellAliveCpt_ number of living cells
 */
void addPattern(bool cell_[][WIDTH],
                const int32_t line_,
                const int32_t column_,
                const int32_t choice_,
                int32_t *cellAliveCpt_);

/**
 * @brief copy a table into another one
 * @param[in] tableauOriginal_  array to copy
 * @param[in] tableauCopie_     copied array
 */
void copie(const bool tableauOriginal_[][WIDTH],
           bool tableauCopie_[][WIDTH]);

/**
 * @brief preparation of the game board according to the wanted configuration
 * @param[in] cells_        cell array
 * @param[in] userChoice_   array configuration for the 1st iteration
 * @param[in] cellAliveCpt_ number of living cells
 */
void prepareCells(bool cells_[][WIDTH],
                  const unsigned char userChoice_,
                  int32_t *cellAliveCpt_);

/******************************************************************************/
/*      _____ _           __  __      _                                       */
/*     |_   _| |_  ___   |  \/  | ___(_)_ _                                   */
/*       | | | ' \/ -_)  | |\/| |/ _ | | ' \                                  */
/*       |_| |_||_\___|  |_|  |_|\___|_|_||_|                                 */
/*                                                                            */
/******************************************************************************/
int main()
{
    bool cells1[HEIGHT][WIDTH] = {false};
    bool cells2[HEIGHT][WIDTH] = {false};
    int32_t cellAliveCpt = 0; // number of cell alive
    int32_t noIteration = 1;  // number of iteration
    const unsigned char choice = RANDOM;

    prepareCells(cells1, choice, &cellAliveCpt);

    while (1)
    {
        lifeGameDisplay(cells1, noIteration, cellAliveCpt);
        noIteration++;
        lifeGameUpdate(cells1, cells2, &cellAliveCpt);
        copie(cells2, cells1);
        usleep(100000);
    }
    return 0;
}

/* Function definitions ------------------------------------------------------*/
bool isCellAlive(const bool cell_[][WIDTH],
                 const int32_t line_,
                 const int32_t column_)
{
    return cell_[line_][column_];
}

void setCell(bool cell_[][WIDTH],
             const int32_t line_,
             const int32_t column_,
             const bool life_,
             int32_t *cellCpt_)
{
    if (line_ > 0 && line_ < WIDTH && column_ > 0 && column_ < WIDTH)
        cell_[line_][column_] = life_;

    *cellCpt_ += life_ ? 1 : 0;

    return;
}

void lifeGameDisplay(const bool cell_[][WIDTH],
                     const int32_t noIteration_,
                     const int32_t cellAliveCpt_)
{
    int32_t line = 0;
    int32_t column = 0;

    puts("");

    // Affichage première ligne
    printf("+");
    for (column = 0; column < WIDTH; column++)
    {
        printf("-");
    }
    printf("+       %d      %d\n", noIteration_, cellAliveCpt_);

    // Affichage contenu tableau
    for (line = 0; line < HEIGHT; line++)
    {
        printf("|");
        for (column = 0; column < WIDTH; column++)
        {
            printf("%c", isCellAlive(cell_, line, column) ? '#' : ' ');
        }
        printf("|\n");
    }

    // Affichage dernière ligne
    printf("+");
    for (column = 0; column < WIDTH; column++)
    {
        printf("-");
    }
    printf("+");

    // explicit flush of the stdout to avoid being buffered
    fflush(stdout);

    return;
}

void lifeGameUpdate(const bool cellsA_[][WIDTH],
                    bool cellsB_[][WIDTH],
                    int32_t *cellAliveCpt_)
{
    int32_t line = 0;
    int32_t column = 0;
    int32_t frameLine = 0;
    int32_t frameColumn = 0;
    int32_t sumCellAliveFrame = 0;

    *cellAliveCpt_ = 0; // reset of cpt

    for (line = 1; line < (WIDTH - 1); line++)
    {
        for (column = 1; column < (HEIGHT - 1); column++)
        {
            for (frameLine = line - 1; frameLine <= line + 1; frameLine++)
            {
                for (frameColumn = column - 1; frameColumn <= column + 1; frameColumn++)
                {
                    if ((isCellAlive(cellsA_, frameLine, frameColumn) &&
                         (frameLine != line || frameColumn != column)))
                    {
                        sumCellAliveFrame++;
                    }
                }
            }

            if (sumCellAliveFrame == 3)
            {
                // if 3 adjacent cells are alive, it will live
                setCell(cellsB_, line, column, true, cellAliveCpt_);
            }
            else if (sumCellAliveFrame == 2)
            {
                // if 2 adjacent cells are alive, it will keep its previous state
                setCell(cellsB_, line, column, isCellAlive(cellsA_, line, column), cellAliveCpt_);
            }
            else
            {
                // else it will die
                setCell(cellsB_, line, column, false, cellAliveCpt_);
            }

            sumCellAliveFrame = 0;
        }
    }

    return;
}

void addPattern(bool cell_[][WIDTH],
                const int32_t line_,
                const int32_t column_,
                const int32_t choice_, int32_t *cellAliveCpt_)
{
    int32_t indexA = 0;
    int32_t indexB = 0;

    switch (choice_)
    {
    case 1: // BLOCK 2x2
        setCell(cell_, line_, column_, true, cellAliveCpt_);
        setCell(cell_, line_, column_ + 1, true, cellAliveCpt_);
        setCell(cell_, line_ + 1, column_, true, cellAliveCpt_);
        setCell(cell_, line_ + 1, column_ + 1, true, cellAliveCpt_);
        break;
    case 2: // TUB
        setCell(cell_, line_ - 1, column_, true, cellAliveCpt_);
        setCell(cell_, line_, column_ - 1, true, cellAliveCpt_);
        setCell(cell_, line_, column_ + 1, true, cellAliveCpt_);
        setCell(cell_, line_ + 1, column_, true, cellAliveCpt_);
        break;
    case 3: // navire
        setCell(cell_, line_ - 1, column_ - 1, true, cellAliveCpt_);
        setCell(cell_, line_, column_ - 1, true, cellAliveCpt_);
        setCell(cell_, line_ - 1, column_, true, cellAliveCpt_);
        setCell(cell_, line_ + 1, column_ + 1, true, cellAliveCpt_);
        setCell(cell_, line_, column_ + 1, true, cellAliveCpt_);
        setCell(cell_, line_ + 1, column_, true, cellAliveCpt_);
        break;
    case 4: // clignotants
        for (indexA = -1; indexA <= 1; indexA++)
        {
            setCell(cell_, line_, column_ + indexA, true, cellAliveCpt_);
        }
        break;
    case 5: // galaxie de Kok
        for (indexA = -4; indexA <= -3; indexA++)
        {
            for (indexB = -1; indexB <= 4; indexB++)
            {
                setCell(cell_, line_ + indexA, column_ + indexB, true, cellAliveCpt_);
            }
        }
        for (indexA = -4; indexA <= 1; indexA++)
        {
            for (indexB = -4; indexB <= -3; indexB++)
            {
                setCell(cell_, line_ + indexA, column_ + indexB, true, cellAliveCpt_);
            }
        }
        for (indexA = -1; indexA <= 4; indexA++)
        {
            for (indexB = 3; indexB <= 4; indexB++)
            {
                setCell(cell_, line_ + indexA, column_ + indexB, true, cellAliveCpt_);
            }
        }
        for (indexA = 3; indexA <= 4; indexA++)
        {
            for (indexB = -4; indexB <= 1; indexB++)
            {
                setCell(cell_, line_ + indexA, column_ + indexB, true, cellAliveCpt_);
            }
        }
        break;
    case 6: // pentadécathlon
        setCell(cell_, line_ - 1, column_ - 2, true, cellAliveCpt_);
        setCell(cell_, line_ - 1, column_ + 3, true, cellAliveCpt_);
        for (indexA = -4; indexA <= 5; indexA++)
        {
            if (indexA != -2 & indexA != 3)
            {
                setCell(cell_, line_, column_ + indexA, true, cellAliveCpt_);
            }
        }
        setCell(cell_, line_ + 1, column_ - 2, true, cellAliveCpt_);
        setCell(cell_, line_ + 1, column_ + 3, true, cellAliveCpt_);
        break;
    case 7: // planeur
        setCell(cell_, line_, column_ + 1, true, cellAliveCpt_);
        setCell(cell_, line_ + 1, column_ + 2, true, cellAliveCpt_);
        for (indexA = 0; indexA <= 2; indexA++)
        {
            setCell(cell_, line_ + 2, column_ + indexA, true, cellAliveCpt_);
        }
        break;
    case 8: // LWSS
        setCell(cell_, line_, column_, true, cellAliveCpt_);
        setCell(cell_, line_, column_ + 3, true, cellAliveCpt_);
        setCell(cell_, line_ + 1, column_ + 4, true, cellAliveCpt_);
        setCell(cell_, line_ + 2, column_, true, cellAliveCpt_);
        setCell(cell_, line_ + 2, column_ + 4, true, cellAliveCpt_);
        for (indexA = 1; indexA <= 4; indexA++)
        {
            setCell(cell_, line_ + 3, column_ + indexA, true, cellAliveCpt_);
        }
        break;
    default:
        puts("Unknown structure");
        break;
    }

    return;
}

void copie(const bool tableauOriginal_[][WIDTH], bool tableauCopie_[][WIDTH])
{
    int32_t line = 0;
    int32_t column = 0;
    int32_t cellAliveCpt = 0;

    for (line = 1; line < (WIDTH - 1); line++)
    {
        for (column = 1; column < (HEIGHT - 1); column++)
        {
            setCell(tableauCopie_, line, column, isCellAlive(tableauOriginal_, line, column), &cellAliveCpt);
        }
    }

    return;
}

void prepareCells(bool cells_[][WIDTH],
                  const unsigned char userChoice_,
                  int32_t *cellAliveCpt_)
{
    short line = 0;
    short column = 0;
    int32_t randomNo = 0;
    int32_t cpt = 0;
    const int32_t lineCenter = (HEIGHT / 2) - 1;
    const int32_t columnCenter = (WIDTH / 2) - 1;

    switch (userChoice_)
    {
    case 1: // Structure stable : BLOCK
        addPattern(cells_, lineCenter, columnCenter, BLOCK, cellAliveCpt_);
        break;

    case 2: // Structure stable : TUB
        addPattern(cells_, lineCenter, columnCenter, TUB, cellAliveCpt_);
        break;

    case 3: // Strcture stable : navire
        addPattern(cells_, lineCenter, columnCenter, NAVIRE, cellAliveCpt_);
        break;

    case 4: // Structure oscillante : clignotant
        addPattern(cells_, lineCenter, columnCenter, CLIGNOTANT, cellAliveCpt_);
        break;

    case 5: // Structure oscillante : galaxie de Kok
        addPattern(cells_, lineCenter, columnCenter, GALAXIEDEKOK, cellAliveCpt_);
        break;

    case 6: // Structure oscillante : pentadecathlon
        addPattern(cells_, lineCenter, columnCenter, PENTADECATHLON, cellAliveCpt_);
        break;

    case 7: // Vaisseau de type planeur
        addPattern(cells_, 4, 4, PLANEUR, cellAliveCpt_);
        break;

    case 8: // Random structure
        srand(0);
        do
        {
            line = (rand() % (HEIGHT - 1));
            column = (rand() % (WIDTH - 1));
            if (randomNo < (RAND_MAX / 2) && isCellAlive(cells_, line, column) == false)
            {
                setCell(cells_, line, column, true, cellAliveCpt_);
                cpt++;
            }
        } while (cpt < (HEIGHT - 2) * (WIDTH - 2) / 2);
        break;

    case 9: // 100% filling
        for (line = 1; line < HEIGHT - 1; line++)
        {
            for (column = 1; column < WIDTH - 1; column++)
            {
                setCell(cells_, line, column, true, cellAliveCpt_);
            }
        }
        break;

    default:
        puts("Unkwown configuration value.");
        break;
    }

    return;
}
/* End of main.c -------------------------------------------------------------*/
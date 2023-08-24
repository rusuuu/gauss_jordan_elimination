#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

void allocationCalculus(int& linesNumer, int& rowsNumber)
{
    std::ifstream readFile("ExtendedMatrixInput.txt");

    float element;
    while (readFile.peek() != '\n')
    {
        readFile >> element;
        rowsNumber++;
    }
    linesNumer = 1;
    while (!readFile.eof())
    {
        readFile >> element;
        if (readFile.peek() == '\n') linesNumer++;
    }
    readFile.close();
}

void allocationFunction(float**& extendedMatrix, int& linesNumber, int& rowsNumber)
{
    extendedMatrix = new float* [linesNumber];

    for (int line = 0; line < linesNumber; line++)
    {
        extendedMatrix[line] = new float[rowsNumber];
    }
}

void deallocation(float**& extendedMatrix, int& linesNumber, int& rowsNumber)
{
    for (int line = 0; line < linesNumber; line++)
    {
        delete[] extendedMatrix[line];
    }
    delete[] extendedMatrix;
}

void inputFunction(float**& extendedMatrix, int& linesNumber, int& rowsNumber)
{
    std::ifstream readFile("ExtendedMatrixInput.txt");
    allocationCalculus(linesNumber, rowsNumber);
    allocationFunction(extendedMatrix, linesNumber, rowsNumber);

    for (int line = 0; line < linesNumber; line++)
    {
        for (int row = 0; row < rowsNumber; row++)
        {
            readFile >> extendedMatrix[line][row];
        }
    }
    readFile.close();
}

void outputFunction(float**& extendedMatrix, int& linesNumber, int& rowsNumber)
{
    for (int line = 0; line < linesNumber; line++)
    {
        for (int row = 0; row < rowsNumber; row++)
        {
            std::cout << extendedMatrix[line][row] << " ";
        }
        std::cout << std::endl;
    }
}

float gcd(float a, float b)
{
    a = fabs(a);
    b = fabs(b);

    if (a < b) return gcd(b, a);
    if (fabs(b) < 0.001) return a;
    else return (gcd(b, a - floor(a / b) * b));
}

int simplifyLine(float**& extendedMatrix, int currentLine, int& rowsNumber)
{
    int row = 0;
    while (extendedMatrix[currentLine][row] == 0 && row < rowsNumber)
    {
        row++;
    }
    float GCD = extendedMatrix[currentLine][row];
    if (GCD > 1 || GCD <= -1)
    {
        for (row = row + 1; row < rowsNumber; row++)
        {
            if (extendedMatrix[currentLine][row])
            {
                GCD = gcd(GCD, extendedMatrix[currentLine][row]);
            }
        }

        if (GCD > 1 || GCD <= -1)
        {
            for (row = 0; row < rowsNumber; row++)
            {
                extendedMatrix[currentLine][row] /= GCD;
            }
        }
    }

    return GCD;
}

void swapLines(float**& extendedMatrix, int firstLine, int secondLine, int& rowsNumber)
{
    for (int row = 0; row < rowsNumber; row++)
    {
        extendedMatrix[firstLine][row] = extendedMatrix[firstLine][row] + extendedMatrix[secondLine][row];
        extendedMatrix[secondLine][row] = extendedMatrix[firstLine][row] - extendedMatrix[secondLine][row];
        extendedMatrix[firstLine][row] = extendedMatrix[firstLine][row] - extendedMatrix[secondLine][row];
    }
}

void GaussRule(float**& extendedMatrix, int currentLine, int currentRow, int amplifierLine, int& rowsNumber)
{
    float amplifier = -extendedMatrix[currentLine][currentRow] / extendedMatrix[amplifierLine][currentRow];

    for (int row = currentRow; row < rowsNumber; row++)
    {
        extendedMatrix[currentLine][row] = extendedMatrix[currentLine][row] + extendedMatrix[amplifierLine][row] * amplifier;
    }
}

int incompatibleCheck(float**& extendedMatrix, int& linesNumber, int rowsNumber)
{
    for (int line = 0; line < linesNumber; line++)
    {
        int numberOfNot0 = 0;
        for (int row = 0; row < rowsNumber - 1; row++)
        {
            if (extendedMatrix[line][row]) numberOfNot0++;
        }
        if (numberOfNot0 == 0 && extendedMatrix[line][rowsNumber - 1] != 0) return 1;
    }
    return 0;
}

void applicationOfGaussRule(float**& extendedMatrix, int& linesNumber, int& rowsNumber)
{
    int line = 0, row = 0;
    while (row < rowsNumber && line < linesNumber)
    {
        if (extendedMatrix[line][row] == 0)
        {
            int secondaryLine = line + 1;
            while (secondaryLine < linesNumber && extendedMatrix[secondaryLine][row] == 0)
            {
                secondaryLine++;
            }
            if (secondaryLine < linesNumber)
            {
                swapLines(extendedMatrix, row, secondaryLine, rowsNumber);
                outputFunction(extendedMatrix, linesNumber, rowsNumber);
                std::cout << std::endl << std::endl;
            }
            else row++;
        }

        if (extendedMatrix[line][row] != 0)
        {
            if (extendedMatrix[line][row] != 1)
            {
                float amplifier = 1 / extendedMatrix[line][row];
                for (int index = row; index < rowsNumber; index++)
                {
                    extendedMatrix[line][index] = extendedMatrix[line][index] * amplifier;
                }
                outputFunction(extendedMatrix, linesNumber, rowsNumber);
                std::cout << std::endl << std::endl;
            }

            for (int secondaryLine = 0; secondaryLine < linesNumber; secondaryLine++)
            {
                float simplifier = simplifyLine(extendedMatrix, secondaryLine, rowsNumber);
                if (simplifier > 1 || simplifier < -1)
                {
                    outputFunction(extendedMatrix, linesNumber, rowsNumber);
                    std::cout << std::endl << std::endl;
                }
                if (secondaryLine != line)
                {
                    if (extendedMatrix[secondaryLine][row] != 0)
                    {
                        GaussRule(extendedMatrix, secondaryLine, row, line, rowsNumber);
                        outputFunction(extendedMatrix, linesNumber, rowsNumber);
                        std::cout << std::endl << std::endl << std::endl;
                    }
                }
            }
            line++;
            row++;
        }

        if (incompatibleCheck(extendedMatrix, linesNumber, rowsNumber) == 1)
        {
            std::cout << "The introduced linear equation system is independent and inconsistent!" << std::endl << std::endl;
            exit(1);
        }
    }
}

int typeOfUnknown(float**& extendedMatrix, int& linesNumber, int& rowsNumber, int& currentLine, int& currentRow)
{
    int type = 1;

    if (currentRow > 0)
    {
        for (int row = currentRow - 1; row >= 0; row--)
        {
            if (extendedMatrix[currentLine][row] == 1) type = -1;
        }
    }

    return type;
}

void interpretationOfFinalMatrix(float**& extendedMatrix, int& linesNumber, int& rowsNumber)
{
    int unknowns = rowsNumber - 1;
    int principalUnknowns = 0, secondaryUnknowns = 0;

    for (int row = 0; row < rowsNumber - 1; row++)
    {
        for (int line = 0; line < linesNumber; line++)
        {
            if (extendedMatrix[line][row] != 0)
            {
                if (typeOfUnknown(extendedMatrix, linesNumber, rowsNumber, line, row) == 1)
                {
                    principalUnknowns++;
                    break;
                }
                if (typeOfUnknown(extendedMatrix, linesNumber, rowsNumber, line, row) == -1)
                {
                    secondaryUnknowns++;
                    break;
                }
            }
        }
    }

    if (unknowns == principalUnknowns)
    {
        std::cout << std::endl << std::endl; std::cout << "The introduced linear equation system is independent and consistent!" << std::endl << std::endl;
    }
    else
    {
        std::cout << std::endl << std::endl; std::cout << "The introduced linear equation system is dependent and consistent with " << secondaryUnknowns << " secondary unknowns!" << std::endl << std::endl;
    }

    struct solution
    {
        int type, order, line, row;
    };
    solution* solutionsVector = new solution[rowsNumber - 1];

    int contorOfSolutionsVector = 0, contorOfPrincipalUnknowns = 1, contorOfSecondaryUnknowns = 1;
    for (int row = 0; row < rowsNumber - 1; row++)
    {
        for (int line = 0; line < linesNumber; line++)
        {
            if (extendedMatrix[line][row] != 0)
            {
                if (typeOfUnknown(extendedMatrix, linesNumber, rowsNumber, line, row) == 1)
                {
                    solutionsVector[contorOfSolutionsVector].type = 1;
                    solutionsVector[contorOfSolutionsVector].order = contorOfPrincipalUnknowns++;
                    solutionsVector[contorOfSolutionsVector].line = line;
                    solutionsVector[contorOfSolutionsVector].row = row;
                    contorOfSolutionsVector++;
                    break;
                }
                if (typeOfUnknown(extendedMatrix, linesNumber, rowsNumber, line, row) == -1)
                {
                    solutionsVector[contorOfSolutionsVector].type = -1;
                    solutionsVector[contorOfSolutionsVector].order = contorOfSecondaryUnknowns++;
                    solutionsVector[contorOfSolutionsVector].line = line;
                    solutionsVector[contorOfSolutionsVector].row = row;
                    contorOfSolutionsVector++;
                    break;
                }
            }
        }
    }

    std::cout << "Solution = {";
    for (int index = 0; index < contorOfSolutionsVector; index++)
    {
        std::cout << " x" << index + 1 << " = ";
        if (solutionsVector[index].type == -1)
        {
            std::cout << "α" << solutionsVector[index].order;
        }
        else
        {
            std::cout << extendedMatrix[solutionsVector[index].line][rowsNumber - 1];
            for (int row = solutionsVector[index].row + 1; row < rowsNumber - 1; row++)
            {
                if (extendedMatrix[solutionsVector[index].line][row])
                {
                    if (extendedMatrix[solutionsVector[index].line][row] < 0)
                    {
                        std::cout << "+";
                    }
                    std::cout << (-1) * extendedMatrix[solutionsVector[index].line][row] << "α" << solutionsVector[row].order;
                }
            }
        }
        if (index < contorOfSolutionsVector - 1) std::cout << ",";
    }
    std::cout << " }" << std::endl << std::endl;
}

int main()
{
    int linesNumber = 0, rowsNumber = 0;
    float** extendedMatrix;

    std::cout << std::setprecision(3);
    inputFunction(extendedMatrix, linesNumber, rowsNumber);
    outputFunction(extendedMatrix, linesNumber, rowsNumber);
    std::cout << std::endl << std::endl;
    applicationOfGaussRule(extendedMatrix, linesNumber, rowsNumber);
    interpretationOfFinalMatrix(extendedMatrix, linesNumber, rowsNumber);
    deallocation(extendedMatrix, linesNumber, rowsNumber);

    return 0;
}
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

#define SIZE 4

struct Matrix {
    std::vector<std::vector<int>> _data;
    int _nRows = SIZE;
    int _nCols = SIZE;

    std::vector<int> &operator[](const int index) {
        return _data[index];
    }

    Matrix operator=(Matrix &m) {
        _data = m._data;
        _nRows = m._nRows;
        _nCols = m._nCols;
//
//        Matrix &tmp = *this;
//        for (int i = 0; i < SIZE; i++) {
//            for (int j = 0; j < SIZE; j++) {
//                tmp[i][j] = m[i][j];
//            }
//        }

        return *this;
    }

    Matrix(std::vector<int> &array, int nRows, int nCols) {
        _nRows = nRows;
        _nCols = nCols;
        _data.assign(_nRows, std::vector<int>(_nCols));
        int arrayIndex = 0;
        for (int i = 0; i < _nRows; i++) {
            for (int j = 0; j < _nCols; j++) {
                _data[i][j] = array[arrayIndex];
                arrayIndex++;
            }
        }
    }

    void deleteRow(int row) {
        _data.erase(_data.begin() + row);
        _nRows--;
    }

    std::vector<int> getVecOfMins() {
        Matrix &tmp = *this;
        std::vector<int> vec(_nRows);
        for (int i = 0; i < _nRows; i++) {
            int min = tmp[i][0];
            for (int j = 0; j < _nCols; j++) {
                if (tmp[i][j] < min)
                    min = tmp[i][j];
            }
            vec[i] = min;
        }

        return vec;
    }

    //находит сумму минимальных элементов матрицы, что будет являться "нижней" границей
    int findTheLowerBound() {
        std::vector<int> vecOfMins = getVecOfMins();
        int sum = 0;
        for (auto &elem: vecOfMins) {
            sum += elem;
        }

        return sum;
    }

    //вычитает минимальные элементы строки из нее
    void subtractMins() {
        Matrix &tmp = *this;
        std::vector<int> vec = getVecOfMins();
        for (int i = 0; i < _nRows; i++) {
            int min = vec[i];
            for (int j = 0; j < _nCols; j++) {
                if(tmp[i][j] != INT32_MAX) {
                    tmp[i][j] -= min;
                }
            }
        }
    }

    void print() {
        Matrix &tmp = *this;
        for (int i = 0; i < _nRows; i++) {
            for (int j = 0; j < _nCols; j++) {
                if(tmp[i][j] == INT32_MAX) {
                    std::cout << "inf" << ' ';
                } else {
                    std::cout << tmp[i][j] << std::setw(3) << ' ';
                }
            }
            std::cout << '\n';
        }
    }

    void transpose() {
        Matrix &tmp = *this;

        std::vector<int> vec(_nRows * _nCols, 0);
        Matrix m(vec, _nCols, _nRows);
        for (int i = 0; i < _nRows; i++) {
            for (int j = 0; j < _nCols; j++) {
                m[j][i] = tmp[i][j];
            }
        }

        tmp = m;
    }

    std::vector<int> findMarksOfNullCells() {
        std::vector<int> position(3);
        int sumOfMinElements = 0;
        int maxSum = 0;
        for (int i = 0; i < _nRows; i++) {
            for (int j = 0; j < _nCols; j++) {
                Matrix tmp = *this;
                if (tmp[i][j] == 0) {
                    auto &row = tmp[i];
                    row.erase(row.begin() + j);
                    sumOfMinElements += *std::min_element(row.begin(), row.end());
                    tmp.transpose();
                    auto &col = tmp[j];
                    col.erase(col.begin() + i);
                    sumOfMinElements += *std::min_element(col.begin(), col.end());
                    tmp.transpose();
                    if (maxSum < sumOfMinElements) {
                        maxSum = sumOfMinElements;
                        position = {i, j, maxSum};
                    }

                    sumOfMinElements = 0;
                }
            }
        }

        return position;
    }

    void firstReduction(int &sum, int &sumWithoutPath, std::vector<int> &positionOfNull) {
        Matrix &tmp = *this;
        sum += findTheLowerBound();
        subtractMins();
        transpose();
        sum += findTheLowerBound();
        subtractMins();
        transpose();

        positionOfNull = tmp.findMarksOfNullCells();
        int priceWithoutPath = positionOfNull[2];
        sumWithoutPath = sum + priceWithoutPath;
    }

    void reduction(int &sum, int &sumWithoutPath, std::vector<int> positionOfNull) {
        Matrix &tmp = *this;
        tmp.firstReduction(sum, sumWithoutPath, positionOfNull);
        int rowDel = positionOfNull[0];
        int colDel = positionOfNull[1];
        int priceWithoutPath = positionOfNull[2];

        while(tmp._nRows != 1) {
            if (sum < sumWithoutPath) {
                tmp.deleteRow(rowDel);
                tmp.transpose();
                tmp.deleteRow(colDel);
                tmp.transpose();
                tmp.firstReduction(sum, sumWithoutPath, positionOfNull);
                rowDel = positionOfNull[0];
                colDel = positionOfNull[1];
                priceWithoutPath = positionOfNull[2];
            }
        }
    }

//    int firstReduction(std::vector<int> position) {
//        Matrix tmp = *this;
//        tmp.deleteRow(position[0]);
//        tmp.transpose();
//        tmp.deleteRow(position[1]);
//        tmp.transpose();
//
//        int sumWithoutElem = tmp.findTheLowerBound();
//        sumWithoutElem += position[2];
//        int sumWithElem;
//
//        return sumWithoutElem;
//    }
};

int main() {
//    std::vector<int> vec = {INT32_MAX, 10, 3, 5, 8, INT32_MAX, 3, 4, 6, 4, INT32_MAX, 7, 2, 2, 1, INT32_MAX};
//    Matrix path(vec, SIZE, SIZE);
//    int sum = path.findTheLowerBound();
//    path.subtractMins();
//    path.transpose();
//    sum += path.findTheLowerBound();
//    path.subtractMins();
//    path.transpose();
//    std::vector<int> position = path.findMarksOfNullCells();
////    int res = path.firstReduction(position);
//
////    path.print();
////    std::cout << res;
//
//    int sumWithoutElem = sum + position[2];
//    int sumWithElem = sum;
//
//    path[position[1]][position[0]] = INT32_MAX;
//
//    path.deleteRow(position[0]);
//    path.transpose();
//    path.deleteRow(position[1]);
//    path.transpose();
//
//
//
//    sum += path.findTheLowerBound();
//    path.subtractMins();
//    path.transpose();
//    sum += path.findTheLowerBound();
//    path.subtractMins();
//    path.transpose();
//
//    position = path.findMarksOfNullCells();
//    sumWithoutElem = sumWithElem + position[2];
//    sumWithElem = sum;
//
//    path.deleteRow(position[0]);
//    path.transpose();
//    path.deleteRow(position[1]);
//    path.transpose();
//
//    position = path.findMarksOfNullCells();
//    sumWithoutElem = sumWithElem + position[2];
//    sumWithElem = sum;
//
//    path.deleteRow(position[0]);
//    path.transpose();
//    path.deleteRow(position[1]);
//    path.transpose();
//
//    position = path.findMarksOfNullCells();
//
//
//
//    std::cout << sumWithoutElem;
//
//    path.print();

    std::vector<int> vec = {INT32_MAX, 10, 3, 5, 8, INT32_MAX, 3, 4, 6, 4, INT32_MAX, 7, 2, 2, 1, INT32_MAX};
    Matrix path(vec, SIZE, SIZE);
    int sum = 0;
    int sumWithoutPath;
    std::vector<int> positionOfNull = {0};

//    path.reduction(sum, sumWithoutPath, positionOfNull);
    path.print();

//    path.firstReduction(sum);
//    std::vector<int> positionOfNull = path.findMarksOfNullCells();
//    int rowDel = positionOfNull[0];
//    int colDel = positionOfNull[1];
//    int priceWithoutPath = positionOfNull[2];
//    sumWithoutPath = sum + priceWithoutPath;
//    path[colDel][rowDel] = INT32_MAX;
//    path.deleteRow(rowDel);
//    path.transpose();
//    path.deleteRow(colDel);
//    path.transpose();
//    path.firstReduction(sum);
//    positionOfNull = path.findMarksOfNullCells();
//    sumWithoutPath = sum + positionOfNull[2];
//    std::cout << sumWithoutPath;







    return 0;
}

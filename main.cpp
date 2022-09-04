#include <iostream>
#include <vector>
#include <algorithm>

#define SIZE 4

struct Matrix {
    std::vector<std::vector<int>> _data;
    int _nRows = SIZE;
    int _nCols = SIZE;

    std::vector<int> &operator[](const int index) {
        return _data[index];
    }

    Matrix operator=(Matrix &m) {
        Matrix &tmp = *this;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                tmp[i][j] = m[i][j];
            }
        }

        return tmp;
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

    int findTheLowerBound() {
        Matrix &tmp = *this;
        std::vector<int> vec = getVecOfMins();
        for (int i = 0; i < _nRows; i++) {
            int min = vec[i];
            for (int j = 0; j < _nCols; j++) {
                tmp[i][j] -= min;
            }
        }

        int sum = 0;
        for (auto &elem: vec) {
            sum += elem;
        }

        return sum;
    }

    void print() {
        Matrix &tmp = *this;
        for (int i = 0; i < _nRows; i++) {
            for (int j = 0; j < _nRows; j++) {
                std::cout << tmp[i][j] << ' ';
            }
            std::cout << '\n';
        }
    }

    void transpose() {
        Matrix &tmp = *this;
        std::vector<int> vec(SIZE * SIZE, 0);
        Matrix m(vec, SIZE, SIZE);
        for (int i = 0; i < _nRows; i++) {
            for (int j = 0; j < _nCols; j++) {
                m[i][j] = tmp[j][i];
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
                    if(maxSum < sumOfMinElements) {
                        maxSum = sumOfMinElements;
                        position = {i, j, maxSum};
                    }

                    sumOfMinElements = 0;
                }
            }
        }

        return position;
    }

    void reduction(std::vector<int> position) {
        Matrix &tmp = *this;
        tmp.deleteRow(position[0]);
        tmp.transpose();
        tmp.deleteRow(position[1]);
        tmp.transpose();

        int sumWithoutElem = tmp.findTheLowerBound() + position[2];
        int sumWithElem;

        return sumWithoutElem;



    }
};

int main() {
    std::vector<int> vec = {INT32_MAX, 7, 6, 2, 7, INT32_MAX, 3, 1, 6, 3, INT32_MAX, 4, 2, 1, 4, INT32_MAX};
    Matrix path(vec, SIZE, SIZE);
    int sum = path.findTheLowerBound();
    path.transpose();
    sum += path.findTheLowerBound();
    path.transpose();
    std::vector<int> position = path.findMarksOfNullCells();
//    path.reduction(position);

    path.print();

    return 0;
}

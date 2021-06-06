#include "Filter.h"

using namespace DSP;
Filter::Filter() { // Shady constructor. Fix later...
    _aSize = 1;
    _bSize = 1;
    _a = (double*) &_aSize;
    _b = (double*) &_bSize;
    _k = 1;
    _inputList = new double[_bSize];
    _outputList = new double[_aSize];
}

Filter::Filter(double *b, int bSize, double *a, int aSize, double k) {
    _b = b;
    _a = a;
    _k = k;
    _bSize = bSize;
    _aSize = aSize;
    _inputList = new double[bSize];
    _outputList = new double[aSize];
    clear();
}

Filter::~Filter() {
    delete[] _inputList;
    delete[] _outputList;
}

void Filter::_shiftList(double *list, int listSize, double value) {
    for (int i = listSize - 1; i > 0; i--) {
        list[i] = list[i - 1];
    }
    list[0] = value;
}

void Filter::clear() {
    for (int i = 0; i < _bSize; i++) _inputList[i] = 0;
    for (int i = 0; i < _aSize; i++) _outputList[i] = 0;
}

double Filter::getLatest() {
    return _outputList[0];
}

double Filter::filterValue(double xNew) {
    _shiftList(_inputList, _bSize, xNew);
    _shiftList(_outputList, _aSize, 0);
    
    double bxSum = 0;
    double aySum = 0;
    for (int i = 0; i < _bSize; i++) bxSum += _b[i] * _inputList[i];
    for (int i = 1; i < _aSize; i++) aySum += _a[i] * _outputList[i];

    double yNew = _k * (bxSum - aySum) / _a[0];
    _outputList[0] = yNew;

    return yNew;
}

void Filter::filterList(double *filteredList, double *unfilteredList, int listSize) {
    clear();
    for (int i = 0; i < listSize; i++) {
        filteredList[i] = filterValue(unfilteredList[i]);
    }
}
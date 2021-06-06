#include "MultiChannelFilter.h"

using namespace DSP;
MultiChannelFilter::MultiChannelFilter(int channels, double *b, int bSize, double *a, int aSize, double k) {
    _numFilters = channels;
    _filters = new Filter[channels];
    _latest = new double[_numFilters];

    for (int i = 0; i < _numFilters; i++) {
        _filters[i] = Filter(b, bSize, a, aSize, k);
    }
    
} 

MultiChannelFilter::~MultiChannelFilter() {
    delete _filters;
    delete _latest;
}

void MultiChannelFilter::filterValue(double *yNew, double *xNew) {
    for (int i = 0; i < _numFilters; i++) {
        yNew[i] = _filters[i].filterValue(xNew[i]);
        _latest[i] = yNew[i];
    }
}

double *MultiChannelFilter::getLatest() {
    return _latest;
}

void MultiChannelFilter::clear() {
    for (int i = 0; i < _numFilters; i++) {
        _filters[i].clear();
    }
}
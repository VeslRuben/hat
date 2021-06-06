#ifndef MULTI_CHANNEL_FILTER_H
#define MULTI_CHANNEL_FILTER_H

#include "Filter.h"
namespace DSP {
class MultiChannelFilter {
    public:
        MultiChannelFilter(int channels, double *b, int bSize, double *a, int aSize, double k = 1);
        ~MultiChannelFilter();

        void filterValue(double *yNew, double *xNew);
        // void filterList(double *filteredList, double *unfilteredList, int listSize);
        double *getLatest();
        void clear();

    private:
        Filter *_filters;
        int _numFilters;
        double *_latest;
};
}
#endif
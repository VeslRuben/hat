#ifndef FILTER_H
#define FILTER_H

namespace DSP {
class Filter {
    public:
        Filter();
        Filter(double *b, int bSize, double *a, int aSize, double k = 1.0);
        ~Filter();

        double filterValue(double xNew);
        void filterList(double *filteredList, double *unfilteredList, int listSize);
        double getLatest();
        void clear();

    private:
        double *_b;
        double *_a;
        double _k;
        double *_inputList;
        double *_outputList;

        int _bSize;
        int _aSize;

        void _shiftList(double *list, int listSize, double value);
};
}

#endif
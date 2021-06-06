#include "Filter.h"
#include <iostream>
#include <math.h>
#include "MultiChannelFilter.h"



void printList(std::string name, double *list, int len);

int main() {
    double b[] = {0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1};
    double a[] = {1};
    DSP::Filter filter = DSP::Filter(b, 10, a, 1);

    // Test step input
    int inputLen = 20;
    double input[inputLen];
    double output[inputLen];
    for (int i = 0; i < inputLen; i++) {
        input[i] = 1;
    }
    filter.filterList(output, input, inputLen);


    inputLen = 200;
    double x1[inputLen];
    double x2[inputLen];
    double xSum[inputLen];
    double t[inputLen];
    double y[inputLen];
    t[0] = 0;

    for (int i = 1; i < inputLen; i++) {
        t[i] = t[i-1] + (2.0 / (double)inputLen); 
    }

    double w1 = 2*M_PI*3;
    double w2 = 2*M_PI*30;

    for (int i = 0; i < inputLen; i++) {
        x1[i] = sin(w1*t[i]);
        x2[i] = 0.2*sin(w2*t[i]);
        xSum[i] = x1[i] + x2[i] + 2;
    }

    filter.filterList(y, xSum, inputLen);

    printList("time", t, inputLen);
    printList("X", xSum, inputLen);
    printList("Y", y, inputLen);

    
    DSP::MultiChannelFilter mcf = DSP::MultiChannelFilter(3, b, 10, a, 1);
    
    

    return 1;
}

void printList(std::string name, double *list, int len) {
    std::cout << name << " = [" << list[0];
    for (int i = 1; i < len; i++) {std::cout << ", " << list[i];}
    std::cout << "]" << std::endl;
}
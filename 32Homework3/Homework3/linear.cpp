//
//  main.cpp
//  32Homework3.2
//
//  Created by Shreya Donepudi on 12/02/17.
//  Copyright © 2017 Brute. All rights reserved.
//
#include <iostream>
#include <cmath>
#include <cassert>
using namespace std;
//The following is a declaration of a function that takes a double and returns true if a particular property of that double is true, and false otherwise. (Such a function is called a predicate.)

//bool somePredicate(double x);
//Here is an example of an implementation of the predicate x is negative:

// Return false if the somePredicate function returns false for at
// least one of the array elements; return true otherwise.
bool allTrue(const double a[], int n)
{
    if (n <= 0)
        return false;
    if (n == 1)
        return somePredicate(a[0]);
    return (somePredicate(*a) && allTrue(a+1, n-1));
}

// Return the number of elements in the array for which the
// somePredicate function returns false.
int countFalse(const double a[], int n)
{
    if (n <= 0)
        return 0;
    if (!somePredicate(a[0]))
            return 1 + countFalse(a+1, n-1);
    else
        return (countFalse(a+1, n-1));
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns false.  If there is no such
// element, return -1.
int firstFalse(const double a[], int n)
{
    if (n <= 0)
        return -1;
    if (!somePredicate(a[0]))
        return 0;
    else
    {
        if (firstFalse(a+1, n-1) == -1)
            return -1;
        return 1 + firstFalse(a + 1, n-1);
    }
}

// Return the subscript of the smallest double in the array (i.e.,
// the one whose value is <= the value of all elements).  If more
// than one element has the same smallest value, return the smallest
// subscript of such an element.  If the array has no elements to
// examine, return -1.
int indexOfMin(const double a[], int n)
{
    if (n <= 0)
        return -1;
    if (n == 1)
        return 0;
    int index = 1 + indexOfMin(a+1, n-1);
    if (a[0] <= a[index])
        return 0;
    else
        return index;
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 <= 0)
        return true; // 0
    if (n2 > n1)
        return false; // this is not a valid scenario
    if (a1[0] == a2[0])
        return includes(a1+1, n1-1, a2+1, n2-1);
    else
        return includes(a1+1, n1-1, a2, n2);
}

/*int main()
{
    // tests for allTrue
    double drr[10] = { -9, 1, -3, -9, -9};
    assert(allTrue(drr, 5) == false);
    double err[10] = { 9, -1, -3, -9, 9};
    assert(allTrue(err, 5) == false);
    double frr[10] = { -9, -1, -3, -9, -9};
    assert(allTrue(frr, 5) == true);
    assert(allTrue(frr, -1) == false);
    assert (allTrue(frr, 0) == false);
    cout << "Passed all tests for allTrue" << endl;
    
    //tests for countFalse
    double s[5] = { 2, 1 , 6, 1, -9};
    assert(countFalse(s, 5) == 4);
    double h[5] = { -2, -1 , -6, -4, -9};
    assert(countFalse(h, 5) == 0);
    assert(countFalse(h, 0) == 0);
    assert(countFalse(h, -9) == 0);
    double r[5] = {2, 1 , 6, 8, 9};
    assert(countFalse(r, 5) == 5);
    cout << "Passed all tests for countFalse" << endl;
    
    
    // tests for first False
    double arr[10] = { -9, -1, -3, -9, 9};
    assert(firstFalse(arr, 5) == 4);
    assert(firstFalse(arr, 0) == -1);
    assert(firstFalse(arr, -8) == -1);
    double brr[10] = { 9, -1, -3, -9, 9};
    assert(firstFalse(brr, 5) == 0);
    double crr[10] = { -9, -1, 3, -9, 9};
    assert(firstFalse(crr, 5) == 2);
    double grr[10] = { -9, -1, -3, -9, -9};
    assert(firstFalse(grr, 5) == -1);
    cout << "Passed all tests for firstFalse" << endl;
    
    // tests for indexOfMin
    double e[5] = { 2, 1 , 6, 1, 9};
    assert(indexOfMin(e, 5) == 1);
    double y[5] = { -2, -1 , -6, -4, -9};
    assert(indexOfMin(y, 5) == 4);
    assert(indexOfMin(y, 0) == -1);
    assert(indexOfMin(y, -9) == -1);
    double a[5] = {1, 7 , 6, 8, 9};
    assert(indexOfMin(a, 5) == 0);
    cout << "Passed all tests for indexOfMin" << endl;
    
    
    // tests for includes
    double d[7] = {10, 50, 40, 20, 50, 40, 30};
    double o[3] = {50, 40, 20};
    double n[8] = {3, 5, 3, 0, 9, 67, 98, 90};
    double p[1] = {50};
    double u[1] = {10};
    double i[1] = {30};
    double v[3] = {50, 40, 30};
    assert(includes(d, 7, o, 3) == true);
    assert(includes(d, -7, o, 3) == false);
    assert(includes(d, -7, o, -3) == true);
    assert(includes(d, 7, o, -3) == true);
    assert(includes(d, 0, o, 9) == false);
    assert(includes(d, 0, o, 0) == true);
    assert(includes(d, 7, o, 0) == true);
    assert(includes(d, 7, n, 8) == false);
    assert(includes(d, 7, n, 3) == false);
    assert(includes(d, 7, p, 1) == true);
    assert(includes(d, 7, u, 1) == true);
    assert(includes(d, 7, i, 1) == true);
    assert(includes(d, 7, v, 3) == true);
    assert(includes(d, 7, v, 1) == true);
    cout << "Passed all tests for includes" << endl;
}*/

#include <iostream>
#include <cassert>
using namespace std;

// Return the number of ways that all n2 elements of a2 appear in
       // the n1 element array a1 in the same order (though not necessarily
       // consecutively).  The empty sequence (i.e. one where n2 is 0)
       // appears in a sequence of length n1 in 1 way, even if n1 is 0.
       // For example, if a1 is the 7 element array
       //        10 50 40 20 50 40 30
       // then for this value of a2     the function must return
       //        10 20 40                        1
       //        10 40 30                        2
       //        20 10 40                        0
       //        50 40 30                        3
int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 <= 0) { //base case, empty n2 means everything was found
        return 1;
    }
    if (n1 <= 0 || n1 < n2) {
        return 0;
    }
    int count = 0;
    if (a1[0] == a2[0]) {
        count += countIncludes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);//count occurences of the next two characters
    }
       count += countIncludes(a1 + 1, n1 - 1, a2, n2);//keep looking for instances of current first character of a2
       return count;
}

// Exchange two doubles
void exchange(double& x, double& y)
{
    double t = x;
    x = y;
    y = t;
}

// Rearrange the elements of the array so that all the elements
// whose value is > divider come before all the other elements,
// and all the elements whose value is < divider come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= divider, or n if there is no such element, and firstLess is
// set to the index of the first element that is < divider, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > divider
//   * for firstNotGreater <= i < firstLess, a[i] == divider
//   * for firstLess <= i < n, a[i] < divider
// All the elements > divider end up in no particular order.
// All the elements < divider end up in no particular order.
void divide(double a[], int n, double divider,
    int& firstNotGreater, int& firstLess)
{
    if (n < 0)
        n = 0;

    // It will always be the case that just before evaluating the loop
    // condition:
    //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
    //  Every element earlier than position firstNotGreater is > divider
    //  Every element from position firstNotGreater to firstUnknown-1 is
    //    == divider
    //  Every element from firstUnknown to firstLess-1 is not known yet
    //  Every element at position firstLess or later is < divider

    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess)
    {
        if (a[firstUnknown] < divider)
        {
            firstLess--;
            exchange(a[firstUnknown], a[firstLess]);
        }
        else
        {
            if (a[firstUnknown] > divider)
            {
                exchange(a[firstNotGreater], a[firstUnknown]);
                firstNotGreater++;
            }
            firstUnknown++;
        }
    }
}

// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n)
{
    if (n <= 1) {
        
    }
    else {
        int firstNotGreat = 0;
        int firstLesser = 0;
        divide(a, n, a[0], firstNotGreat, firstLesser);
        order(a, firstNotGreat);
        order(a + firstLesser, n - firstLesser);
    }
}

int main() {
    // Test case 2: Array with one element
    double singleElementArray[] = { 42 };
    order(singleElementArray, 1);
    assert(singleElementArray[0] == 42); // Single element, no change expected

    // Test case 3: Array with multiple elements
    double array[] = { 10, 50, 40, 20, 50, 40, 30 };
    int n = sizeof(array) / sizeof(array[0]);
    order(array, n);
    for (int i = 0; i < 7; i++) {
        cout << array[i];
    }
    cout << endl;
    
    double a[] = { -1, -2, -1, -2, -1 };
    order(a, 5);
    for (int i = 0; i < 5; i++) {
        cout << a[i];
    }
    cout << endl;
    double b[] = { -1, -2 };
    assert(countIncludes(a, 5, b, 2) == 6);
    b[0] = 1;
    assert(countIncludes(a, 5, b, 2) == 0);
    double c[] = { 10, 50, 40, 20, 50, 40, 30 };
    double d[] = { 10, 20, 40 };
    double e[] = { 10, 40, 30 };
    double f[] = { 50, 40, 30 };
    double g[] = { 20, 10, 40 };
    assert(countIncludes(c, 7, d, 3) == 1);
    assert(countIncludes(c, 7, e, 3) ==2);
    assert(countIncludes(c, 7, f, 3) == 3);
    assert(countIncludes(c, 7, g, 3) == 0);
    return 0;
}
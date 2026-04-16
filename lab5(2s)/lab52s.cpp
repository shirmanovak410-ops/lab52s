//
// Created by Ксения on 12.04.2026.
//
#include <iostream>
#include <stdexcept>
#include <string>
class ArithmeticProgressionException : public std::invalid_argument {
private:
    std::string what_;
    double firstElement;
    double difference;
    double* array;
    int size;
public:
    ArithmeticProgressionException(const std::string& ch, double first, double diff, double* arr, int sz)
        : std::invalid_argument(ch), what_(ch), firstElement(first), difference(diff), size(sz) {
        if (sz > 0 && arr != nullptr) {
            array = new double[sz];
            for (int i = 0; i < sz; i++) {
                array[i] = arr[i];
            }
        } else {
            array = nullptr;
        }
    }
    ArithmeticProgressionException(const ArithmeticProgressionException& other)
        : std::invalid_argument(other), what_(other.what_),
          firstElement(other.firstElement), difference(other.difference), size(other.size) {
        if (size > 0 && other.array != nullptr) {
            array = new double[size];
            for (int i = 0; i < size; i++) {
                array[i] = other.array[i];
            }
        } else {
            array = nullptr;
        }
    }
    ~ArithmeticProgressionException() {
        delete[] array;
    }
    const char* what() const noexcept override {
        return what_.c_str();
    }
    void printAll() const {
        std::cout << "Exception: " << what() << std::endl;
        std::cout << "First element: " << firstElement << std::endl;
        std::cout << "Difference: " << difference << std::endl;
        std::cout << "Array size: " << size << std::endl;
        std::cout << "Array elements: ";
        if (array != nullptr && size > 0) {
            for (int i = 0; i < size; i++) {
                std::cout << array[i] << " ";
            }
        } else {
            std::cout << "(invalid or empty array)";
        }
        std::cout << std::endl;
    }
    double getFirstElement() const { return firstElement; }
    double getDifference() const { return difference; }
    int getSize() const { return size; }
};

class ArithmeticProgression {
private:
    double firstElement;
    double difference;
    double* array;
    int size;
    bool isValidProgression(double* arr, int sz, double first, double diff) const {
        if (arr == nullptr || sz <= 0) {
            return false;
        }
        for (int i = 0; i < sz; i++) {
            double expected = first + i * diff;
            if (arr[i] != expected) {
                return false;
            }
        }
        return true;
    }
public:
    ArithmeticProgression(double first, double diff, double* arr, int sz)
        : firstElement(first), difference(diff), size(sz) {

        if (sz <= 0) {
            throw ArithmeticProgressionException("Array size must be positive", first, diff, arr, sz);
        }

        if (arr == nullptr) {
            throw ArithmeticProgressionException("Array pointer is null", first, diff, arr, sz);
        }

        if (!isValidProgression(arr, sz, first, diff)) {
            throw ArithmeticProgressionException("Array is not an arithmetic progression", first, diff, arr, sz);
        }

        array = new double[size];
        for (int i = 0; i < size; i++) {
            array[i] = arr[i];
        }
    }
    ~ArithmeticProgression() {
        delete[] array;
    }
    ArithmeticProgression(const ArithmeticProgression& other)
        : firstElement(other.firstElement), difference(other.difference), size(other.size) {
        array = new double[size];
        for (int i = 0; i < size; i++) {
            array[i] = other.array[i];
        }
    }
    ArithmeticProgression& operator=(const ArithmeticProgression& other) {
        if (this != &other) {
            delete[] array;
            firstElement = other.firstElement;
            difference = other.difference;
            size = other.size;
            array = new double[size];
            for (int i = 0; i < size; i++) {
                array[i] = other.array[i];
            }
        }
        return *this;
    }
    void print() const {
        std::cout << "Arithmetic progression:" << std::endl;
        std::cout << "First element: " << firstElement << std::endl;
        std::cout << "Difference: " << difference << std::endl;
        std::cout << "Array (first " << size << " elements): ";
        for (int i = 0; i < size; i++) {
            std::cout << array[i] << " ";
        }
        std::cout << std::endl;
    }
    double getNthElement(int n) const {
        return firstElement + n * difference;
    }
    double getFirstElement() const { return firstElement; }
    double getDifference() const { return difference; }
    int getSize() const { return size; }

    friend std::ostream& operator<<(std::ostream& os, const ArithmeticProgression& ap) {
        os << "AP(first=" << ap.firstElement << ", diff=" << ap.difference << "): ";
        for (int i = 0; i < ap.size; i++) {
            os << ap.array[i] << " ";
        }
        return os;
    }
};
int main() {
    std::cout << "1: WITHOUT EXCEPTION (VALID PROGRESSION)" << std::endl;
    try {
        double arr1[] = {2, 5, 8, 11, 14, 17, 20};
        ArithmeticProgression ap1(2, 3, arr1, 7);
        ap1.print();
        std::cout << "10th element: " << ap1.getNthElement(9) << std::endl;
        std::cout << ap1 << std::endl;

        ArithmeticProgression ap2 = ap1;
        std::cout << "Copied progression: " << ap2 << std::endl;
    } catch (const ArithmeticProgressionException& e) {
        e.printAll();
    }
    std::cout << std::endl;
    std::cout << "2:WITH EXCEPTION (NOT A PROGRESSION)" << std::endl;
    try {
        double arr2[] = {2, 5, 9, 11, 14};
        ArithmeticProgression ap3(2, 3, arr2, 5);
        ap3.print();
    } catch (const ArithmeticProgressionException& e) {
        e.printAll();
    }
    std::cout << std::endl;
    std::cout << "3:WITH EXCEPTION (INVALID SIZE)" << std::endl;
    try {
        double arr3[] = {1, 2, 3, 4};
        ArithmeticProgression ap4(1, 1, arr3, -2);
        ap4.print();
    } catch (const ArithmeticProgressionException& e) {
        e.printAll();
    }
    std::cout << std::endl;
    std::cout << "4:WITH EXCEPTION" << std::endl;
    try {
        ArithmeticProgression ap5(5, 2, nullptr, 3);
        ap5.print();
    } catch (const ArithmeticProgressionException& e) {
        e.printAll();
    }
    std::cout << std::endl;
    std::cout << "5:WITH EXCEPTION" << std::endl;
    try {
        double arr5[] = {10, 13, 16, 19};
        ArithmeticProgression ap6(1, 3, arr5, 4);
        ap6.print();
    } catch (const ArithmeticProgressionException& e) {
        e.printAll();
    }
    std::cout << std::endl;
    std::cout << "6:VALID PROGRESSION WITH NEGATIVE DIFFERENCE" << std::endl;
    try {
        double arr6[] = {20, 17, 14, 11, 8, 5, 2};
        ArithmeticProgression ap7(20, -3, arr6, 7);
        std::cout << ap7 << std::endl;
        std::cout << "15th element: " << ap7.getNthElement(14) << std::endl;
    } catch (const ArithmeticProgressionException& e) {
        e.printAll();
    }
    std::cout << std::endl;
    std::cout << "7:VALID PROGRESSION WITH ZERO DIFFERENCE" << std::endl;
    try {
        double arr7[] = {5, 5, 5, 5, 5};
        ArithmeticProgression ap8(5, 0, arr7, 5);
        std::cout << ap8 << std::endl;
        std::cout << "100th element: " << ap8.getNthElement(99) << std::endl;
    } catch (const ArithmeticProgressionException& e) {
        e.printAll();
    }
    return 0;
}

// tmp_containers.h

// Header file which contains classes of temporary vector containers, purposed to return array value to outside of function
// 상속을 통한 다형성 구현

#include <string> // Allows the string type data
#include <vector> // Allows dynamic arrays, easy to manage, take advantages at preventing memory leakage



// Class of temporary dynamic arrays, purposed for returning array to outside of functions
class Temporary_stores {

    protected :
        virtual void cleanElements() {} // declare virtual function

};

// integer-type temporary vector array
class Ts_int : protected Temporary_stores {

    public :
        // create temporary vector to return integer-type array data to outside of functions
        std::vector <int> intHeap;

        virtual void cleanElements() override { intHeap.clear(); } // override virtual function from parent

    Ts_int() { cleanElements(); }; // constructor function

};

// string-type temporary vector array
class Ts_str : protected Temporary_stores {

    public :
        // create temporary vector to return string-type array data to outside of functions
        std::vector <std::string> strHeap;

        virtual void cleanElements() override { strHeap.clear(); } // override virtual function from parent

    Ts_str() { cleanElements(); }; // constructor function

};

// wide-string-type temporary vector array
class Ts_wstr : protected Temporary_stores {

    public :
        // create temporary vector to return wide-string-type array data to outside of functions
        std::vector <std::wstring> wstrHeap;

        virtual void cleanElements() override { wstrHeap.clear(); } // override virtual function from parent

    Ts_wstr() { cleanElements(); }; // constructor function

};

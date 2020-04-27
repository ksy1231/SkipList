//
//  main.cpp
//  SkipList
//
//  Created by Soo Yun Kim on 4/10/20.
//  Copyright © 2020 Soo Yun Kim. All rights reserved.
//

#include "skiplist.hpp"

#include <sstream>

int totalPassed = 0;
int totalTests = 0;

template<typename T>
string isOK(const T &got, const T &expected) {
    ++totalTests;
    if (got == expected) {
        ++totalPassed;
        return "OK: ";
    } else {
        cout << "    Got   " << got << "\n expected " << expected << endl;
        return "ERR: ";
    }
}

void test03() {
    SkipList s(3);
    s.Add(15);
    s.Add(30);
    s.Add(5);
    s.Add(10);
    s.Add(25);
    s.Add(20);
    cout << "After adding numbers" << endl;
    cout << s << endl;
    s.Remove(15);
    cout << "After removing 15 " << endl;
    cout << s << endl;
    s.Remove(25);
    cout << "After removing 25 " << endl;
    cout << s << endl;
}

void test04() {
    SkipList s(5);
    for (int i = 0; i < 20; ++i) {
        int number = rand() % 100;
        s.Add(number);
        cout << "After adding " << number << endl;
        cout << s << endl;
    }
}

void test02() {
    SkipList skip(3);
    stringstream ss;
    ss << skip;
    cout << isOK(ss.str(),
                 "Level: 2 -- -2147483648, 2147483647, \n"s +
                 "Level: 1 -- -2147483648, 2147483647, \n"s +
                 "Level: 0 -- -2147483648, 2147483647, \n"s)
         << "Empty SkipList of depth=3" << endl;
    srand(100);
    skip.Add(10);
    skip.Add(30);
    skip.Add(5);
    skip.Add(25);
    ss.str("");
    ss << skip;
//    cout << isOK(ss.str(),
//                 "Level: 2 -- -2147483648, 25, 2147483647, \n"s +
//                 "Level: 1 -- -2147483648, 5, 10, 25, 30, 2147483647, \n"s +
//                 "Level: 0 -- -2147483648, 5, 10, 25, 30, 2147483647, \n"s)
//         << "SkipList of depth=3 with 10, 30, 5, 25" << endl;
    cout << isOK(skip.Contains(10), true) << "Contains 10" << endl;
    cout << isOK(skip.Contains(30), true) << "Contains 30" << endl;
    cout << isOK(skip.Contains(71), false) << "Does not contain 71" << endl;
}

void test01() {
    SkipList skip;
    stringstream ss;
    ss << skip;
    cout << isOK(ss.str(), "Level: 0 -- -2147483648, 2147483647, \n"s)
         << "Empty SkipList" << endl;
    skip.Add(10);
    skip.Add(30);
    skip.Add(5);
    skip.Add(25);
    ss.str("");
    ss << skip;
    cout << isOK(ss.str(), "Level: 0 -- -2147483648, 5, 10, 25, 30, 2147483647, \n"s)
         << "Added 10, 30, 5, 25," << endl;
}

int main(int argc, const char * argv[]) {
    test03();
    cout << "----------------------------------------------------------------------------" << endl;
    test04();
    cout << "Because of the random nature of SkipList, the skip list levels may not match\n" << endl;
    cout << "----------------------------------------------------------------------------" << endl;
    test01();
    cout << "----------------------------------------------------------------------------" << endl;
    test02();
    cout << "Passed: " << totalPassed << "/" << totalTests << endl;
    cout << "----------------------------------------------------------------------------" << endl;
    return 0;
}

#include <iostream>

using namespace std;
class testClassNumOne {
	public:
		testClassNumOne();
		void defaultVariableTester (int t);
		

};




void testClassNumOne::defaultVariableTester (int a=5) {
	cout << "a is "<< a << endl;
}

testClassNumOne::testClassNumOne(){
		  defaultVariableTester();
		}

void mainTest () {
	testClassNumOne* x = new testClassNumOne();
	x->defaultVariableTester(); 
	//string y("test");
}
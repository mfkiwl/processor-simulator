#include <stdio.h>


class A {
	public:
	    int val;
    
        A(int val) : val(val) {}
};

void doubleVal(A *a) {
	a->val *= 2;
}

int main(void) {
    A a(3);
    printf("%d\n", a.val);
    doubleVal(&a);
    printf("%d\n", a.val);
    return 0;
}
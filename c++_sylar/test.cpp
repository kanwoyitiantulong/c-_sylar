#include <iostream>


class test {
public:
	test(int v) :var(v) {}
	~test() {}

	void print_var() { std::cout << var << std::endl; }
private:
	int var;
};

int main() {
	test t(1);
	t.print_var();
	return 0;
}
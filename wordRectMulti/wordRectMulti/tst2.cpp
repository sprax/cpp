

#include <iostream>
using namespace std;

struct Lo { };

class Mo : private Lo {
	friend struct Po;
	friend struct F;
};

Lo global_Lo;
Mo global_Mo;

struct No {
	virtual Lo* f() {
		cout << "Lo* No::f()" << endl;
		return &global_Lo;
	}
};

struct Po : No {
	Mo* f() {
		cout << "Mo* Po::f()" << endl;
		return &global_Mo;
	}
};

struct So;

struct Qo : No {

	//   Error:
	//   So is incomplete
	//   So* f();
};

struct G : No {

	//   Error:
	//   Lo is an inaccessible base class of Mo
	//   Mo* f();
};

void test_covariance() {
	Po d;
	No* cp = &d;
	Po* dp = &d;

	Lo* ap = cp->f();
	Mo* bp = dp->f();
	delete ap;
	delete bp;
};

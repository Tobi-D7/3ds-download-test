#include "includes.hpp"

class Structs {
public:
	struct ButtonPos {
		int x;
		int y;
		int w;
		int h;
	};

	struct Key {
		std::string character;
		int x;
		int y;
		int w;
	};
};
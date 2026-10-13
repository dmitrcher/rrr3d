#include "NativeEndpointSortV78.h"
#include <iostream>

int main() {
    unsigned fixtures = 0, checkedWords = 0;
    if (!(std::cin >> fixtures)) return 2;
    for (unsigned f = 0; f < fixtures; ++f) {
        unsigned count; if (!(std::cin >> count)) return 2;
        std::vector<NativeEndpointSortV78::Endpoint> input(count), expected(count);
        for (auto &v : input) if (!(std::cin >> v.coordinate >> v.ownerAndType)) return 2;
        for (auto &v : expected) if (!(std::cin >> v.coordinate >> v.ownerAndType)) return 2;
        NativeEndpointSortV78::sort(input);
        for (unsigned i = 0; i < count; ++i) {
            if (input[i].coordinate != expected[i].coordinate ||
                input[i].ownerAndType != expected[i].ownerAndType) {
                std::cerr << "Native mismatch fixture " << f << " endpoint " << i << '\n';
                return 1;
            }
            checkedWords += 2;
        }
    }
    std::cout << fixtures << " Native sort operations; " << checkedWords << " raw words exact\n";
}

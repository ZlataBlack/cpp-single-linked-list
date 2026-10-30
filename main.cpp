#include "single-linked-list.h"

#include <cassert>
#include <string>

void Test0() {
    using namespace std;

    const SingleLinkedList<int> empty_int_list;
    assert(empty_int_list.GetSize() == 0u);
    assert(empty_int_list.IsEmpty());

    const SingleLinkedList<string> empty_string_list;
    assert(empty_string_list.GetSize() == 0u);
    assert(empty_string_list.IsEmpty());
}

int main() {
    Test0();
}

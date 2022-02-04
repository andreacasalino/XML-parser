#pragma once

#include <XML-Parser/Tag.h>
using namespace xmlPrs;

///       root
///        /|\
///       / | \
///      /  |  \
///     B   A   C
///         |
///         D
///         |
///         E
///
/// tag A has attributes {"A1 = a1", "A2 = a2"}
/// tag B has attributes {"B1 = b1"}
/// tag E has attributes {"E1 = e1"}
Root generate_T_structure();

bool is_valid_T_structure(const Root &root);

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
Root generate_T_structure();

bool is_valid_T_structure(const Root &root);

#ifndef HASH_UTILS_H
#define HASH_UTILS_H

#include <string>

// Computes and returns the SHA-256 hash of the input string.
// Commonly used for securely storing passwords or verifying data integrity.
std::string sha256(const std::string& input);

#endif  // HASH_UTILS_H

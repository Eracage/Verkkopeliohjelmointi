#pragma once

#ifndef REGEXER_H
#define REGEXER_H

#include <regex>

std::string regexArgPort(int argc, char* argv[], int first = 0, std::string def = "27000");
std::string regexArgIP(int argc, char* argv[], int first = 0, std::string def = "85.29.113.77");

#endif

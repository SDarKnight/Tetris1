#include "pch.h"

Rotation Figure::types[][4] = {
	{ R"(
						OO
						OO
	)", R"(
						OO
						OO
	)", R"(
						OO
						OO
	)", R"(
						OO
						OO
	)" },					// 0
	{ R"(
						O
						OOO
	)", {R"(
						OO
						O
						O
	)", 1, 0}, {R"(
						OOO
						  O
	)",-1, 1}, {R"(
						 O
						 O
						OO
	)",0,-1} },				// 1
	{ R"(
						  O
						OOO
	)", {R"(
						O
						O
						OO
	)", 1, 0}, {R"(
						OOO
						O
	)",-1, 1}, {R"(
						OO
						 O
						 O
	)",0,-1} },				// 2
	{ {R"(
						OOOO
	)",-1,1}, {R"(
						O
						O
						O
						O
	)", 2, -1}, {R"(
						OOOO
	)",-2, 2}, {R"(
						O
						O
						O
						O
	)",1,-2} },				// 3
	{ types[3][0], types[3][1], types[3][2], types[3][3] }, // 4
	{ R"(
						 O
						OOO
	)", {R"(
						O
						OO
						O
	)", 1, 0}, {R"(
						OOO
						 O
	)",-1, 1}, {R"(
						 O
						OO
						 O
	)",0,-1} },				// 5
	{ R"(
						 OO
						OO
	)", {R"(
						O
						OO
						 O
	)", 1, 0}, {R"(
						 OO
						OO 
	)",-1, 1}, {R"(
						O
						OO
						 O
	)",0,-1} },				// 6
	{ R"(
						OO
						 OO
	)", {R"(
						 O
						OO
						O
	)", 1, 0}, {R"(
						OO
						 OO
	)",-1, 1}, {R"(
						 O
						OO
						O
	)",0,-1} }				// 7
};

const int Figure::figures = _countof(types);
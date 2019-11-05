#!/usr/bin/env bash

# Based on a script by nicbauma.

RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
NC=$(tput sgr0)

minlen=7
maxlen=$(($(tput cols)/2-5))
cutlen="-$((maxlen-3))"

if [ -f ".algolab" ]; then
	pname=$(cat ".algolab")
fi
pname=${pname:-algo}
pname=${2:-${pname}}

CMAKE_FILE=$(cat <<'EOF'
cmake_minimum_required(VERSION 3.1)
project(pname)

set(CMAKE_VERBOSE_MAKEFILE on)

find_package(CGAL REQUIRED COMPONENTS Core)
find_package(Boost REQUIRED)
include_directories(${CGAL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
include( ${CGAL_USE_FILE} )

FILE(GLOB_RECURSE sources src/*.cpp)
add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME} ${CGAL_LIBRARIES} ${Boost_LIBRARIES} ${MPFR_LIBRARIES} ${GMP_LIBRARIES} ${THREAD_LIBRARIES})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

target_compile_options(${PROJECT_NAME} PRIVATE -O2 -Wall -Wextra -pedantic -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond)
EOF
)
CMAKE_FILE="${CMAKE_FILE/pname/${pname}}"

CXX_TEMPLATE=$(cat <<'EOF'
#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
	ios_base::sync_with_stdio(false);

	cout << "Hello World!\n";

	return EXIT_SUCCESS;
}
EOF
)

function compile_debug() {
	mkdir -p build/debug
	cd build/debug
	cmake -DCMAKE_BUILD_TYPE=debug ../..
	make
	cd ../../
}

function compile_release() {
	mkdir -p build/release
	cd build/release
	cmake -DCMAKE_BUILD_TYPE=release ../..
	make
	cd ../../
}

case "$1" in
	init)
		echo "Creating CMake project..."
		echo "$pname" > .algolab

		mkdir -p src
		if [ ! -f src/algorithm.cpp ]; then
			echo "$CXX_TEMPLATE" > src/algorithm.cpp
		fi

		echo "$CMAKE_FILE" > CMakeLists.txt
		;;
	debug)
		compile_debug
		gdb --exec="build/debug/${pname}"
		;;
	memcheck)
		compile_debug
		valgrind "build/debug/${pname}"
		;;
	compile)
		compile_release
		;;
	run)
		compile_release
		echo "Running ${pname}:"
		echo ""
		"./build/release/${pname}"
		;;
	test)
		compile_release
		echo "Running all tests for ${pname}:"
		echo ""
		find . -name '*.in' | sort | while read line; do
			tname=${line::-3}
			printf "Test '${tname:2}': "
			result=$("./build/release/${pname}" < "$tname.in")
			ok=$?
			if [ "$ok" != 0 ]
			then
				echo "Runtime error!"
			else
				expected=$(cat "$tname.out")

				if [ "$result" == "$expected" ]
				then
					printf "${GREEN}PASSED${NC}\n"
				else
					# Truncate lines if longer than half the terminal.
					len=$(echo "$expected" | awk '(NR==1||length>longest){longest=length} END {print longest}')
					if [ "$len" -gt "$maxlen" ]; then
						len="$maxlen"
					fi
					if [ "$len" -lt "$minlen" ]; then
						len=$minlen
					fi
					len=$((len+3))
					dashes=$(printf "%-$((len*2))s" "-")

					printf "${RED}FAILED${NC}\n"

					echo "${dashes// /-}"
					printf "%-${len}s %-${len}s\n" "Expected" "Was"
					echo "${dashes// /-}"
					while read -u3 e; read -u4 w;
					do
						equal=0; [ "$e" == "$w" ] && equal=1

						if [ "${#e}" -gt "$maxlen" ]; then
							e="$(echo "$e" | cut -c $cutlen)..."
						fi
						if [ "${#w}" -gt "$maxlen" ]; then
							w="$(echo "$w" | cut -c $cutlen)..."
						fi

						if [ "$equal" == 0 ]; then
							printf "${RED}%-${len}s %-${len}s${NC}\n" "$e" "$w"
						else
							printf "%-${len}s %-${len}s\n" "$e" "$w"
						fi

					done 3< <(echo "$expected") 4< <(echo "$result")
					echo "${dashes// /-}"
					echo ""
				fi
			fi
		done
		;;
	*)
		echo "Usage: $0 (init|debug|memcheck|compile|run|test)"
esac

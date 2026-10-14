# Compiler — override on the command line, e.g. `make CXX=g++-13`.
CXX ?= g++

# LEMON include paths. The build directory must contain a generated
# lemon/config.h (run `cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 .` inside
# ../lemon-lib/build the first time). On systems where LEMON is installed
# system-wide, the second -I can be dropped.
LEMON_INC ?= -I../lemon-lib -I../lemon-lib/build

# Compilation flags
#  -O3              : standard high optimisation
#  -DNDEBUG         : remove asserts in hot paths
#  -march=native    : let the compiler use SIMD/cache-line widths of the host
#  -funroll-loops   : helpful for the small inner null-model loops
#  -flto            : whole-program inlining of header-only templates
#  -ffp-contract=off: do not merge a*b+c into one FMA instruction. Clang does
#                     this by default and GCC does not. Without this flag, the
#                     same seed can give different results with each compiler.
#  -std=c++17       : modern standard library; required by some new code
# Note: -ffast-math is intentionally NOT enabled — modularity is a sum of
# products and aggressive re-association can perturb the partition.
CXXFLAGS ?= -O3 -DNDEBUG -march=native -funroll-loops -flto -ffp-contract=off -std=c++17 -Wall

.PHONY: all run_gen_louvain test clean

all: run_gen_louvain

run_gen_louvain:
	$(CXX) $(CXXFLAGS) $(LEMON_INC) run_gen_louvain.cpp -o run_gen_louvain.sh

# Build and run the tests. The test data is in this directory.
test:
	$(CXX) $(CXXFLAGS) $(LEMON_INC) test/test_louvain.cpp -o test/test_louvain
	./test/test_louvain .

clean:
	rm -f run_gen_louvain.sh test/test_louvain

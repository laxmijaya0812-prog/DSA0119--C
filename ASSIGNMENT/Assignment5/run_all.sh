#!/usr/bin/env bash
# run_all.sh -- compile and run every program of Assignment 5 and save the real output
# next to the source (TaskXX/output_*.txt).  Usage:  ./run_all.sh
# Compiler: g++ (C++17).  Flags: -std=c++17 -Wall -Wextra  (+ -O2 for timings, + -fsanitize=address for ASAN runs)
set -u
CXX=${CXX:-g++}
STD="-std=c++17 -Wall -Wextra"
root=$(cd "$(dirname "$0")" && pwd)
bindir=$(mktemp -d)
bin="$bindir/prog"
RUNENV=""

# build_run <dir> <output file> "<extra flags>" <source> [program arguments]
build_run() {
    local dir=$1 out=$2 flags=$3 src=$4
    shift 4
    (
        cd "$root/$dir" || exit 1
        {
            echo "\$ $CXX $STD $flags $src -o prog"
            if $CXX $STD $flags "$src" -o "$bin" 2>&1; then
                echo "\$ ${RUNENV:+$RUNENV }./prog $*"
                env $RUNENV "$bin" "$@" 2>&1
                echo "[exit code $?]"
            else
                echo "[compilation failed - expected for the intentionally failing variants]"
            fi
        } > "$out"
    )
    echo "  $dir/$out"
}

echo "Compiler: $($CXX --version | head -1)"
ASAN="-g -fsanitize=address"
O2="-O2"

echo "Task A1"
build_run TaskA1 output_a1_space_demo.txt "" a1_space_demo.cpp

echo "Task A2"
build_run TaskA2 output_predict_a2.txt "" predict_a2.cpp
build_run TaskA2 output_predict_a2_override_error.txt "" predict_a2_override_error.cpp
build_run TaskA2 output_predict_a2_nonvirtual_dtor_asan.txt "$ASAN" predict_a2_nonvirtual_dtor.cpp
build_run TaskA2 output_a2_alt_dynamic_cast.txt "$O2" a2_alt_dynamic_cast.cpp

echo "Task A3"
build_run TaskA3 output_a3_payers.txt "" a3_payers.cpp
build_run TaskA3 output_a3_alt_stdfunction.txt "$O2" a3_alt_stdfunction.cpp

echo "Task A4"
build_run TaskA4 output_a4_exceptions.txt "" a4_exceptions.cpp
build_run TaskA4 output_a4_dtor_throw_demo.txt "" a4_dtor_throw_demo.cpp
build_run TaskA4 output_a4_alt_returncode.txt "" a4_alt_returncode.cpp

echo "Task A5"
build_run TaskA5 output_predict_a5.txt "" predict_a5.cpp
build_run TaskA5 output_a5_manual_delete_asan.txt "$ASAN -DMANUAL_DELETE" a5_alt_manual_delete.cpp
build_run TaskA5 output_a5_raii_asan.txt "$ASAN" a5_alt_manual_delete.cpp

echo "Task B1"
build_run TaskB1 output_test_b1.txt "" test_b1.cpp
build_run TaskB1 output_test_b1_asan.txt "$ASAN" test_b1.cpp
build_run TaskB1 output_complete_b1.txt "" complete_b1.cpp
build_run TaskB1 output_complete_b1_asan.txt "$ASAN" complete_b1.cpp
build_run TaskB1 output_complete_b1_nonvirtual_total.txt "" complete_b1_nonvirtual_total.cpp
build_run TaskB1 output_b1_runtime_binding_virtual.txt "-DVIRT=virtual" b1_runtime_binding.cpp
build_run TaskB1 output_b1_runtime_binding_nonvirtual.txt "-DVIRT=" b1_runtime_binding.cpp
build_run TaskB1 output_b1_virtual_dtor_ok_asan.txt "$ASAN -DVDTOR=virtual" b1_virtual_dtor.cpp
build_run TaskB1 output_b1_nonvirtual_dtor_asan.txt "$ASAN -DVDTOR=" b1_virtual_dtor.cpp
RUNENV="ASAN_OPTIONS=new_delete_type_mismatch=0" build_run TaskB1 output_b1_nonvirtual_dtor_leak_asan.txt "$ASAN -DVDTOR=" b1_virtual_dtor.cpp
RUNENV=""
build_run TaskB1 output_b1_alt_templates.txt "$O2" b1_alt_templates.cpp

echo "Task B2"
build_run TaskB2 output_predict_b2.txt "" predict_b2.cpp
build_run TaskB2 output_complete_b2.txt "" complete_b2.cpp
build_run TaskB2 output_complete_b2_throw_e.txt "" complete_b2_throw_e.cpp
build_run TaskB2 output_b2_rethrow_slicing.txt "" b2_rethrow_slicing.cpp
build_run TaskB2 output_b2_error_tests.txt "" b2_error_tests.cpp
build_run TaskB2 output_b2_error_tests_asan.txt "$ASAN" b2_error_tests.cpp --asan-safe
build_run TaskB2 output_b2_alt_optional.txt "$O2" b2_alt_optional.cpp

echo "Task B3"
build_run TaskB3 output_b3_correctness.txt "" b3_correctness.cpp
build_run TaskB3 output_b3_benchmark.txt "$O2" b3_benchmark.cpp
build_run TaskB3 output_b3_exception_cost.txt "$O2" b3_exception_cost.cpp

echo "Task B4"
( cd "$root/TaskB4" && { cmp ../TaskB1/billing.hpp billing.hpp && echo "billing.hpp is identical to TaskB1/billing.hpp"; sha256sum billing.hpp ../TaskB1/billing.hpp; } > output_b4_billing_unchanged.txt 2>&1 )
echo "  TaskB4/output_b4_billing_unchanged.txt"
build_run TaskB4 output_regression_b1_asan.txt "$ASAN" regression_b1.cpp
build_run TaskB4 output_test_b4_asan.txt "$ASAN" test_b4.cpp
build_run TaskB4 output_b4_alt_plugin.txt "$O2" b4_alt_plugin.cpp

rm -rf "$bindir"
echo "Done. Take your own terminal screenshots into each TaskXX/screenshots/ folder."

#!/bin/bash

set -e

IMPLEMENTATIONS=(epee nlohmann rapidjson json_dto)

SOURCE_DIR="$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)"
SAMPLE_FILE="$SOURCE_DIR/sample.json"

work_dir=$(mktemp -d)

cleanup() {
    if [[ -d $work_dir ]]; then
        rm -rf "$work_dir";
    fi
}

trap cleanup EXIT

cd "$work_dir"

declare -A SIZES COMPILE_TIME RUN_TIME NUM_ALLOCATIONS BYTES_ALLOCATED

function populate_sizes() {
    local result impl size

    # Use vm size for determining relevant size, so debug symbols don't count
    result=$(bloaty -d compileunits --csv speedtest)

    for impl in "${IMPLEMENTATIONS[@]}"; do
        size="$(grep "$impl" <<< "$result" | cut -f2 -d,)"
        SIZES[$impl]=$size
    done
}

function populate_compile_time() {
    local impl compile_time

    for impl in "${IMPLEMENTATIONS[@]}"; do
        # parsing time output is… inconvenient
        # we use command time rather than builtin for more consistent output
        compile_time="$(command time -f '%e' make -B "src/$impl.o" |& tail -n 1)"
        COMPILE_TIME[$impl]=$compile_time
    done
}

function populate_run_time() {
    local impl run_time

    for impl in "${IMPLEMENTATIONS[@]}"; do
        run_time=$(./speedtest "$impl" "$SAMPLE_FILE" |& tail -n 1 | cut -f2 -d: )
        RUN_TIME[$impl]=${run_time%ns}
    done
}

function populate_meminfo() {
    local impl vgout

    for impl in "${IMPLEMENTATIONS[@]}"; do
        read -ra vgout < <(valgrind ./speedtest "$impl" "$SAMPLE_FILE" |& \
            grep -F "total heap usage" | \
            tr -d ',')
        NUM_ALLOCATIONS[$impl]=${vgout[4]}
        BYTES_ALLOCATED[$impl]=${vgout[8]}
    done
}


# Need debug symbols to determine individual sizes
cmake "$SOURCE_DIR" -DCMAKE_BUILD_TYPE=RelWithDebInfo
make "-j$(nproc)"
populate_sizes

# Use optimized build for the rest of the test
cmake "$SOURCE_DIR" -DCMAKE_BUILD_TYPE=Release
make easylogging # Ensure easylogging doesn't give us overhead
populate_compile_time

# Buils the remaining bits
make "-j$(nproc)"
populate_run_time
populate_meminfo

echo "implementation,compile_size,compile_time,num_allocations,bytes_allocated"

for impl in "${IMPLEMENTATIONS[@]}"; do
    printf "%s,%d,%f,%f,%d,%d\n" "$impl" "${SIZES[$impl]}" "${COMPILE_TIME[$impl]}" \
        "${RUN_TIME[$impl]}" "${NUM_ALLOCATIONS[$impl]}" "${BYTES_ALLOCATED[$impl]}"

done


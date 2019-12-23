# JSON speed test

In this repository we attempt to compare the (de)serialization performance of
various JSON libraries. 

## Included code and licenses

This project includes code from `epee` by Andrey N. Sabelnikov, and code from
Easylogging++. All code is copyright their respective authors.

## Test results

We measure the following metrics for each implementation:

- total compiled size, using [Bloaty](https://github.com/google/bloaty),
  (`bloaty -d compileunits`)
- total compile time, using `command time` to measure the compile time for the
  specific unit,
- run time per iteration of deserializing/serializing averaged over 100000
  cycles,
- total number of allocations over the above iterations,
- total number of bytes allocated over the above iterations.

The following table shows the result of our testing. Lower values are bettter.

| implementation | compile size | compile time | run time per iteration (ns) | num allocations | bytes allocated |
| --- | ---: | ---: | ---: | ---: | ---: |
| `epee` | 376791 | 10.98 | 4051.310 | 4000367 | 524407227 |
| `nlohmann` | 118827 | 3.11 | 4152.760 | 8800367 | 421407214 |
| `rapidjson` | 10881 | 1.32 | 1060.800 | 1800369 | 13339207422 |
| `json_dto` | 59809 | 1.44 | 959.696 | 1700369 |13340607422 |

Some caveats apply: memory usage always includes 365 allocations for the
initialization of `easylogging`, which results in approximately 15kb more
allocated. The compile size and time for `epee` does not include the size
of its dependency `easylogging`.

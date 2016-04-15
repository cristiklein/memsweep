Memory Sweaper
==============
The goal of this program is to measure the performance of the swap subsystem.

It works as follows. First, it allocates a contiguous region of memory. Then, it reads the first byte from every memory page, usually 4KiB. At the end, it reports the average latency for accessing one page. The program can do this continuously or for a given number of iterations.

Example Output
--------------

* Sweeping an area that fits in RAM, i.e., no swapping: `memsweep -s 5g -i 5`

        Sweeping 5000000000 bytes, in 4096 bytes increment, for 5 iterations
        [92223.147257] Iteration 0, average latency per page access 768ns
        [92223.159045] Iteration 1, average latency per page access 9ns
        [92223.173036] Iteration 2, average latency per page access 11ns
        [92223.190125] Iteration 3, average latency per page access 13ns
        [92223.208255] Iteration 4, average latency per page access 14ns

* Sweeping an area that does not fit in RAM: `memsweep -s 10g -i 5`

        Sweeping 10000000000 bytes, in 4096 bytes increment, for 5 iterations
        [92226.984627] Iteration 0, average latency per page access 1491ns
        [92236.828277] Iteration 1, average latency per page access 4031ns
        [92246.126018] Iteration 2, average latency per page access 3808ns
        [92254.806316] Iteration 3, average latency per page access 3555ns
        [92263.127276] Iteration 4, average latency per page access 3408ns


# Valgrind User Manual: Massif - A Heap Profiler

## Table of Contents
- [Valgrind User Manual: Massif - A Heap Profiler](#valgrind-user-manual-massif---a-heap-profiler)
  - [Table of Contents](#table-of-contents)
  - [9.1. Overview](#91-overview)
  - [9.2. Using Massif and ms\_print](#92-using-massif-and-ms_print)
    - [9.2.1. An Example Program](#921-an-example-program)
    - [9.2.2. Running Massif](#922-running-massif)
    - [9.2.3. Running ms\_print](#923-running-ms_print)
    - [9.2.4. The Output Preamble](#924-the-output-preamble)
    - [9.2.5. The Output Graph](#925-the-output-graph)
    - [9.2.6. The Snapshot Details](#926-the-snapshot-details)
    - [9.2.7. Forking Programs](#927-forking-programs)
    - [9.2.8. Measuring All Memory in a Process](#928-measuring-all-memory-in-a-process)
    - [9.2.9. Acting on Massif's Information](#929-acting-on-massifs-information)
  - [9.3. Using massif-visualizer](#93-using-massif-visualizer)
  - [9.4. Massif Command-line Options](#94-massif-command-line-options)
  - [9.5. Massif Monitor Commands](#95-massif-monitor-commands)
  - [9.6. Massif Client Requests](#96-massif-client-requests)
  - [9.7. ms\_print Command-line Options](#97-ms_print-command-line-options)
  - [9.8. Massif's Output File Format](#98-massifs-output-file-format)

---

## 9.1. Overview

Massif is a heap profiler that measures how much heap memory your program uses, including both useful space and extra bytes allocated for book-keeping and alignment purposes. It can also measure stack memory usage, though this is disabled by default.

Heap profiling helps reduce memory usage, which provides several benefits:
- Speeds up programs by improving cache interaction and avoiding paging.
- Reduces the chance of exhausting swap space.
- Identifies space leaks that traditional leak-checkers (like Memcheck) might miss—these occur when memory isn't lost but remains unused.

Massif not only reports total heap memory usage but also provides detailed information about which parts of your program are responsible for allocations.

Additionally, Massif supports **Execution Trees** memory profiling using the `--xtree-memory` command-line option and the `xtmemory` monitor command.

---

## 9.2. Using Massif and ms_print

To use Massif effectively:
1. Compile your program with debugging info (`-g` option).
2. Run Massif to gather profiling data.
3. Use `ms_print` to present the data in a readable format.

### 9.2.1. An Example Program

Consider this C program that allocates various blocks on the heap:

```c
#include <stdlib.h>

void g(void) {
    malloc(4000);
}

void f(void) {
    malloc(2000);
    g();
}

int main(void) {
    int i;
    int* a[10];

    for (i = 0; i < 10; i++) {
        a[i] = malloc(1000);
    }

    f();
    g();

    for (i = 0; i < 10; i++) {
        free(a[i]);
    }

    return 0;
}
```

### 9.2.2. Running Massif

Run Massif on your program like this:

```bash
valgrind --tool=massif prog
```

This generates a file named `massif.out.<pid>` (where `<pid>` is the process ID). You can change the filename using the `--massif-out-file` option.

### 9.2.3. Running ms_print

To view the results:

```bash
ms_print massif.out.12345
```

This produces:
- A graph showing memory consumption over time.
- Detailed information about allocation sites, including the peak memory usage point.

Using a separate script (`ms_print`) separates data gathering from presentation, allowing future enhancements.

### 9.2.4. The Output Preamble

The output starts with a preamble showing how the program, Massif, and `ms_print` were invoked:

```
Command: example
Massif arguments: (none)
ms_print arguments: massif.out.12797
```

### 9.2.5. The Output Graph

The graph shows memory consumption over the program's execution. By default, Massif uses "instructions executed" as the time unit. For short programs, consider using `--time-unit=B` to measure time in bytes allocated/deallocated.

Example graph with `--time-unit=B`:

```
KB
19.63   ^                                                                        #
        |                                                                        #
        |                                                                        #
        |                                                                        #
        |                                                                        #
        |                                                                        #
        |                                                                        #
        |                                                                        #
        |                                                                        #
        |                                                                        #
        |                                                                        #
        |                                                                        #
        |                                                                        #
        |                                                                        #
        |                                                                        #
        |                                                                        #
        |                                                                        #
        |                                                                       :#
        |                                                                       :#
        |                                                                       :#
        0 +----------------------------------------------------------------------->ki  0  113.4
Number of snapshots: 25
Detailed snapshots: [9, 14(peak), 24]
```

Why is most of the graph empty, with only a couple of bars at the very end? By default, Massif uses "instructions executed" as the unit of time. For very short-run programs such as the example, most of the executed instructions involve the loading and dynamic linking of the program. The execution of `main` (and thus the heap allocations) only occur at the very end. For a short-running program like this, we can use the `--time-unit=B` option to specify that we want the time unit to instead be the number of bytes allocated/deallocated on the heap and stack(s).

If we re-run the program under Massif with this option, and then re-run ms_print, we get this more useful graph:

```
19.63^                                               ###                           
     |                                               #                            
     |                                               #  ::                        
     |                                               #  : :::                     
     |                                      :::::::::#  : :  ::                   
     |                                      :        #  : :  : ::                 
     |                                      :        #  : :  : : :::              
     |                                      :        #  : :  : : :  ::            
     |                            :::::::::::        #  : :  : : :  : :::         
     |                            :         :        #  : :  : : :  : :  ::       
     |                        :::::         :        #  : :  : : :  : :  : ::     
     |                     @@@:   :         :        #  : :  : : :  : :  : : @    
     |                   ::@  :   :         :        #  : :  : : :  : :  : : @    
     |                :::: @  :   :         :        #  : :  : : :  : :  : : @    
     |              :::  : @  :   :         :        #  : :  : : :  : :  : : @    
     |            ::: :  : @  :   :         :        #  : :  : : :  : :  : : @    
     |         :::: : :  : @  :   :         :        #  : :  : : :  : :  : : @    
     |       :::  : : :  : @  :   :         :        #  : :  : : :  : :  : : @    
     |    :::: :  : : :  : @  :   :         :        #  : :  : : :  : :  : : @    
     |  :::  : :  : : :  : @  :   :         :        #  : :  : : :  : :  : : @  
  0 +----------------------------------------------------------------------->KB
     0                                                                   29.48
Number of snapshots: 25 Detailed snapshots: [9, 14 (peak), 24]
```

The size of the graph can be changed with ms_print's `--x` and `--y` options. Each vertical bar represents a snapshot, i.e. a measurement of the memory usage at a certain point in time. If the next snapshot is more than one column away, a horizontal line of characters is drawn from the top of the snapshot to just before the next snapshot column. The text at the bottom show that 25 snapshots were taken for this program, which is one per heap allocation/deallocation, plus a couple of extras. Massif starts by taking snapshots for every heap allocation/deallocation, but as a program runs for longer, it takes snapshots less frequently. It also discards older snapshots as the program goes on; when it reaches the maximum number of snapshots (100 by default, although changeable with the `--max-snapshots` option) half of them are deleted. This means that a reasonable number of snapshots are always maintained.

Most snapshots are *normal*, and only basic information is recorded for them. Normal snapshots are represented in the graph by bars consisting of ':' characters.

Some snapshots are *detailed*. Information about where allocations happened are recorded for these snapshots, as we will see shortly. Detailed snapshots are represented in the graph by bars consisting of '@' characters. The text at the bottom show that 3 detailed snapshots were taken for this program (snapshots 9, 14 and 24). By default, every 10th snapshot is detailed, although this can be changed via the `--detailed-freq` option.

Finally, there is at most one *peak* snapshot. The peak snapshot is a detailed snapshot, and records the point where memory consumption was greatest. The peak snapshot is represented in the graph by a bar consisting of '#' characters. The text at the bottom shows that snapshot 14 was the peak.

Massif's determination of when the peak occurred can be wrong, for two reasons.

*   Peak snapshots are only ever taken after a deallocation happens. This avoids lots of unnecessary peak snapshot recordings (imagine what happens if your program allocates a lot of heap blocks in succession, hitting a new peak every time). But it means that if your program never deallocates any blocks, no peak will be recorded. It also means that if your program does deallocate blocks but later allocates to a higher peak without subsequently deallocating, the reported peak will be too low.
*   Even with this behaviour, recording the peak accurately is slow. So by default Massif records a peak whose size is within 1% of the size of the true peak. This inaccuracy in the peak measurement can be changed with the `--peak-inaccuracy` option.

The following graph is from an execution of Konqueror, the KDE web browser. It shows what graphs for larger programs look like.

```
    MB
3.952^                                                                    #      
     |                                                                   @#:     
     |                                                                 :@@#:     
     |                                                            @@::::@@#:      
     |                                                            @ :: :@@#::     
     |                                                          @@@ :: :@@#::     
     |                                                       @@:@@@ :: :@@#::     
     |                                                    :::@ :@@@ :: :@@#::     
     |                                                    : :@ :@@@ :: :@@#::     
     |                                                  :@: :@ :@@@ :: :@@#::      
     |                                                @@:@: :@ :@@@ :: :@@#:::     
     |                           :       ::         ::@@:@: :@ :@@@ :: :@@#:::     
     |                        :@@:    ::::: ::::@@@:::@@:@: :@ :@@@ :: :@@#:::     
     |                     ::::@@:  ::: ::::::: @  :::@@:@: :@ :@@@ :: :@@#:::     
     |                    @: ::@@:  ::: ::::::: @  :::@@:@: :@ :@@@ :: :@@#:::     
     |                    @: ::@@:  ::: ::::::: @  :::@@:@: :@ :@@@ :: :@@#:::     
     |                    @: ::@@:::::: ::::::: @  :::@@:@: :@ :@@@ :: :@@#:::     
     |                ::@@@: ::@@:: ::: ::::::: @  :::@@:@: :@ :@@@ :: :@@#:::     
     |             :::::@ @: ::@@:: ::: ::::::: @  :::@@:@: :@ :@@@ :: :@@#:::     
     |           @@:::::@ @: ::@@:: ::: ::::::: @  :::@@:@: :@ :@@@ :: :@@#:::   
  0 +----------------------------------------------------------------------->Mi
     0                                                                   626.4
Number of snapshots: 63 Detailed snapshots: [3, 4, 10, 11, 15, 16, 29, 33, 34, 36, 39, 41, 42, 43, 44, 49, 50, 51, 53, 55, 56, 57 (peak)]
```

Note that the larger size units are KB, MB, GB, etc. As is typical for memory measurements, these are based on a multiplier of 1024, rather than the standard SI multiplier of 1000. Strictly speaking, they should be written KiB, MiB, GiB, etc.

### 9.2.6. The Snapshot Details
Returning to our example, the graph is followed by the detailed information for each snapshot. The first nine snapshots are normal, so only a small amount of information is recorded for each one:

```
--------------------------------------------------------------------------------
  n        time(B)         total(B)   useful-heap(B) extra-heap(B)    stacks(B)
--------------------------------------------------------------------------------
  0              0                0                0             0            0
  1          1,008            1,008            1,000             8            0
  2          2,016            2,016            2,000            16            0
  3          3,024            3,024            3,000            24            0
  4          4,032            4,032            4,000            32            0
  5          5,040            5,040            5,000            40            0
  6          6,048            6,048            6,000            48            0
  7          7,056            7,056            7,000            56            0
  8          8,064            8,064            8,000            64            0
```

Each normal snapshot records several things.

*   **Its number.**
*   **The time it was taken.** In this case, the time unit is bytes, due to the use of `--time-unit=B`.
*   **The total memory consumption at that point.**
*   **The number of useful heap bytes allocated at that point.** This reflects the number of bytes asked for by the program.
*   **The number of extra heap bytes allocated at that point.** This reflects the number of bytes allocated in excess of what the program asked for. There are two sources of extra heap bytes.
    *   First, every heap block has administrative bytes associated with it. The exact number of administrative bytes depends on the details of the allocator. By default Massif assumes 8 bytes per block, as can be seen from the example, but this number can be changed via the `--heap-admin` option.
    *   Second, allocators often round up the number of bytes asked for to a larger number, usually 8 or 16. This is required to ensure that elements within the block are suitably aligned. If N bytes are asked for, Massif rounds N up to the nearest multiple of the value specified by the `--alignment` option.
*   **The size of the stack(s).** By default, stack profiling is off as it slows Massif down greatly. Therefore, the stack column is zero in the example. Stack profiling can be turned on with the `--stacks=yes` option.

The next snapshot is detailed. As well as the basic counts, it gives an allocation tree which indicates exactly which pieces of code were responsible for allocating heap memory:

```
  9          9,072            9,072            9,000            72            0
99.21% (9,000B) (heap allocation functions) malloc/new/new[], --alloc-fns, etc.
->99.21% (9,000B) 0x804841A: main (example.c:20)
```

The allocation tree can be read from the top down. The first line indicates all heap allocation functions such as `malloc` and C++ `new`. All heap allocations go through these functions, and so all 9,000 useful bytes (which is 99.21% of all allocated bytes) go through them. But how were `malloc` and `new` called? At this point, every allocation so far has been due to line 20 inside `main`, hence the second line in the tree. The `->` indicates that main (line 20) called `malloc`.

Let's see what the subsequent output shows happened next:

```
--------------------------------------------------------------------------------
  n        time(B)         total(B)   useful-heap(B) extra-heap(B)    stacks(B)
--------------------------------------------------------------------------------
 10         10,080           10,080           10,000            80            0
 11         12,088           12,088           12,000            88            0
 12         16,096           16,096           16,000            96            0
 13         20,104           20,104           20,000           104            0
 14         20,104           20,104           20,000           104            0
99.48% (20,000B) (heap allocation functions) malloc/new/new[], --alloc-fns, etc.
->49.74% (10,000B) 0x804841A: main (example.c:20)
|
->39.79% (8,000B) 0x80483C2: g (example.c:5)
| ->19.90% (4,000B) 0x80483E2: f (example.c:11)
| | ->19.90% (4,000B) 0x8048431: main (example.c:23)
| |
| ->19.90% (4,000B) 0x8048436: main (example.c:25)
|
->09.95% (2,000B) 0x80483DA: f (example.c:10)
->09.95% (2,000B) 0x8048431: main (example.c:23)
```

The first four snapshots are similar to the previous ones. But then the global allocation peak is reached, and a detailed snapshot (number 14) is taken. Its allocation tree shows that 20,000B of useful heap memory has been allocated, and the lines and arrows indicate that this is from three different code locations: line 20, which is responsible for 10,000B (49.74%); line 5, which is responsible for 8,000B (39.79%); and line 10, which is responsible for 2,000B (9.95%).

We can then drill down further in the allocation tree. For example, of the 8,000B asked for by line 5, half of it was due to a call from line 11, and half was due to a call from line 25.

In short, Massif collates the stack trace of every single allocation point in the program into a single tree, which gives a complete picture at a particular point in time of how and why all heap memory was allocated.

Note that the tree entries correspond not to functions, but to individual code locations. For example, if function `A` calls `malloc`, and function `B` calls `A` twice, once on line 10 and once on line 11, then the two calls will result in two distinct stack traces in the tree. In contrast, if `B` calls `A` repeatedly from line 15 (e.g. due to a loop), then each of those calls will be represented by the same stack trace in the tree.

Note also that each tree entry with children in the example satisfies an invariant: the entry's size is equal to the sum of its children's sizes. For example, the first entry has size 20,000B, and its children have sizes 10,000B, 8,000B, and 2,000B. In general, this invariant almost always holds. However, in rare circumstances stack traces can be malformed, in which case a stack trace can be a sub-trace of another stack trace. This means that some entries in the tree may not satisfy the invariant -- the entry's size will be greater than the sum of its children's sizes. This is not a big problem, but could make the results confusing. Massif can sometimes detect when this happens; if it does, it issues a warning:

```
Warning: Malformed stack trace detected.  In Massif's output, 
         the size of an entry's child entries may not sum up 
         to the entry's size as they normally do.
```

However, Massif does not detect and warn about every such occurrence. Fortunately, malformed stack traces are rare in practice.

Returning now to ms_print's output, the final part is similar:

```
--------------------------------------------------------------------------------
  n        time(B)         total(B)   useful-heap(B) extra-heap(B)    stacks(B)
--------------------------------------------------------------------------------
 15         21,112           19,096           19,000            96            0
 16         22,120           18,088           18,000            88            0
 17         23,128           17,080           17,000            80            0
 18         24,136           16,072           16,000            72            0
 19         25,144           15,064           15,000            64            0
 20         26,152           14,056           14,000            56            0
 21         27,160           13,048           13,000            48            0
 22         28,168           12,040           12,000            40            0
 23         29,176           11,032           11,000            32            0
 24         30,184           10,024           10,000            24            0
99.76% (10,000B) (heap allocation functions) malloc/new/new[], --alloc-fns, etc.
->79.81% (8,000B) 0x80483C2: g (example.c:5)
| ->39.90% (4,000B) 0x80483E2: f (example.c:11)
| | ->39.90% (4,000B) 0x8048431: main (example.c:23)
| |
| ->39.90% (4,000B) 0x8048436: main (example.c:25)
|
->19.95% (2,000B) 0x80483DA: f (example.c:10)
| ->19.95% (2,000B) 0x8048431: main (example.c:23)
|
->00.00% (0B) in 1+ places, all below ms_print's threshold (01.00%)
```

The final detailed snapshot shows how the heap looked at termination. The 00.00% entry represents the code locations for which memory was allocated and then freed (line 20 in this case, the memory for which was freed on line 28). However, no code location details are given for this entry; by default, Massif only records the details for code locations responsible for more than 1% of useful memory bytes, and ms_print likewise only prints the details for code locations responsible for more than 1%. The entries that do not meet this threshold are aggregated. This avoids filling up the output with large numbers of unimportant entries. The thresholds can be changed with the `--threshold` option that both Massif and ms_print support.

### 9.2.7. Forking Programs

If your program forks, the child inherits the parent's profiling data. If the output filename doesn't include `%p` (process ID), parent and child outputs will be intermingled, making the file unreadable by `ms_print`.

### 9.2.8. Measuring All Memory in a Process

By default, Massif only measures heap memory (allocated via `malloc`, `new`, etc.). It does not measure memory allocated via lower-level system calls like `mmap`, `mremap`, or `brk`.

To measure all memory (including code, data, BSS segments, and stacks), use `--pages-as-heap=yes`. This treats every page allocated via `mmap` as a distinct block. Note that `--stacks=yes` is incompatible with `--pages-as-heap=yes`.

When using `--pages-as-heap=yes`, `ms_print` output changes slightly:
- The allocation source is listed as `(page allocation syscalls) mmap/mremap/brk, --alloc-fns, etc.` instead of `(heap allocation functions)`.

### 9.2.9. Acting on Massif's Information

- Start by examining the **peak snapshot**.
- Analyze the graph's shape to ensure memory usage aligns with expectations; investigate unexpected spikes.
- View detailed snapshots in a wide window or text editor for easier navigation.

---

## 9.3. Using massif-visualizer

`massif-visualizer` is a graphical tool for viewing Massif data, often easier to use than `ms_print`. It is not included with Valgrind but is available online.

---

## 9.4. Massif Command-line Options

- `--heap=<yes|no>` [default: yes]: Enable/disable heap profiling.
- `--heap-admin=<size>` [default: 8]: Administrative bytes per block.
- `--stacks=<yes|no>` [default: no]: Enable/disable stack profiling (slows Massif significantly).
- `--pages-as-heap=<yes|no>` [default: no]: Profile memory at the page level instead of malloc'd blocks.
- `--depth=<number>` [default: 30]: Maximum depth of allocation trees.
- `--alloc-fn=<name>`: Treat specified functions as heap allocation functions (useful for wrappers around `malloc`/`new`).
- `--ignore-fn=<name>`: Ignore direct heap allocations in specified functions.
- `--threshold=<m.n>` [default: 1.0]: Significance threshold for heap allocations (as a percentage).
- `--peak-inaccuracy=<m.n>` [default: 1.0]: Allowable inaccuracy for peak memory recording.
- `--time-unit=<i|ms|B>` [default: i]: Time unit for profiling (`i`=instructions, `ms`=milliseconds, `B`=bytes).
- `--detailed-freq=<n>` [default: 10]: Frequency of detailed snapshots.
- `--max-snapshots=<n>` [default: 100]: Maximum number of snapshots recorded.
- `--massif-out-file=<file>` [default: massif.out.%p]: Output filename (supports `%p` for PID and `%q` for environment variables).

---

## 9.5. Massif Monitor Commands

Massif supports monitor commands via Valgrind's gdbserver:
- `snapshot [<filename>]`: Take a snapshot.
- `detailed_snapshot [<filename>]`: Take a detailed snapshot.
- `all_snapshots [<filename>]`: Save all captured snapshots.
- `xtmemory [<filename>]`: Generate an xtree heap memory report.

---

## 9.6. Massif Client Requests

Massif implements two core client requests:
- `VALGRIND_MALLOCLIKE_BLOCK`
- `VALGRIND_FREELIKE_BLOCK`

These are described in the Client Request mechanism documentation.

---

## 9.7. ms_print Command-line Options

- `-h --help`: Show help message.
- `--version`: Show version number.
- `--threshold=<m.n>` [default: 1.0]: Filter allocation tree entries below this percentage.
- `--x=<4..1000>` [default: 72]: Graph width (columns).
- `--y=<4..1000>` [default: 20]: Graph height (rows).

---

## 9.8. Massif's Output File Format

Massif's output is plain text, designed to be human- and machine-readable. The exact format is currently Massif-specific and undocumented, but plans exist to generalize it for broader tool compatibility in the future.
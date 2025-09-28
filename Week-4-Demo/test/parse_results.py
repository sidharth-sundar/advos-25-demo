#import pandas as pd
import re
from statistics import mean

ops_arr = [1000000]
readers_arr = [1, 5, 9]
dirs = ["race", "spinlock", "rwspinlock", "seqlock", "rcu"]
arr = [] # record tuples: (dir, ops, readers, ns/read, ns/write, final value of counter)

for ops in ops_arr:
    for readers in readers_arr:
        for dir in dirs:
            filename = f"results/{dir}_ops-{ops}_readers-{readers}.out"

        
            with open(filename, "r") as f:
                ns_per_reader = [] # this is going to lead to an inaccurate metric: average of averages, but it's sufficient for our use case
                ns_per_writer = []
                final_value = -1
                for line in f.readlines():
                    w = "writer" in line
                    r = "reader" in line
                    if w or r:
                        match = re.search(r'(\d+)\s*ns', line)
                        match2 = re.search(r'(\d+)\s*ops', line)

                        if match and match2:
                            ns_per_rw = int(match.group(1)) / int(match2.group(1))
                            ns_per_reader.append(ns_per_rw) if r else ns_per_writer.append(ns_per_rw)

                    elif "Final" in line:
                            final_value = int(line.split()[-1]) # get value after completed ops
                print(ns_per_reader, ns_per_writer)
                arr.append((dir, ops, readers, mean(ns_per_reader), mean(ns_per_writer), final_value))

for entry in arr:
    print(entry)

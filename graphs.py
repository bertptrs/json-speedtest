import sys

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


df = pd.read_csv(sys.argv[1])

target_dir = sys.argv[2]

x_pos = np.arange(len(df['implementation'])) + 0.5

fig, ax = plt.subplots()

ax2 = ax.twinx()
b1 = ax.bar(x_pos - 0.2, df['compile_time'], width=0.4, label="Compile time")
ax.set_ylabel('compile time (s)')
ax.set_xticks(x_pos)
ax.set_xticklabels(df['implementation'])
b2 = ax2.bar(x_pos + 0.2, df['compile_size'], width=0.4, color='r',
             label="Binary size")
ax2.set_ylabel('bytes')
ax.legend(handles=[b1, b2])
fig.tight_layout()
plt.savefig(target_dir + '/compile.svg')

plt.close()

fig, ax = plt.subplots()

ax.bar(x_pos, df['run_time'])
ax.set_ylabel('time/iter (ns)')
ax.set_xticks(x_pos)
ax.set_xticklabels(df['implementation'])
fig.tight_layout()
plt.savefig(target_dir + '/runtime.svg')

plt.close()

fig, ax = plt.subplots()

ax2 = ax.twinx()
b1 = ax.bar(x_pos - 0.2, df['num_allocations'], width=0.4,
            label="Allocations")
ax.set_ylabel('# allocations')
ax.set_xticks(x_pos)
ax.set_xticklabels(df['implementation'])
b2 = ax2.bar(x_pos + 0.2, df['bytes_allocated'] / (1024 ** 3), width=0.4,
             color='r', label="Bytes allocated")
ax2.set_ylabel('GiB')
ax2.legend(handles=[b1, b2])
fig.tight_layout()
plt.savefig(target_dir + '/allocations.svg')

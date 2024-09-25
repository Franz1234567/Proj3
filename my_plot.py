import matplotlib.pyplot as plt

#speeds = [2181, 1024, 339, 333, 333, 324, 331, 326, 325, 325, 329, 328, 323]
speeds = [2173, 1025, 328, 1870, 1115, 323, 1868, 1121, 326, 1870, 1117, 325, 1867, 1117, 322]

time = list(range(len(speeds)))

#plt.axhline(y=100, color='r', linestyle='--', label="Reference Speed (100 pps)")
plt.axhline(y=400, color='r', linestyle='--', label="Reference Speed (400 pps)")

plt.plot(time, speeds, marker='o', label='Speed')

plt.title("Speed Evolution by Speed Controller")
plt.xlabel("Time (seconds)")
plt.ylabel("Speed (pulses per second)")
plt.ylim(0, 2800)
plt.legend()
plt.show()
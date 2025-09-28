import os
import random

def write_dat_only (test_id, cache_size, requests, folder="."):
    filename = f"{test_id:03d}"
    path_dat = os.path.join (folder, f"{filename}.dat")
    with open (path_dat, "w") as f:
        f.write (f"{cache_size}\n")
        f.write (f"{len(requests)}\n")
        f.write (" ".join(map(str, requests)) + "\n")

def main ():
    test_cases = []
    random.seed (2025)

    for _ in range (20):
        size = random.randint (10, 50)                  # size: 10–50
        n = random.randint (1000, 5000)                 # requests n: 1K–5K
        key_range = random.randint (50, 200)
        reqs = [random.randint (1, key_range) for _ in range (n)]
        test_cases.append ((size, reqs))

    for i, (size, reqs) in enumerate (test_cases, 1):
        write_dat_only (i, size, reqs, folder=".")

    print (f"Generated {len(test_cases)} stress tests (001.dat to {len(test_cases):03d}.dat)")

if __name__ == "__main__":
    main()

import os
import random

def simulate_optimal_hits_fast (cache_size, requests):
    if cache_size == 0:
        return 0
    n = len (requests)
    if n == 0:
        return 0

    # find_next_occurrence
    next_use = [10**9] * n
    last_occurrence = {}
    for i in range (n - 1, -1, -1):
        key = requests[i]
        if key in last_occurrence:
            next_use[i] = last_occurrence[key]
        last_occurrence[key] = i

    cache = set()
    hits = 0

    for i in range (n):
        key = requests[i]
        if key in cache:
            hits += 1
        else:
            if len (cache) >= cache_size:
                victim = None
                furthest = -1
                for page in cache:
                    next_idx = 10**9
                    for j in range (i + 1, n):
                        if requests[j] == page:
                            next_idx = j
                            break
                    if next_idx > furthest:
                        furthest = next_idx
                        victim = page
                cache.remove (victim)
            cache.add (key)
    return hits

def simulate_optimal_hits (cache_size, requests):
    if cache_size == 0:
        return 0

    cache = set()
    hits = 0
    n = len (requests)

    for i, key in enumerate (requests):
        if key in cache:
            hits += 1
        else:
            if len (cache) >= cache_size:
                victim = None
                furthest = -1
                for page in cache:
                    next_use = 10**9
                    for j in range (i + 1, n):
                        if requests[j] == page:
                            next_use = j
                            break
                    if next_use > furthest:
                        furthest = next_use
                        victim = page
                cache.remove (victim)
            cache.add (key)
    return hits

def generate_phase_requests (phases, phase_size, key_range):
    requests = []
    for phase in range (phases):
        phase_keys = list (range(phase * key_range + 1, (phase + 1) * key_range + 1))
        for _ in range (phase_size):
            requests.append (random.choice(phase_keys))
    return requests

def generate_bursty_requests (total, burst_prob = 0.1, common_keys = 10, rare_keys = 100):
    requests = []
    common = list (range(1, common_keys + 1))
    rare = list (range(common_keys + 1, common_keys + rare_keys + 1))
    for _ in range (total):
        if random.random() < burst_prob:
            requests.append (random.choice(rare))
        else:
            requests.append (random.choice(common))
    return requests

def write_test (test_id, cache_size, requests, folder):
    filename = f"{test_id:03d}"
    path_dat = os.path.join (folder, f"{filename}.dat")
    path_ans = os.path.join (folder, f"{filename}.ans")

    with open (path_dat, "w") as f:
        f.write (f"{cache_size}\n")
        f.write (f"{len(requests)}\n")
        f.write (" ".join(map(str, requests)) + "\n")

    hits = simulate_optimal_hits (cache_size, requests)
    with open (path_ans, "w") as f:
        f.write (f"{hits}\n")

def main ():
    test_cases = []

    # small tests
    test_cases.extend ([
        (3, [1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5]),
        (2, [1, 2, 1, 2, 1, 2]),
        (1, [1, 2, 3, 4, 5]),
        (4, [1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5]),
        (0, [1, 2, 3, 4]),
    ])

    # random middle tests
    random.seed (2025)
    for _ in range (8):
        size = random.randint (5, 20)
        n = random.randint (100, 500)
        reqs = [random.randint (1, 50) for _ in range (n)]
        test_cases.append ((size, reqs))

    # local tests
    for phase_count in [3, 5]:
        for phase_size in [50, 100]:
            reqs = generate_phase_requests (phases = phase_count, phase_size = phase_size, key_range = 10)
            size = 15
            test_cases.append ((size, reqs))

    # bursty tests
    for total in [200, 500, 1000]:
        reqs = generate_bursty_requests (total = total, burst_prob = 0.05, common_keys = 8, rare_keys = 50)
        size = 10
        test_cases.append ((size, reqs))

    # stress tests
    test_cases.append ((20, [random.randint (1, 100) for _ in range (1500)]))
    test_cases.append ((30, [random.randint (1, 200) for _ in range (2000)]))

    for i, (size, reqs) in enumerate (test_cases, 1):
        write_test(i, size, reqs, folder = ".")

    print (f"Generated {len(test_cases)} tests in Ideal/ (001.dat to {len(test_cases):03d}.dat)")

if __name__ == "__main__":
    main()

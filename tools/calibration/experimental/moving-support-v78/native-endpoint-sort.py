"""Research reconstruction of immutable PhysXCore.dll RVA 0x327e0.

Sorts descending unsigned encoded coordinates; equal coordinates compare only
the endpoint type. Shape IDs never break ties. This reproduces the original
unstable swaps, including selection sort for partitions of at most five items.
No captured wheel permutation or case identifier is an input to this function.
"""

def less(a, b):
    return a[0] < b[0] or (a[0] == b[0] and bool(a[1] & 0x80000000)
                          and not bool(b[1] & 0x80000000))

def native_sort(endpoints):
    a = list(endpoints)
    pending = [(0, len(a) - 1)]
    while pending:
        lo, hi = pending.pop()
        while lo < hi:
            if hi - lo < 5:
                for start in range(lo, hi):
                    largest = start
                    for index in range(start + 1, hi + 1):
                        if less(a[largest], a[index]):
                            largest = index
                    a[start], a[largest] = a[largest], a[start]
                break
            mid = (lo + hi) // 2
            for first, second in ((lo, mid), (lo, hi), (mid, hi)):
                if less(a[first], a[second]):
                    a[first], a[second] = a[second], a[first]
            pivot = a[mid]
            a[mid], a[hi - 1] = a[hi - 1], a[mid]
            left, right = lo, hi - 1
            while True:
                left += 1
                while less(pivot, a[left]):
                    left += 1
                right -= 1
                while less(a[right], pivot):
                    right -= 1
                if left >= right:
                    break
                a[left], a[right] = a[right], a[left]
            a[left], a[hi - 1] = a[hi - 1], a[left]
            if left - lo > hi - left:
                pending.append((lo, left - 1))
                lo = left + 1
            else:
                pending.append((left + 1, hi))
                hi = left - 1
    return a

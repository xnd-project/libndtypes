from itertools import accumulate


class UnbalancedTree(Exception):
    pass

class Shape(list):
    pass

class Data(list):
    pass

class Opt(int):
    def __init__(self, v):
        if v != 0:
            raise ValueError
        self.v = int(0)
    def __repr__(self):
        return "Opt(%d)" % self.v
    def __eq__(self, other):
        if isinstance(other, Opt):
            return self.v == other.v
        return False
    def __hash__(self):
        return hash(self.v)


class FixedDim(int):
    def __init__(self, v):
        self.v = int(v)
    def __repr__(self):
        return "FixedDim(%d)" % self.v

class VarDim(list):
    def __init__(self, v):
        self.v = list(v)
    def __repr__(self):
        return "VarDim(%s)" % self.v

class Bitmap(int):
    def __init__(self, v):
        self.v = int(v)
    def __repr__(self):
        return "Bitmap(%s)" % self.v

def is_value_list(a):
    return all(not isinstance(v, list) for v in a)

def shapes_and_data(a):
    acc = []
    def f(cls, n, a):
        if n == len(acc):
            acc.append(cls([]))
        if not isinstance(acc[n], cls):
            raise UnbalancedTree
        if a is None:
            if cls == Shape:
                acc[n].append(Opt(0))
            else:
                acc[n].append(None)
        elif isinstance(a, list):
            acc[n].append(len(a))
            cls = Data if is_value_list(a) else Shape
            for v in a:
                f(cls, n+1, v)
        elif isinstance(a, int):
            acc[n].append(a)
        else:
            raise TypeError("unexpected type: %s" % a)
    f(Shape, 0, a)
    return acc[:-1], acc[-1]

def offsets_and_bitmaps(shapes):
    acc = []
    for lst in shapes:
        opt = False
        s = set(lst)
        n = len(set(lst))
        if Opt(0) in s:
            s.remove(Opt(0))
            opt = True
        if n == 0:
            acc.append(FixedDim(0))
        elif n == 1:
            acc.append(Bitmap(lst[0]))
            acc.append(FixedDim(lst[0]))
        else:
            acc.append(Bitmap(len(lst)))
            acc.append(VarDim([0] + list(accumulate(lst))))
    return acc


if __name__ == '__main__':
    a = [[[0, 1], [2, 3]], [[4, 5, None], None, [7]], [[8, 9]]]
    # a = [[[0, 1, 10], [2, 3]], [[4, 5], [6, 7]], [[8, 9], [10, 11]]]
    shapes, data = shapes_and_data(a)
    print("(shapes, data): %s" % ((shapes, data),))
    ret = [data, Bitmap(len(data))] + list(reversed(offsets_and_bitmaps(shapes)))
    print(ret)

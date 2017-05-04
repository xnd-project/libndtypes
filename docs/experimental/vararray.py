from itertools import accumulate


class UnbalancedTree(Exception):
    pass

class Shape(list):
    pass

class Data(list):
    pass

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
                acc[n].append(0)
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

def offsets(shapes):
    return tuple(list(accumulate(shape)) for shape in shapes)


if __name__ == '__main__':
    a = [[[0, 1], [2, 3]], [[4, 5, None], None, [7]], [[8, 9]]]
    shapes, data = shapes_and_data(a)
    print("(shapes, data): %s" % ((shapes, data),))
    print(offsets(shapes))


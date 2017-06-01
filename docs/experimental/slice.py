# Definition of generalized slicing and indexing (includes vararrays).

def getitem(lst, indices):
    """generalized for arbitary indices and slices"""
    if not indices:
        return lst
    i, indices = indices[0], indices[1:]
    item = list.__getitem__(lst, i)
    if isinstance(i, int):
        return getitem(item, indices)
    return [getitem(x, indices) for x in item]

class Array(list):
    def __getitem__(self, indices):
        if not isinstance(indices, tuple):
            indices = (indices,)
        if not all(isinstance(x, int) or isinstance(x, slice) for x in indices):
            raise TypeError(
                "index must be int or slice or a tuple of integers and slices")
        return getitem(self, indices)

if __name__ == '__main__':
    x = Array([[1],
               [2,3],
               [4,5,6]])
    print(x[:, 3:0:-1])





"""
    Testing entropy and information_gain functions
"""
x_book_debug = np.array([[True, True, True, "Mammal"],
                         [True, True, True, "Mammal"],
                         [True, True, False, "Reptile"],
                         [False, True, True, "Mammal"],
                         [True, True, True, "Mammal"],
                         [True, True, True, "Mammal"],
                         [True, False, False, "Reptile"],
                         [True, True, False, "Reptile"],
                         [True, True, True, "Mammal"],
                         [False, True, True, "Reptile"]])
y_book_debug = x_book_debug[:, -1]
print(y_book_debug)
x_book_debug = np.delete(x_book_debug, -1, 1)
print(x_book_debug)
print(x_book_debug.shape, " ", y_book_debug.shape)
print(FEATURE_NAMES)
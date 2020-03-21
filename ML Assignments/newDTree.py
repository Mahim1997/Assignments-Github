"""
    Can't have CONTINUOUS data [Will be binarized first]
    Decision Tree Classifier
"""    
class DTreeClassifier:
    def __init__(self):
        self.d_tree_root = DTreeNode()
        self.max_depth = 5
    
    def form_a_leaf_node(self, labels):
        leaf_node = DTreeNode()
        leaf_node.is_leaf_node = True
        leaf_node.classification = return_majority_label(labels)
        return leaf_node
    ##### Can't have CONTINUOUS data [ONLY CATEGORICAL/BINARIZED DATA IS ALLOWED !!]
    def recursive_fit(self, X, Y, current_depth, max_depth):
        ## 1. If current-depth == max-depth [Base Case]
        if current_depth == max_depth:
            return self.form_a_leaf_node(Y) # return the leaf node with majority of the current labels
        ## 2. If EXACTLY one label ... return that
        if (len(np.unique(Y)) == 1):
            return self.form_a_leaf_node(Y) # return the leaf node with majority of the current labels
        current_depth = current_depth + 1  # increment current_depth variable
        val_max_IG = col_max_IG = -1
        for col in range(0, X.shape[1]):  ## for each number of columns/features
            ig_this_col = calculate_information_gain(X, Y, col, "CATEGORICAL")
#             print("col = ", col, " ig_col = ", ig_current_col)
            if ig_this_col > val_max_IG:
                val_max_IG = ig_this_col
                col_max_IG = col
        
        ## 3. Max IG obtained is 0 [no more examples/features]
        if val_max_IG == 0:
            return self.form_a_leaf_node(Y)
        
        d_tree_node = DTreeNode()
        d_tree_node.classification = return_majority_label(Y)  ### Saves the pluarility value of THIS node [used in prediction]
        ## -> Recursion
        for val_of_this_feature in np.unique(X[:, col_max_IG]):
            index_child = (X[:, col_max_IG] == val_of_this_feature)
            X_child = X[index_child]
            Y_child = Y[index_child]

            d_tree_node.is_leaf_node = False  ### is an internal node
            d_tree_node.feature_col = col_max_IG  ### question is to be asked on this column/feature
            # d_tree_node.feature_values.append(val_of_this_feature)
            if (X_child.shape[0] == 0): # no more examples [DON'T return as other values of list will be empty]
                d_tree_node.children.children[val_of_this_feature] = self.form_a_leaf_node(Y)  # return parent's max plurality value
                # d_tree_node.sub_trees.append(self.form_a_leaf_node(labels))
            else:
            ##### DICTIONARY {key = feature_val, value = sub-tree}
                sub_tree = self.recursive_fit(X_child, Y_child, current_depth, max_depth)
                d_tree_node.children[val_of_this_feature] = sub_tree
                # d_tree_node.sub_trees.append(sub_tree)
        ### END OF FOR LOOP, return d_tree_node
        return d_tree_node
    
    def printTree(self):
        print("-->Inside printTree")
        self.d_tree_root.printTree()

    def fit(self, examples, labels, max_depth = 5):
        dt_node = self.recursive_fit(examples, labels, current_depth = 0, max_depth=max_depth)
        self.d_tree_root = dt_node
        print("Fit done for, max-depth = ", max_depth)
        
    def predict_one_example(self, example_to_predict):
        self.d_tree_root_backup = self.d_tree_root
        root = self.d_tree_root
        if root == None:
            raise Exception('Root of the Decision Tree is null !! [In predict()]')
        while root is not None:
            if root.is_leaf_node == True: # classify ... since, it is the leaf
                self.d_tree_root = self.d_tree_root_backup
                return root.classification
            else:
                col_to_process = root.feature_col
                val_present_in_example = example_to_predict[col_to_process]
                bool_found = False
                print("In col = ", col_to_process, " len root feature vals = ", len(root.feature_values))
                # for idx in range(len(root.feature_values)):
                keys_feature_vals = list(root.children.keys())
                for feature_val in keys_feature_vals:
                    if example_to_predict[root.feature_col] == feature_val:
                        root = root.children[root.feature_col]
                        bool_found = True
                        break
                    
                if bool_found == False: ### SHOULD RETURN PARENT's PLURALITY VALUE
                    raise Exception("The value of ", val_present_in_example, " doesn't exist for col_idx = ", col_to_process)
                
    def predict(self, examples_test):
        labels = []
        for example in examples_test:
            labels.append(self.predict_one_example(example))
        return labels
    
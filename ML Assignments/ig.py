"""
    Input: X (examples), Y (labels), feature_column(idx on X), feature_type [CONTINUOUS/CATEGORICAL],
            use_custom_columns {optional}, custom_columns_list {optional/=}
    Output: Information Gain wrt that feature [CATEGORICAL/BINARIZED]
            Dictionary of information gains wrt each values of that feature [CONTINUOUS]
    Dependency: Uses calculate_entropy() function written above
    Calculation:
        for each value v of examples[feature_column]:
            Partition new_examples[v] by choosing that feature.val == v [if categorical]
            Partition new_examples[v] by choosing that feature.val <= v [if continuous]
            calculate entropy of new_examples, H(S_feature_val_v)
            calculate num_examples(S_feature_val_v)/num_examples(parent_node)
            Use formula IG = H(S{parent}) - [Sum of |S_val|/|S| * H(S_val)]
    Treat either as continuous, or categorical [binarized data is treated as categorical]
"""
def calculate_information_gain(X, Y, feature_column, feature_type,
                              use_custom_columns = False, custom_columns_list = None):  # WORKING
    entropy_parent_node = calculate_entropy(X, Y) # entropy of parent node
    if ((use_custom_columns == True)) :
        unique_vals_features = custom_columns_list
    else:
        unique_vals_features = np.unique(X[:, feature_column])
#         print("-->>DOING FOR ALL UNIQUE CONTINUOUS COLUMNS, feature-col = ", feature_column,
#             " , feature_type = ", feature_type, " len_unique_feature_vals = ", len(unique_vals_features))
    if feature_type == "CONTINUOUS":
#         print("-->>FEATURE_COL = ", feature_column, " .. inside if feature type == CONTINUOUS")
        # Partition into two sets ... x <= val and x > val
        info_gain_dict = {}
        for val in unique_vals_features:
            idx_left_bool = X[:, feature_column] <= float(val)
            idx_right_bool = X[:, feature_column] > float(val)
            data_left = X[idx_left_bool]
            label_left = Y[idx_left_bool]
            data_right = X[idx_right_bool]
            label_right = Y[idx_right_bool]
            entropy_left = calculate_entropy(data_left, label_left)
            entropy_right = calculate_entropy(data_right, label_right)
            info_gain = entropy_parent_node - (
                ((len(data_left)/len(X)) * entropy_left) + 
                ((len(data_right)/len(X)) * entropy_right)
            )
            info_gain_dict[val] = info_gain
#         print("+++-->>Inside calculate_info_gain() [CONTINUOUS] ... dictionary-len = ", 
#               len(info_gain_dict), " ... printing info_gain_dict .. ", info_gain_dict)
        return info_gain_dict
    else:# CATEGORICAL
        ## Partition into FOR EACH FEATURE
#         entropy_per_val = {} # empty dictionary
        num_examples_parent = len(X)
        cumulative_entropy = 0.0 # cumulative entropy for all features
        if num_examples_parent == 0: # SOMEHOW comes down to this
            return 0
        for val in unique_vals_features:
            idx_equal_to_feature = X[:, feature_column] == val
            data_of_feature = X[idx_equal_to_feature]
            label_of_feature = Y[idx_equal_to_feature]
            entropy_of_feature = calculate_entropy(data_of_feature, label_of_feature)
#             print("val = ", val, " , entropy of feature = ", entropy_of_feature)
            proportion_of_examples_in_feature = float(len(data_of_feature)) / float(num_examples_parent)
#             print("proportion of examples in feature = ", proportion_of_examples_in_feature)
            cumulative_entropy = cumulative_entropy + (proportion_of_examples_in_feature * entropy_of_feature)
#             print("cumulative entropy = ", cumulative_entropy, " parent entropy = ", entropy_parent_node)
        #             entropy_per_val[val] = entropy_of_feature
        # now subtract from parent's entropy to return the information gain
        info_gain = entropy_parent_node - cumulative_entropy
        return info_gain
        
        
        
def calculate_entropy(examples, labels, epsilon_small = 0.0000000000000000001): # WORKING
    labels_unique = np.unique(labels) # obtain the unique labels of the data
    # gives unique label_names, and counts for each unique label_names
    label_names, label_counts = np.unique(labels,
                                         return_counts = True) 
    label_probabilities = label_counts/sum(label_counts)
    label_log_probabilities = np.log2((label_probabilities + epsilon_small))
    label_products = label_probabilities * label_log_probabilities
#     print(len(examples), " , ", label_names , " , " , label_probabilities, " , ", label_log_probabilities)
#     print(label_products)
    entropy = -1 * sum(label_products)
    if entropy == 0.0:  # to not return -0.0
        entropy = 0.0
    return entropy
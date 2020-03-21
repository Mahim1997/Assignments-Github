"""
    Good for printing using pprint but not good for predictions
    Data only in categorical/binarized form. [Can't Have Continuous data]
    Can form M-ary tree .
    ***Feature is not removed as for next iteration, it will not have a good IG value ... so will never be chosen again.
    Tree forms as a decision_tree
    tree = { {best_feature_question}:{} }  -->> Good for pretty print pprint(tree)
    for val in data[best_feature].unique_vals:
        subtree = DecisionTreeAlgorithm(partition_examples, partition_labels,
                                        parent_examples, parent_labels, depth_current, depth_max)
        tree[subtree][val] = subtree
"""
def decision_tree_fit(examples, labels, current_depth = 0, max_depth = 5):
    #### Base cases
    ##1. Has ONLY ONE LABEL [return that label]
    if len(np.unique((labels))) == 1:
        return return_majority_label(labels)
    ##2. Check if depth has reached ..
    if current_depth == max_depth:
        print("-->>MAX DEPTH CONDITION, current_depth = ", current_depth, " , max-depth = ", max_depth)
        return return_majority_label(labels)
    
    current_depth = current_depth + 1
#     columns_of_features = np.arange(examples.shape[1]) # no. of columns
    max_info_gain_col = -1
    max_info_gain_val = -99999
    for col in range(0, examples.shape[1]): 
        info_gain_this_col =  calculate_information_gain(examples, labels, 
                                                         col, "CATEGORICAL")
        ## Calculates the column of max information gain...
        if info_gain_this_col > max_info_gain_val:
            max_info_gain_val = info_gain_this_col
            max_info_gain_col = col
    ##### Base cases continued
    ##3. Max IG == 0 [Either no more features or no more examples]
    if max_info_gain_col == 0:
        return return_majority_label(labels)
    ### Now recursive calls ...
    unique_feature_vals = np.unique(examples[:, max_info_gain_col])
    ## Initiate the decision_tree for each best_feature value
    decision_tree = {max_info_gain_col:{}}
    for feature_val in unique_feature_vals:
        ## Partition the data into these parts ... i.e. wrt feature_val
        partition_data_index = examples[:, max_info_gain_col] == feature_val
        partition_examples = examples[partition_data_index]
        partition_labels   = labels[partition_data_index]
        #### Base case continued ... 
        ## 4. If no more examples remaining ... return parent's majority label.
        if len(partition_examples) == 1:  # Return majority of parent's labels
            return return_majority_label(labels)
        else:
            decision_tree[max_info_gain_col][feature_val] = decision_tree_fit(partition_examples, partition_labels, 
                                                                    current_depth, max_depth)

    return decision_tree
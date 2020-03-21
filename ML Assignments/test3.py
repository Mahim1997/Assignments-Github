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
def decision_tree_fit(names_columns, dictionary_of_binarized_split_vals,
    feature_types_list, examples, labels, parent_examples, parent_labels,
    current_depth = 0, max_depth = 5):
    #### Base cases
    # decision_tree = {}
    ##1. Has ONLY ONE LABEL [return that label]
    if len(np.unique((labels))) == 1:
        return return_majority_label(labels)
    ##2. Check if depth has reached ..
    if current_depth == max_depth:
        print("-->>MAX DEPTH CONDITION, current_depth = ", current_depth, " , max-depth = ", max_depth)
        return return_majority_label(labels)
    
    current_depth = current_depth + 1
    columns_of_features = np.arange(X.shape[1]) # no. of columns
    information_gains = {}  # {Col_Feature: IG_of_feature(max IG if continuous)}
    for col in columns_of_features: 
        info_gain_this_col =  calculate_information_gain(examples, labels, col, feature_types_list[col])
        if feature_types_list[col] == "CONTINUOUS": # then take the max gain .. info_gain_this_col is a decision_tree
            print("----->>>CAN'T HAVE CONTINUOUS FEATURE ... BINARIZE BEFOREHAND .... returning NONE")
#             return None
            v = list(info_gain_this_col.values())
            k = list(info_gain_this_col.keys())
            max_split_val_continuous = k[v.index(max(v))]
            info_gain = max(v)
            information_gains[col] = info_gain
        else:
            information_gains[col] = info_gain_this_col
    # For loop done ... now calculate the max IG wrt WHICH feature ?
    keys = list(information_gains.keys())  # keys contain columns
    vals = list(information_gains.values()) # vals contain the IG / max IG values
    best_feature = keys[vals.index(max(vals))] # The best feature column that has the maximum IG
    max_IG = max(vals)
    ##### Base cases continued
    ##3. Max IG == 0 [Either no more features or no more examples]
    if max_IG == 0:
        return return_majority_label(labels)
    ### Now recursive calls ...
    unique_feature_vals = np.unique(X[:, best_feature])
    ## Initiate the decision_tree for each best_feature value
    decision_tree = {best_feature:{}}
#     decision_tree = {names_columns[best_feature]: {}}
    type_of_feature = feature_types_list[col]
    
    if type_of_feature == "CONTINUOUS":
        left_partition_idx = examples[:, best_feature] <= max_split_val_continuous
        right_partition_idx = examples[:, best_feature] > max_split_val_continuous
        left_partition_examples  = examples[left_partition_idx]
        right_partition_examples = examples[right_partition_idx]
        left_partition_labels = labels[left_partition_idx]
        right_partition_labels = labels[right_partition_idx]
        feature_val_question_left_partition = "{} <= {}".format(best_feature, max_split_val_continuous)
        feature_val_question_right_partition = "{} > {}".format(best_feature, max_split_val_continuous)
        
        decision_tree[best_feature][feature_val_question_left_partition] = decision_tree_fit(names_columns,
        dictionary_of_binarized_split_vals, feature_types_list, left_partition_examples, left_partition_labels, 
        examples, labels, current_depth, max_depth)
        
        decision_tree[best_feature][feature_val_question_right_partition] = decision_tree_fit(names_columns,
        dictionary_of_binarized_split_vals, feature_types_list, right_partition_examples, right_partition_labels, 
        examples, labels, current_depth, max_depth)
    
    else: # CATEGORICAL/BINARIZED
        for feature_val in unique_feature_vals:
            ## Partition the data into these parts ... i.e. wrt feature_val
            partition_data_index = examples[:, best_feature] == feature_val
            partition_examples = examples[partition_data_index]
            partition_labels   = labels[partition_data_index]
            #### Base case continued ... 
            ## 4. If no more examples remaining ... return parent's majority label.
            if type_of_feature == "BINARIZED":
                if feature_val == 0:
                    feature_val_question = "{} == {}".format(best_feature, 0)
                elif feature_val == 1:
                    feature_val_question = "{} == {}".format(best_feature, 1)
            else:  # CATEGORICAL
                feature_val_question = "{} == {}".format(best_feature, feature_val)
            if len(partition_examples) == 1:
    #             decision_tree[best_feature][feature_val] = return_majority_label(labels)
                decision_tree[best_feature][feature_val_question] = return_majority_label(labels)
            else:
    #             decision_tree[best_feature][feature_val] = ...
                decision_tree[best_feature][feature_val_question] = decision_tree_fit(names_columns,
            dictionary_of_binarized_split_vals, feature_types_list, partition_examples, partition_labels, 
            examples, labels, current_depth, max_depth)

    return (decision_tree)
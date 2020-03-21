def predict_for_decision_tree(query, tree, default_label=0):
    for key in list(query.keys()):
        if key in list(tree.keys()):
            try:
                sub_tree = dtree[key][example_dict[key]]
            except:
                return default_label
            
            if isinstance(sub_tree, dict): # recursive query
                return predict_for_decision_tree(sub_tree, example_dict, default_label)
            else: # base case
                return sub_tree
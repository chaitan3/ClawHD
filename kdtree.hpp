#ifndef KDTREE_H
#define KDTREE_H
#include <list>

template <class data_type> 
class node {
    public:
    data_type data;
    node* left;
    node* right;
    node (data_type d) {
        this -> data = d;
        this -> left = NULL;
        this -> right = NULL;
    }
};

template <class data_type>
class kdtree {
    node<data_type>* root;
    
    void actual_insert (data_type data, node <data_type>* curr, int depth) {
        if (curr == NULL) {
            this -> root = new node <data_type> (data);
        }
        else {
            bool result;
            if (depth % 2 == 0) {
                result = data -> x_compare (curr -> data);
            }
            else {
                result = data -> y_compare (curr -> data);
            }
            if (result) {
                if (curr -> left == NULL) {
                    curr -> left = new node <data_type> (data);
                }
                else {
                    actual_insert (data, curr -> left, depth + 1);
                }
            }
            else {
                if (curr -> right == NULL) {
                    curr -> right = new node <data_type> (data);
                }
                else {
                    actual_insert (data, curr -> right, depth + 1);
                }
            }
        }
    }
    
    void actual_range_search (list <data_type>* elements, coords* c_top_left, coords* c_bottom_right,
        node <data_type>* curr, int depth) {
         
        if (curr == NULL) return;   
            
        coords *c_pos = curr -> data -> get_coords ();
        int x = c_pos -> x;
        int y = c_pos -> y;
        
        if (depth % 2 == 0) {
            if (x < c_top_left -> x)
                actual_range_search (elements, c_top_left, c_bottom_right, curr -> right, depth + 1);
            else if (x > c_bottom_right -> x)
                actual_range_search (elements, c_top_left, c_bottom_right, curr -> left, depth + 1);
            else {
                if ((c_top_left -> y <= y) && (y <= c_bottom_right -> y))
                    elements -> push_back (curr -> data);
                actual_range_search (elements, c_top_left, c_bottom_right, curr -> left, depth + 1);
                actual_range_search (elements, c_top_left, c_bottom_right, curr -> right, depth + 1);
            }
        }
        else {
            if (y < c_top_left -> y)
                actual_range_search (elements, c_top_left, c_bottom_right, curr -> right, depth + 1);
            else if (y > c_bottom_right -> y)
                actual_range_search (elements, c_top_left, c_bottom_right, curr -> left, depth + 1);
            else {
                if ((c_top_left -> x <= x) && (x <= c_bottom_right -> x))
                    elements -> push_back (curr -> data);
                actual_range_search (elements, c_top_left, c_bottom_right, curr -> left, depth + 1);
                actual_range_search (elements, c_top_left, c_bottom_right, curr -> right, depth + 1);
            }
        }
    }
    void actual_print_tree (node<data_type>* curr, int depth, bool right) {
        coords *c_pos = curr -> data -> get_coords ();
        cout << depth << " " << c_pos -> x << " " << c_pos -> y << " " << right << endl;
        if (curr -> left != NULL)
            actual_print_tree (curr -> left, depth + 1, false);
        if (curr -> right != NULL)
            actual_print_tree (curr -> right, depth + 1, true);
    }
    
    public:
    void insert (data_type data) {
        this -> actual_insert (data, this -> root, 0);
    }
    
    list <data_type>* range_search (coords* c_top_left, coords* c_bottom_right) {
        list <data_type>* interior_elements = new list <data_type> ();
        actual_range_search (interior_elements, c_top_left, c_bottom_right, this -> root, 0);
        return interior_elements;
    }
    
    void print_tree () {
        actual_print_tree (this -> root, 0, false);
    }
    
    kdtree () {
        this -> root = NULL;
    }
};
        

#endif

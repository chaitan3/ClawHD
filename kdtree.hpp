#ifndef KDTREE_H
#define KDTREE_H
#include <vector>

template <class data_type> 
class node {
    public:
    data_type data;
    node* left;
    node* right;
    node (data_type d) {
        this -> data = d;
        this -> left = nullptr;
        this -> right = nullptr;
    }
};

template <class data_type>
class kdtree {
    node<data_type>* root;
    
    void free (node <data_type>* curr) {
        if (curr == nullptr)
            return;
        free (curr -> left);
        free (curr -> right);
        delete curr -> data;
        delete curr;
    }
    
    void actual_insert (node <data_type>* new_node, node <data_type>* curr, int depth) {
        if (new_node == nullptr)
            return;
        
        data_type data = new_node -> data;
        if (curr == nullptr) {
            this -> root = new_node;
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
                if (curr -> left == nullptr) {
                    curr -> left = new_node;
                }
                else {
                    actual_insert (new_node, curr -> left, depth + 1);
                }
            }
            else {
                if (curr -> right == nullptr) {
                    curr -> right = new_node;
                }
                else {
                    actual_insert (new_node, curr -> right, depth + 1);
                }
            }
        }
    }
    
    void actual_range_search (vector <data_type>* elements, coords* c_top_left, coords* c_bottom_right,
        node <data_type>* curr, int depth) {
         
        if (curr == nullptr) return;   
            
        coords *c_pos = curr -> data -> get_coords ();
        int x = c_pos -> x;
        int y = c_pos -> y;
        
        if (depth % 2 == 0) {
            if (x < c_top_left -> x)
                actual_range_search (elements, c_top_left, c_bottom_right, curr -> right, depth + 1);
            else if (x > c_bottom_right -> x)
                actual_range_search (elements, c_top_left, c_bottom_right, curr -> left, depth + 1);
            else {
                if ((c_top_left -> y <= y) && (y <= c_bottom_right -> y)) {
                    elements -> push_back (curr -> data);
                }
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
                if ((c_top_left -> x <= x) && (x <= c_bottom_right -> x)) {
                    elements -> push_back (curr -> data);
                }
                actual_range_search (elements, c_top_left, c_bottom_right, curr -> left, depth + 1);
                actual_range_search (elements, c_top_left, c_bottom_right, curr -> right, depth + 1);
            }
        }
    }
    void actual_print_tree (node<data_type>* curr, int depth, bool right) {
        coords *c_pos = curr -> data -> get_coords ();
        cout << depth << " " << c_pos -> x << " " << c_pos -> y << " " << right << endl;
        if (curr -> left != nullptr)
            actual_print_tree (curr -> left, depth + 1, false);
        if (curr -> right != nullptr)
            actual_print_tree (curr -> right, depth + 1, true);
    }
    
    public:
    void insert (data_type data) {
        node <data_type>* new_node = new node <data_type> (data);
        this -> actual_insert (new_node, this -> root, 0);
    }
    
    void remove (data_type data) {
        int depth = 0;
        bool result;
        node <data_type>* curr = this -> root;
        node <data_type>** conn;
        
        while (curr -> data != data) {
            if (depth % 2 == 0) 
                result = data -> x_compare (curr -> data);
            else
                result = data -> y_compare (curr -> data);
            if (result) {
                conn = &(curr -> left);
                curr = curr -> left;
            }
            else {
                conn = &(curr -> right);
                curr = curr -> right;
            }
            depth += 1;
        }
        if (curr -> left != nullptr) {
            *conn = curr -> left;
            this -> actual_insert (curr -> right, curr -> left, depth);
        }
        else if (curr -> right != nullptr) {
            *conn = curr -> right;
            this -> actual_insert (curr -> left, curr -> right, depth);
        }
        else {
            *conn = nullptr;
        }
        delete curr;
    }
    
    vector <data_type>* range_search (coords* c_top_left, coords* c_bottom_right) {
        vector <data_type>* interior_elements = new vector <data_type> ();
        actual_range_search (interior_elements, c_top_left, c_bottom_right, this -> root, 0);
        return interior_elements;
    }
    
    void print_tree () {
        actual_print_tree (this -> root, 0, false);
    }
    
    kdtree () {
        this -> root = nullptr;
    }
    
    ~kdtree () {
        this -> free (this -> root);
    }
};
        

#endif

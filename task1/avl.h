#include <iostream>
#include <vector>

class AVLNode {
public:
    int key;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(int k): key(k), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    AVLNode* root;

    int height(AVLNode* node) {
        return node ? node->height : 0;
    }

    int balanceFactor(AVLNode* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        return x;
    }

    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        return y;
    }

    AVLNode* insert(AVLNode* node, int key) {
        if (!node) return new AVLNode(key);
        if (key < node->key) node->left = insert(node->left, key);
        else if (key > node->key) node->right = insert(node->right, key);
        else return node;
        node->height = 1 + std::max(height(node->left), height(node->right));
        int balance = balanceFactor(node);
        if (balance > 1 && key < node->left->key) return rightRotate(node);
        if (balance < -1 && key > node->right->key) return leftRotate(node);
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    AVLNode* minValueNode(AVLNode* node) {
        AVLNode* current = node;
        while (current->left) current = current->left;
        return current;
    }

    AVLNode* deleteNode(AVLNode* root, int key) {
        if (!root) return root;
        if (key < root->key) root->left = deleteNode(root->left, key);
        else if (key > root->key) root->right = deleteNode(root->right, key);
        else {
            if (!root->left || !root->right) {
                AVLNode* temp = root->left ? root->left : root->right;
                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp;
                }
                delete temp;
            } else {
                AVLNode* temp = minValueNode(root->right);
                root->key = temp->key;
                root->right = deleteNode(root->right, temp->key);
            }
        }
        if (!root) return root;
        root->height = 1 + std::max(height(root->left), height(root->right));
        int balance = balanceFactor(root);
        if (balance > 1 && balanceFactor(root->left) >= 0) return rightRotate(root);
        if (balance > 1 && balanceFactor(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        if (balance < -1 && balanceFactor(root->right) <= 0) return leftRotate(root);
        if (balance < -1 && balanceFactor(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        return root;
    }

    void inorder(AVLNode* root, std::vector<int>& result) {
        if (root) {
            inorder(root->left, result);
            result.push_back(root->key);
            inorder(root->right, result);
        }
    }

    bool search(AVLNode* root, int key) {
        if (!root) return false;
        if (root->key == key) return true;
        if (key < root->key) return search(root->left, key);
        return search(root->right, key);
    }

public:
    AVLTree(): root(nullptr) {}

    void insert(int key) { root = insert(root, key); }
    void remove(int key) { root = deleteNode(root, key); }
    bool search(int key) { return search(root, key); }
    std::vector<int> getInorder() {
        std::vector<int> result;
        inorder(root, result);
        return result;
    }
};

/*int main() {
    AVLTree avl;
    avl.insert(10);
    avl.insert(20);
    avl.insert(30);
    avl.insert(40);
    avl.insert(50);
    avl.insert(25);

    std::vector<int> inorder = avl.getInorder();
    std::cout << "Inorder traversal: ";
    for (int val : inorder) std::cout << val << " ";
    std::cout << std::endl;

    avl.remove(30);
    inorder = avl.getInorder();
    std::cout << "After removing 30: ";
    for (int val : inorder) std::cout << val << " ";
    std::cout << std::endl;

    std::cout << "Is 25 in the tree? " << (avl.search(25) ? "Yes" : "No") << std::endl;
    std::cout << "Is 30 in the tree? " << (avl.search(30) ? "Yes" : "No") << std::endl;

    return 0;
}*/
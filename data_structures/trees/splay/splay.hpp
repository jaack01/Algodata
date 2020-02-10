#pragma once

#include <algorithm>
#include <functional>
#include <optional>
#include <queue>

namespace alda {
template <typename T> class SplayTree {
public:
  using Callback = std::function<void(T &)>;

private:
  class SplayTreeNode {
    friend class SplayTree;

  private:
    SplayTreeNode *left{nullptr};
    SplayTreeNode *right{nullptr};

  public:
    T key;

  public:
    SplayTreeNode() = default;

    SplayTreeNode(const T &key) : key(key) {}

    SplayTreeNode(const SplayTreeNode &) = delete;
    SplayTreeNode(SplayTreeNode &&) = delete;
    SplayTreeNode &operator=(const SplayTreeNode &) = delete;
    SplayTreeNode &operator=(SplayTreeNode &&) = delete;
    ~SplayTreeNode() = default;
  };

private:
  SplayTreeNode *mRoot{nullptr};

private:
  void pre_order_traversal(SplayTreeNode *root, Callback callback) {
    if (!root)
      return;
    callback(root->key);
    pre_order_traversal(root->left, callback);
    pre_order_traversal(root->right, callback);
  }

  void in_order_traversal(SplayTreeNode *root, Callback callback) {
    if (!root)
      return;
    in_order_traversal(root->left, callback);
    callback(root->key);
    in_order_traversal(root->right, callback);
  }

  void post_order_traversal(SplayTreeNode *root, Callback callback) {
    if (!root)
      return;
    post_order_traversal(root->left, callback);
    post_order_traversal(root->right, callback);
    callback(root->key);
  }

  void breadth_first_traversal(SplayTreeNode *root, Callback callback) {
    if (!root)
      return;
    std::queue<SplayTreeNode *> queue;
    queue.push(root);
    while (!queue.empty()) {
      SplayTreeNode *current{queue.front()};
      callback(current->key);
      queue.pop();
      if (current->left)
        queue.push(current->left);
      if (current->right)
        queue.push(current->right);
    }
  }

private:
  SplayTreeNode *minimum(SplayTreeNode *root) {
    if (!root)
      return nullptr;
    while (root->left)
      root = root->left;
    return root;
  }

  SplayTreeNode *maximum(SplayTreeNode *root) {
    if (!root)
      return nullptr;
    while (root->right)
      root = root->right;
    return root;
  }

private:
  auto height(const SplayTreeNode *root) {
    if (!root) return 0;
    return std::max(height(root->left), height(root->right)) + 1;
  }

  auto size(const SplayTreeNode *root) {
    if (!root)
      return 0;
    return size(root->left) + size(root->right) + 1;
  }

private:
  SplayTreeNode *insert(SplayTreeNode *root, const T &key) {
    if (!root)
      return new SplayTreeNode(key);
    if (key < root->key)
      root->left = insert(root->left, key);
    else if (root->key < key)
      root->right = insert(root->right, key);

    root->height = std::max(height(root->left), height(root->right)) + 1;

    if (balance(root) > 1) {
      if (key < root->left->key) {
        return rotate_right(root);
      }
      if (root->left->key < key) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
      }
    } else if (balance(root) < -1) {
      if (root->right->key < key) {
        return rotate_left(root);
      }
      if (key < root->right->key) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
      }
    }

    return root;
  }

  template <typename U> SplayTreeNode *remove(SplayTreeNode *root, const U &key) {
    if (!root)
      return nullptr;
    if (key < root->key)
      root->left = remove(root->left, key);
    else if (root->key < key)
      root->right = remove(root->right, key);
    else {
      if (!root->left && !root->right) {
        delete root;
        root = nullptr;
      } else if (!root->left) {
        SplayTreeNode *tmp{root};
        root = root->right;
        delete tmp;
        tmp = nullptr;
      } else if (!root->right) {
        SplayTreeNode *tmp{root};
        root = root->left;
        delete tmp;
        tmp = nullptr;
      } else {
        SplayTreeNode *min{minimum(root->right)};
        root->key = min->key;
        root->right = remove(root->right, min->key);
        // SplayTreeNode * max{ maximum(root->left) };
        // root->key = max->key;
        // root->left = remove(root->left, max->key);
      }
    }

    if (!root)
      return nullptr;

    root->height = std::max(height(root->left), height(root->right)) + 1;

    if (balance(root) > 1) {
      if (balance(root->left) >= 0) {
        return rotate_right(root);
      }
      root->left = rotate_left(root->left);
      return rotate_right(root);
    }
    if (balance(root) < -1) {
      if (balance(root->right) <= 0) {
        return rotate_left(root);
      }
      root->right = rotate_right(root->right);
      return rotate_left(root);
    }

    return root;
  }

  template <typename U> SplayTreeNode *search(SplayTreeNode *root, const U &key) {
    while (root) {
      if (root->key < key) {
        root = root->right;
      } else if (key < root->key) {
        root = root->left;
      } else {
        return root;
      }
    }
    return nullptr;
  }

private:
  void clear(SplayTreeNode *root) {
    if (!root)
      return;
    if (root->left)
      clear(root->left);
    if (root->right)
      clear(root->right);
    delete root;
    root = nullptr;
  }

public:
  SplayTree() = default;

  SplayTree(const SplayTree &) = delete;
  SplayTree(SplayTree &&) = delete;
  SplayTree &operator=(const SplayTree &) = delete;
  SplayTree &operator=(SplayTree &&) = delete;

  ~SplayTree() { clear(); }

public:
  void pre_order_traversal(Callback callback) {
    pre_order_traversal(mRoot, callback);
  }

  void in_order_traversal(Callback callback) {
    in_order_traversal(mRoot, callback);
  }

  void post_order_traversal(Callback callback) {
    post_order_traversal(mRoot, callback);
  }

  void breadth_first_traversal(Callback callback) {
    breadth_first_traversal(mRoot, callback);
  }

public:
  auto minimum() {
    auto min = minimum(mRoot);
    return min ? std::optional<std::reference_wrapper<T>>{min->key}
               : std::nullopt;
  }

  auto maximum() {
    auto max = maximum(mRoot);
    return max ? std::optional<std::reference_wrapper<T>>{max->key}
               : std::nullopt;
  }

public:
  auto height() { return height(mRoot); }

  auto size() { return size(mRoot); }

public:
  void insert(const T &key) { mRoot = insert(mRoot, key); }

  template <typename U> void remove(const U &key) {
    mRoot = remove(mRoot, key);
  }

  template <typename U> auto search(const U &key) {
    auto res = search(mRoot, key);
    return res ? std::optional<std::reference_wrapper<T>>{res->key}
               : std::nullopt;
  }

public:
  void clear() {
    clear(mRoot);
    mRoot = nullptr;
  }
};
} // namespace forest
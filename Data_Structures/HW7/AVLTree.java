/**
 * The AVLTree class implements an AVL tree which is a self-balancing binary search tree.
 * It supports insertion, deletion, and searching of Stock objects, as well as tree traversals.
 */
public class AVLTree {

  /**
   * The Node class represents a node in the AVL tree.
   */
  private class Node {

    Stock stock;
    Node left, right;
    int height;

    /**
     * Constructor to create a new node with the given stock.
     *
     * @param stock the stock to be stored in the node
     */
    Node(Stock stock) {
      this.stock = stock;
      this.height = 1;
    }
  }

  private Node root;

  /**
   * Inserts a stock into the AVL tree.
   *
   * @param stock the stock to be inserted
   */
  public void insert(Stock stock) {
    root = insert(root, stock);
  }

  /**
   * Helper method to insert a stock into the subtree rooted at the given node.
   *
   * @param node the root of the subtree
   * @param stock the stock to be inserted
   * @return the new root of the subtree
   */
  private Node insert(Node node, Stock stock) {
    if (node == null) {
      return new Node(stock);
    }

    if (stock.getSymbol().compareTo(node.stock.getSymbol()) < 0) {
      node.left = insert(node.left, stock);
    } else if (stock.getSymbol().compareTo(node.stock.getSymbol()) > 0) {
      node.right = insert(node.right, stock);
    } else {
      // Duplicate stock symbol, update the stock information
      node.stock = stock;
      return node;
    }

    node.height = 1 + Math.max(height(node.left), height(node.right));
    return balance(node);
  }

  /**
   * Deletes a stock from the AVL tree based on the stock symbol.
   *
   * @param symbol the symbol of the stock to be deleted
   */
  public void delete(String symbol) {
    root = delete(root, symbol);
  }

  /**
   * Helper method to delete a stock from the subtree rooted at the given node.
   *
   * @param node the root of the subtree
   * @param symbol the symbol of the stock to be deleted
   * @return the new root of the subtree
   */
  private Node delete(Node node, String symbol) {
    if (node == null) {
      return node;
    }

    if (symbol.compareTo(node.stock.getSymbol()) < 0) {
      node.left = delete(node.left, symbol);
    } else if (symbol.compareTo(node.stock.getSymbol()) > 0) {
      node.right = delete(node.right, symbol);
    } else {
      if (node.left == null || node.right == null) {
        Node temp = null;
        if (temp == node.left) {
          temp = node.right;
        } else {
          temp = node.left;
        }

        if (temp == null) {
          temp = node;
          node = null;
        } else {
          node = temp;
        }
      } else {
        Node temp = minValueNode(node.right);
        node.stock = temp.stock;
        node.right = delete(node.right, temp.stock.getSymbol());
      }
    }

    if (node == null) {
      return node;
    }

    node.height = Math.max(height(node.left), height(node.right)) + 1;
    return balance(node);
  }

  /**
   * Finds the node with the minimum value in the subtree rooted at the given node.
   *
   * @param node the root of the subtree
   * @return the node with the minimum value
   */
  private Node minValueNode(Node node) {
    Node current = node;
    while (current.left != null) {
      current = current.left;
    }
    return current;
  }

  /**
   * Searches for a stock in the AVL tree based on the stock symbol.
   *
   * @param symbol the symbol of the stock to be searched
   * @return the stock if found, null otherwise
   */
  public Stock search(String symbol) {
    Node result = search(root, symbol);
    return (result != null) ? result.stock : null;
  }

  /**
   * Helper method to search for a stock in the subtree rooted at the given node.
   *
   * @param node the root of the subtree
   * @param symbol the symbol of the stock to be searched
   * @return the node containing the stock, or null if not found
   */
  private Node search(Node node, String symbol) {
    if (node == null || node.stock.getSymbol().equals(symbol)) {
      return node;
    }

    if (symbol.compareTo(node.stock.getSymbol()) < 0) {
      return search(node.left, symbol);
    }

    return search(node.right, symbol);
  }

  /**
   * Balances the given node to maintain the AVL tree property.
   *
   * @param node the node to be balanced
   * @return the new root of the subtree
   */
  private Node balance(Node node) {
    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1) {
      if (getBalanceFactor(node.left) >= 0) {
        return rightRotate(node);
      } else {
        node.left = leftRotate(node.left);
        return rightRotate(node);
      }
    }

    if (balanceFactor < -1) {
      if (getBalanceFactor(node.right) <= 0) {
        return leftRotate(node);
      } else {
        node.right = rightRotate(node.right);
        return leftRotate(node);
      }
    }

    return node;
  }

  /**
   * Performs a left rotation on the given node.
   *
   * @param x the node to be rotated
   * @return the new root of the subtree
   */
  private Node leftRotate(Node x) {
    Node y = x.right;
    Node T2 = y.left;

    y.left = x;
    x.right = T2;

    x.height = Math.max(height(x.left), height(x.right)) + 1;
    y.height = Math.max(height(y.left), height(y.right)) + 1;

    return y;
  }

  /**
   * Performs a right rotation on the given node.
   *
   * @param y the node to be rotated
   * @return the new root of the subtree
   */
  private Node rightRotate(Node y) {
    Node x = y.left;
    Node T2 = x.right;

    x.right = y;
    y.left = T2;

    y.height = Math.max(height(y.left), height(y.right)) + 1;
    x.height = Math.max(height(x.left), height(x.right)) + 1;

    return x;
  }

  /**
   * Returns the height of the given node.
   *
   * @param node the node whose height is to be returned
   * @return the height of the node
   */
  private int height(Node node) {
    if (node == null) {
      return 0;
    }
    return node.height;
  }

  /**
   * Returns the balance factor of the given node.
   *
   * @param node the node whose balance factor is to be returned
   * @return the balance factor of the node
   */
  private int getBalanceFactor(Node node) {
    if (node == null) {
      return 0;
    }
    return height(node.left) - height(node.right);
  }

  /**
   * Performs an in-order traversal of the AVL tree and prints the stocks.
   */
  public void inOrderTraversal() {
    inOrderTraversal(root);
  }

  /**
   * Helper method to perform an in-order traversal of the subtree rooted at the given node.
   *
   * @param node the root of the subtree
   */
  private void inOrderTraversal(Node node) {
    if (node != null) {
      inOrderTraversal(node.left);
      System.out.println(node.stock);
      inOrderTraversal(node.right);
    }
  }

  /**
   * Performs a pre-order traversal of the AVL tree and prints the stocks.
   */
  public void preOrderTraversal() {
    preOrderTraversal(root);
  }

  /**
   * Helper method to perform a pre-order traversal of the subtree rooted at the given node.
   *
   * @param node the root of the subtree
   */
  private void preOrderTraversal(Node node) {
    if (node != null) {
      System.out.println(node.stock);
      preOrderTraversal(node.left);
      preOrderTraversal(node.right);
    }
  }

  /**
   * Performs a post-order traversal of the AVL tree and prints the stocks.
   */
  public void postOrderTraversal() {
    postOrderTraversal(root);
  }

  /**
   * Helper method to perform a post-order traversal of the subtree rooted at the given node.
   *
   * @param node the root of the subtree
   */
  private void postOrderTraversal(Node node) {
    if (node != null) {
      postOrderTraversal(node.left);
      postOrderTraversal(node.right);
      System.out.println(node.stock);
    }
  }
}

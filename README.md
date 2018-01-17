# Search Engine

A simple search engine that build a BST for words that scan from the input text files and get the query from the user, parse it and error check it and then evaluate the query through that BST.

## How to run

This project is using cmake for its building process management. This project can be easily cloned:
~~~
git clone https://github.com/anvari1313/search-engine.git
~~~

And then run it using a cmake with respect to your Operating System.

## More about this project

### Technical tips

#### What is BST?

BST or binary search tree is  a node-based binary tree data structure which has the following properties:
* The left subtree of a node contains only nodes with keys less than the node’s key.
* The right subtree of a node contains only nodes with keys greater than the node’s key.
* The left and right subtree each must also be a binary search tree.
* There must be no duplicate nodes.
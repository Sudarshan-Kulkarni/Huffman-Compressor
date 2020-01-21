#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <fstream>
#include <map>
#include<queue>
#include<string>
#include<algorithm>
#include<direct.h>
using namespace std;

class Huffman
{
    private:
    //creating a class to represent each node in the huffman tree.
        class Node
        {
            char id;
            int freq;
            Node* leftChild;
            Node* rightChild;
            friend class Huffman;
            
            Node()
            {
                leftChild=NULL;
                rightChild=NULL;                
            }

            Node(char c, int val)
            {
                id = c;
                freq = val;
                leftChild=NULL;
                rightChild=NULL;
            }  
        };
        class compare
        {           
	        public:
                bool operator()(const Node* c1, const Node* c2) const
                {
                    return c1->freq > c2->freq;
                }
        };

        // all data structures we'll use during compression and decompression
        fstream file_in, file_out;
        string fileName, fileContents;
        map<char,int>frequencyTable;
        priority_queue<Node*, vector<Node*>, compare>pq;
        Node* root=NULL;
        map<char, string> codeMap;

    public:

        // Main functions
        Node* constructHuffmanTree();
        void compressFile(string);
        void decompressFile(string);
        void memFlush();
        void traverseFlush(Node *);

        // Compression helper functions
        string readFileContents(string);
        map<char,int> constructFrequencyTable(string);
        void constructPriorityQueue();
        void traverseTree(Node*, string);
        void writeToFile();
        void writefileHeader();
        void writeCompressedFileContents();

        // Decompression helper funtions
        map<char, int> reconstructFrequencyTable(string);
        string decompressUtil();
        string translateHuffman(vector<bool>);

};

#endif
#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <sstream>

struct Node {
    char data;
    int freq;
    Node *left, *right;
    
    Node(char d, int f) : data(d), freq(f), left(nullptr), right(nullptr) {}
};

struct CompareNode {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

class Huffman {
private:
    std::unordered_map<char, std::string> codes;
    std::unordered_map<char, int> freqMap;
    Node* root;

    void buildFrequencyMap(const std::string& text) {
        for (char c : text) {
            freqMap[c]++;
        }
    }

    void buildHuffmanTree() {
        std::priority_queue<Node*, std::vector<Node*>, CompareNode> pq;
        
        for (auto& pair : freqMap) {
            pq.push(new Node(pair.first, pair.second));
        }
        
        while (pq.size() > 1) {
            Node* left = pq.top(); pq.pop();
            Node* right = pq.top(); pq.pop();
            
            Node* parent = new Node('\0', left->freq + right->freq);
            parent->left = left;
            parent->right = right;
            pq.push(parent);
        }
        
        root = pq.top();
    }

    void generateCodes(Node* node, std::string code) {
        if (node == nullptr) return;
        
        if (node->data != '\0') {
            codes[node->data] = code;
        }
        
        generateCodes(node->left, code + "0");
        generateCodes(node->right, code + "1");
    }

    std::string encode(const std::string& text) {
        std::string encoded;
        for (char c : text) {
            encoded += codes[c];
        }
        return encoded;
    }

    void saveEncodedFile(const std::string& encoded, const std::string& outputFile) {
        std::ofstream outFile(outputFile, std::ios::binary);
        
        // Save frequency map
        uint16_t mapSize = freqMap.size();
        outFile.write(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
        for (const auto& pair : freqMap) {
            outFile.write(&pair.first, sizeof(pair.first));
            outFile.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
        }

        // Save encoded data
        uint8_t paddingLength = 8 - encoded.length() % 8;
        outFile.write(reinterpret_cast<char*>(&paddingLength), sizeof(paddingLength));
        std::string paddedEncoded = encoded + std::string(paddingLength, '0');
        
        for (size_t i = 0; i < paddedEncoded.length(); i += 8) {
            char byte = std::bitset<8>(paddedEncoded.substr(i, 8)).to_ulong();
            outFile.put(byte);
        }

        outFile.close();
    }

    void readEncodedFile(const std::string& inputFile, std::string& encoded) {
        std::ifstream inFile(inputFile, std::ios::binary);
        
        // Read frequency map
        uint16_t mapSize;
        inFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
        for (uint16_t i = 0; i < mapSize; ++i) {
            char c;
            int freq;
            inFile.read(&c, sizeof(c));
            inFile.read(reinterpret_cast<char*>(&freq), sizeof(freq));
            freqMap[c] = freq;
        }

        // Read padding info
        uint8_t paddingLength;
        inFile.read(reinterpret_cast<char*>(&paddingLength), sizeof(paddingLength));

        // Read encoded data
        std::stringstream ss;
        char byte;
        while (inFile.get(byte)) {
            ss << std::bitset<8>(byte);
        }
        encoded = ss.str();
        encoded = encoded.substr(0, encoded.length() - paddingLength);

        inFile.close();
    }

    std::string decode(const std::string& encoded) {
        std::string decoded;
        Node* current = root;
        for (char bit : encoded) {
            if (bit == '0') {
                current = current->left;
            } else {
                current = current->right;
            }
            
            if (current->left == nullptr && current->right == nullptr) {
                decoded += current->data;
                current = root;
            }
        }
        return decoded;
    }

public:
    void compress(const std::string& inputFile, const std::string& outputFile) {
        std::ifstream inFile(inputFile, std::ios::binary);
        if (!inFile) {
            std::cerr << "Error opening input file." << std::endl;
            return;
        }
        std::string text((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        inFile.close();

        buildFrequencyMap(text);
        buildHuffmanTree();
        generateCodes(root, "");

        std::string encoded = encode(text);
        saveEncodedFile(encoded, outputFile);

        std::cout << "File compressed successfully." << std::endl;
    }

    void decompress(const std::string& inputFile, const std::string& outputFile) {
        std::string encoded;
        readEncodedFile(inputFile, encoded);

        buildHuffmanTree();
        std::string decoded = decode(encoded);

        std::ofstream outFile(outputFile, std::ios::binary);
        if (!outFile) {
            std::cerr << "Error opening output file." << std::endl;
            return;
        }
        outFile.write(decoded.c_str(), decoded.length());
        outFile.close();

        std::cout << "File decompressed successfully." << std::endl;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " [compress/decompress] [input_file] [output_file]" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];

    Huffman huffman;

    if (mode == "compress") {
        huffman.compress(inputFile, outputFile);
    } else if (mode == "decompress") {
        huffman.decompress(inputFile, outputFile);
    } else {
        std::cout << "Invalid mode. Use 'compress' or 'decompress'." << std::endl;
        return 1;
    }

    return 0;
}
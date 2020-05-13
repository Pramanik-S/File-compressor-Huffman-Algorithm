/*File Compressor Using Huffman Algorithm
	Author:: SUMAN PRAMANIK
	github:: @Pramanik-S*/
#include<bits/stdc++.h>
using namespace std;
//Global Declearation
unordered_map<unsigned char, int> frequencies;
unordered_map<unsigned char, string> codeBook;
//node of huffman tree
class node {
public:
    unsigned char data;
    unsigned int frequency;
    unsigned char minimum;
    node *left, *right;
    node() {
        this->left = this->right = NULL;
    }
    node(const node &t) {
        this->data = t.data;
        this->frequency = t.frequency;
        this->minimum = t.minimum;
        this->left = t.left;
        this->right = t.right;
    }
    node(unsigned char data, unsigned int frequency) {
        this->data = this->minimum = data;
        this->frequency = frequency;
        this->left = this->right = NULL;
    }
    node(node *n1, node *n2) {
        this->frequency = n1->frequency + n2->frequency;
        this->right = n1;
        this->left = n2;
        this->minimum = min(n1->minimum, n2->minimum);
    }
};
//Comparator for priority_queue
struct compare {
	bool operator()(const node *n1, const node *n2) const {
		if(n1->frequency > n2->frequency) return true;
		if(n1->frequency == n2->frequency and n1->minimum > n2->minimum) return true;
		return false;
	}
};
//Creating huffman tree using priority_queue
node* createTree() {
	priority_queue<node*, vector<node*>, compare> pq;
	for(int i = 0; i < 256; i++) if(frequencies[i]) pq.push(new node(i, frequencies[i]));
	while(pq.size() > 1) {
		node *n1 = pq.top(); pq.pop();
		node *n2 = pq.top(); pq.pop();
		pq.push(new node(n1, n2));
	}
	return pq.top();
}
//Creating huffman code for every character
void createCodeBook(node *root, string &code) {
	if(!root->left and !root->right) {
		codeBook[root->data] = code;
		return;
	}
	if(root->left) {
		code += '0';
		createCodeBook(root->left, code);
		code.erase(code.end()-1);
	}
	if(root->right) {
		code += '1';
		createCodeBook(root->right, code);
		code.erase(code.end()-1);
	}
}
//Creating compressed file
void createCompressedFile() {
    for(unsigned int i = 0; i < 256; i++){
        cout << (char)(0x000000ff & frequencies[i]);
        cout << (char)((0x0000ff00 & frequencies[i]) >> 8);
        cout << (char)((0x00ff0000 & frequencies[i]) >> 16);
        cout << (char)((0xff000000 & frequencies[i]) >> 24);
    }
    unsigned char next;
    char nextByte = 0;
    int bitCount = 0;
    cin.clear();
    cin.seekg(0);
    cin >> noskipws;
    while(cin >> next) {
    	for(unsigned int i = 0; i < codeBook[next].size(); i++, bitCount++) {
            if(bitCount == 8) {
                cout << nextByte;
                nextByte = 0;
                bitCount = 0;
            }
            if(codeBook[next][i] == '1')
                nextByte = nextByte | (0x01 << bitCount);
        }
    }
    if(bitCount)
        cout << nextByte;
}
void compress() {
	unsigned char next;
	cin >> noskipws;
	while(cin >> next) frequencies[next]++;
	node *root = createTree();
	string code;
	createCodeBook(root, code);
	createCompressedFile();
}
void decompress() {
	cin >> noskipws;
    char nextByte;
    for(int i = 0; i < 256; i++)
        cin.read((char *)&frequencies[i],4);
    node *root = createTree();
    string code;
    createCodeBook(root, code);
    while(cin >> nextByte) for(int i = 0; i < 8; i++) {
        if((nextByte >> i) & 0x01) code += '1';
        else code += '0';
        for(int i = 0; i < 256; i++) if(codeBook[i] == code) {
            if(frequencies[i]) {
                printf("%c", (unsigned char)i);
                code.clear();
                frequencies[i]--;
                break;
            }
            else return;
        } 
    }
}
int main(int argc, char const *argv[]) {
	if(argc == 2 and argv[1][0] == '-' and argv[1][1] == 'd') decompress();
	else compress();
	return 0;
}

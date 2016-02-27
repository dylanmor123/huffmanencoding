
#include "bit_io.hpp"
#include "common.hpp"
#include <iostream>

using namespace eecs214;
using namespace std;
unsigned int frequency_table_clone[256];
int original_file_size = 0;
node* forest_clone[256];
node* my_huff_clone;
node* make_tree(int i, int our_frequency_table);
node* remove_smallest_tree(int size, node*f[]);
string walk_tree(char c, node* huffman_tree);
void decode_frequency_table(bifstream& in);
void decode(bifstream& in, ofstream& out, node* huffman_tree, node* huffman_tree2);

int main(int argc, const char **argv)
{
	const char *in_file, *out_file;
	if (argc != 3) {
		in_file = "C:\\Users\\Dylan\\Documents\\Visual Studio 2013\\Projects\\huff.cpp\\huff.cpp\\theoutput.txt";

		out_file = "newoutput.txt";
	}
	else {
		in_file = argv[1];
		out_file = argv[2];
	}


	bifstream in(in_file);
	assert_good(in, argv);

	ofstream out(out_file, ios::binary);
	assert_good(out, argv);
	in.read_bits(original_file_size, 32);
	decode_frequency_table(in);
	int kk = 0;
	for (int i = 0; i < 256; i++) {
		if (frequency_table_clone[i] != 0) {
			forest_clone[kk] = make_tree(i, frequency_table_clone[i]);
			cout << forest_clone[kk]->byte << "     " << forest_clone[kk]->weight << "\n";
			kk++;
		}
	}
	int q = 0;
	while (forest_clone[0] != nullptr) {
		node* a = remove_smallest_tree(kk, forest_clone);
		node* b = remove_smallest_tree(kk, forest_clone);
		if (a != nullptr && b != nullptr) {
			node* c = new node(a, b);
			a->parent = c;
			b->parent = c;
			while (q < kk) {
				if (forest_clone[q] == nullptr) {
					forest_clone[q] = c;
					q = 0;
					break;
				}
				else {
					q++;
				}
			}
		}
		else {
			my_huff_clone = a;
			break;
		}
	}
	decode(in, out, my_huff_clone, my_huff_clone);
	return 0;
}

void decode_frequency_table(bifstream& in)
{
	char c;

	for (int ii = 0; ii < 256; ii++){
		(in.read_bits(frequency_table_clone[ii], 32));
	}
}

node* make_tree(int k, int our_frequency_table) {
	node *a_node = new node(our_frequency_table, k);
	return a_node;
}

node* remove_smallest_tree(int size, node*f[]) {
	int jj = 0;
	int m = 0;
	while (f[jj] == nullptr && m < size) {
		jj++;
		m++;
	}
	if (m == size) {
		return nullptr;
	}
	else {
		int a = f[jj]->weight;
		while (jj < size) {
			if (f[jj] != nullptr && a > f[jj]->weight) {
				m = jj;
				a = f[jj]->weight;
				jj++;
			}
			else {
				jj++;
			}
		}
		node* result = f[m];
		{
			f[m] = nullptr;
		}
		return result;
	}
}



void decode(bifstream& in, ofstream& out, node* huffman_tree, node* huffman_tree2) {
	int ii = 0;
	bool b;
	in.read(b);
	while (ii < original_file_size) {
		while (!(huffman_tree->left_child == nullptr && huffman_tree->right_child == nullptr)) {
			if (b) {
				huffman_tree = huffman_tree->right_child;
			}
			else if (!b) {
				huffman_tree = huffman_tree->left_child;
			}
			in.read(b);
		}
		out.write((const char*)&huffman_tree->byte, 1);
		huffman_tree = huffman_tree2;
		ii++;
	}
}
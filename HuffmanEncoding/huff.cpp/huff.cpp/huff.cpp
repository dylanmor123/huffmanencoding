#include "bit_io.hpp"
#include "common.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace eecs214;
using namespace std;

unsigned int frequency_table[256];
node* forest[256];
node* my_huff;
string serial_table[256];
void encode(istream& in, bofstream& out);
node* make_tree(int i, int our_frequency_table);
node* remove_smallest_tree(int size, node*f[]);
string walk_tree(char c, node* huffman_tree);
void buildCwt(node* huffy, string currentCodeword, string serial_table[]);
void print_frequency_table(unsigned int ft[], bofstream& out);
void real_encode(istream& in, bofstream& out);

int main(int argc, const char **argv)
{
	const char *in_file, *out_file;
	if (argc != 3) {
		in_file = "hamlet.txt";
		out_file = "theoutput.txt";
	}
	else {
		in_file = argv[1];
		out_file = argv[2];
	}

	ifstream in(in_file, ios::binary);
	assert_good(in, argv);

	bofstream out(out_file);
	assert_good(out, argv);
	encode(in, out);
	int kk = 0;
	for (int i = 0; i < 256; i++) {
		if (frequency_table[i] != 0) {
			forest[kk] = make_tree(i, frequency_table[i]);
			cout << forest[kk]->byte << "     " << forest[kk]->weight << "\n";
			kk++;
		}
	}
	int q = 0;
	while (forest[0] != nullptr) {
		node* a = remove_smallest_tree(kk, forest);
		node* b = remove_smallest_tree(kk, forest);
		if (a != nullptr && b != nullptr) {
			node* c = new node(a, b);
			a->parent = c;
			b->parent = c;
			while (q < kk) {
				if (forest[q] == nullptr) {
					forest[q] = c;
					q = 0;
					break;
				}
				else {
					q++;
				}
			}
		}
		else {
			my_huff = a;
			break;
		}
	}
	buildCwt(my_huff, "", serial_table);
	in.clear();
	in.seekg(0, ios::beg);
	if (my_huff != nullptr) {
		out.write_bits(my_huff->weight, 32);
		print_frequency_table(frequency_table, out);
		real_encode(in, out);
	}
	return 0;
}

void encode(istream& in, bofstream& out)
{
	char c;
	unsigned char z;
	while (in.read(&c, 1)) {
		z = c; 
		frequency_table[z]++;
	}
}

void real_encode(istream& in, bofstream& out)
{
	char d;
	unsigned char y;
	int ii = 0;
	int file_size = 0;
	while (in.read(&d, 1)) {
		y = d;
		string word = serial_table[y];
		while (ii < serial_table[y].length()) {
			char bit = word[ii];
			out.write(bit == '1');
			ii++;
		}
		ii = 0;
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

void buildCwt(node* huffy, string currentCodeword, string serial_table[]){
	if (huffy == nullptr){}
	else {
		if ((huffy->left_child == nullptr) && (huffy->right_child == nullptr)) {
			(serial_table[huffy->byte]) = currentCodeword;
		}

		else {
			buildCwt(huffy->left_child, currentCodeword + '0', serial_table);
			buildCwt(huffy->right_child, currentCodeword + '1', serial_table);
		}
	}
}

void print_frequency_table(unsigned int ft[], bofstream& out) {
	for (int i = 0; i < 256; i++) {
		out.write_bits(ft[i], 32);
	}
}


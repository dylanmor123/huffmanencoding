#pragma once
template <class Stream>
void assert_good(const Stream& stream, const char** argv)
{
	if (!stream.good()) {
		perror(argv[0]);
		exit(1);
	}
}

inline void usage(const char** argv)
{
	fprintf(stderr, "Usage: %s INFILE OUTFILE\n", argv[0]);
	exit(1);
}


class node
{
public:
	node* left_child;
	node* right_child;
	node* parent;
	int weight;
	unsigned char byte;
	std::string prefix_code;

public:
	node();
	node(node* x, node* y);
	node(int w, int b);
	node(char b, std::string code);
	bool on_left(node huff, node* parent);
	bool on_right(node huff, node* parent);
};


node::node(int w, int b)
{
	weight = w;
	byte = b;
	left_child = nullptr;
	right_child = nullptr;
	parent = nullptr;
	prefix_code = "";
}

node::node()
{
	weight = 0;
	byte = 0;
	left_child = nullptr;
	right_child = nullptr;
	parent = nullptr;
	prefix_code = "";
}

node::node(node* x, node* y) {
	weight = x->weight + y->weight;
	byte = '\0';
	left_child = x;
	right_child = y;
	parent = nullptr;
	prefix_code = "";
}

node::node(char b, std::string code)
{
	weight = 0;
	byte = b;
	left_child = nullptr;
	right_child = nullptr;
	parent = nullptr;
	prefix_code = code;
}

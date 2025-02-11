#include <iostream>
#include <vector>
#include <array> 
#include <algorithm>
#include <fstream>
#include <string>

struct message
{
	float probability;
	char symbol;
	std::string code;
};

struct node
{
	float probability;
	std::vector<message*> messages;
};

float calculate_entropy(std::vector<message> alphabet)
{
	float entropy = 0;
	for (int i = 0, lim = alphabet.size(); i < lim; i++)
	{
		entropy += alphabet[i].probability * log2(1 / alphabet[i].probability);
	}
	return entropy;
}

float calculate_average_code_length(std::vector<message> alphabet)
{
	float average_code_length = 0;
	for (int i = 0, lim = alphabet.size(); i < lim; i++)
	{
		average_code_length += alphabet[i].probability * alphabet[i].code.length();
	}
	return average_code_length;
}

int main()
{
    std::cout << "Huffman Coding\n--------------\n";

    std::vector<message> alphabet{}; 

	std::ifstream file("input.txt");
	std::string line;
	while (std::getline(file, line))
	{
		std::string symbol = line.substr(0, 1);
		std::string probability = line.substr(2, line.length() - 2);
		alphabet.push_back(message{
			std::stof(probability),
			symbol[0],
			""
			});

		
	}
	file.close();
   
	std::vector<node*> nodes;
	for (int i = 0,lim = alphabet.size(); i < lim; i++)
	{
		node* n = new node();
		n->probability = alphabet[i].probability;
		n->messages.push_back(&alphabet[i]);
		nodes.push_back(n);
	}

    while (nodes.size() != 1)
    {
		std::sort(nodes.begin(), nodes.end(), [](node* a, node* b) {return a->probability < b->probability; });

        for (message* ptr : nodes[0]->messages)
        {
            ptr->code = "1" + ptr->code;
        }
        for (message* ptr : nodes[1]->messages)
        {
            ptr->code = "0" + ptr->code;
        }

		node* n = new node();
		n->probability = nodes[0]->probability + nodes[1]->probability;
		n->messages.insert(n->messages.end(), nodes[0]->messages.begin(), nodes[0]->messages.end());
		n->messages.insert(n->messages.end(), nodes[1]->messages.begin(), nodes[1]->messages.end());
       
		nodes.erase(nodes.begin(), nodes.begin() + 2);
		nodes.push_back(n);


    }

	
    
	std::cout << "Symbol\tProbability\tCode\n\n";
    for (int i = 0,lim = alphabet.size(); i < lim ; i++)
    {
        std::cout << alphabet[i].symbol << "\t" << alphabet[i].probability << "\t" << alphabet[i].code << std::endl;
    }

	std::cout << "Entropy: " << calculate_entropy(alphabet) << std::endl;
	std::cout << "Average Code Length: " << calculate_average_code_length(alphabet) << std::endl;
}

#include <vector>
#include <fstream>
#include "Gate.h"
#include "Input.h"
#include "Output.h"    // adding necessary libraries
#include "And.h"
#include "Or.h"
#include "Not.h"
#include "FlipFlop.h"
#include "Decoder.h"
using namespace std;

void readCircuitFile(vector<Gate *> &gates);
void runProgram(vector<Gate *> &gates);

int main()
{
    vector<Gate *> gates;    // declaring a vector for storage the gates
    readCircuitFile(gates);
    runProgram(gates);
}

void readCircuitFile(vector<Gate *> &gates)
{
    ifstream file;
    string word;
    file.open("circuit.txt");
    if (!file)
    {
        cout << "File could not opened !";
        exit(1);
    }
    while (file >> word)
    {
        if (word == "INPUT")
        {
            while (file.peek() != '\n')   // pushing identifiers to gates
            {
                file >> word;
                gates.push_back(new Input(word));
            }
        }
        else if (word == "OUTPUT")
        {
            while (file.peek() != '\n')
            {
                file >> word;
                gates.push_back(new Output(word));
            }
        }
        else if (word == "AND" || word == "OR")
        {
            bool isAnd = false;
            if (word == "AND")
            {
                isAnd = true;
            }
            int i = 0;
            string id;
            Gate *input1, *input2;
            while (file.peek() != '\n')
            {
                file >> word;
                if (i == 0)
                {
                    id = word;
                }
                else
                {
                    for (int j = 0; j < gates.size(); ++j)
                    {
                        if (gates[j]->getId() == word)
                        {
                            if (i == 1)
                            {
                                input1 = gates[j];
                                break;
                            }
                            else
                            {
                                input2 = gates[j];
                                break;
                            }
                        }
                    }
                }
                i++;
            }
            if (isAnd)
            {
                gates.push_back(new And(id, input1, input2));  // pushing "and" variables to gates vector
            }
            else
            {
                gates.push_back(new Or(id, input1, input2));   // pushing "or" variables to gates vector
            }
        }
        else if (word == "NOT" || word == "FLIPFLOP")
        {
            bool isNot = false;
            if (word == "NOT")
            {
                isNot = true;
            }
            Gate *input;
            int i = 0;
            string id;
            while (file.peek() != '\n')
            {
                file >> word;
                if (i == 0)
                {
                    id = word;
                }
                else
                {
                    for (int i = 0; i < gates.size(); ++i)
                    {
                        if (gates[i]->getId() == word)
                        {
                            input = gates[i];
                            break;
                        }
                    }
                }
                i++;
            }
            if (isNot)
            {
                gates.push_back(new Not(id, input));   // pushing "not" variables to gates vector
            }
            else
            {
                gates.push_back(new FlipFlop(id, input));   // pushing "flipflop" variables to gates vector
            }
        }
        else if (word == "DECODER")
        {
            int i = 0;
            string id;
            Gate *input1, *input2;
            vector<Gate *> outputs;
            while (!file.eof())
            {
                file >> word;
                for (int j = 0; j < gates.size(); ++j)
                {
                    if (gates[j]->getId() == word)
                    {
                        if (i < 4)
                        {
                            outputs.push_back(gates[j]);
                            break;
                        }
                        else if (i == 4)
                        {
                            input1 = gates[j];
                            break;
                        }
                        else
                        {
                            input2 = gates[j];
                            break;
                        }
                    }
                }

                i++;
            }
            gates.push_back(new Decoder(input1, input2, outputs[0], outputs[1], outputs[2], outputs[3]));
        }
    }
}
/*
Function that reads the input file then executes the circuit
*/
void runProgram(vector<Gate *> &gates)
{

    ifstream file2;
    file2.open("input.txt");
    if (!file2) // Check if file is opened or not
    {
        cout << "File could not opened !";
        exit(1);
    }
    char ch = ' ';
    int counter = 0;
    // Read input file char by char
    while (file2.get(ch))
    {
        // If the char is new line then print the circuit output results 
        // And reset the necessary components
        if (ch == '\n')
        {
            // Call the calculate method of decoder
            gates[gates.size() - 1]->calculate();
            //
            counter = 0;
            for (int i = 0; i < gates.size(); ++i)
            {
                // In order to reset the required components
                // Dynamic cast and find the flipflops and outputs from the gate vector
                Output *output = dynamic_cast<Output *>(gates[i]);
                FlipFlop *fp = dynamic_cast<FlipFlop *>(gates[i]);
                if (fp != NULL)
                {
                    fp->reset();
                }
                if (output != NULL)
                {
                    // Print the data in the output to the stdout then
                    // Reset the value of the data of output
                    cout << output->getData() << " ";
                    output->setData(0);
                }
            }
            cout << endl;
        }
        // If the char is not empty and not new line so that means the char is 1 or 0
        // Assign that data to the Input components which is in the gates vector
        else if (ch != ' ')
        {
            for (int i = counter; i < gates.size(); ++i)
            {
                // In order to find Input component in the gates 
                // Dynamically cast to the Input
                Input *input = dynamic_cast<Input *>(gates[i]);
                if (input != NULL)
                {
                    if (ch == '1')
                    {
                        input->setData(true);
                        break;
                    }
                    else
                    {
                        input->setData(false);
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            counter++;
        }
    }
    // After loop the char becomes EOF 
    // So do the same statements after loop 
    gates[gates.size() - 1]->calculate();
    counter = 0;
    for (int i = 0; i < gates.size(); ++i)
    {
        Output *output = dynamic_cast<Output *>(gates[i]);
        FlipFlop *fp = dynamic_cast<FlipFlop *>(gates[i]);
        if (fp != NULL)
        {
            fp->reset();
        }
        if (output != NULL)
        {
            cout << output->getData() << " ";
            output->setData(0);
        }
    }
    cout << endl;
}
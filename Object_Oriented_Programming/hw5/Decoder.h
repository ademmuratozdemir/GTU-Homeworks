#ifndef DECODER_H
#define DECODER_H
#include "Gate.h"
class Decoder : public Gate
{
private:
    static int counter;
    Gate *input1, *input2;
    Gate *output1, *output2, *output3, *output4;

public:
    Decoder(Gate *input1, Gate *input2, Gate *output1, Gate *output2, Gate *output3, Gate *output4);
    virtual bool calculate();
};

Decoder::Decoder(Gate *input1, Gate *input2, Gate *output1, Gate *output2, Gate *output3, Gate *output4)
    : Gate()
{
    this->input1 = input1;
    this->input2 = input2;
    this->output1 = output1;
    this->output2 = output2;
    this->output3 = output3;
    this->output4 = output4;
}
// implementation of "decoder"
bool Decoder::calculate()
{
    bool val1 = input1->calculate();
    bool val2 = input2->calculate();

    if (val1 == 0 && val2 == 0)
    {
        output1->setData(1);
    }
    else if (val1 == 0 && val2 == 1)
    {
        output2->setData(1);
    }
    else if (val1 == 1 && val2 == 0)
    {
        output3->setData(1);
    }
    else
    {
        output4->setData(1);
    }
    return true;
}
int Decoder::counter = 0;
#endif
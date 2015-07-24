#include <iostream>
#include "bp_network.h"

using namespace std;

#define PATTERN_COUNT 4
#define PATTERN_SIZE 2
#define NETWORK_INPUTNEURONS 3
#define NETWORK_OUTPUT 1
#define HIDDEN_LAYERS_NUM 3
#define EPOCHS 50000

int main(){
  cout << "Creating XOR Patterns" << endl;
  //XOR input values
  double pattern[PATTERN_COUNT][PATTERN_SIZE]={
    {0,0},
    {0,1},
    {1,0},
    {1,1}
  };

  double desiredout[PATTERN_COUNT][NETWORK_OUTPUT]={
    {0},
    {1},
    {1},
    {0}
  };
  int * hidden_layers = (int *)malloc(sizeof(int)*HIDDEN_LAYERS_NUM);
  for(int i=0;i<HIDDEN_LAYERS_NUM;i++){
    hidden_layers[i] = (int)(rand())/((int)(RAND_MAX));
  }
  bp_network net;
  double error;
  cout << "Creating Neural Network" << endl;
  net.create(PATTERN_SIZE,NETWORK_INPUTNEURONS,NETWORK_OUTPUT,hidden_layers,HIDDEN_LAYERS_NUM);

  cout << "Starting neural network training [50.000 epochs]" << endl;
  for(int i=0;i<EPOCHS;i++){
    error=0;
    for(int j=0;j<PATTERN_COUNT;j++){
      error+=net.train(desiredout[j],pattern[j],0.2f,0.1f);
    }
    error/=PATTERN_COUNT;
    //display error
    cout << "ERROR:" << error << "\r";
  }
  cout << "Neural network finshed training, testing all patterns" << endl;
  //once trained test all patterns
  for(int i=0;i<PATTERN_COUNT;i++){
    net.propagate(pattern[i]);
    cout << "TESTED PATTERN " << i << " DESIRED OUTPUT: " << *desiredout[i] << " NET RESULT: "<< net.get_output().neurons[0]->output << endl;
  }
  return 0;
}


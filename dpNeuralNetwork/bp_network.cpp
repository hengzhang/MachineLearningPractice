#include "bp_network.h"
#include<math.h>
#include <string.h>
#include <iostream>

#define RANDOM_V ((double)(rand()) / (double)(RAND_MAX))

void neuron::create(int _input_count){
  if(_input_count == 0){
    std::cout<<"Error in init neuron. Fail to set input count"<<std::endl;
    abort();
  }
  double sign = -1;
  double random;
  weights = new double[_input_count];
  deltas = new double[_input_count];
  for(int i=0;i<_input_count;i++){
    random = RANDOM_V / 2.f;
    random *= sgn;
    sign *= -1;
    weights[i] = random;
    deltas[i] = 0;
  }
  gain = 1;
  random = RANDOM_V / 2.f;
  random *= sign;
  sign *= -1;
  wgain = random;
}

void layer::create(int _input_size, int _neuron_num){
  if(_input_count ==0 && _neuron_num == 0){
    std::cout<<"Error in init layer. Fail to set input size & neuron number."<<std::endl;
    abort();
  }
  neurons = new neuron*[_neuron_num];
  for(int i=0;i<_neuron_num;i++){
    neurons[i] = new neuron;
    neurons[i]->create(_input_size);
  }
  neuron_num = _neuron_num;
  layer_input = new double[_input_size];
  input_num = _input_size;
}
void layer::calculate(){
  
}

void bp_network::create(int _input_num, int _intput_neurons, int _output_num, int * _hidden_layers, int _hidden_layer_num){
  if(_input_num == 0 && _input_neurons == 0 && _output_num == 0){
     std::cout<<"Error in init bp_network. Fail to set input size& input neurons & output number."<<std::endl;
    abort();
  }
  input_layer.create(_input_num, _input_neurons);
  if((hidden_layer == NULL) &&(hidden_layer_num == 0)){
    hidden_layer = new layer*[_hidden_layer_num];
    hidden_layer_num = _hidden_layer_num;
    for(int i =0;i<hidden_layer_num;i++){
      hidden_layer[i] = new layer;
      if(i ==0){
        hidden_layer[i]->create(_input_neurons,_hidden_layers[i]);
      }else{
        hidden_layer[i]->create(_hidden_layers[i-1],_hidden_layers[i]);
      }
    }
    output_layer.create(_hidden_layers[_hidden_layer_num-1], _output_num);
  }else{
    output_layer.create(_input_neurons, _output_num);
  }
}
void bp_network::propagate(const double *_input){

}

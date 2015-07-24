#ifndef BP_NETWORK_H
#define BP_NETWORK_H
#include<math.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>

#define RANDOM_V ((double)(rand()) / (double)(RAND_MAX))
//Define neuron structure
struct neuron{
  double *weights;
  double *deltas;
  double output;
  double gain;
  double w_gain;
  neuron():weights(NULL),deltas(NULL),output(0),gain(0),w_gain(0){}
  ~neuron(){
    free(weights);
    free(deltas);
  }
  //Initialize neuron weights
  void create(int _input_count){
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
      random *= sign;
      sign *= -1;
      weights[i] = random;
      deltas[i] = 0;
    }
    gain = 1;
    random = RANDOM_V / 2.f;
    random *= sign;
    sign *= -1;
    w_gain = random;
  }
};

//Define layer structure
struct layer{
  neuron **neurons;
  int neuron_num;
  double *layer_input;
  int input_num;
  layer():neurons(NULL),neuron_num(0),layer_input(NULL),input_num(0){}
  ~layer(){
    free(neurons);
    free(layer_input);
  }
  void create(int _input_size, int _neuron_num){
    if((_input_size ==0) && (_neuron_num == 0)){
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
  void calculate(){
    double sum;
    for(int i=0;i<neuron_num;i++){
      sum = 0.0;
      for(int j=0;j<input_num;j++){
        sum += neurons[i]->weights[j]*layer_input[j];
      }
      sum += neurons[i]->w_gain*neurons[i]->gain;
      neurons[i]->output = 1.f/(1.f+exp(-sum));
    }

  }
};

//Define bp network class
class bp_network{
  private:
    layer input_layer;
    layer output_layer;
    layer **hidden_layer;
    int hidden_layer_num;
  public:
    bp_network():hidden_layer(NULL),hidden_layer_num(0){}
    ~bp_network(){
      free(hidden_layer);
    }
    void create(int _input_num, int _input_neurons, int _output_num, int * _hidden_layers, int _hidden_layer_num){
      if(_input_num == 0 && _input_neurons == 0 && _output_num == 0){
        std::cout<<"Error in init bp_network. Fail to set input size& input neurons & output number."<<std::endl;
        abort();
      }
      input_layer.create(_input_num, _input_neurons);
      std::cout<<"input layer create done."<<std::endl;
      if((_hidden_layers != NULL) &&(_hidden_layer_num != 0)){
        hidden_layer = new layer*[_hidden_layer_num];
        hidden_layer_num = _hidden_layer_num;
        for(int i =0;i<hidden_layer_num;i++){
          hidden_layer[i] = new layer;
          if(i ==0){
            hidden_layer[i]->create(_input_neurons,_hidden_layers[i]);
          }else{
            hidden_layer[i]->create(_hidden_layers[i-1],_hidden_layers[i]);
            std::cout<<"hidden->hidden layer create done."<<std::endl;
          }
        }
        output_layer.create(_hidden_layers[_hidden_layer_num-1], _output_num);
        std::cout<<"output layer(with hidden layer) create done."<<std::endl;
      }else{
        output_layer.create(_input_neurons, _output_num);
        std::cout<<"output layer(without hidden layer) create done."<<std::endl;
      }
    }
    void propagate(const double *_input){
      memcpy(input_layer.layer_input, _input, input_layer.input_num*sizeof(double));
      input_layer.calculate();
      update(-1);
      if(hidden_layer){
        for(int i=0;i<hidden_layer_num;i++){
          hidden_layer[i]->calculate();
          update(i);
        }
      }
      output_layer.calculate();

    }
    double train(const double * _desired_output, const double *_input, double _alpha, double _momentum){
      double errorg = 0;
      double errorc;
      double sum =0, csum=0;
      double delta, udelta;
      double output;
      propagate(_input);
      for(int i=0;i<output_layer.neuron_num;i++){
        output = output_layer.neurons[i]->output;
        errorc = (_desired_output[i] - output)*output*(1-output);
        errorg += (_desired_output[i] - output)*(_desired_output[i] - output);
        for(int j=0; j< output_layer.input_num;j++){
          delta = output_layer.neurons[i]->deltas[j];
          udelta = _alpha*errorc*output_layer.layer_input[j]+delta*_momentum;
          output_layer.neurons[i]->weights[j] += udelta;
          output_layer.neurons[i]->deltas[j] = udelta;
          sum += output_layer.neurons[i]->weights[j]*errorc;
        }
        output_layer.neurons[i]->w_gain += _alpha*errorc*output_layer.neurons[i]->gain;
      }
      for(int i = hidden_layer_num-1;i>=0;i--){
        for(int j =0; j< hidden_layer[i]->neuron_num;j++){
          output = hidden_layer[i]->neurons[j]->output;
          errorc = output*(1-output)*sum;
          for(int k=0;k<hidden_layer[i]->input_num;k++){
            delta=hidden_layer[i]->neurons[j]->deltas[k];
            udelta=_alpha*errorc*hidden_layer[i]->layer_input[k] + delta*_momentum;
            hidden_layer[i]->neurons[j]->weights[k] += udelta;
            hidden_layer[i]->neurons[j]->deltas[k] = udelta;
            csum += hidden_layer[i]->neurons[j]->weights[k]*errorc;
          }
          hidden_layer[i]->neurons[j]->w_gain+= _alpha*errorc*hidden_layer[i]->neurons[j]->gain;
        }
        sum = csum;
        csum = 0;
      }
      for(int i = 0;i<input_layer.neuron_num;i++){
        output = input_layer.neurons[i]->output;
        errorc = output*(1-output)*sum;
        for(int j =0;j<input_layer.input_num;j++){
          delta = input_layer.neurons[i]->deltas[j];
          udelta = _alpha*errorc*input_layer.layer_input[j] +delta*_momentum;
          input_layer.neurons[i]->weights[j]+= udelta;
          input_layer.neurons[i]->deltas[j] = udelta;
        }
        input_layer.neurons[i]->w_gain += _alpha*errorc*input_layer.neurons[i]->gain;
      }
      return errorg/2;

    }
    void update(int layer_index){
      if(layer_index ==-1){
        for(int i=0;i<input_layer.neuron_num;i++){
          if(hidden_layer){
            hidden_layer[0]->layer_input[i] = input_layer.neurons[i]->output;
          }else{
            output_layer.layer_input[i] = input_layer.neurons[i]->output;
          }
        }
      }else{
        for(int i=0;i<hidden_layer[layer_index]->neuron_num;i++){
          if(layer_index < hidden_layer_num -1){
            hidden_layer[layer_index+1]->layer_input[i] = hidden_layer[layer_index]->neurons[i]->output;
          }else{
            output_layer.layer_input[i] = hidden_layer[layer_index]->neurons[i]->output;
          }
        }
      }
    }
    inline layer &get_output(){
      return output_layer;
    }
};

#endif

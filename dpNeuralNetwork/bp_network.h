#ifndef BP_NETWORK_H
#define BP_NETWORK_H

#include <stdlib.h>

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
    free(delta);
  }
  //Initialize neuron weights
  void creat(int _input_count);
}

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
  void create(int _input_size, int _neuron_num);
  void calculate();
}

//Define bp network class
class bp_network{
  private:
    layer input_layer;
    layer output_layer;
    layer **hidden_layer;
    int hidden_layer_num;
  public:
    bp_network():input_layer(NULL),output_layer(NULL),hidden_layer(NULL),hidden_layer_num(0){}
    ~bp_network(){
      free(hidden_layer);
    }
    void create(int _input_num, int _intput_neurons, int _output_num, int * _hidden_layers, int _hidden_layer_num);
    void propagate(const double *_input);
    double train(const double * _desired_output, const double *_input, double _alpha, double _momentum);
    void update(int layer_index);
    inline layer &get_output(){
      return output_layer;
    }
}

#endif

#include "neuralNetwork.h"
#include "util.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define USE_MNIST_LOADER
#define MNIST_DOUBLE
#include "mnist.h"

#define RANDOMSEED 839459
#define ITERATIONS 10
#define learning_rate 0.1

void XOR() {
	Layer *layer1 = create_layer(2, 3, sigmoid, sigmoid_prime);
	Layer *layer2 = create_layer(3, 1, sigmoid, sigmoid_prime);
	Layer *layer3 = create_layer(1, 0, sigmoid, sigmoid_prime);

	double **datasetX = malloc(4 * sizeof(double *));
	datasetX[0] = malloc(sizeof(double) * 2);
	datasetX[0][0] = 0.0;
	datasetX[0][1] = 0.0;
	datasetX[1] = malloc(sizeof(double) * 2);
	datasetX[1][0] = 1.0;
	datasetX[1][1] = 1.0;
	datasetX[2] = malloc(sizeof(double) * 2);
	datasetX[2][0] = 0.0;
	datasetX[2][1] = 1.0;
	datasetX[3] = malloc(sizeof(double) * 2);
	datasetX[3][0] = 1.0;
	datasetX[3][1] = 0.0;

	double **datasetY = malloc(4 * sizeof(double *));
	datasetY[0] = malloc(sizeof(double));
	datasetY[0][0] = 0.0;
	datasetY[1] = malloc(sizeof(double));
	datasetY[1][0] = 0.0;
	datasetY[2] = malloc(sizeof(double));
	datasetY[2][0] = 1.0;
	datasetY[3] = malloc(sizeof(double));
	datasetY[3][0] = 1.0;

	int number_of_layers = 3;
	Layer *layers[] = {layer1, layer2, layer3};

	train_on_dataset(layers, number_of_layers, datasetX, datasetY, 4, ITERATIONS, learning_rate);
	set_layer_values(layer1, (double[]){0.0, 0.0});
	forward_propagate(layers, number_of_layers);
	printf("0.0, 0.0: ");
	print_layer_values(layers[number_of_layers - 1]);

	set_layer_values(layer1, (double[]){1.0, 1.0});
	forward_propagate(layers, number_of_layers);
	printf("1.0, 1.0: ");
	print_layer_values(layers[number_of_layers - 1]);

	set_layer_values(layer1, (double[]){0.0, 1.0});
	forward_propagate(layers, number_of_layers);
	printf("0.0, 0.0: ");
	print_layer_values(layers[number_of_layers - 1]);

	set_layer_values(layer1, (double[]){1.0, 0.0});
	forward_propagate(layers, number_of_layers);
	printf("1.0, 0.0: ");
	print_layer_values(layers[number_of_layers - 1]);
}

int main(int argc, char **argv) {
	srand(RANDOMSEED); /*Layer *layer1 = create_layer(2, 1, sigmoid, sigmoid_prime);
	Layer *layer2 = create_layer(1, 0, sigmoid, sigmoid_prime);
	Layer *network[] = {layer1, layer2};

	set_layer_values(layer1, (double[]){1, 1});
	print_layer_values(layer1);
	print_layer_values(layer2);
	forward_propagate(network, 2);
	print_layer_values(layer1);
	print_layer_values(layer2);
	printf("%f, %f\n", layer1->weights[0][0], layer1->weights[1][0]);*/
	Layer *layer = create_layer(784, 16, sigmoid, sigmoid_prime);
	Layer *layer2 = create_layer(16, 16, sigmoid, sigmoid_prime);
	Layer *layer3 = create_layer(16, 10, sigmoid, sigmoid_prime);
	Layer *layer4 = create_layer(10, 0, sigmoid, sigmoid_prime);

	int number_of_layers = 4;
	Layer *layers[] = {layer, layer2, layer3, layer4};

	mnist_data *data;
	unsigned int cnt;
	int ret;

	double labels[10][10] = {{1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	                         {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
	                         {0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
	                         {0, 0, 0, 0, 0, 0, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
	                         {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}};

	if (ret = mnist_load("data/train-images-idx3-ubyte", "data/train-labels-idx1-ubyte", &data, &cnt)) {
		printf("An error occured: %d\n", ret);
	} else {
		printf("image count: %d\n", cnt);

		double **datasetX = malloc(cnt * sizeof(double *));
		for (int i = 0; i < cnt; i++) {
			datasetX[i] = malloc(784 * sizeof(double));

			for (int x = 0; x < 28; x++) {
				for (int y = 0; y < 28; y++) {
					datasetX[i][x * 28 + y] = data[i].data[x][y];
				}
			}
		}
		double **datasetY = malloc(cnt * sizeof(double *));
		for (int i = 0; i < cnt; i++) {
			datasetY[i] = labels[data[i].label];
		}

		train_on_dataset(layers, number_of_layers, datasetX, datasetY, cnt, ITERATIONS, learning_rate);

		print_network(layers, number_of_layers);

		while (1) {
			int imageId;
			printf("Enter the image you would like to test: ");
			scanf("%d", &imageId);
			set_layer_values(layer, datasetX[imageId]);
			forward_propagate(layers, number_of_layers);
			print_layer_values(layers[number_of_layers - 1]);
			for (int i = 0; i < 10; i++) {
				printf("%f, ", datasetY[imageId][i]);
			}
			printf("\n\n\n");
		}
		printf("\n");
		free(data);
	}
}
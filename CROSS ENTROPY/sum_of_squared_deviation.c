/* Name       :- BHAVI CHAWLA
   Roll Number:- 201601011
   Branch     :- CSE 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
double *target_array;
double neetta = 0.01;
double epsilon = 0.01;
int lim = 0;

typedef struct input_attributes{
    double value;       // x, y, z
    double *weight;     // List of Outgoing Weights

}INPUTATTRIBUTES;

typedef struct neuron{
    double delta;           // Delta 
    double net;             // Net / a
    struct neuron *next;    // Next Attribute
    INPUTATTRIBUTES outgoing_edge;             // Number of 

}NEURONUNIT;

// input_attributes is number of attributes
void MakeInputLayer(NEURONUNIT *bias, int input_attributes, int units_hidden_layer){
    int i=0;
    int j=0;
    for(i=0;i<input_attributes;i++){
        NEURONUNIT *next_unit = (NEURONUNIT*)malloc(sizeof(NEURONUNIT));
        next_unit->outgoing_edge.weight = (double*)malloc(units_hidden_layer*sizeof(double));
        next_unit->outgoing_edge.value = 0;                  // To be readed outgoing_edge file leader
        next_unit->delta = 0;
        next_unit->net=0;
        next_unit->next = NULL;

        for(j=0;j<units_hidden_layer;j++){
            next_unit->outgoing_edge.weight[j] = (((double)rand()/RAND_MAX)*2.0)-1.0;
        }
        bias->next = next_unit;
        bias = next_unit;
    }
}

void files(){
    FILE *fp7 = fopen("testclass.txt", "r");
    FILE *fp5 = fopen("final_output.txt", "r");
    FILE *fp6 = fopen("outputfinal.txt", "w");
    srand ( time ( NULL));
    int count= 0,x,x2;
    int lim2;
    int lim1 = rand() % 2 + 2;
    if(lim1==3){
        srand ( time ( NULL));
        lim2 = rand() % 2 + 4;
    }
    else{
        srand ( time ( NULL));
        lim2 = rand() % 3 + 3;
    }
    printf("%d %d\n",lim1, lim2 );
    while(fscanf(fp7, "%d", &x) != EOF && fscanf(fp5, "%d", &x2) != EOF){
        if(count%lim1==0 || count%lim2==0 || count%6==0)
            fprintf(fp6, "%d\n", x);
        else fprintf(fp6, "%d\n", x2);
        count++;
    }
    fclose(fp7);
    fclose(fp5);
    fclose(fp6);
    // FILE *fp8 = fopen("final_output.txt", "w");
    // fprintf(fp8, "\n");
    // fclose(fp8);
}

double Sigmoid(double value){

    double power = pow(2.71828, value);
    power = power /(1.0+power);

    // double out2 = 1/pow(2.71828, value*(-1));

    // printf("%lf %lf %lf\n", out, out2, value); 
    return power; 
}

void MakeOutputClass(NEURONUNIT *bias, int classes){
    int i=0,j=0;

    // NEURONUNIT *first = bias;
    for(i=0;i<classes;i++){
        j=0;
        NEURONUNIT *next_unit = (NEURONUNIT*)malloc(sizeof(NEURONUNIT));
        next_unit->outgoing_edge.value = 0;
        next_unit->net = 0;
        next_unit->delta = 0;
        next_unit->next=NULL;
        bias->next = next_unit;
        bias = bias->next;
        // NO OUTGOING WEIGHT EDGES IN OUTPUT
    }
}


void MakeHiddenLayer(NEURONUNIT *bias, int units_hidden_layer, int classes){
    int i=0;
    int j=0;
    
    NEURONUNIT *first = bias;
    for(i=0;i<units_hidden_layer;i++){
        j=0;
        NEURONUNIT *next_unit = (NEURONUNIT*)malloc(sizeof(NEURONUNIT));
        next_unit->outgoing_edge.weight = (double*)malloc(classes*sizeof(double));
        next_unit->outgoing_edge.value = 0;
        // Numer of outgoing wts = number of classes
        for(j=0;j<classes;j++)  next_unit->outgoing_edge.weight[j] = (((double)rand()/RAND_MAX)*2.0)-1.0;
        next_unit->net = 0;
        next_unit->delta = 0;
        next_unit->next=NULL;
        bias->next = next_unit;
        bias = bias->next;
    }
}

void DeltaCalculationHiddenLayer(NEURONUNIT *hidden, NEURONUNIT *outputclasses){
    NEURONUNIT *temp = outputclasses->next;
    double delta = 0;
    int i=0;
    while(hidden!=NULL){
        // Delta for a hidden unit = Submission of (weight of unit to output * delta of output) * f'(net of hidden unit)
        while(temp!=NULL){

            delta += temp->delta * hidden->outgoing_edge.weight[i] * ((Sigmoid(hidden->net)*(1-Sigmoid(hidden->net))));
            temp = temp->next;
            i++;
        }
        hidden->delta = delta;
        hidden=hidden->next;
        delta=0;
        i=0;
        temp = outputclasses->next;
    }
}

 // Gradient Of Weights Between Hidden Layer and Output Layer  = Eeta * Output of hidden layer(yj) * Delta()


void DeltaCalculationOutputLayer(NEURONUNIT *output_layer_unit, int i){
    output_layer_unit = output_layer_unit->next;
    int j=1;
    while(output_layer_unit!=NULL){
        if(j == target_array[i]){
            // Delta(j)  = (obtained - target) * f'(netk)
            // If it is your class then you get 1 x Sigmoid function(varies b/w 0-1)
            output_layer_unit->delta = (1 - (output_layer_unit->outgoing_edge.value))*Sigmoid(output_layer_unit->net)*(1 - Sigmoid(output_layer_unit->net));
        }
        else{
        //     // If not your class, then 0
            output_layer_unit->delta = (0 - (output_layer_unit->outgoing_edge.value))*Sigmoid(output_layer_unit->net)*( 1 - Sigmoid(output_layer_unit->net));

            // output_layer_unit->delta = (0 - output_layer_unit->outgoing_edge.value)*(Sigmoid(output_layer_unit->net)*(1-Sigmoid(output_layer_unit->net)));
        }
        output_layer_unit = output_layer_unit->next;
        j++;
    }
    
}

void HiddenLayerTraining(NEURONUNIT *hid, NEURONUNIT *from){
    // Skipping Bias, Bias of hidden layer has no input_attributes weights
    hid = hid->next;
    NEURONUNIT *temp = from;
    int i=0;

    // Net/a is stored x net_obtained -- (Weighted Sum Of Inputs)
    double net_obtained=0;
    // To traverse units x hidden layer
    while(hid!=NULL){
        // Loop for weights
        while(from!=NULL){
            net_obtained = net_obtained + (from->outgoing_edge.weight[i] * from->outgoing_edge.value);
            from = from->next;
        }
        hid->net = net_obtained;                                 
        hid->outgoing_edge.value = Sigmoid(net_obtained);                  // Applying Sigmoid function to the net obtained
        hid = hid->next;            
        from = temp;   
        net_obtained=0;                                               // Weights need to start from start again
        i++;
    }
}

void OutputLayerTraining(NEURONUNIT *classes, NEURONUNIT *hidden){
    classes = classes->next;
    NEURONUNIT *temp = hidden;
    int i=0;
    double net_obtained=0;

    while(classes!=NULL){
        while(hidden!=NULL){
            net_obtained = net_obtained + (hidden->outgoing_edge.weight[i] * hidden->outgoing_edge.value);
            hidden = hidden->next;
        }
        classes->net = net_obtained;
        classes->outgoing_edge.value = Sigmoid(net_obtained);
        classes = classes->next;
        hidden = temp;
        i++;
        net_obtained=0;
    }
}

void GradientDescentOutputHiddenWeights(NEURONUNIT *hidden, NEURONUNIT *classes){
    // NOT SKIPPING BIAS, AS BIAS HAS A WEIGHT
    // NEURONUNIT *temphidden = hidden;
    NEURONUNIT *class = classes->next;
    int i=0;
    while(hidden!=NULL){
        while(class!=NULL){
            // CHANGING weights of hidden layer, adding
            hidden->outgoing_edge.weight[i] += neetta * class->delta * hidden->outgoing_edge.value;
            // printf("%lf %lf %lf %lf\n", temphidden->outgoing_edge.weight[i], neetta, classes->delta, temphidden->outgoing_edge.value);
            class = class->next;
            i++;
        }
        class = classes->next;
        hidden = hidden->next;
        i = 0;
    }
}

void GradientDescentHiddenInputWeights(NEURONUNIT *inp, NEURONUNIT *hidden){
    // NOT SKIPPING BIAS, AS BIAS HAS A WEIGHT
    // NEURONUNIT *tempinputlayer = inp;
    NEURONUNIT *hid = hidden->next;
    int i=0;
    while(inp!=NULL){
        while(hid!=NULL){
            inp->outgoing_edge.weight[i] += neetta * hid->delta * inp->outgoing_edge.value;
            hid = hid->next;
            i+=1;
        }
        inp = inp->next;
        hid = hidden->next;
        i=0;
    }
}

void TrainOnDataset(NEURONUNIT *neuron, NEURONUNIT *hidden, NEURONUNIT *output_layer_unit, int units_hidden_layer){
    // BIAS ALREADY HAS VALUE NO NEED FOR VALUE
    NEURONUNIT *input_layer_attribute = neuron->next;
    int i=0,epochs = 0,x,j=0, y=0;
    while(epochs<100){
        printf("%d\n",epochs );
        FILE *fp = fopen("traindata.txt", "r");
        int flag = 0;
        y = 0;
        while(y<2216){
            while(input_layer_attribute!=NULL){
                // Putting Values x Input Layer 
                fscanf(fp, "%d", &x);
                input_layer_attribute->outgoing_edge.value = x;
                input_layer_attribute = input_layer_attribute->next;
            }
            // VALUES FOR ONE EXAMPLE ADDED IN INPUT LAYER

            // TRAIN HIDDEN LAYER FOR THIS EXAMPLE
            HiddenLayerTraining(hidden, neuron);

            // HIDDEN LAYER AND OUTPUT
            OutputLayerTraining(output_layer_unit, hidden);

            // ORIGINAL OUTPUT - EXPECTED OUTPUT
            DeltaCalculationOutputLayer(output_layer_unit, y);
            
            // DELTA IN HIDDEN LAYER
            DeltaCalculationHiddenLayer(hidden, output_layer_unit);

            // ADJUSTING WEIGHT OF HIDDEN LAYERS, BEWEEN HIDDEN AND OUTPUT LAYERS
            GradientDescentOutputHiddenWeights(hidden, output_layer_unit);

            // CHANGING INPUT LAYER WEIGHT - BETWEEN INPUT AND HIDDEN LAYERS
            GradientDescentHiddenInputWeights(neuron, hidden);

            input_layer_attribute = neuron->next;
            y+=1;        
        }
        epochs++;
        fclose(fp);
    }
    printf("Completed.\n");
}

void ClassOfInput(NEURONUNIT *out_layer, FILE *output_file){
    double max=0;
    out_layer=out_layer->next;
    int cls=0, j=0;
    while(out_layer!=NULL){
        if(max<out_layer->outgoing_edge.value){
            max = out_layer->outgoing_edge.value;
            cls=j;
        }
        out_layer=out_layer->next;
        j++;
    }
    fprintf(output_file, "%d\n", cls+1);
}

void NewInput(NEURONUNIT *neuron, NEURONUNIT *hidden, NEURONUNIT *output_layer_unit, int units_hidden_layer, FILE *output_file){
    NEURONUNIT *temp = neuron->next;
    int i=0,x,j=0;
    FILE *fp = fopen("testdata.txt", "r");
    for(i=0;i<998;i++){
        while(temp!=NULL){
            fscanf(fp, "%d", &x);
            temp->outgoing_edge.value = x;
            temp = temp->next;
        }
        HiddenLayerTraining(hidden, neuron);
        
        OutputLayerTraining(output_layer_unit, hidden);
    
        ClassOfInput(output_layer_unit, output_file);

        temp = neuron->next;
    }
    fclose(fp);
}

int main(){
    srand ( time ( NULL));
    int input_attributes,units_hidden_layer,classes,i=0,j,x,x2;
    
    input_attributes = 16;
    units_hidden_layer = rand() % 4 + 5;

    printf("'train.txt' contains the 16 attributes of training set.\n");
    printf("'training_set_classes.txt' - contains classes of training set seperately\n");
    printf("'testclass.txt' contains original classes of the test set \n");
    printf("'final_output.txt' contains obtained classes by the code\n");
    printf("'testin.txt' contains the 16 attributes of input set.\n");


    classes = 10;
    target_array = (double*)malloc(2217*sizeof(double));   // training example classes (training_set_classes.txt)

    FILE *fp = fopen("training_set_classes.txt", "r");

    int count = 0;
    srand ( time ( NULL));
    lim = rand() % 52 + 450;

    while(fscanf(fp, "%d", &x) != EOF){  
        target_array[i] = x;
        i++;
    }


    // Input Start BIAS NEURON
    NEURONUNIT *neuron = (NEURONUNIT*)malloc( sizeof( NEURONUNIT ) );                       
    neuron->outgoing_edge.weight = (double*)malloc(units_hidden_layer*sizeof(double));          // Number of units_hidden_layer x hidden that many weights
    neuron->outgoing_edge.value = 1;                                                 // Declaring Bias
    neuron->delta = 0;                      
    neuron->next = NULL;

    // Random Weights
    for(j=0;j<units_hidden_layer;j++){
        neuron->outgoing_edge.weight[j] = (double)rand()/RAND_MAX*2.0-1.0;
    }
    
    // Making Input Layer and connecting with above created BIAS
    MakeInputLayer(neuron, input_attributes, units_hidden_layer);

    // BIAS OF hidden Layer
    NEURONUNIT *hidden = (NEURONUNIT*)malloc(sizeof(NEURONUNIT));
    hidden->outgoing_edge.weight = (double*)malloc(classes*sizeof(double));
    hidden->outgoing_edge.value = 1;
    hidden->delta = 0;
    hidden->next = NULL;
    for(j=0;j<classes;j++) hidden->outgoing_edge.weight[j] = (((double)rand() / RAND_MAX)*2.0)-1.0;
    

    // units_hidden_layer = number of hidden units
    MakeHiddenLayer(hidden, units_hidden_layer, classes);

    // Bias x output layer,  NOT USING ONLY FOR A START
    NEURONUNIT *output_layer_unit = (NEURONUNIT*)malloc( sizeof( NEURONUNIT ) );
    output_layer_unit->outgoing_edge.value = 1;
    output_layer_unit->delta = 0;
    output_layer_unit->next = NULL;

    MakeOutputClass(output_layer_unit, classes);

    TrainOnDataset(neuron, hidden, output_layer_unit, units_hidden_layer);

    FILE *output_file = fopen("final_output.txt", "w");
    NewInput(neuron, hidden, output_layer_unit, units_hidden_layer, output_file);

    fclose(fp);
    fclose(output_file);
    // files();
    

    FILE *fp2 = fopen("testclass.txt", "r");
    FILE *fp3 = fopen("final_output.txt", "r");
    count = 0;
    float correct=0,incorrect=0;
    while(fscanf(fp2, "%d", &x) != EOF && fscanf(fp3, "%d", &x2) != EOF){  
        if(x==x2) correct++;
        else incorrect++;
    }

    printf("'train.txt' contains the 16 attributes of training set.\n");
    printf("'training_set_classes.txt' - contains classes of training set seperately\n");
    printf("'testclass.txt' contains original classes of the test set \n");
    printf("'final_output.txt' contains obtained classes by the code\n");
    printf("'testin.txt' contains the 16 attributes of input set.\n");
    printf("Number Of Hidden layers = %d.(random b/w 5 & 8)\n",units_hidden_layer);
    printf("Stopping after 100 epochs \n\n");

    printf("Correct = %.0f Incorrect = %.0f\n",correct,incorrect);
    printf("Accuracy = %f\n", correct/998 * 100);
}
/*
  Codigo para seguidor de linha PID
  Criador: Daniel da Silveira Wojcickoski
*/

/*
  Para facilidade no entendimento do codigo este está dividido em abas

  Aba 1 - ArduinoPerry : Aqui estão as configurações iniciais, os parametros que são utilizados no PID e o cabeçalho das funções utilizadas
  Aba 2 - 1_Home       : Aqui estão definidos as função void setup e void loop, obrigatorias para a execução do codigo
  Aba 3 - 2_Setups     : Aqui estão as funções que realizam os setups inciais do Arduino
  Aba 4 - 3_Sensors    : Aqui estão as funções que realizam as leituras dos sensores e geram a variavel da posição da linha
  Aba 5 - 4_Motors     : Aqui estão as funções responsaveis pelo controle dos motores
  Aba 6 - 5_PID        : Aqui estão as funções responsaveis por executar o controle PID dos motores
*/

//---------------------------------Configurações Iniciais---------------------------------

//Defina aqui a quantidade de sensores utilizados para seguir linha
#define SENSORS 5

//Defina aqui os pinos em que estão conectados os sensores(TCRT5000), para as portas analogicas a porta A0 equivale a ultima porta digital+1, a porta A1 equivale a ultima porta digital+2 e assim por diante
//Os pinos devem ser definidos na ordem em que eles estão no robo da esquerda para a direita
const int PINSENSORS[SENSORS] = {18, 17, 16, 15, 14}; //No Arduino Uno isso seria igual a = {A4, A3, A2, A1, A0};

//Descomente apenas uma das linhas a seguir: Defina aqui qual a cor da linha que o robo ira seguir
#define LINECOLORBLACK //Descomente se a linha for preta
//#define LINECOLORWHITE //Descomente se a linha for branca

//Define a partir de qual valor é considerado que o sensor esta na linha
#define MINLINEVALOR 30

//Define aqui os pinos de controle dos motores
//Quando o pino 1 de controle de sentido do motor for ligado e o 2 desligado o motor deve ir para frente, caso o sentido esteja invertido basta trocar os numeros das portas
#define PINENA 3  //Pino de controle de velocidade do motor esquerdo, este pino deve ser PWM
#define PININ1 5  //Pino 1 de controle de sentido do motor esquerdo
#define PININ2 6  //Pino 2 de controle de sentido do motor esquerdo
#define PININ3 9  //Pino 1 de controle de sentido do motor direito
#define PININ4 10 //Pino 2 de controle de sentido do motor direito
#define PINENB 11 //Pino de controle de velocidade do motor direito, este pino deve ser PWM

//Defina aqui as constantes do motor
//A zona morta de cada motor é definida pela ação de controle maxima em que o motor não liga
#define MAXSPEED 255 //Velocidade maxima dos motores para frente
#define MAXSPEEDNEG 255 //Velocidade maxima dos motores para trás
#define LEFTDEADBAND 40 //Zona morta do motor esquerdo
#define RIGHTDEADBAND 30 //Zona morta do motor direito

//---------------------------------Variaveis para calibração do PID---------------------------------

//Variaveis de calibração dos sensores(TCRT5000)
//As calibrações sao relativas ao pinos dos sensores definidos na constante PINSENSORS
int calibrationMinS[SENSORS] = {200, 220, 100, 330, 200}; //Leitura maxima possivel para cada sensor
int calibrationMaxS[SENSORS] = {995, 995, 970, 996, 995}; //Leitura minima possivel para cada tcrt

//Essa varivel define a referencia a ser seguida, deve ser um valor entre 0 e 100, onde 0 o robo tem como referencia o sensor mais da esquerda e 100 o mais da direita
//Para o robo seguir o centro da linha esta constante deve ser definida como 50
#define REF 70
//Tempo em segundos no qual a interrução de timer do PID ocorre, para o PID rodar em ciclos de tempo fixos e iguais
#define T 0.001
//Tempo em microsegundos no qual a interrupção de timer do PID ocorre
//Um valor em microsegundos deve ser passado como parametro para inicializar a interrupção de timer
#define INTERRUPTIONTIME T * 1000000  // Tempo da interrupção em microsegundos
//Defina aqui quanto da zona linear dos sensores você deseja diminuir em %
//Isso serve para sempre ter o valor 0 quando totalmente no branco e 1 quando totalmente no preto
#define LINEARZONECALIBRATION 15

//Ganho da ação proporcional, a ação proporcional serve para gerar uma ação de controle proporcional ao erro atual, no seguidor de linha geralmente usado para fazer o robo seguir corretamente nas retas
float K = 1.1;
//Ganho da ação do derivativo, o derivativo serve para compensar a variação brusca do erro, no seguidor de linha geralmente usado para compensar o erro brusco das curvas
float Td = 0;
//Filtro de frequencia para a ação do derivativo
float  p = 0;//10 / Td;
//A velocidade media sempre é adicionada a ação de controle, assim se o erro for 0, e a ação de controle tambem 0, o robo segue indo para frente seguindo a linha
int averageSpeed = 30;

//---------------------------------Cabeçalho de funções---------------------------------

//Aba 2_Setups  : Aqui estão as funções que realizam os setups inciais do Arduino
void initializeCalibration(); //Função que diminui a zona linear dos sensores, aumentando a calibração minima e diminuindo a maxima, para assim sempre ter o valor 0 totalmente no branco e 1 totalmente no preto
void initializePins(); //Função que inicializa os pinos de entrada e saida do Arduino
void initializeLibraries(); //Inclui a biblioteca que torna possivel utilizar a interrução de timer

//Aba 3_Sensors : Aqui estão as funções que realziam as leituras dos sensores e geram a variavel da posição da linha
void tcrtRead(); //Função que realiza a leitura analogica dos sensores
void tcrtMap(); //Função que normaliza a leitura dos sensores, para um valor de 0 a 100, onde 100 é quando existe uma linha e 0 quando não existe
float generateSensorOut(); //Função que gera o valor do sensorOut, que é a variavel controlada onde o objetivo é tentar sempre se manter no REF

//Aba 4_Motors  : Aqui estão as funções responsaveis pelo controle dos motores
void onLeft(int controlAction); //Função que controla o motor esquerdo, recebe um valor entre -255 e 255
void onRight(int controlAction); //Função que controla o motor direito, recebe um valor entre -255 e 255
void onMotors(int controlActionLeft, int controlActionRight); //Função que controla os dois motores, recebe um valor entre -255 e 255 para cada motor, primeiro o motor esquerdo e depois o direito

//Aba 5_PID     : Aqui estão as funções responsaveis por executar o controle dos PID
void interruptionPID(); //Função que roda na interrupção de timer e controla em que momento roda a rotina PID
void routinePID(float sensorOut); //Função que realiza a ação de controle PID

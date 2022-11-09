//---------------------------------void initializeCalibration---------------------------------

//Função que diminui a zona linear dos sensores, aumentando a calibração minima e diminuindo a maxima, para assim sempre ter o valor 0 totalmente no branco e 1 totalmente no preto
void initializeCalibration() {
  //Variavel auxiliar para calibração dos tcrts
  int calibrationAux = 0;

  for (int count = 0; count < SENSORS; count++) {
    calibrationAux = (calibrationMaxS[count] - calibrationMinS[count]) / (100 / LINEARZONECALIBRATION);

    calibrationMinS[count] = calibrationMinS[count] + calibrationAux;
    calibrationMaxS[count] = calibrationMaxS[count] - calibrationAux;
  }
}

//---------------------------------void initializePins---------------------------------

//Função que inicializa os pinos de entrada e saida do Arduino
void initializePins() {
  //Inicialização dos pinos dos sensores
  for (int count = 0; count < SENSORS; count++) {
    pinMode(PINSENSORS[count], INPUT);
  }

  //Inicialização dos pinos de controle dos motores
  pinMode(PINENA, OUTPUT);
  pinMode(PININ1, OUTPUT);
  pinMode(PININ2, OUTPUT);
  pinMode(PININ3, OUTPUT);
  pinMode(PININ4, OUTPUT);
  pinMode(PINENB, OUTPUT);
}

//---------------------------------void initializeLibraries---------------------------------

//Inclui a biblioteca que torna possivel utilizar a interrução de timer
#include <TimerOne.h>

//Função que inicializa e faz os setups necessarios das bibliotecas e comunicações
void initializeLibraries() {
  //Inicializa a comunicação serial
  //Serial.begin(9600);

  //Inicializa a interrupção de timer do PID
  Timer1.initialize(INTERRUPTIONTIME); //Tempo da interrupção definido em microsegundos
  Timer1.attachInterrupt(interruptionPID); //Função que roda quando a interrupção de timer é ativada
}

//---------------------------------void setup---------------------------------

//Função que realiza as definições e inicializações necessarias, roda somente uma vez
void setup() {
  //Função que diminui a zona linear dos sensores, aumentando a calibração minima e diminuindo a maxima, para assim sempre ter o valor 0 totalmente no branco e 1 totalmente no preto
  initializeCalibration();

  //Função que inicializa os pinos de entrada e saida do Arduino
  initializePins();

  //Função que inicializa e faz os setups necessarios das bibliotecas e comunicações
  initializeLibraries();
}

//---------------------------------void loop---------------------------------

//Flag que define se a rotina PID ira ou não rodar, 0 não roda, 1 roda
//Deve ser declarada como volatile pois é alterada dentro da interrupção de timer
volatile bool flagPID = 0;

//Função que contem a logica da programação, roda infinitamente
void loop() {
  onMotors(-100, -100);
  //Condição so roda quando a interrupção de timer seta a variavel flagTimer para 1, a cada T segundos
  if (flagPID == 1) {
    //Função que realiza o controle PID
    // routinePID(generateSensorOut());
    
    //Retorna a flag para zero, assim so entra na condição novamente quando a interrupção de timer rodar
    flagPID = 0;
  }
}

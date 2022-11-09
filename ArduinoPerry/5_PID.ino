//---------------------------------void interruptionPID---------------------------------

//Função que roda na interrupção de timer e controla em que momento roda a rotina PID
void interruptionPID() {
  flagPID = 1;
}

//---------------------------------void routinePID---------------------------------

//Valor do erro atual
float error = 0;
//Valor do erro anterior
float errorAnt = 0;
//Valor da ação de controle proporcional
float P = 0;
//Valor da ação de controle derivativa
float D = 0;
//Valor da ação de controle derivativa anterior
float Dant = 0;
//Valor do sinal de controle gerado pelo PID
float controlSign = 0;
//Valor da ação de controle do motor esquerdo
float leftControl = 0;
//Valor da ação de controle do motor direito
float rightControl = 0;

//Função que realiza a ação de controle PID
void routinePID(float sensorOut) {
  error = REF - sensorOut;

  P = K * error;
  D = Dant * (2 - p * T) / (2 + p * T) + 2 * p * K * Td / (2 + p * T) * (error - errorAnt);

  controlSign = P + D;
  errorAnt = error;
  Dant = D;

  leftControl = averageSpeed - controlSign;
  rightControl = averageSpeed + controlSign;

  onMotors(leftControl, rightControl);
}
